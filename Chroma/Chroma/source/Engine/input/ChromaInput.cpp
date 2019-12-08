#include "ChromaInput.h"
#include "ChromaInput.h"




double ChromaInput::CaptureMouseX;
double ChromaInput::CaptureMouseY;

void ChromaInput::initialize()
{
	// glfw attach callbacks
	glfwSetCursorPosCallback(mWindow, mouse_aim_callback);
	glfwSetScrollCallback(mWindow, mouse_scroll_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_click_callback);
}

void ChromaInput::bindMousePickerCallback(std::function<void()> callback)
{
	mMousePickerCallback = callback;
}

bool ChromaInput::isPressed(Key KeySelection)
{
	switch (KeySelection) {
	case LEFT_SHIFT:
		return glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		break;
	case RIGHT_SHIFT:
		return glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		break;
	case LEFT_ALT:
		return glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
		break;
	case RIGHT_ALT:
		return glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
		break;
	case SPACEBAR:
		return glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS;
		break;
	case ESCAPE:
		return glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS;
		break;
	case A:
		return glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS;
		break;
	case C:
		return glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_PRESS;
		break;
	case D:
		return glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS;
		break;
	case E:
		return glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS;
		break;
	case P:
		return glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS;
		break;
	case Q:
		return glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS;
		break;
	case S:
		return glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS;
		break;
	case Y:
		return glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_PRESS;
		break;
	case W:
		return glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS;
		break;
	case LEFT_MOUSE:
		return glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
		break;
	case RIGHT_MOUSE:
		return glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
		break;
	case MIDDLE_MOUSE:
		return glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS;
		break;
	case LEFT_MOUSE_RELEASE:
		return glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE;
		break;
	case RIGHT_MOUSE_RELEASE:
		return glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE;
		break;
	case MIDDLE_MOUSE_RELEASE:
		return glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE;
		break;
	}
		
	return false;
}

void ChromaInput::process()
{
	// check if should close on this frame
	if (isPressed(ESCAPE)) {
		glfwSetWindowShouldClose(mWindow, true);
		return;
	}

	// toggle wireframe
	if (isPressed(P))
	{
		int polyMode;
		glGetIntegerv(GL_POLYGON_MODE, &polyMode);

		if (polyMode == GL_LINE){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}


	// window capture release mouse
	int cursorMode = glfwGetInputMode(mWindow, GLFW_CURSOR);
	if (isPressed(LEFT_ALT))
	{
		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	cursorEnabled = cursorMode == GLFW_CURSOR_DISABLED;

	// update mouse coordinates;
	if (cursorEnabled)
	{
	}
	updateMouseCoordinates();
	// mouse picker
	updateMousePicker();

}

void ChromaInput::bindWindow(GLFWwindow* windowVal)
{
	mWindow = windowVal;
	initialize();
}

// glfw callbacks
void ChromaInput::mouse_aim_callback(GLFWwindow* window, double xpos, double ypos)
{ 
	CaptureMouseX = xpos;
	CaptureMouseY = ypos;

}

void ChromaInput::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	return;
}

void ChromaInput::mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
}

void ChromaInput::updateMouseCoordinates()
{
	MouseX = CaptureMouseX;
	MouseY = CaptureMouseY;
}

void ChromaInput::updateMousePicker()
{
	if (isPressed(LEFT_MOUSE))
	{
		PickedMouseX = MouseX;
		PickedMouseY = MouseY;
		lastMouseRay = screenToWorldRay(PickedMouseX, PickedMouseY);
		mMousePickerCallback();
	}
}

glm::vec3 ChromaInput::screenToWorldRay(float const& mouseX, float const& mouseY)
{
	// get Window dimensions
	glfwGetWindowSize(mWindow, &screenWidth, &screenHeight);

	//calculate NDC Coordinates
	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;
	float z = 1.0f;

	// Normalized Device Coordinates
	glm::vec3 ray_nds(x, y, z);

	// Homogenous Clip Coordinates
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

	// Eye Coordinates
	// we bring the homogenous clip coordinates into eye space using the 
	// inverse of the projection matrix
	glm::vec4 ray_eye = glm::inverse(mCamera->getProjectionMatrix()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	// World Coordinates
	// we bring the view/eye coordinates into world space with the 
	// inverse of the view matrix
	glm::vec3 ray_world = glm::vec3((glm::inverse(mCamera->getViewMatrix()) * ray_eye));
	ray_world = glm::normalize(ray_world);


	return ray_world;
}


ChromaInput::ChromaInput(GLFWwindow* windowVal)
{
	mWindow = windowVal;
	initialize();
}

ChromaInput::~ChromaInput()
{
}
