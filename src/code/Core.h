
#ifndef _CORE_H_
#define _CORE_H_
#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "code/Shader.h"
#include "code/Camera.h"
#include "code/CreateShape.h"



class Core
{
		int skybox;
public:
		//setting veriables for main
		unsigned int captureFBO;
		unsigned int captureRBO;

		void SetFrameBuffers();


};



#endif // !_CORE_H_
