#ifndef _CHROMA_DEBUG_RENDERER_
#define _CHROMA_DEBUG_RENDERER_

// stl
#include <iostream>
// thirdparty
#include <glm/glm.hpp>

class ChromaDebugRenderer
{
public:
	void drawLine(glm::vec3 from, glm::vec3 to);

	void Render();
};

#endif