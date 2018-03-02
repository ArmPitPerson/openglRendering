///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef GLFWAPPLICATION_H
#define GLFWAPPLICATION_H

#include "inputManager.h"

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
	GLFWwindow * mWindow = nullptr;

    // ImGui Context
    ImGuiContext* mImGuiContext = nullptr;

	// Input Manager
	InputManager mInputManager;

};


#endif // GLFWAPPLICATION_H
