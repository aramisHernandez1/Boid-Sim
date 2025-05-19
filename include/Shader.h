#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cassert>
#include <fstream>
#include <memory>
#include <string>

class Shader {

public:
	Shader() { ID = glCreateProgram(); }

	void activate() { glUseProgram(ID); }
	void deactivate() { glUseProgram(0); }
	unsigned int get() { return ID; }
	void destroy() { glDeleteProgram(ID); }

	// Attach a shader to the current shader program
	void attach(std::string const& fileName) {

		//Load shader from source
		std::ifstream fd(fileName.c_str());
		if (fd.fail()) {
			fprintf(stderr, "Something went wrong when attaching the Shader file at \"%s\".\n"
				"This file may not exist or is inacessible.\n",
				fileName.c_str());
			return; 
		}

		auto src = std::string(std::istreambuf_iterator<char>(fd), std::istreambuf_iterator<char>());

		//Create our shader object
		const char* source = src.c_str();
		auto shader = create(fileName);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		//Display any errors
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			//Get log length than assign it to a char array 
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLenth);
			std::unique_ptr<char[]> buffer(new char[logLenth]);
			
			//fill array with log info and print our error
			glGetProgramInfoLog(shader, logLenth, nullptr, buffer.get());
			fprintf(stderr, "%s\n%s", fileName.c_str(), buffer.get());
		}

		assert(success);

		//Finally attach the shader, and than free the alocated memory
		glAttachShader(ID, shader);
		std::cout << "Shader attached" << std::endl;

		glDeleteShader(shader);
	}
	
	//Links all shader programs
	void link() {
		//Link all current attached shaders
		glLinkProgram(ID);
		std::cout << "Linked complete" << std::endl;

		//Display any errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			//Get log length than assign it to a char array 
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLenth);
			std::unique_ptr<char[]> buffer(new char[logLenth]);

			//fill array with log info and print our error
			glGetProgramInfoLog(ID, logLenth, nullptr, buffer.get());
			fprintf(stderr, "%s\n", buffer.get());
		}

		assert(success);
	}

	// Sets up a basic vertex and fragment shader. The two needed shaders
	void makeBasicShader(std::string const& vertFileName, std::string const& fragFileName) {
		attach(vertFileName);
		attach(fragFileName);
		link();
	}

	unsigned int getUniformFromName(const std::string& name) {
		return glGetUniformLocation(ID, name.c_str());
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}



private:
	//Disable copying and assignment 
	Shader(Shader const&) = delete;
	Shader& operator =(Shader const&) = delete;


	//Private variables
	unsigned int ID;
	int success;
	int logLenth;

	//Private functions

	//Helper function for setting up shader type
	unsigned int create(std::string const& fileName) {
		//Get the file extension i.e.(.vert, .frag, .geom, ...)
		auto index = fileName.rfind(".");
		auto extension = fileName.substr(index + 1);

		/*Match extension with correct shader type 
		Can add more types if needed here */
		if (extension == "vert") return glCreateShader(GL_VERTEX_SHADER);
		else if (extension == "frag") return glCreateShader(GL_FRAGMENT_SHADER);
		else if (extension == "geom") return glCreateShader(GL_GEOMETRY_SHADER);
		else if (extension == "comp") return glCreateShader(GL_COMPUTE_SHADER);
		else return false;

	}


	// TEST THIS OUT LATER
	//Helper function to get error log
	char* getInfoLog(unsigned int shader) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLenth);
		std::unique_ptr<char[]> buffer(new char[logLenth]);

		//fill array with log info and print our error
		glGetProgramInfoLog(shader, logLenth, nullptr, buffer.get());

		return buffer.get();
	}
};