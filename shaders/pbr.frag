#version 410 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

//material uniforms, tweakable with imgui


//light uniforms
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

const float PI = 3.14159265359;


void main()
{
	vec3 norm = normalize(Normal);
	vec3 camDir = normalize(camPos - WorldPos);

}
