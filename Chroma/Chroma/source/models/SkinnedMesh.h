#ifndef _SKINNEDMESH_H_
#define _SKINNEDMESH_H_
#include "Mesh.h"
#include <vector>
class SkinnedMesh : public Mesh
{
public:
	std::vector<Vert> get_vertices() { return vertices; };
	SkinnedMesh();
	~SkinnedMesh();
};

#endif