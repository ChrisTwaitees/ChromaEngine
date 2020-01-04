#ifndef _CHROMA_TERRAIN_
#define _CHROMA_TERRAIN_
#include "model/PlanePrimitive.h"

class Terrain : public PlanePrimitive
{
private:
	// Transform Data
	glm::mat4 TerrainIdentityMatrix = glm::mat4(1);
	glm::mat4 TerrainTransformMatrix = glm::mat4(1);
	double TerrainHeight;

	// initialize
	void Initialize();

public:
	/*  Functions  */
	void setTerrainHeight(double newTerrainHeight);
	double getTerrainHeight() { return TerrainHeight; };
	
	glm::mat4 GetTransform() override;

	Terrain();
	Terrain(Camera* camera_val);
	~Terrain();
};

#endif