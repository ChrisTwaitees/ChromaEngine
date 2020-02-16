#ifndef _CHROMA_TEXTURE_LOADER_H
#define _CHROMA_TEXTURE_LOADER_H

#include <texture/Texture.h>

namespace Chroma
{
	class TexureLoader
	{
	public:
		static std::vector<Texture> LoadTextures(std::string sourcePath);
		static std::vector<Texture> LoadTextures(std::string m_SourcePath, std::string dir);
		static Texture LoadTexture(std::string m_SourcePath, std::string dir);

	private:
		


	};
}



#endif