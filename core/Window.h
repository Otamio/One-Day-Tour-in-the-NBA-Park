#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#else
#include <GL/glew.h>
// #include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

/* Core module */
#include "shader.h"

/* Load Environment */
#include "../myEnv/src/scene/scene.h"
#include "../myMuseum/Museum.h"

class Window
{
public:
	static int width;
	static int height;
	static const char *windowTitle;
	/* Motion Parameters */
	static bool Pressed_MouseLeft;
	static bool Pressed_MouseRight;
	static double Cursor_Pos_x;
	static double Cursor_Pos_y;
	static glm::vec3 Cursor_Pos_3d;
	/* Main Functions */
	static bool initializeProgram();
	static bool initializeObjects(const std::string &ObjPath);
	static void cleanUp();
	static GLFWwindow *createWindow(int width, int height);
	static void resizeCallback(GLFWwindow *window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
	static void mouseBottonCallback(GLFWwindow *window, int button, int action, int mods);

	/* Scene Switch Parameters */
	static EnvScene *myEnvScene;
	static Museum *myMuseum;

	/* Helper function */
	static glm::vec3 trackBallMapping(glm::vec2 point);
};

#endif
