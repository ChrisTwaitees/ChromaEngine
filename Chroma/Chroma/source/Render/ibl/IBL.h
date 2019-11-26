#ifndef _CHROMA_IBL_
#define _CHROMA_IBL_
// stl
#include <string>
// chroma
#include <component/ChromaMeshComponent.h>
#include <model/BoxPrimitive.h>
#include <texture/HDRTexture.h>
#include <shader/Shader.h>

class IBL
{
	HDRTexture m_HDRtexture{ "resources/textures/ibl/newportloft_ibl/newportloft.hdr" };
	ChromaMeshComponent* m_skyBox{ new BoxPrimitive() };
	std::string fragSrc{"resources/shaders/fragIBL.glsl"};
	std::string vtxSrc{"resources/shaders/vertexCubeMap.glsl"};
	Shader m_shader{fragSrc, vtxSrc};

	void initialize();
public:

	void Draw();

	template <typename UniformType>
	void setUniform(std::string uniformName, UniformType uniform)
	{
		m_shader.use();
		m_shader.setUniform(uniformName, uniform);
	}
	IBL();
	~IBL();
};

#endif