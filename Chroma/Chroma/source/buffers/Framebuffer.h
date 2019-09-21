#ifndef _CHROMA_FRAMEBUFFER_
#define _CHROMA_FRAMEBUFFER_
#include <glad/glad.h>

class Framebuffer
{
private:
	unsigned int FBO;
	unsigned int texColorBuffer;
	void initialize();
public:
	unsigned int get_texColor() { return texColorBuffer; };
	Framebuffer();
	~Framebuffer();
};

#endif