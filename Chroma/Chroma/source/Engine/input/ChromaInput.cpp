#include "ChromaInput.h"
#include "ChromaInput.h"




double ChromaInput::CaptureMouseX;
double ChromaInput::CaptureMouseY;

void ChromaInput::initialize()
{
	// glfw attach callbacks
	glfwSetCursorPosCallback(window, mouse_aim_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
}

bool ChromaInput::isPressed(Key KeySelection)
{
	switch (KeySelection) {
	case LEFT_SHIFT:
		return glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		break;
	case RIGHT_SHIFT:
		return glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		break;
	case SPACEBAR:
		return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		break;
	case A:
		return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		break;
	case C:
		return glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
		break;
	case D:
		return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		break;
	case E:
		return glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
		break;
	case Q:
		return glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
		break;
	case S:
		return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		break;
	case W:
		return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		break;
	}
		
	return false;
}

void ChromaInput::process()
{
	// update mouse coordinates;
	updateMouseCoordinates();

	// check if should close on this frame
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return;
	}

	// window capture release mouse
	int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	cursorEnabled = cursorMode == GLFW_CURSOR_DISABLED;

}

void ChromaInput::bindWindow(GLFWwindow* windowVal)
{
	window = windowVal;
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

void ChromaInput::updateMouseCoordinates()
{
	MouseX = CaptureMouseX;
	MouseY = CaptureMouseY;
}

ChromaInput::ChromaInput()
{

}

ChromaInput::ChromaInput(GLFWwindow* windowVal)
{
	window = windowVal;
	initialize();
}

ChromaInput::~ChromaInput()
{
}
