#include <window.h>
#include <program.h>


#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <openglDebug.h>






#define USE_GPU_ENGINE 0
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = USE_GPU_ENGINE;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = USE_GPU_ENGINE;
}

//Initalize camera
//Camera* camera = new Camera();

/*
* Call back functions used mainly for input
* -------------------------------------------
*/


//-------------------------f-------------------------


GLFWwindow* initProgram() {
	if (!glfwInit())
		return false;


#pragma region report opengl errors to std
	//enable opengl debugging output.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#pragma endregion


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //you might want to do this when testing the game for shipping


	GLFWwindow* window = window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	//Set up window and input functions
	glfwMakeContextCurrent(window);



	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//glfwSwapInterval(1);


#pragma region report opengl errors to std
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#pragma endregion


	return window;
}



int main(void)
{



	//shader building
	//Shader* ourShader;
	//ourShader = new Shader();
	//ourShader->attach(RESOURCES_PATH"vertex.vert");
	//ourShader->attach(RESOURCES_PATH"fragment.frag");
	//ourShader->link();


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
	//Model ourModel(RESOURCES_PATH"objects/boid.obj");

	

	//there is no need to call the clear function for the libraries since the os will do that for us.
	//by calling this functions we are just wasting time.
	//glfwDestroyWindow(window);
	//glfwTerminate();

	GLFWwindow* window = initProgram();

	runProgram(window);


	return 0;
}

