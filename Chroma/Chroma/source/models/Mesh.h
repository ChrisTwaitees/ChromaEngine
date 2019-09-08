#ifndef _MESH_H_
#define _MESH_H_
// stl
#include <vector>
#include <string>
#include "../shaders/Shader.h"
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// glad 
#include <glad/glad.h>

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
struct MeshTexture {
	unsigned int id;
	enum TYPE {DIFFUSE = 0, SPECULAR = 1};
	int type;
	std::string path;
};

class Mesh
{
private:
	/*  Render Data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions  */
	virtual void setupMesh();
public:
	/*  Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;
	/*  Functions  */
	void Draw(Shader shader);

	Mesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<MeshTexture> textures_val);
	~Mesh();
};

#endif