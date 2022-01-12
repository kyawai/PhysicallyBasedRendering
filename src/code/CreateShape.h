#ifndef _CREATE_SHAPE_
#define _CREATE_SHAPE_

#include "Shader.h"
#include "Texture.h"
#include <string>
#include <glm/gtx/string_cast.hpp>


class CreateShape
{
		unsigned int sphereVAO = 0;
		unsigned int indexCount;
		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		unsigned int vbo, ebo;
		std::vector<Texture> textures;
public:
		void loadSphere();
		void loadQuad();
		void loadCube();


		//create public world veriables that will be accessed to create shape
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<float> data;
		void addTexture(Texture _tex);

};

#endif
