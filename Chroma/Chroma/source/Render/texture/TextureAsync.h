#pragma once
////////////////////////////////////////////////////////////////////////////////
//
// Example of converting stb_image to a libjpeg-style background process.
// This enables you to stream JPEG files from the internet and interleave
// decoding time without frameout on the main thread.
//
// The example uses three parts of the C++11 threads API.
// 1) std::async   This is used to run the loader in its own thread.
// 2) std::mutex   This is used to protect the members of the class from race
//                 conditions.
// 3) std::condition_variable
//                 This is used to synchronise the two halves of the fifo
//                 used to pass data to stbi_load_* and prevents the threads
//                 from spinning uselesly while waiting.

// compile with g++ (or clang++) -std=c++11 -O3 main.cpp -lpthread

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "stb_image.h"



#include <common/PrecompiledHeader.h>

// A class to load JPEG and other images using stbi_image in the style
// of libjpeg/libjpeg-turbo. This facilitates loading on other threads
// of data that arrives very slowly over the internet and also allows
// decoding of JPEG data in parallel (by creating multiple instances).
class StbLoader {
public:
	// The constructor kicks off a load
	// Run stbi_load asyncronously.
	// instanceName is an abitrary string to track
	StbLoader(const char* instanceName = "unknown") {
		this->instanceName_ = instanceName;
		future = std::async(
			std::launch::async, [this] {
				printf("starting\n");
				stbi_io_callbacks cb = {
				  img_read, img_skip, img_eof
				};
				stbi_uc* result = stbi_load_from_callbacks(&cb, this, &w, &h, nullptr, 3);
				isError = result == nullptr;
				return result;
			}
		);
	}

	// Add some data to be decoded. Returns the number of bytes we are able to take
	// at this time.
	int addData(char* data, int size) {
		// Mutex prevents read() and addData() operating concurrently.
		std::unique_lock<std::mutex> lck(mtx);
		int bytes_added = 0;

		// We can only add a max of FIFO_SIZE bytes at a time.
		while (size && fifo_in < fifo_out + FIFO_SIZE) {
			fifo[fifo_in++ % FIFO_SIZE] = *data++;
			bytes_added++;
			size--;
		}
		if (DEBUG) printf("addData [%d %d] %d\n", fifo_in, fifo_out, bytes_added);

		lck.unlock();

		// Tell read() that data has arrived.
		cv.notify_one();
		return bytes_added;
	}

	// If we have an error in the stream, terminate the decoder.
	void setError() {
		isEof = true;
		cv.notify_one();
		auto res = future.get();
		if (res) stbi_image_free(res);
	}

	// Get the decode image from stbi_load.
	stbi_uc* getDecodedImage() {
		isEof = true;
		if (DEBUG) printf("getDecodedImage()\n");
		return future.get();
	}

	~StbLoader() {
	}

	// These will be valid after getDecodedImage() has returned.
	int width() const { return w; }
	int height() const { return h; }

	// If the decoder terminates early, this gets set.
	bool error() const { return isError; }
	const char* instanceName() const { return instanceName_; }

private:
	std::mutex mtx;
	std::condition_variable cv;
	std::future<stbi_uc*> future;
	int w = 0;
	int h = 0;
	static const int FIFO_SIZE = 32768;
	//static const bool DEBUG = false;
	std::array<char, FIFO_SIZE> fifo;
	int fifo_in = 0;
	int fifo_out = 0;
	bool isEof = false;
	bool isError = false;
	const char* instanceName_ = "";

	// stbi_load will call this to get bytes 
	int read(char* dat, int size) {
		std::unique_lock<std::mutex> lck(mtx);

		// Wait until addData adds at least one byte or we have an error.
		cv.wait(lck, [this] { return fifo_in > fifo_out || isEof; });

		int bytes_read = 0;
		while (size && fifo_out != fifo_in) {
			*dat++ = fifo[fifo_out++ % FIFO_SIZE];
			bytes_read++;
			size--;
		}

		if (DEBUG) printf("read [%d %d] %d\n", fifo_in, fifo_out, bytes_read);
		return bytes_read;
	}

	// stbi_load will call this to skip bytes. It will cause addData to
	// add bytes until fifo_in reaches fifo_out.
	void skip(int size) {
		std::unique_lock<std::mutex> lck(mtx);
		if (DEBUG) printf("skip [%d %d] %d\n", fifo_in, fifo_out, size);
		fifo_out += size;
	}

	// stbi_load will call this to check for eof.
	// stbi will only call this if the file has an error.
	int eof() {
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck, [this] { return fifo_in > fifo_out || isEof; });
		int result = isEof ? 1 : 0;
		bool isEofCopy = isEof;
		if (DEBUG) printf("eof [%d %d] %d\n", fifo_in, fifo_out, result);
		return result;
	}

	// Pass C callback on to the class.
	static int img_read(void* user, char* dat, int size) { return ((StbLoader*)user)->read(dat, size); }
	static void img_skip(void* user, int n) { return ((StbLoader*)user)->skip(n); }
	static int img_eof(void* user) { return ((StbLoader*)user)->eof(); }
};

//int main() {
//	auto start = std::chrono::system_clock::now();
//
//	// Put this in your class.
//	StbLoader imageLoader("big.jpg");
//
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//
//	auto f = std::ifstream("big.jpg", std::ios::binary);
//	while (!imageLoader.error()) {
//		char tmp[32768];
//
//		// Simulate transmission delay
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//		f.read(tmp, sizeof(tmp));
//
//		// Simulate data error
//		// tmp[256] = 0xff;
//
//		int bytesLeft = (int)f.gcount();
//		if (!bytesLeft) break;
//		int bytesWritten = 0;
//		while (bytesLeft && !imageLoader.error()) {
//			int wr = imageLoader.addData(tmp + bytesWritten, bytesLeft);
//			bytesWritten += wr;
//			bytesLeft -= wr;
//		}
//
//		// simulate stream error
//		// imageLoader.setError();
//		// break;
//	}
//
//	stbi_uc* data = imageLoader.getDecodedImage();
//	int w = imageLoader.width();
//	int h = imageLoader.height();
//	printf("%p %d %d\n", data, w, h);
//
//	auto end = std::chrono::system_clock::now();
//	printf("%f\n", std::chrono::duration<double>(end - start).count());
//}
