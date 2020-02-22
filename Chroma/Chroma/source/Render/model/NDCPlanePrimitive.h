#ifndef _CHROMA_NDCPLANE_
#define _CHROMA_NDCPLANE_

//common
#include <common/PrecompiledHeader.h>

#include <model/StaticMesh.h>


class NDCPlanePrimitive
{
public:
	// draw
	void BindDrawVAO();

	NDCPlanePrimitive();
	~NDCPlanePrimitive();

protected:
	// quad data
	float quadData[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// vertex array / vertex buffer
	unsigned int VAO, VBO;

	void SetupQuad();
};

#endif
