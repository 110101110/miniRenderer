#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//hardcode for now, will have to add proper window class
const unsigned int SRC_WIDTH = 1200;
const unsigned int SRC_HEIGHT = 800;

int main(){

	//init window
	if (!glfwInit()) return -1;

	//mandatory hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macos only

	//create window object
	GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "HELLO WINDOW", NULL, NULL);
	if (!window){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//init GLAD (function pointers management)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//render loop
	while (!glfwWindowShouldClose(window)){

		//input esc for now to exit the program
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, true);
		}
		glClearColor(0.86, 0.15, 0.53, 0.8);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
