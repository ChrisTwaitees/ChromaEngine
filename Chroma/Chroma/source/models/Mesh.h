#ifndef _MESH_H_
#define _MESH_H_
#include <vector>
#include <string>
struct Vert {
	int position;
	int normal;
};
struct Material {
	const char* texturePath;
	int GL_Texture_ID;
	int uniforms;
};
class Mesh
{
protected:
	std::vector<Vert> vertices;
	std::vector<Material> materials;
public:
	virtual std::vector<Vert> get_vertices() = 0;
	Mesh();
	~Mesh();
};

#endif