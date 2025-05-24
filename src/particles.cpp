#include <particles.h>

#include <glad/glad.h>

#include <vector>

Particles::Particles() {
	numParticlesX = 100;
	numParticlesY = 100;
	numParticlesZ = 100;

	totalParticles = numParticlesX * numParticlesY * numParticlesZ;

}

void Particles::Init() {
	//Init frag and comp shader

	//D
	InitBuffers();
}

void Particles::InitBuffers() {
	vector<glm::vec4> Positions(totalParticles);
	//Calc positions here

	glCreateBuffers(1 &posBuf);

	unsigned int bufSize = (int)Positions.size()
}