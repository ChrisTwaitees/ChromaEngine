#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <iostream>
#include "../memory/ChromaUID.h"

class Texture
{
protected:
	void generateTexture();
public:
	/*  Texture Data  */
	unsigned int ShaderID;
	enum TYPE { DIFFUSE = 0, SPECULAR = 1, SHADOWMAP = 2, NORMAL=3 };
	TYPE type = TYPE::DIFFUSE;
	// paths
	std::string filename;
	std::string directory;
	std::string path;

	/*   Functions   */
	int loadFromFile(std::string filename ="", std::string dir ="");
	int loadFromFile(std::string sourcepath);

	void bind();
	/* Constructors */
	Texture(unsigned int newID);
	Texture(std::string sourcePath);
	Texture();
	~Texture();
};
#endif
