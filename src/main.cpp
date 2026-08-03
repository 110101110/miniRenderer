#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//include imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/Camera.hpp"
#include "renderer/Shader.hpp"

const unsigned int SRC_WIDTH = 1200;
const unsigned int SRC_HEIGHT = 800;

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
	//init window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()){
		return 1;
	}

	//mandatory hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macos only
#endif

	//create window object
	GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Engine Sandbox", NULL, NULL);
	if (!window){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// init GLAD (function pointers management)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsLight();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(windwo, true);
	ImGui_ImplOpenGL3_Init("410 core");

	//render loop
	while (!glfwWindowShouldClose(window)){

		glfwPollEvents();

		//handle escape key
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, true);
		}

		//start imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//debug control gui
		ImGui::Begin("Engine Debug Metrics");
		ImGui::Text("Framerate: %.1f FPS", io.Framerate);
		ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
		ImGui::Separator();
		ImGui::ColorEdit3("Viewport Clear Color", clearColor);
		ImGui::End();

		// render background
		glClearColor(0.86, 0.15, 0.53, 0.8);
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);

		//render with imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	//cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
