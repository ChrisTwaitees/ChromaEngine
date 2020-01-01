#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <iostream>
#include <core/Core.h>

class Texture
{
protected:
	int width, height, nrComponents;
	virtual void generateTexture();
public:
	/*  Texture Data  */
	unsigned int ID;

	enum TYPE { ALBEDO, NORMAL, METROUGHAO, METALNESS, ROUGHNESS, AO, SHADOWMAP, HDR};
	TYPE type = TYPE::ALBEDO;

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
