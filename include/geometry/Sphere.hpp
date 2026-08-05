#pragma once

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>
#include<cmath>

class Sphere{
	public:
	unsigned int vao, vbo, ebo;
	unsigned int indexCount;

	Sphere(float radius = 1.0f, unsigned int rings = 32, unsigned int sectors = 32){
		std::vector<float> data;
		std::vector<unsigned int> indices;

		const float PI = 3.14159265359f;

		for (unsigned int r = 0; r <= rings; ++r)
		{
			float v = (float) r / (float) rings;
			float phi = v * PI;
			for(unsigned int s = 0; s <= sectors; ++s)
			{
				float u = (float) s / (float) sectors;
				float theta = u * 2.0f * PI;

				//spherical to cartesian conversion
				float x = radius * std::cos(theta) * std::sin(phi);
				float y = radius * std::cos(phi);
				float z = radius * std::sin(theta) * std::sin(phi);

				glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

				//push vertex attributes to data vector
				data.push_back(x); data.push_back(y); data.push_back(z); // aPos
				data.push_back(normal.x); data.push_back(normal.y);data.push_back(normal.z); //aNormal
				data.push_back(u); data.push_back(v); // texCoords
			}
		}
		//generate indices (for triangles)
		for (unsigned int r = 0; r < rings; ++r)
		{
			for (unsigned int s = 0; s < sectors; ++s)
			{
				unsigned int cur = r * (sectors + 1) + s;
				unsigned int next = cur + sectors + 1;
				//first triangle
				indices.push_back(cur);
				indices.push_back(next);
				indices.push_back(cur + 1);
				//second triangle
				indices.push_back(next);
				indices.push_back(next + 1);
				indices.push_back(cur + 1);
			}
		}
		indexCount = static_cast<unsigned int>(indices.size());

		//create GPU Buffers vao, vbo, ebo
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		//attribute pointers (for vertex shader)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * (sizeof(float))));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void draw(){
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	~Sphere(){
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, & vbo);
		glDeleteBuffers(1, &ebo);
	}
};
