#include "ChromaScreenManager.h"

bool ChromaScreenManager::initialize()
{
	// Configure Window
	if (!configureWindow())
	{
		std::cout << "Failed to Initialize Chroma Window: " << std::endl;
		return false;
	}

	// Configure Renderer
	if (!configureRenderer())
	{
		std::cout << "Failed to Initialize Chroma Renderer: " << std::endl;
		glfwTerminate();
		return false;
	}

	// Attach GUI
	if (!configureGui())
	{
		std::cout << "Failed to Initialize Chroma GUI: " << std::endl;
		glfwTerminate();
		return false;
	}

	// configure Scene
	if (!configureScene())
	{
		std::cout << "Failed to Initialize Chroma GUI: " << std::endl;
		glfwTerminate();
		return false;
	}
}

bool ChromaScreenManager::configureWindow()
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
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CHROMA", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);


	// capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

bool ChromaScreenManager::configureGui()
{
	gui.attachWindow(*window);
	return true;
}

bool ChromaScreenManager::configureRenderer()
{
	// OpenGL 3
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Enabling Render Features
	// ---------------------------------------
	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// Enable Blending
	glEnable(GL_BLEND);
	// Enable Face Culling
	glEnable(GL_CULL_FACE);
	// Enabling MSAA
	glEnable(GL_MULTISAMPLE);
	// Setting Clear Color
	glClearColor(SCREEN_DEFAULT_COLOR.x, SCREEN_DEFAULT_COLOR.y, SCREEN_DEFAULT_COLOR.z, 1.0f);
	return true;
}

bool ChromaScreenManager::configureScene()
{
	//camera = new Camera;
	return true;
}


// CHROMA SCREEN MANAGER LOOP
// --------------------

void ChromaScreenManager::Start()
{
	// process
	processTime();
	// start gui
	gui.Start();
}

void ChromaScreenManager::End()
{
	
	//draw GUI
	drawGUI();
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ChromaScreenManager::Close()
{
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}


// Convenience Methods
// --------------------

void ChromaScreenManager::ToggleSkybox()
{
	useSkybox = useSkybox ? false : true;
}

void ChromaScreenManager::ToggleBloom()
{
	useBloom = useBloom ? false : true;
}


void ChromaScreenManager::updateRendererViewportDimensions(int width, int height)
{
	// make sure the viewport matches the new window dimensions;  
	glViewport(0, 0, width, height);
}

void ChromaScreenManager::updateCamera()
{
	//camera->processKeyboardInput(*window, delta);
	//camera->processMouseInput(MOUSE_XOFFSET, MOUSE_YOFFSET);
}

void ChromaScreenManager::processTime()
{
	float GameTime = glfwGetTime();
	delta = GameTime - lastFrame;
	lastFrame = GameTime;
}


void ChromaScreenManager::drawGUI()
{
	// heading
	ImGui::Text("Chroma Debugging Text");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	// toggle skybox
	if (ImGui::Button("Toggle SkyBox"))
		ToggleSkybox();

	// exposure
	ImGui::SliderFloat("Exposure", &exposure, 0.0f, 2.0f);
	ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f);

	if (ImGui::Button("Toggle Bloom"))
		ToggleBloom();

	gui.End();
}



// glfw callbacks
// --------------------
void ChromaScreenManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	updateRendererViewportDimensions(width, height);
}




// structors
// --------------------
ChromaScreenManager::ChromaScreenManager()
{
	initialize();
}

ChromaScreenManager::~ChromaScreenManager()
{
}
