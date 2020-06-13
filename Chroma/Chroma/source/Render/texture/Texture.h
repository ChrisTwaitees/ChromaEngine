#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//common
#include <common/PrecompiledHeader.h>
#include <common/CoreCommon.h>

struct TextureData
{
	// RenderID
	unsigned int ID{ 0 };

	// Dimensions
	int width{ 0 }, height{ 0 };
	int nrComponents{ 0 };

	// Source
	std::string sourcePath{ "" };

	// Data
	unsigned char* imageData{ nullptr };

	// Status
	bool isInitialized{ false };
	bool isLoaded{ false };

	// Constructors
	TextureData() {};
	//TextureData(const TextureData& rhs); // copy
	//TextureData& operator=(const TextureData& rhs); // copy
	//TextureData(TextureData&& rhs); // copy
	//~TextureData(); // delete

	// Type
	Chroma::Type::Texture type{Chroma::Type::Texture::kAlbedo};
};


class Texture
{
public:

	Texture(unsigned int newID);
	Texture(std::string sourcePath);
	Texture(std::string sourcePath, Chroma::Type::Texture type);
	Texture(TextureData const& textData);
	bool operator <(const Texture& rhs) const;

	// Constructors
	//Texture(const Texture& rhs); // copy
	//Texture& operator=(const Texture& rhs); // copy
	//Texture(Texture&& rhs); // copy
	Texture() {};
	~Texture();

	// load
	virtual void LoadFromFile(const std::string& sourcePath);

	// texture data
	TextureData& GetTextureData() { return m_TextureData; }

	// render id
	unsigned int GetID();
	virtual void SetID(const unsigned int& newID) { m_TextureData.ID = newID; m_TextureData.isLoaded = true; m_TextureData.isInitialized = true; };
	virtual void SetID(Texture& refTexture);

	// type
	Chroma::Type::Texture GetType() { return m_TextureData.type; }
	void SetType(const Chroma::Type::Texture& newType) { m_TextureData.type = newType; }

	std::string GetSourcePath() { return m_TextureData.sourcePath; }

	// draw
	virtual void Bind();

	// clear
	static void ClearTexureMemory(unsigned int const& TextureID);
	void Destroy();

protected:

	// TextureData
	TextureData m_TextureData;
	std::shared_ptr<TextureData> m_SharedTextureData{ std::make_shared<TextureData>() };

	// Init
	virtual void InitializeTexture();
};
#endif
