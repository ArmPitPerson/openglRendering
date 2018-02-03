///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef GLFWAPPLICATION_H
#define GLFWAPPLICATION_H

struct GLFWwindow;


class GLFWApplication {
public:
	GLFWApplication();
	~GLFWApplication();

	void run();

private:
	// Application Window
	GLFWwindow* mWindow = nullptr;

};


#endif // GLFWAPPLICATION_H
