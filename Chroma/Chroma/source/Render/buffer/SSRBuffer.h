#ifndef CHROMA_SSR_BUFFER_H
#define CHROMA_SSR_BUFFER_H

#include "buffer/IFramebuffer.h"

namespace Chroma
{
	class SSRBuffer :	public IFramebuffer
	{
	public:
		unsigned int GetSSRReflectedUVTexture() { return m_SSRReflectedUVs; }

		void Draw() override;

		SSRBuffer();
		~SSRBuffer() {};

	protected:
		// Shader
		const char* fragSource{ "resources/shaders/fragSSRBuffer.glsl" };

		// Reflected UVs Texture
		unsigned int m_SSRReflectedUVs;

		// funcs
		void Init() override;
		void ResizeBuffers() override;

		// shader
		Shader* m_ScreenShader{ new Shader(fragSource, vtxSource) };

	};
}

#endif // CHROMA_SSR_BUFFER_H