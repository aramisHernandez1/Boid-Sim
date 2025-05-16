#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <openglDebug.h>
#include <stb_image/stb_image.h>


#include <iostream>


#include <Shader.h>
#include <window.h>
#include <camera.h>
#include <model.h>



#define USE_GPU_ENGINE 0
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = USE_GPU_ENGINE;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = USE_GPU_ENGINE;
}

//Camera
Camera* camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	camera->handleKeyInputs(key, action);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	camera->handleCursorPosInput(xposIn, yposIn);
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
	camera->handleMouseInputs(button, action);
}


//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{

	if (!glfwInit())
		return -1;


#pragma region report opengl errors to std
	//enable opengl debugging output.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#pragma endregion


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //you might want to do this when testing the game for shipping


	GLFWwindow *window = window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);


	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//glfwSwapInterval(1);


#pragma region report opengl errors to std
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#pragma endregion

	//Flip stb_image to make textures look okay
	stbi_set_flip_vertically_on_load(true);

	//Configure our openGL global state
	glEnable(GL_DEPTH_TEST);

	//shader building
	Shader* ourShader;
	ourShader = new Shader();
	ourShader->makeBasicShader(RESOURCES_PATH"1.model_loading.vert", RESOURCES_PATH"1.model_loading.frag");

	Model ourModel(RESOURCES_PATH"objects/Old English Street Lamp/Old English Street Lamp.obj");

	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		//Time per-frame logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		// render
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use shader
		ourShader->activate();
		

		/*
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		*/

		camera->updateCamera(deltaTime);

		//Projection and view transformations
		ourShader->setMat4("projection", camera->getProjMatrix());
		ourShader->setMat4("view", camera->getViewMatrix());

		//render our loaded model
		glm::mat4 model = glm::mat4(0.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  //Translate down so its at center of screen
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); //Scale because its too big
		ourShader->setMat4("model", model);
		ourModel.Draw(*ourShader);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//there is no need to call the clear function for the libraries since the os will do that for us.
	//by calling this functions we are just wasting time.
	//glfwDestroyWindow(window);
	//glfwTerminate();
	return 0;
}

