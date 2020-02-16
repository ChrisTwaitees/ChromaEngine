#ifndef _CHROMA_TEXTURE_LOADER_H
#define _CHROMA_TEXTURE_LOADER_H

#include <common/CoreCommon.h>
#include <texture/Texture.h>
#include <texture/HDRTexture.h>
#include <texture/CubeMap.h>

namespace Chroma
{
	class TexureLoader
	{
	public:
		static Texture Load2DTexture(std::string sourcePath);
		static Texture Load2DTexture(std::string m_SourcePath, std::string dir);
		static TextureData Load2DTextureData(std::string sourcePath);

		static HDRTexture LoadHDRTexture(std::string sourcePath);
		static HDRTexture LoadHDRTexture(std::string m_SourcePath, std::string dir);
		static TextureData LoadHDRTextureData(std::string sourcePath);

		static CubeMap LoadCubeMapTexture(std::string directory);
		static TextureData LoadCubeMapTextureData(std::string sourcePath);

	private:
		static void Generate2DTexture(TextureData& textData);
		static void GenerateHDRTexture(TextureData& textData);
		static void GenerateCubeMapTexture(TextureData& textData);
	};
}



#endif