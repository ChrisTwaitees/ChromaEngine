#ifndef _CHROMA_TERRAIN_
#define _CHROMA_TERRAIN_
#include "../models/StaticMesh.h"
#include "../entity/ChromaEntity.h"


class Terrain : public StaticMesh
{
private:
	// default shader
	std::string fragShaderSource = "resources/shaders/fragLitReflect.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexShaderLighting.glsl";
	Shader shader;

	// default texture
	std::string defaultTextureSource = "resources/textures/terrain1.jpeg";

	/* Transform Data */
	glm::mat4 TerrainTransformMatrix = glm::mat4(1);

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
	/*  Functions  */
	void BindDrawVAO() override;
	void Draw(Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix) override;
	void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix) override;


	Terrain();
	Terrain(Camera* camera_val);
	~Terrain();
};

#endif