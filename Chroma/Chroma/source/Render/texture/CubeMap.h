#ifndef _CHROMA_CUBEMAP_
#define _CHROMA_CUBEMAP_

#include <texture/Texture.h>

namespace Chroma
{
	class CubeMap : public Texture
	{
	public:
		CubeMap(std::string cubeMapsDirectory);
		CubeMap(std::shared_ptr<TextureData> textData);
		CubeMap() {};
		~CubeMap();

	};
}
#endif