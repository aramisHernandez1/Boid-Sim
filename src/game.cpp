#include <game.h>

#include <camera.h>
#include <particles.h>

Camera* camera;
Particles* particles;

//Time handling init
float deltaTime = 0.0f;
float lastFrame = 0.0f;



//Input Functions for user input
//--------------------------------
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
//--------------------------------------


void initGame(GLFWwindow* window) {
	camera = new Camera();
	particles = new Particles();
	particles->Init();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);
}


void updateFrame(GLFWwindow* window) {
	//Time per-frame logic
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	camera->updateCamera(deltaTime);

}

void renderFrame(GLFWwindow* window) {
	int windHeight, windWidth;
	glfwGetWindowSize(window, &windWidth, &windHeight);
	glViewport(0, 0, windWidth, windHeight);

	//Render logic here
	glm::mat4 VP = camera->getProjMatrix() * camera->getViewMatrix();
	particles->Render(VP);

}