/// OpenGL - Carl Findahl - 2018

/*
 * The main application that manages the
 * GL Context and Window. Ensures proper
 * order for construction/destruction of
 * GL objects. Where the main loop is and
 * most of the testing is performed.
 */


#ifndef GLFWAPPLICATION_H
#define GLFWAPPLICATION_H

#include "inputManager.h"
#include "renderer.h"

struct GLFWwindow;
struct ImGuiContext;


class GLFWApplication
{
public:
	GLFWApplication();
	~GLFWApplication();

	void run();

private:
	// Application Window
	GLFWwindow* mWindow = nullptr;

    // ImGui Context
    ImGuiContext* mImGuiContext = nullptr;

	// Input Manager
	InputManager mInputManager;

    // Renderer
    Renderer mRenderer;

};


#endif // GLFWAPPLICATION_H
