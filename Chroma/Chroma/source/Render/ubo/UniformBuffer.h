#ifndef CHROMA_UNIFORM_BUFFER_H
#define CHROMA_UNIFORM_BUFFER_H
#include <common/PrecompiledHeader.h>
#include <shader/Shader.h>

namespace Chroma
{
	class UniformBuffer
	{
	public:

		UniformBuffer() { Init(); };
		~UniformBuffer() {};

		void BindUniformBlockIndex(const Shader& shaderToBind);
		virtual void OnUpdate() = 0;

	protected:

		void Init();
		void Bind() { glBindBuffer(GL_UNIFORM_BUFFER, m_UBO); };
		void UnBind() { glBindBuffer(GL_UNIFORM_BUFFER, 0); };

		virtual void Setup() = 0;

		std::string m_Name;
		unsigned int m_UBO;
		unsigned int m_Size{152};
		unsigned int m_BindingPointIndex;

		static unsigned int UBO_GlobalBindingPointIndex;

	};
}

#endif // CHROMA_UNIFORM_BUFFER_H