#ifndef _CHROMA_VERTEX_
#define _CHROMA_VERTEX_

#include <glm/glm.hpp>

struct ChromaVertex
{

	glm::vec3 m_position{ 0.0 };
	glm::vec3 m_normal{ 0.0 };
	glm::vec3 m_tangent{ 0.0 };
	glm::vec3 m_bitangent{ 0.0 };
	glm::vec2 m_texCoords{ 0.0 };

	glm::vec3 GetPosition() { return m_position; };
	glm::vec3 getNormal() { return m_normal; };
	glm::vec3 getTangent() { return m_tangent; };
	glm::vec3 getBitangent() { return m_bitangent; };
	glm::vec2 getTexCoords() { return m_texCoords; };

	void setPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void setNormal(glm::vec3 newNormal) { m_normal = newNormal; };
	void setTangent(glm::vec3 newTangent) { m_tangent = newTangent; };
	void setBitangent(glm::vec3 newBitangent) { m_bitangent = newBitangent; };
	void setTexCoords(glm::vec2 newTexCoords) { m_texCoords = newTexCoords; };
};


#endif