#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <iostream>
class Texture
{
protected:
	void generateTexture();
public:
	/*  Texture Data  */
	unsigned int id;
	enum TYPE { DIFFUSE = 0, SPECULAR = 1 };
	int type;
	// paths
	std::string filename;
	std::string directory;
	std::string path;

	/*   Functions   */
	int loadFromFile(std::string filename ="", std::string dir ="");
	int loadFromFile(std::string sourcepath);


	void bind();
	/* Constructors */
	Texture(std::string sourcePath);
	Texture();
	~Texture();
};
#endif
