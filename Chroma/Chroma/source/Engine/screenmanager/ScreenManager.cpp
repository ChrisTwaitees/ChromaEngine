#include "ScreenManager.h"

namespace Chroma
{
	GLFWwindow* ScreenManager::m_Window;

	void ScreenManager::Init()
	{
		// Configure Window
		if (!ConfigureWindow())
		{
			CHROMA_FATAL("Failed to Initialize Chroma Window");
		}
	}

	bool ScreenManager::ConfigureWindow()
	{
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);

		// glfw window creation
		// --------------------
		m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CHROMA", NULL, NULL);
		if (m_Window == NULL)
		{
			CHROMA_FATAL("Failed to Initialize GLFW window");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_Window);

		return true;
	}



	// CHROMA SCREEN MANAGER LOOP
	// --------------------
	void ScreenManager::EndLoop()
	{
		//draw GUI
		drawGUI();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void ScreenManager::Close()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}


	// Convenience Methods
	// --------------------

	void ScreenManager::ToggleSkybox()
	{
		useSkybox = useSkybox ? false : true;
	}

	void ScreenManager::ToggleBloom()
	{
		useBloom = useBloom ? false : true;
	}

	void ScreenManager::ToggleDebug()
	{
		drawDebug = drawDebug ? false : true;
	}

	void ScreenManager::TogglePhysicsDebug()
	{
		drawPhysicsDebug = drawPhysicsDebug ? false : true;
	}

	void ScreenManager::ToggleAnimationDebug()
	{
		DebugAnim = DebugAnim ? false : true;
		drawSkeletonsDebug = DebugAnim ? false : true;
	}

	void ScreenManager::ToggleGraphicsDebug()
	{
		drawGraphicsDebug = drawGraphicsDebug ? false : true;
	}


	void ScreenManager::UpdateDimensions(int width, int height)
	{
		// make sure the viewport matches the new window dimensions;  
		glViewport(0, 0, width, height);
	}


	void ScreenManager::processTime()
	{
		float GameTime = glfwGetTime();
		m_Delta = GameTime - lastFrame;
		lastFrame = GameTime;
	}


	void ScreenManager::drawGUI()
	{
		// Time
		ImGui::Text("Deltatime %.3f ms/frame (%.1f FPS)", Chroma::Time::GetDeltaTime(), Chroma::Time::GetFPS);
		ImGui::SliderFloat("Time Multiply", &timeSpeed, 0.0, 3.0);
		Chroma::Time::SetSpeed(timeSpeed);

		//// exposure
		//ImGui::SliderFloat("Exposure", &exposure, 0.0f, 2.0f);
		//ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f);

		// bloom
		if (ImGui::Button("Toggle Bloom"))
			ToggleBloom();

		//// debug draw
		if (ImGui::Button("Toggle Physics Debug"))
			TogglePhysicsDebug();

		if (ImGui::Button("Toggle Graphics Debug"))
			ToggleGraphicsDebug();

		// Graphics Debug Dropdowns 
		ImGui::Combo("Graphics Debug", &graphicsDebugSelected, GraphicsDebugs, IM_ARRAYSIZE(GraphicsDebugs));

		// camera dropdowns
		ImGui::Combo("Cameras", &cameraSelected, cameras, IM_ARRAYSIZE(cameras));

		// Animation 
		if (ImGui::Button("Toggle Animation Debug"))
			ToggleAnimationDebug();
		char test;
		ImGui::InputText("Animation Clip Name : ", AnimClipName, IM_ARRAYSIZE(AnimClipName));
		ImGui::Text("Current Animation : %s", AnimClipName);
		ImGui::SliderFloat("Animation Clip Position", &DebugAnimClipPos, 0.0, 1.0);
		// Display Selected Entity
		ImGui::Text("Selected Entity : %s", SelectedEntity.c_str());
		gui.End();
	}

	unsigned int ScreenManager::getScreenWidth()
	{
		glfwGetWindowSize(window, &width, &height);
		return width;
	}

	unsigned int ScreenManager::getScreenHeight()
	{
		glfwGetWindowSize(window, &width, &height);
		return height;
	}

	std::pair<int, int> ScreenManager::GetWidthHeight()
	{
		return std::make_pair(width, height);
	}



	// glfw callbacks
	// --------------------
	void ScreenManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		UpdateDimensions(width, height);
	}




	// structors
	// --------------------
	ScreenManager::ScreenManager()
	{
		Initialize();
	}

	ScreenManager::~ScreenManager()
	{
	}

}
