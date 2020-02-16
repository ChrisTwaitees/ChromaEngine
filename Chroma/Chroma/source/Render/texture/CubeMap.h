#ifndef _CHROMA_CUBEMAP_
#define _CHROMA_CUBEMAP_

#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <core/Core.h>


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
	unsigned int m_textureID{ 0 };
	void generateCubeMap();
	void Bind();

	CubeMap(std::string cubeMapsDirectory);
	CubeMap() {};
	~CubeMap();
};

#endif