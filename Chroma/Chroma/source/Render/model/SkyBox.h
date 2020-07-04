#ifndef _CHROMA_SKYBOX_
#define _CHROMA_SKYBOX_
#include "model/StaticMesh.h"
#include "texture/CubeMap.h"

namespace Chroma
{
	enum ColorSpace { LINEAR, HDR };

	class SkyBox
	{
	public:
		// getters and setters
		unsigned int getCubeMapID() { return m_cubeMap.GetID(); };

		void SetColorSpace(ColorSpace newColorSpace) { m_colorSpace = newColorSpace; };
		ColorSpace getColorSpace() { return m_colorSpace; };

		void SetCubeMapID(const unsigned int& newID) { m_cubeMap.SetID(newID); }

		void Draw();
		SkyBox();
		~SkyBox();

	private:

		void Initialize();
		ColorSpace m_colorSpace = LINEAR;
		unsigned int VAO, VBO;

		CubeMap m_cubeMap;

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
}

#endif