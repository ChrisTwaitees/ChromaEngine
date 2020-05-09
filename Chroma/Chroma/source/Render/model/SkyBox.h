#ifndef _CHROMA_SKYBOX_
#define _CHROMA_SKYBOX_
#include "model/StaticMesh.h"
#include "texture/CubeMap.h"


enum ColorSpace { LINEAR, HDR };

class SkyBox
{

public:
	// getters and setters
	void setCubeMapID(int const& newCubeMapID) { m_cubeMapID = newCubeMapID; };
	unsigned int getCubeMapID() { return m_cubeMap.ID; };

	void setColorSpace(ColorSpace newColorSpace) { m_colorSpace = newColorSpace; };
	ColorSpace getColorSpace() { return m_colorSpace; };

	void Draw();
	SkyBox();
	~SkyBox();

private:

	void Initialize();
	ColorSpace m_colorSpace = LINEAR;
	unsigned int VAO, VBO;

	CubeMap m_cubeMap;
	int m_cubeMapID;
	Shader m_linearShader{ "resources/shaders/fragSkyBox.glsl" , "resources/shaders/vertexSkyBox.glsl" };
	Shader m_HDRShader{ "resources/shaders/fragHDRSkyBox.glsl" , "resources/shaders/vertexSkyBox.glsl" };
	std::string defaultImageDir{ "resources/textures/skybox/blueskywater" };

	std::vector<float> skyBoxVerts = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};



};

#endif