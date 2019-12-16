#ifndef _CHROMA_SKINNED_VERTEX_
#define _CHROMA_SKINNED_VERTEX_

#include <glm/glm.hpp>
#include <vector>
#include <ChromaConfig.h>

struct ChromaSkinnedVertex
{
	// vert data
	glm::vec3 m_position{ 0.0 };
	glm::vec3 m_normal{ 0.0 };
	glm::vec3 m_tangent{ 0.0 };
	glm::vec3 m_bitangent{ 0.0 };
	glm::vec2 m_texCoords{ 0.0 };
	glm::ivec4 m_jointIDs { 0 };
	glm::vec4 m_jointWeights{ 0.0 };

	// getters
	glm::vec3 GetPosition() const { return m_position; };
	glm::vec3 getNormal() const { return m_normal; };
	glm::vec3 getTangent() const { return m_tangent; };
	glm::vec3 getBitangent() const { return m_bitangent; };
	glm::vec2 getTexCoords() const { return m_texCoords; };
	// setters
	void SetPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void SetNormal(glm::vec3 newNormal) { m_normal = newNormal; };
	void SetTangent(glm::vec3 newTangent) { m_tangent = newTangent; };
	void SetBitangent(glm::vec3 newBitangent) { m_bitangent = newBitangent; };
	void SetTexCoords(glm::vec2 newTexCoords) { m_texCoords = newTexCoords; };
};




#endif