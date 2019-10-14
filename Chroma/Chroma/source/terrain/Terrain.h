#ifndef _CHROMA_TERRAIN_
#define _CHROMA_TERRAIN_
#include "../models/StaticMesh.h"
#include "../entity/ChromaEntity.h"


class Terrain : public StaticMesh
{
private:


	// Default Shader
	std::string fragShaderSource = "resources/shaders/fragLitReflect.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexShaderLighting.glsl";

	// Default Texture
	std::string defaultTextureSource = "resources/textures/terrain1.jpeg";

	// Transform Data
	glm::mat4 TerrainIdentityMatrix = glm::mat4(1);
	glm::mat4 TerrainTransformMatrix = glm::mat4(1);
	double TerrainHeight;

	// quad data
	std::vector<float> quadData = {
		// positions           // normals            // texture coords
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, 1.0f,   0.0f, 0.0f
	};

	// initialize
	void initialize();
	void setupQuad();
public:
	/*  Functions  */
	void BindDrawVAO() override;

	void setTerrainHeight(double newTerrainHeight);
	double getTerrainHeight() { return TerrainHeight; };
	
	glm::mat4 getTransformationMatrix() override;

	Terrain();
	Terrain(Camera* camera_val);
	~Terrain();
};

#endif