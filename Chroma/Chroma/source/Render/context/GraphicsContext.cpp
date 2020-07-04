#include "GraphicsContext.h"

#include <render/Render.h>
#include <platform/OpenGL/OpenGLContext.h>

namespace Chroma
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* windowHandle)
	{
		switch (Render::GetAPI())
		{
			case(Render::API::OpenGL) : 
				{
					return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(windowHandle));
					break;
				}
			default:
				{
					CHROMA_ASSERT(false, "RenderAPI has no corresponding render context.");
					break;
				}
		}

		CHROMA_ASSERT(false, "RenderAPI has no corresponding render context.");
		return nullptr;
	}
}