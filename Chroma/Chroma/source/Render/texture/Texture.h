#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <iostream>
#include <core/Core.h>

struct TextureData
{
	unsigned int ID, width, height, nrComponents;
	std::string sourcePath;

};


class Texture
{

public:
	/*  Texture Data  */
	unsigned int ID {0} ;

	enum TYPE { ALBEDO, NORMAL, METROUGHAO, METALNESS, ROUGHNESS, AO, SHADOWMAP, HDR, TRANSLUCENCY};
	TYPE type {TYPE::ALBEDO};

	std::string& GetSourcePath() { return m_SourcePath; }

	void Bind();
	/* Constructors */
	Texture(unsigned int newID);
	Texture(std::string sourcePath);
	Texture();
	~Texture();

protected:
	// paths
	/*   Functions   */
	int loadFromFile(std::string m_SourcePath = "", std::string dir = "");
	int loadFromFile(std::string sourcepath);
	std::string m_SourcePath;
	int width, height, nrComponents;
	virtual void generateTexture();
};
#endif
