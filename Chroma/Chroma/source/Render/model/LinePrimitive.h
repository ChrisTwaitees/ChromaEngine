#ifndef _CHROMA_LINEPRIMITIVE_
#define _CHROMA_LINEPRIMITIVE_
#include "model/StaticMesh.h"

class LinePrimitive : public StaticMesh
{
private:
	int VAO;
public:
	glm::vec3 start{ 0.0f, 0.0f, 0.0f };
	glm::vec3 end{ 0.0f, 0.0f, 0.0f };

	void Draw(Shader& shader) override;
	LinePrimitive(glm::vec3 start_val, glm::vec3 end_val) : start{ start_val }, end{ end_val }{};
	~LinePrimitive();
};

#endif