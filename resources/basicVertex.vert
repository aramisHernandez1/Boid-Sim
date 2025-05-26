#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

uniform mat4 gWVP = mat4(1.0);

out vec2 aTexCoords;

void main(){
	gl_Position = gWVP * vec4(Position, 1.0);
	aTexCoords = TexCoords;

}