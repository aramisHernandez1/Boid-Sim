#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <openglDebug.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

//Initalize camera
Camera* camera = new Camera();

/*
* Call back functions used mainly for input
* -------------------------------------------
*/
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

//--------------------------------------------------


//Time handling init
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


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //you might want to do this when testing the game for shipping


	GLFWwindow *window = window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//Set up window and input functions
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
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
	ourShader->attach(RESOURCES_PATH"vertex.vert");
	ourShader->attach(RESOURCES_PATH"fragment.frag");
	ourShader->link();


	/*
	//Lighting shader
	Shader* lightShader;
	lightShader = new Shader();
	lightShader->makeBasicShader(RESOURCES_PATH"vertex.vert", RESOURCES_PATH"light.frag");

	//Our light source vertices (the white cube).
	float vertices[] = {
		// back face
		-0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		// front face
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		// left face
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		// right face
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		 // bottom face
		 -0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f,  0.5f,
		  0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f, -0.5f,

		 // top face
		 -0.5f,  0.5f, -0.5f,
		 -0.5f,  0.5f,  0.5f,
		  0.5f,  0.5f,  0.5f,
		  0.5f,  0.5f,  0.5f,
		  0.5f,  0.5f, -0.5f,
		 -0.5f,  0.5f, -0.5f
	};

	//Set up light source data for render
	unsigned int lightVBO, lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(lightVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	*/


	//Load our model
	Model ourModel(RESOURCES_PATH"objects/boid.obj");

	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		//Time per-frame logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->updateCamera(deltaTime);
		

		/*
		//Start the light shader, this shader is just a different vert shader to draw the white cube
		lightShader->activate();
		//Light attributes
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 lightPos = glm::vec3(5.3f, 15.0f, -4.0f);

		lightShader->setMat4("projection", camera->getProjMatrix());
		lightShader->setMat4("view", camera->getViewMatrix());
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		lightShader->setMat4("model", model);
		lightShader->setVec3("lightColor", lightColor);
		lightShader->setVec3("viewPos", camera->getViewMatrix()[3]);

		//Draw our light, which is the white cube
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//De activate so we can use our model shader
		lightShader->deactivate();
		*/

		//Use main shader for the model
		ourShader->activate();

		//Projection and view transformations
		ourShader->setMat4("projection", camera->getProjMatrix());
		ourShader->setMat4("view", camera->getViewMatrix());

		//Setting up our lighting properties
		ourShader->setVec3("light.position", glm::vec3(1.0f, 1.0f, 2.0f));
		ourShader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		ourShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//View position
		ourShader->setVec3("viewPos", camera->getViewMatrix()[3]);

		ourShader->setFloat("matieral.shine", 32.0f);

		//render our loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  //Translate down so its at center of screen
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); //Scale because its too big
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)); //rotates our shape
		ourShader->setMat4("model", model);

		//Finally draw our model
		ourModel.Draw(*ourShader);
		
		ourShader->deactivate();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//there is no need to call the clear function for the libraries since the os will do that for us.
	//by calling this functions we are just wasting time.
	//glfwDestroyWindow(window);
	//glfwTerminate();
	return 0;
}

