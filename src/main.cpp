#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//include imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/Camera.hpp"
#include "renderer/Shader.hpp"
#include "geometry/Sphere.hpp"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

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
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine Sandbox", NULL, NULL);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsLight();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");

	Shader blinnShader("shaders/blinn_phong.vert", "shaders/blinn_phong.frag");
	Sphere sphere(1.0f, 64, 64);
	Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));

	// Material & Light Variables for ImGui Tweaking
	float objColor[3] = {0.2f, 0.6f, 1.0f}; // Nice cyan/blue color
	float lightPos[3] = {2.0f, 2.0f, 2.0f};
	float lightColor[3] = {1.0f, 1.0f, 1.0f};
	float shininess = 64.0f;
	float alpha = 1.0f;
	float reflectivity = 0.0f;

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
		ImGui::Begin("Blinn-Phong Control Panel");
		ImGui::ColorEdit3("Object Color", objColor);
		ImGui::SliderFloat("Shininess (Specular)", &shininess, 8.0f, 256.0f);
		ImGui::SliderFloat("Alpha (Transparency)", &alpha, 0.0f, 1.0f);
		ImGui::SliderFloat("Reflectivity", &reflectivity, 0.0f, 1.0f);
		ImGui::Separator();
		ImGui::DragFloat3("Light Position", lightPos, 0.1f);
		ImGui::ColorEdit3("Light Color", lightColor);
		ImGui::End();

		//we can dp one panel for camera
		// ImGui::Begin("camera control");
		// ImGui::End();

		// render background
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		blinnShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		//set matrix uniforms
		blinnShader.setMat4("projection", projection);
		blinnShader.setMat4("view", view);
		blinnShader.setMat4("model", model);

		//light and material uniforms
		glUniform3fv(glGetUniformLocation(blinnShader.ID, "objColor"), 1, objColor);
		glUniform3fv(glGetUniformLocation(blinnShader.ID, "lightPos"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(blinnShader.ID, "lightColor"), 1, lightColor);
		glUniform3fv(glGetUniformLocation(blinnShader.ID, "camPos"), 1, &camera.Position[0]);

		blinnShader.setFloat("shininess", shininess);
		blinnShader.setFloat("alpha", alpha);
		blinnShader.setFloat("reflectivity", reflectivity);
		blinnShader.setBool("useSkybox", false);

		// render with geometry
		sphere.draw();
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
