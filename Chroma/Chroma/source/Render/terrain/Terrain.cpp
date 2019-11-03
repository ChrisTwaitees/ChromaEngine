#include "Terrain.h"

void Terrain::initialize()
{
	// binding texture
	bindTexture(Texture(defaultDiffuseTexSource));
	Texture defaultNormal(defaultNormalTexSource);
	defaultNormal.type= Texture::NORMAL;
	bindTexture(defaultNormal);

	// assigning shader
	mShader = new Shader(fragShaderSource, vtxShaderSource);

	// Setting Default Transform
	TerrainIdentityMatrix = glm::scale(TerrainIdentityMatrix, glm::vec3(10.0f));
	TerrainIdentityMatrix = glm::rotate(TerrainIdentityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	TerrainTransformMatrix = TerrainIdentityMatrix;
}


void Terrain::setTerrainHeight(double newTerrainHeight)
{
	TerrainTransformMatrix = glm::translate(TerrainIdentityMatrix, glm::vec3(0.0f, newTerrainHeight, 0.0f));
}

glm::mat4 Terrain::getTransformationMatrix()
{
	return TerrainTransformMatrix;
}

Terrain::Terrain()
{
	initialize();
	setupQuad();
}


Terrain::~Terrain()
{
}
