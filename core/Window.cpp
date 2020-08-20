#include "Window.h"

/* Load Environment Parameters */
#include "../myEnv/src/config.h"

/* Window Statistics */
int Window::width;
int Window::height;
const char *Window::windowTitle = "The NBA Museum Park";

/* Motion Parameters */
bool Window::Pressed_MouseLeft = false;
bool Window::Pressed_MouseRight = false;
double Window::Cursor_Pos_x;
double Window::Cursor_Pos_y;
glm::vec3 Window::Cursor_Pos_3d;
double lastX = Window::width / 2;
double lastY = Window::height / 2;

/* Scene_loader */
EnvScene *Window::myEnvScene = nullptr;
Museum *Window::myMuseum = nullptr;

bool Window::initializeProgram() {
#ifdef ENABLE_SCENE
	myEnvScene = new EnvScene(Window::width, Window::height);
	if (!myEnvScene->initialize())
		return false;
#endif
	return true;
}

bool Window::initializeObjects(const std::string &ObjPath)
{
#ifdef ENABLE_SCENE
	myEnvScene->createObjects();
#endif
	return true;
}

void Window::cleanUp()
{
#ifdef ENABLE_SCENE
	delete myEnvScene;
#endif
}

GLFWwindow *Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow *window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

#ifdef ENABLE_SCENE
	// Set camera aspect ratio
	if (myEnvScene != nullptr)
		myEnvScene->resize(width, height);
#endif
}

void Window::idleCallback()
{
#ifdef ENABLE_SCENE
	myEnvScene->update();
#endif
}

void Window::displayCallback(GLFWwindow *window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef ENABLE_SCENE
	myEnvScene->draw();
#endif

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */

	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
#ifdef ENABLE_SCENE
		case GLFW_KEY_W:
			if (action == GLFW_PRESS || action == GLFW_RELEASE)
				myEnvScene->toggle_forward();
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS || action == GLFW_RELEASE)
				myEnvScene->toggle_backward();
			break;
		case GLFW_KEY_Q:
			if (action == GLFW_PRESS || action == GLFW_RELEASE)
				myEnvScene->toggle_up();
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS || action == GLFW_RELEASE)
				myEnvScene->toggle_down();
			break;
		case GLFW_KEY_O:
			myEnvScene->cameraSpeedUp();
			break;
		case GLFW_KEY_P:
			myEnvScene->cameraSpeedDown();
			break;
		case GLFW_KEY_T:
			myEnvScene->toggle_toon();
			break;
		case GLFW_KEY_Y:
			myEnvScene->toggle_silhouette();
			break;
		case GLFW_KEY_N:
			myEnvScene->switchCamera();
			break;
		case GLFW_KEY_1:
			myEnvScene->toggle_reflect();
			break;
		case GLFW_KEY_2:
			myEnvScene->toggle_refract();
			break;
		case GLFW_KEY_3:
			myEnvScene->toggle_restore();
			break;
		case GLFW_KEY_6:
			myEnvScene->toggle_collections();
			break;
		case GLFW_KEY_7:
			myEnvScene->toggle_terrain();
			break;
		case GLFW_KEY_8:
			myEnvScene->toggle_water();
			break;
		case GLFW_KEY_9:
			myEnvScene->reshape_terrain();
			break;
#endif
		default:
			break;
		}
	}
}

void Window::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	myEnvScene->cameraScroll(yoffset);
}

void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
	if (!Pressed_MouseLeft)
		return;

	glfwGetCursorPos(window, &Cursor_Pos_x, &Cursor_Pos_y);
#ifdef ENABLE_SCENE
	myEnvScene->cameraRotate(Cursor_Pos_x - lastX, lastY - Cursor_Pos_y);
#endif
	lastX = Cursor_Pos_x;
	lastY = Cursor_Pos_y;

	return;
}

void Window::mouseBottonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			Pressed_MouseLeft = true;
			glfwGetCursorPos(window, &Cursor_Pos_x, &Cursor_Pos_y);
			Cursor_Pos_3d = trackBallMapping(glm::vec2(Cursor_Pos_x, Cursor_Pos_y));
			lastX = Cursor_Pos_x;
			lastY = Cursor_Pos_y;
		}

	} else { // action == GLFW_RELEASE
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			Pressed_MouseLeft = false;
	}
}

glm::vec3 Window::trackBallMapping(glm::vec2 point) {

	glm::vec3 v;
	float d;

	v.x = (2.0f * point.x - width) / width;
	v.y = (height - 2.0f * point.y) / height;
	v.z = 0.0f;

	d = glm::length(v);

	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrtf(1.001f - d*d);

	v = glm::normalize(v);
	return v;
}
