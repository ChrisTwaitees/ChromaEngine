#ifndef CHROMA_UNIFORM_BUFFER_H
#define CHROMA_UNIFORM_BUFFER_H
#include <common/PrecompiledHeader.h>
#include <shader/Shader.h>

class UniformBuffer
{
public:

	UniformBuffer(const std::string& name) : m_Name(name) { Initialize(); };
	~UniformBuffer() {};

	void BindUniformBlockIndex(const Shader& shaderToBind);

	void AppendVec3(const glm::vec3& newVec3);
	void Update();

private:

	void Initialize();
	void Bind() { glBindBuffer(GL_UNIFORM_BUFFER, m_UBO); };
	void UnBind() { glBindBuffer(GL_UNIFORM_BUFFER, 0); };
	
	std::string m_Name;
	unsigned int m_UBO;
	unsigned int m_Size{152};
	unsigned int m_BindingPointIndex;

	static unsigned int UBO_GlobalBindingPointIndex;

};

#endif // CHROMA_UNIFORM_BUFFER_H