#ifndef _CHROMA_TERRAIN_
#define _CHROMA_TERRAIN_
#include "../models/PlanePrimitive.h"
#include "../entity/ChromaEntity.h"


class Terrain : public PlanePrimitive
{
private:


	// Default Shader
	std::string fragShaderSource = "resources/shaders/fragLitReflect.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexShaderLighting.glsl";

	// Default Texture
	std::string defaultDiffuseTexSource = "resources/textures/brickwall.jpg";
	std::string defaultNormalTexSource = "resources/textures/brickwall_normal.jpg";

	// Transform Data
	glm::mat4 TerrainIdentityMatrix = glm::mat4(1);
	glm::mat4 TerrainTransformMatrix = glm::mat4(1);
	double TerrainHeight;

	// initialize
	void initialize();

public:
	/*  Functions  */
	void setTerrainHeight(double newTerrainHeight);
	double getTerrainHeight() { return TerrainHeight; };
	
	glm::mat4 getTransformationMatrix() override;

	Terrain();
	Terrain(Camera* camera_val);
	~Terrain();
};

#endif