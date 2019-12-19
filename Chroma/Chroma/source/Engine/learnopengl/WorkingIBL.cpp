#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <texture/stb_image.h>

#include <glm/glm.hpp>
#include <texture/HDRTexture.h>
#include <ibl/IBL.h>

#include <shader/Shader.h>
#include <buffer/Framebuffer.h>
#include <model/BoxPrimitive.h>
#include <learnopengl/TESTCAMERA.h>
#include <model/NDCPlanePrimitive.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000.0;
const unsigned int SCR_HEIGHT = 1000.0;

// camera
TESTCAMERA camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 1000.0 / 2.0;
float lastY = 1000.0 / 2.0;
bool m_FirstMouse = true;

// timing
float m_DeltaTime = 0.0f;
float lastFrame = 0.0f;

int mains()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile shaders
	// -------------------------
	Shader backgroundShader("resources/shaders/fragHDRSkyBox.glsl", "resources/shaders/vertexSkyBox.glsl");
	Shader frambufferShader("resources/shaders/frameBufferFrag.glsl", "resources/shaders/frameBufferVertex.glsl");


	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	backgroundShader.use();
	backgroundShader.SetInt("environmentMap", 0);
	backgroundShader.SetMat4("projection", projection);

	BoxPrimitive cube;
	IBL testIBL;
	//NDCPlanePrimitive testPlane;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		m_DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		ProcessInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render scene, supplying the convoluted irradiance map to the final shader.
		// ------------------------------------------------------------------------------------------
		glm::mat4 view = camera.GetViewMatrix();

		// render skybox (render as last to prevent overdraw)
		backgroundShader.use();
		backgroundShader.SetMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, testIBL.getPrefilterMapID());
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		cube.BindDrawVAO();
		glDepthFunc(GL_LESS);

		//frambufferShader.use();
		//frambufferShader.setUniform("scale", glm::vec2(1.0));
		//frambufferShader.setUniform("offset", glm::vec2(0.0));
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, testIBL.getBRDFLUTID());
		//glDisable(GL_CULL_FACE);
		//glDepthFunc(GL_LEQUAL);
		//cube.BindDrawVAO();
		//glDepthFunc(GL_LESS);


		//testPlane.BindDrawVAO();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * m_DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, m_DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, m_DeltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		m_FirstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

