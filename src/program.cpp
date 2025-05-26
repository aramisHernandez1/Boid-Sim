#include <program.h>
#include <game.h>

#include <stb/stb_image.h>

/*
* Class Purpose: Sets up all OpenGL settings
* and contains our render loop
*/


void runProgram(GLFWwindow* window) {
	//Flip stb_image to make textures look okay
	stbi_set_flip_vertically_on_load(true);

	//Configure our openGL global state
	glEnable(GL_DEPTH_TEST);


	// render
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initGame();

	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updateFrame(window);
		renderFrame(window);


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
}