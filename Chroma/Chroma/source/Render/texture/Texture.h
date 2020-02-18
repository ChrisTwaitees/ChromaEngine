#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <common/CoreCommon.h>


struct TextureData
{
	unsigned int ID;
	int width, height, nrComponents;
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
	void Destroy();
	/* Constructors */
	Texture(unsigned int newID);
	Texture(std::string sourcePath);
	Texture(TextureData textData);
	Texture() {};
	~Texture();

protected:
	// paths
	std::string m_SourcePath;
	int width{ 0 }, height{ 0 }, nrComponents{ 0 };
	void InitTextureData(TextureData const& textData);
};
#endif
