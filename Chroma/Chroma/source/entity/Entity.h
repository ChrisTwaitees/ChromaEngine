#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_
#include "../models/Model.h"
#include "../textures/Texture.h"
#include "../shaders/Shader.h"
#include "../cameras/Camera.h"
#include <vector>

class Entity
{
private:
	// components
	Shader shader;
	Model model;
	Mesh mesh;

	// transforms
	glm::mat4 modelMat;

public:
	// load
	void loadModel(std::string filePath);
	void loadModel(const Model& newModel);

	void loadTexture(std::string filePath);
	void loadTexture(const Texture newTexture);

	void loadMesh(const Mesh& newMesh);
	void loadMesh(std::string filePath);

	// assignments
	void assignShader(const Shader& newShader);

	// update
	void updateMaterialUniforms(Camera& camera);

	Entity();
	~Entity();
};

#endif