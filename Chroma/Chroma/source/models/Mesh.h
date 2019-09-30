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
#include "../entity/Entity.h"

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh : public Entity
{
protected:
	/*  Render Data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions  */
	virtual void setupMesh();
	virtual void updateTransformUniforms(Shader& shader, Camera& camera, glm::mat4& modelMatrix);
	virtual void updateMaterialUniforms(Shader& shader);
	virtual void updateLightingUniforms(Shader& shader, const std::vector<Light>& lights, Camera& camera);
	virtual void updateTextureUniforms(Shader& shader);
	
public:
	/*  Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/*  Functions  */
	virtual void Draw(Shader &shader) override;
	virtual void Draw() override;

	virtual void bindTextures(std::vector<Texture> textures_val) override;
	virtual void bindTexture(Texture texture_val) override;


	Mesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val);
	Mesh();
	~Mesh();
};

#endif