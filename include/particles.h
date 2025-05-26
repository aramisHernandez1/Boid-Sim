#pragma once

#include <Shader.h>


#include <glm/glm.hpp>

#include <vector>

class Particles {

public:
	Particles();

	void Init();
	void Update(float time);
	void Render(const glm::mat4& VP);

private:

	void InitBuffers();
	void CalcPositions(std::vector<glm::vec4>& Positions);
	void ExecuteComputeShader();
	void RenderParticles(const glm::mat4& VP);

	Shader* basicShader;
	Shader* computeShader;


	int numParticlesX = 0;
	int numParticlesY = 0;
	int numParticlesZ = 0;

	int totalParticles = 0;

	float speed, angle;

	unsigned int vao = 0;
	unsigned int posBuf = 0;
	
	unsigned int velBuf = 0;
};