#ifndef _CHROMA_TERRAIN_
#define _CHROMA_TERRAIN_
#include "../models/Mesh.h"
#include "../entity/ChromaEntity.h"


class Terrain : public Mesh
{
private:
	// default shader
	std::string fragShaderSource = "resources/shaders/fragLitReflect.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexShaderLighting.glsl";
	Shader shader;

	// default texture
	std::string defaultTextureSource = "resources/textures/terrain1.jpeg";

	/* Transform Data */
	glm::mat4 modelMat = glm::mat4(1);

	// quad data
	std::vector<float> quadData = {
		// positions           // normals            // texture coords
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f
	};

	// initialize
	void initialize();
	void setupQuad();

	virtual void updateMaterialUniforms(Shader& shader) override;
public:
	virtual void Draw() override;

	Terrain();
	Terrain(Camera* camera_val);
	~Terrain();
};

#endif