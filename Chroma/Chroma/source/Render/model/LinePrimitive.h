#ifndef _CHROMA_LINEPRIMITIVE_
#define _CHROMA_LINEPRIMITIVE_
#include "model/StaticMesh.h"

class LinePrimitive : public Chroma::StaticMesh
{
private:
	glm::vec3 start, end;
	float thickness;

	std::vector<float> lineData = {
		// positions	    normals	             uvs
	// top half
    -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f,   0.0f, 0.5f, 
    -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f,   0.0f, 1.0f, 
     0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f,   1.0f, 1.0f,       
	-0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f,   0.0f, 0.5f, 
	 0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f,   1.0f, 1.0f, 
     0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f,   1.0f, 0.5f,
    -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f,   0.0f, 0.5f,
	// bottom half
	0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 0.0f,   1.0f, 0.5f,
	0.5f, -0.5f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
   -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 0.0f,   0.0f, 0.5f,
	0.5f, -0.5f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
   -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 0.0f,   0.0f, 0.5f
	};

	void SetupMesh() override;

	// Default Shader
	std::string fragSrc = "resources/shaders/fragBasic.glsl";
	std::string vtxSrc = "resources/shaders/vertexLineBillboard.glsl";

public:

	/*  Functions  */
	void BindDrawVAO() override;

	void setStartPos(glm::vec3 newStart) { start = newStart; };
	void setEndPos(glm::vec3 newEnd) { end = newEnd; };

	LinePrimitive(glm::vec3 start_val, glm::vec3 end_val, float thicknessVal = 0.2f);
	~LinePrimitive();
};

#endif