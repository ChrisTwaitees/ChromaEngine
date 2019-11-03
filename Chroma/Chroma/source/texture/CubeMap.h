#ifndef _CHROMA_CUBEMAP_
#define _CHROMA_CUBEMAP_
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include <vector>
class CubeMap 
{
private: 
	std::string dir;
	std::vector<std::string> faces{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

public:
	unsigned int ShaderID{ 0 };
	void generateCubeMap();
	void bind();

	CubeMap(std::string cubeMapsDirectory);
	CubeMap() {};
	~CubeMap();
};

#endif