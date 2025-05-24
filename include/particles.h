#pragma once


#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>

class Particles {

public:
	Particles();

	void Init();
	void Update(float time);
	void Render(const glm::mat4& view, const glm::mat4 proj);

private:

	void InitBuffers();


	int numParticlesX = 0;
	int numParticlesY = 0;
	int numParticlesZ = 0;

	int totalParticles = 0;

	float speed, angle;

	unsigned int vao = 0;
	unsigned int posBuf = 0;
	
	unsigned int velBuf = 0;
};