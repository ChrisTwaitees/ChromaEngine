#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//common
#include <common/PrecompiledHeader.h>
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

	enum TYPE { ALBEDO, NORMAL, METROUGHAO, METALNESS, ROUGHNESS, AO, HDR, TRANSLUCENCY};
	TYPE m_Type {ALBEDO};

	std::string& GetSourcePath() { return m_SourcePath; }

	void Bind();
	void Destroy();

	static void ClearTexureMemory(unsigned int const& TextureID);
	/* Constructors */
	Texture(unsigned int newID);
	Texture(std::string sourcePath);
	Texture(TextureData const& textData);
	bool operator <(const Texture& rhs) const;
	Texture() {};
	~Texture();

protected:
	// paths
	std::string m_SourcePath;
	int width{ 0 }, height{ 0 }, nrComponents{ 0 };
	void InitTextureData(TextureData const& textData);
};
#endif
