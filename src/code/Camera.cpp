#include "Camera.h"

#define windowWidth 1080
#define windowHeight 720
Camera::Camera()
{
		pos = glm::vec3(0, 0, 20);
}

glm::vec3 Camera::GetPos()
{
		return pos;
}

glm::mat4 Camera::GetView()
{
		return viewMat;
}

glm::mat4 Camera::GetProjection()
{
		projectMat = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.f);
		return projectMat;
}

glm::mat4 Camera::GetModel()
{
		model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		viewMat = glm::inverse(model);
		return model;
}

void Camera::SetPos(glm::vec3 _pos)
{
		pos += _pos;
}

void Camera::Update()
{
		//camera movement
		SDL_Event event = { 0 };
		while (SDL_PollEvent(&event))
		{
				if (event.type == SDL_QUIT)
				{
						quit = true;
				}
				if (event.type == SDL_KEYDOWN)
				{
						switch (event.key.keysym.sym)
						{
						case SDLK_a:
								SetPos(glm::vec3(-0.2f, 0, 0));
								break;
						case SDLK_d:
								SetPos(glm::vec3(0.2f, 0, 0));
								break;
						case SDLK_w:
								SetPos(glm::vec3(0, 0.2f, 0));
								break;
						case SDLK_s:
								SetPos(glm::vec3(0, -0.2f, 0));
								break;
						case SDLK_f:
								SetPos(glm::vec3(0, 0, -0.2f));
								break;
						case SDLK_g:
								SetPos(glm::vec3(0, 0, 0.2f));
								break;

						}
				}
		}
		GetModel();
}



