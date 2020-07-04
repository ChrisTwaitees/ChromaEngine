#ifndef CHROMA_OPENGL_GRAPHICSCONTEXTS_H
#define CHROMA_OPENGL_GRAPHICSCONTEXTS_H

#include <context/GraphicsContext.h>

struct GLFWwindow;

namespace Chroma
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}

#endif //CHROMA_OPENGL_GRAPHICSCONTEXTS_H