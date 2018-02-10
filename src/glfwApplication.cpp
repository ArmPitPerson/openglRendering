#include "glfwApplication.h"
#include "gl_cpp.hpp"
#include "GLFW/glfw3.h"
#include "vertexArray.h"
#include "serviceLocator.h"
#include "uniformBlocks.h"
#include "logging.h"
#include "vertex.h"
#include "buffer.h"
#include "shader.h"
#include "linalg.h"
#include "camera.h"
#include <memory>
#include <iostream>


// GLFW Key Callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static InputManager* inputManager = ServiceLocator<InputManager>::get();
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		inputManager->pressKey(key);
	else if (action == GLFW_RELEASE)
		inputManager->releaseKey(key);
}


GLFWApplication::GLFWApplication() {
	ServiceLocator<InputManager>::provide(&mInputManager);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
	mWindow = glfwCreateWindow(1280, 720, "Open GL Rendering", nullptr, nullptr);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwMakeContextCurrent(mWindow);
}

GLFWApplication::~GLFWApplication() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void GLFWApplication::run() {
	Vertex vertices[] = { // For a Cube
			{ -.5f, -.5f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, },
			{  .5f, -.5f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, },
			{  .5f,  .5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, },
			{ -.5f,  .5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, },
			{ -.5f, -.5f, 1.f, .5f, 1.f, .5f, 0.f, 0.f, },
			{  .5f, -.5f, 1.f, 1.f, .6f, 0.f, 1.f, 0.f, },
			{  .5f,  .5f, 1.f, 1.f, .4f, 0.f, 1.f, 1.f, },
			{ -.5f,  .5f, 1.f, 0.f, .8f, 1.f, 0.f, 1.f } };

	unsigned indices[] = { 0, 1, 2, 2, 3, 0, // For that cube
			       0, 4, 7, 7, 3, 0,
			       4, 5, 6, 6, 7, 4,
			       1, 5, 6, 6, 2, 1,
			       2, 3, 7, 7, 6, 2,
			       0, 1, 5, 5, 4, 0};

	// Buffers and Arrays
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices), 36);
	VertexArray vao(vbo, ibo);
	vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
	vao.addAttribute(3, gl::FLOAT, offsetof(Vertex, x));
	vao.addAttribute(2, gl::FLOAT, offsetof(Vertex, u));
	vao.bind();
	
	// Uniforms and Shaders
	Shader shader("vertex.vs", "frag.fs");
	shader.bind();

	UMatrices uniformBlockData;
	UniformBuffer matrixBuffer(sizeof(UMatrices), shader);
	matrixBuffer.setUniformBlock("Matrices");
	matrixBuffer.bind();

	uniformBlockData.modelView = mat4::translate(0.f, 0.f, -2.f);
	uniformBlockData.projection = mat4::perspective(59.f, 1280.f / 720.f, 0.1f, 100.f);
	matrixBuffer.setBlockData(&uniformBlockData, sizeof(uniformBlockData));

	std::cout << uniformBlockData.modelView << "\n\n";
	std::cout << uniformBlockData.projection;

	logCustom()->info("Block size: {}", sizeof(uniformBlockData));

	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LEQUAL);

	// Textures
	unsigned texture;
	float* texData = new float[256 * 256 * 4];
	for (int x = 0; x != 256; ++x) {
		for (int y = 0; y != 256; ++y) {
			texData[(x * 256 + y) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
			texData[(x * 256 + y) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
			texData[(x * 256 + y) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
			texData[(x * 256 + y) * 4 + 3] = 1.0f;
		}		
	}

	gl::CreateTextures(gl::TEXTURE_2D, 1, &texture);
	gl::TextureStorage2D(texture, 1, gl::RGBA32F, 256, 256);
	gl::TextureSubImage2D(texture, 0, 0, 0, 256, 256, gl::RGBA, gl::FLOAT, texData);
 	gl::TextureParameteri(texture, gl::TEXTURE_WRAP_S, gl::REPEAT);
 	gl::TextureParameteri(texture, gl::TEXTURE_WRAP_T, gl::REPEAT);
 	gl::TextureParameteri(texture, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
 	gl::TextureParameteri(texture, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::BindTexture(gl::TEXTURE_2D, texture);

	delete[] texData;

	gl::PointSize(20.f);

	while (!glfwWindowShouldClose(mWindow)) {
		// Event Processing
		mInputManager.clear();
		glfwPollEvents();

		// Input Handling
		if (mInputManager.wasPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, true);

		// Updating
		uniformBlockData.modelView *= mat4::rotate(0.5f, 0.f, 1.f, 0.f);
		matrixBuffer.setPartialBlockData("modelView", uniformBlockData.modelView.data(), 64);

		// Drawing
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
		gl::DrawElements(gl::TRIANGLES, ibo.getCount(), gl::UNSIGNED_INT, nullptr);

		glfwSwapBuffers(mWindow);
	}

	gl::DeleteTextures(1, &texture);
}

