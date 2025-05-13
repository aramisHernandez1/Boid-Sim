#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <window.h>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f),
		GLfloat movementSpeed = 15.0f,
		GLfloat mouseSensitivity = 0.005f)
	{
		cPosition = position;
		cMovementSpeed = movementSpeed;
		cMouseSensitivity = mouseSensitivity;
	}

	//Public functions

	//Getters
	glm::mat4 getViewMatrix() { return matView; }

	glm::mat4 getProjMatrix() { return matProj; }

	//Handle keyboard inputs from a callback mechanism
	void handleKeyInputs(int key, int action) {
		//Keep track of pressed/released buttons
		if (key >= 0 && key <= 512) {
			if (action == GLFW_PRESS) {
				keysInUse[key] = true;
			}
			else if (action == GLFW_RELEASE) {
				keysInUse[key] = false;
			}
		}
	}


	//Handles mouse buttons from callback mechanism
	void handleMouseInputs(int button, int action) {
		//We only want camera to rotate when left mouse is pressed.
		//This ensures this.
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			isMousePressed = true;
		}
		else {
			isMousePressed = false;
			resetMouse = true;
		}
	}

	//Handles cursor positon from callback mechanism
	void handleCursorPosInput(double xpos, double ypos) {
		//We do nothing if mouse isn't pressed.
		if (isMousePressed == false)
			return;

		//When mouse is released we want no movement
		if (resetMouse == true) {
			lastXPos = xpos;
			lastYPos = ypos;
			resetMouse = false;
		}

		//Keep track of pitch and yaw for the current frame
		fYaw = xpos - lastXPos;
		fPitch = ypos - lastYPos;

		//Update last known cursor position
		lastXPos = xpos;
		lastYPos = ypos;
	}


	//Need to add update camera funciton here

private:
	//Disable copying and assignment for camera
	Camera(Camera const&) = delete;
	Camera& operator =(Camera const&) = delete;

	//Update view matrix based on cur info
	void updateViewMatrix() {

		//Adjust cursor movement based on settings
		fPitch *= cMouseSensitivity;
		fYaw *= cMouseSensitivity;

		//Create quats given the current pitch and yaw.
		glm::quat qPitch = glm::quat(glm::vec3(fPitch, 0.0f, 0.0f));
		glm::quat qYaw = glm::quat(glm::vec3(0.0f, fYaw, 0.0f));

		//Reset pitch and yaw values
		fPitch = 0.0f;
		fYaw = 0.0f;

		//Update camera quaternion and normalize
		cQuaternion = qYaw * qPitch * cQuaternion;
		cQuaternion = glm::normalize(cQuaternion);

		//Build rotation matrix
		glm::mat4 matRotation = glm::mat4_cast(cQuaternion);

		//Build translation matrix
		glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f), -cPosition);

		//Update the view matrix
		matView = matRotation * matTranslate;
	}

	//Private variables

	//Camera quaternion, frame pitch and yaw
	glm::quat cQuaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	GLfloat fPitch = 0.0f;
	GLfloat fYaw = 0.0f;

	//Camera position
	glm::vec3 cPosition;

	GLboolean resetMouse = true;
	GLboolean isMousePressed = false;
	GLboolean keysInUse[512] = { 0 };

	//Last cursor position
	GLfloat lastXPos = 0.0f;
	GLfloat lastYPos = 0.0f;

	//Camera settings
	GLfloat cMovementSpeed;
	GLfloat cMouseSensitivity;

	//View matrix
	glm::mat4 matView;

	//Projection Matrix
	glm::mat4 matProj = glm::perspective(glm::radians(80.0f), float(windowWidth) / float(windowHeight), 0.1f, 350.f);
};