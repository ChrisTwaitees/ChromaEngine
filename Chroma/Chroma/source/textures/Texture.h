#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <glad/glad.h>
class Texture
{
private:
	int width, height, nrChannels;
public:
	GLuint ID;
	Texture(const char* sourcepath, int imageFormat);
	int get_width() const { return width; };
	int get_height() const { return height; };
	int get_nrChannels() const { return nrChannels; };
	void bind();
	~Texture();
};
#endif
