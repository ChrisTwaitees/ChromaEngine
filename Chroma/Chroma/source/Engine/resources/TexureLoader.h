#ifndef _CHROMA_TEXTURE_LOADER_H
#define _CHROMA_TEXTURE_LOADER_H

#include <texture/Texture.h>

namespace Chroma
{
	class TexureLoader
	{
		static std::vector<Texture> LoadTextures(std::string sourcePath);
	};
}



#endif