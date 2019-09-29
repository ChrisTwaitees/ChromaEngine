#ifndef _MESH_H_
#define _MESH_H_
// stl
#include <vector>
#include <string>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// glad 
#include <glad/glad.h>
// chroma
#include "../shaders/Shader.h"
#include "../textures/Texture.h"
#include "../cameras/Camera.h"
#include "../lights/Light.h"

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{
protected:
	/*  Render Data  */
	unsigned int VAO, VBO, EBO;
	/* Transform Data */
	glm::mat4 modelMat;
	/*  Functions  */
	virtual void setupMesh();
	virtual void updateLightingUniforms(Shader& shader, const std::vector<Light>& lights, Camera& camera);
	virtual void updateTransformUniforms(Shader& shader,const Camera& camera);
	virtual void updateMaterialUniforms(Shader& shader);
	
public:
	/*  Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/*  Functions  */
	virtual void Draw(Shader &shader);
	virtual void Draw();
	void bindTextures(std::vector<Texture> textures_val);
	void bindTexture(Texture texture_val);


	Mesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val);
	Mesh();
	~Mesh();
};

#endif