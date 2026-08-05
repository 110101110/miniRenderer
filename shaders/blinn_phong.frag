#version 410 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

//material
uniform vec3 objColor;
uniform float shininess;
uniform float alpha; // for transparency
uniform float reflectivity; // for mirror effect

//light
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

// Cubemap Texture for Skybox Reflection (Mirror Effect)
uniform samplerCube skybox;
uniform bool useSkybox;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 camDir = normalize(camPos - WorldPos);

	//ambient
	float ambientStrength = 0.15;
	vec3 ambient = ambientStrength * lightColor;

	//diffuse light
	vec3 lightDir = normalize(lightPos - WorldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular light, using halfway vector
	vec3 halfwayDir = normalize(lightDir + camDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
	vec3 specular = spec * lightColor;

	vec3 bpColor = (ambient + diffuse + specular) * objColor;

	vec3 finalColor = bpColor;

	//mirror reflection using dynamic environment mapping
	if (useSkybox)
	{
		vec3 I = normalize(WorldPos - camPos);
		vec3 R = reflect(I, norm);
		vec3 rflColor = texture(skybox, R).rgb;

		finalColor = mix(bpColor, rflColor, reflectivity);
	}

	FragColor = vec4(finalColor, alpha);
}
