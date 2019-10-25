#ifndef _CHROMA_LINEPRIMITIVE_
#define _CHROMA_LINEPRIMITIVE_
#include "Mesh.h"

class LinePrimitive : public Mesh
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