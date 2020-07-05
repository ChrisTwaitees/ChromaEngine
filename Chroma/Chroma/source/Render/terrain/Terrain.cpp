#include "Terrain.h"
#include <entity/IEntity.h>

namespace Chroma
{

	void Terrain::Init()
	{
		// Setting Default Transform
		TerrainIdentityMatrix = glm::scale(TerrainIdentityMatrix, glm::vec3(10.0f));
		TerrainIdentityMatrix = glm::rotate(TerrainIdentityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		TerrainTransformMatrix = TerrainIdentityMatrix;
	}


	void Terrain::setTerrainHeight(double newTerrainHeight)
	{
		TerrainTransformMatrix = glm::translate(TerrainIdentityMatrix, glm::vec3(0.0f, newTerrainHeight, 0.0f));
	}

	glm::mat4 Terrain::GetWorldTransform()
	{
		return GetParentEntity()->GetTransform() * TerrainTransformMatrix;
	}

	glm::mat4 Terrain::GetTransform()
	{
		return TerrainTransformMatrix;
	}

	Terrain::Terrain()
	{
		Init();
		SetupQuad();
	}


	Terrain::~Terrain()
	{
	}
}
