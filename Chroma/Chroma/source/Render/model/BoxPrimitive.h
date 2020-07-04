#ifndef _BOX_PRIMITIVE_H_
#define _BOX_PRIMITIVE_H_
#include "model/StaticMesh.h"


class BoxPrimitive : public Chroma::StaticMesh
{
private:
	std::vector<ChromaVertex> m_vertices;

	std::vector<float> boxData = {
		     // Back face		    normals	             uvs
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // top-right
		// Right face		                        
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // bottom-left     
		// Bottom face		                        
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // top-right
		// Top face			                        
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f  // bottom-left        
	};
	void setupBox();

public:
	/*  Functions  */
	void BindDrawVAO() override;
	std::vector<ChromaVertex> GetVertices() override;

	/*  Constructors  */
	BoxPrimitive() { setupBox();};
	~BoxPrimitive();
};

#endif