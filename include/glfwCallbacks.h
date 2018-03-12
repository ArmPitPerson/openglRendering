/// OpenGL - by Carl Findahl - 2018

/* 
 * File contains all callbacks provided
 * for use with GLFW
 */

#ifndef GLFWCALLBACKS_H
#define GLFWCALLBACKS_H

struct GLFWwindow;

 // GLFW Key Callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// GLFW Mouse Button Callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// GLFW Cursor Position Callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// GLFW Mouse Scroll Wheel Callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// GLFW Error Callback
void error_callback(int error, const char* description);

#endif // GLFWCALLBACKS_H
