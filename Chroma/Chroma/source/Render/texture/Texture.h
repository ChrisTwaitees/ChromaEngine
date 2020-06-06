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
	Texture(unsigned int newID);
	Texture(std::string sourcePath);
	Texture(TextureData const& textData);

	bool operator <(const Texture& rhs) const;

	Texture() {};
	~Texture();


	/*  Texture Data  */
	unsigned int ID {0} ;

	Chroma::Type::Texture m_Type { Chroma::Type::Texture::kAlbedo };

	std::string GetSourcePath() { return m_SourcePath; }

	void Bind();
	void Destroy();

	static void ClearTexureMemory(unsigned int const& TextureID);



protected:
	// paths
	std::string m_SourcePath;
	int width{ 0 }, height{ 0 }, nrComponents{ 0 };
	void InitTextureData(TextureData const& textData);
};
#endif
