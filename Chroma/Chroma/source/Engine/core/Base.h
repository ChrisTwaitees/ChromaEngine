#ifndef CHROMA_BASE_ENTRY_H
#define CHROMA_BASE_ENTRY_H


// Platform detection using macros
	#ifdef _WIN32
		// Windows x64/x32
	#ifdef _WIN64
		#define CHROMA_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#endif


#define CHROMA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define CHROMA_BIND_EVENT_STATIC_FN(fn) std::bind(&fn, std::placeholders::_1)


#endif



