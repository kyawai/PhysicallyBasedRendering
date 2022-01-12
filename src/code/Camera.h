#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{

		glm::mat4 viewMat;
		glm::mat4 projectMat;
		glm::mat4 model;
		glm::vec3 pos;



public:
		Camera();
		glm::vec3 GetPos();
		glm::mat4 GetView();
		glm::mat4 GetProjection();
		glm::mat4 GetModel();
		void SetPos(glm::vec3 _pos);
		bool quit = false;
		void Update();

};

#endif