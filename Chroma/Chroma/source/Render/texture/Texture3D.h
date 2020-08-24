#ifndef CHROMA_TEXTURE3D_H
#define CHROMA_TEXTURE3D_H


#include <texture/Texture.h>

namespace Chroma
{
	class Texture3D : public Texture
	{
	public:
		Texture3D(const int _width, const int _height, const int _depth, const bool generateMipmaps = true);
		~Texture3D();

		virtual void Bind() override;
		void Clear(const glm::vec4& clearColor);

	private:
		void Init(const bool generateMipMaps);
	private:
		float* m_TextureBuffer;
	};
}


#endif //CHROMA_TEXTURE3D_H