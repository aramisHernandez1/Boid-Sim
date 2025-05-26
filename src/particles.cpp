#include <particles.h>

#include <glad/glad.h>

#include <vector>


//Note unfinished code complete later

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
	std::vector<glm::vec4> Positions(totalParticles);
	CalcPositions(Positions);

	glCreateBuffers(1 &posBuf);

	unsigned int bufSize = (int)Positions.size();

	//Init as a SSBo
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, Positions.data(), GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}

void Particles::CalcPositions(std::vector<glm::vec4>& Positions) {
	glm::vec4 pos(0.0f, 0.0f, 0.0f, 1.0f);

	float distX = 2.0f / numParticlesX;
	float distY = 2.0f / numParticlesY;
	float distZ = 2.0f / numParticlesZ;

	glm::mat4 translation = glm::mat4(1.0f);
	

	int particleIndex = 0;
	for(int x = 0; x < numParticlesX; x++) {
		for (int y = 0; y < numParticlesY; y++) {
			for (int z = 0; z < numParticlesZ; z++) {
				pos.x = distX * x;
				pos.y = distY * y;
				pos.z = distZ * z;
				pos.w = 1.0f;

				pos = translation * pos;
				Positions[particleIndex] = pos;
				particleIndex++;

			}
		}
	}
}

/*
* @Param Our view and project matrix combined
*/
void Particles::Render(const glm::mat4& VP) {
	ExecuteComputeShader();

	RenderParticles(VP);
}

void Particles::ExecuteComputeShader() {
	particleShader.activate();

	//Note must wait for compute shader to finish executing before rendering results
	glDispatchCompute(totalParticles, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

void Particles::RenderParticles(const glm::mat4& VP) {

}