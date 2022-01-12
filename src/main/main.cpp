#define STB_IMAGE_IMPLEMENTATION
#include "code/Camera.h"
#include "code/RenderTexture.h"
#include "code/Shader.h"
#include "code/CreateShape.h"
#include "code/Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "code/Core.h"

//define the window
#define windowWidth 1080
#define windowHeight 720


// --------------------------------------------------------------------------------
//    TO COMPLETE THIS PROJECT I HAVE FOLLOWED THE TUTORIAL FROM LEARNOPENGL
//---------------------------------------------------------------------------------
//                FOUND AT:
//                         https://learnopengl.com/PBR/Theory
//---------------------------------------------------------------------------------
int main()
{
		//create objects of classes from other files
		Core core;
		Camera cam;
		CreateShape shape;
		Texture tex;
		Camera light;
		//veriables
		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		bool quit = false;
		int sky = 0;
		int area = 0;
		int material = 0;
		int textured = 0;
		//set projection
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		//user chooses how to see the project
		std::cout << " Would you like to load the PBR project with or without the skybox ?   1 - With skybox      2 - Without skybox " << std::endl;
		std::cin >> sky;
		if (sky == 1)
		{
				std::cout << "would you like to be inside or outside        1 - inside             2 - outside " << std::endl;
				std::cin >> area;;
		}
		std::cout << "would you like the spheres to be textures or untextured?     1 - textured     2 - untextured" << std::endl;
		std::cin >> textured;
		if (textured == 1)
		{
				std::cout << " Now what material would you like to load the spheres in:      1 - Cloth     2 - Wood" << std::endl;
				std::cin >> material;
				if (material == 1)
				{
						std::cout << "You have chosen cloth" << std::endl;
				}
				if (material == 2)
				{
						std::cout << "You have chosen wood" << std::endl;
				}
		}
		if (textured == 2)
		{
				std::cout << "You have chosen untextured" << std::endl;
		}
		//create the window using SDl
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
				throw std::exception();
		}
		SDL_Window *window = SDL_CreateWindow("pbr",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (!SDL_GL_CreateContext(window))
		{
				throw std::exception();
		}
		if (glewInit() != GLEW_OK)
		{
				throw std::exception();
		}


		//gl Enable to set up the program for the project
		glEnable(GL_DEPTH_TEST);
		//depth function will be less abd eqyak to skybox depth
		glDepthFunc(GL_LEQUAL);
		//seamless cubemapping for cubemap to look more realistic and for lower mip levels
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		stbi_set_flip_vertically_on_load(true);
		//build shaders
		Shader pbr("../shaders/pbrShader.vs", "../shaders/pbrShader.fs");
		Shader equire("../shaders/equiToShader.vs", "../shaders/equiToShader.fs");
		Shader irradiance("../shaders/irradianceShader.vs", "../shaders/irradianceShader.fs");
		Shader prefilter("../shaders/prefilterShader.vs", "../shaders/prefilterShader.fs");
		Shader brdf("../shaders/brdfShader.vs", "../shaders/brdfShader.fs");
		Shader background("../shaders/backgroundShader.vs", "../shaders/backgroundShader.fs");
		Shader pbrTex("../shaders/pbrTexturedShader.vs", "../shaders/pbrTexturedShader.fs");

		//set the untextured veriables
		pbr.useProg();
		pbr.setInt("irradianceMap", 0);
		pbr.setInt("prefilterMap", 1);
		pbr.setInt("brdfLUT", 2);
		pbr.setVec3("albedo", 0.5f, 0.0f, 0.0f);
		pbr.setFloat("ao", 1.0f);

		//set the textures veriables
		pbrTex.useProg();
		pbrTex.setInt("irradianceMap", 0);
		pbrTex.setInt("prefilterMap", 1);
		pbrTex.setInt("brdfLUT", 2);
		pbrTex.setInt("albedoMap", 3);
		pbrTex.setInt("normalMap", 4);
		pbrTex.setInt("metallicMap", 5);
		pbrTex.setInt("roughnessMap", 6);
		pbrTex.setInt("aoMap", 6);

		background.useProg();
		background.setInt("environmentMap", 0);

		//textured materials:
		//cloth:

	  char* alMap = "../models/clothAlbedo.png";
		char* norMap = "../models/clothNormal.png";
		char*  metMap = "../models/clothMetallic.png";
		char*  aMap = "../models/ClothAo.png";
		char*  roughMap = "../models/clothRoughness.png";		
		Texture al;
		Texture nor;
		Texture met;
		Texture ao;
		Texture rough;
		al.LoadTexture(alMap);
		nor.LoadTexture(norMap);
		met.LoadTexture(metMap);
		ao.LoadTexture(aMap);
		rough.LoadTexture(roughMap);

		//wood
		char* alMapWood = "../models/woodalbedo.png";
		char* norMapWood = "../models/woodnormal.png";
		char*  metMapWood = "../models/woodmetal.png";
		char*  aMapWood = "../models/woodao.png";
		char*  roughMapWood = "../models/woodroughness.png";
		Texture alWood;
		Texture norWood;
		Texture metWood;
		Texture aoWood;
		Texture roughWood;
		alWood.LoadTexture(alMapWood);
		norWood.LoadTexture(norMapWood);
		metWood.LoadTexture(metMapWood);
		aoWood.LoadTexture(aMapWood);
		roughWood.LoadTexture(roughMapWood);

		//initialize the lights
		glm::vec3 lightPos[4];
		glm::vec3 lightColours[4];


		lightPos[0] = glm::vec3(-10.0f, 10.0f, 10.0f);
		lightPos[1] = glm::vec3(10.0f, 10.0f, 10.0f);
		lightPos[2] = glm::vec3(-10.0f, -10.0f, 10.0f);
		lightPos[3] = glm::vec3(10.0f, -10.0f, 10.0f);
		lightColours[0] = glm::vec3(300.0f, 300.0f, 300.0f);
		lightColours[1] = glm::vec3(300.0f, 300.0f, 300.0f);
		lightColours[2] = glm::vec3(300.0f, 300.0f, 300.0f);
		lightColours[3] = glm::vec3(300.0f, 300.0f, 300.0f);

		core.SetFrameBuffers();
		cam.GetProjection();


		//load the texture for HDR
		if (area == 1)
		{
				std::string img = "../models/inside.hdr";
				//load the HDR image
				tex.LoadHDR(img);
		}
		if (area == 2)
		{
				std::string img = "../models/skybox.hdr";
				//load the HDR image
				tex.LoadHDR(img);
		}


		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] = //All the positions for the camera to 'look at' to form a cubemap
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		//make cubemap
		//create cubemap parameters
		tex.BindTextureCube();
		//bind equirectangular map shader
		equire.useProg();
		equire.setInt("equirectangularMap", 0);
		equire.setMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.hdrTexture);
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, core.captureFBO);
		// attach for all 6 sides of the cubemap
		for (unsigned int i = 0; i < 6; i++)
		{
				equire.setMat4("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tex.envCubemap, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//load the cubemap
				shape.loadCube();
		}
		//bind buffers
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex.envCubemap);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);



		//create irradiance map
		tex.BindTextureIrr();
		//bind irradiance map shader
		irradiance.useProg();
		irradiance.setInt("environmentMap", 0);
		irradiance.setMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex.envCubemap);
		glViewport(0, 0, 32, 32);
		glBindFramebuffer(GL_FRAMEBUFFER, core.captureFBO);
		//do this for all 6 sides
		for (unsigned int i = 0; i < 6; i++)
		{
				irradiance.setMat4("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tex.irradianceMap, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//load the cubemap
				shape.loadCube();
		}
		//bind the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//create prefilter map
		tex.BindTexturePre();
		//bind prefilter map shader
		prefilter.useProg();
		prefilter.setInt("environmentMap", 0);
		prefilter.setMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex.envCubemap);
		glBindFramebuffer(GL_FRAMEBUFFER, core.captureFBO);
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
				// change buffer for mipmap
				unsigned int mipWidth = 128 * std::pow(0.5, mip);
				unsigned int mipHeight = 128 * std::pow(0.5, mip);
				glBindRenderbuffer(GL_RENDERBUFFER, core.captureRBO);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
				glViewport(0, 0, mipWidth, mipHeight);
				float roughness = (float)mip / (float)(maxMipLevels - 1);
				prefilter.setFloat("roughness", roughness);
				for (unsigned int i = 0; i < 6; ++i)
				{
						prefilter.setMat4("view", captureViews[i]);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tex.prefilterMap, mip);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						shape.loadCube();
				}
		}
		//bind to frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//create BRDF
		tex.BindTextureBRDF();
		// pre compute BRDF
		glBindFramebuffer(GL_FRAMEBUFFER, core.captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, core.captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.brdfLUTTexture, 0);
		glViewport(0, 0, 512, 512);
		//clear the buffer and load the quad
		brdf.useProg();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shape.loadQuad();
		//bind the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (textured == 1) 
		{
				//set the projection of the background shader
				pbrTex.useProg();
				pbrTex.setMat4("projection", projection);
				background.useProg();
				background.setMat4("projection", projection);
		}
		if (textured == 2)
		{
				//set the projection of the background shader
				pbr.useProg();
				pbr.setMat4("projection", projection);
				background.useProg();
				background.setMat4("projection", projection);
		}


		//set the viewport
		glViewport(0, 0, windowWidth, windowHeight);
		//start the main loop of the project
		while (!quit)
		{
				//clear the window and prepare for drawing
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				//	activate and bind the textures that will be needed
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, tex.irradianceMap);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_CUBE_MAP, tex.prefilterMap);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, tex.brdfLUTTexture);
				if (material == 1)
				{
						//active
						glActiveTexture(GL_TEXTURE3);
						glBindTexture(GL_TEXTURE_2D, al.textureID);
						glActiveTexture(GL_TEXTURE4);
						glBindTexture(GL_TEXTURE_2D, nor.textureID);
						glActiveTexture(GL_TEXTURE5);
						glBindTexture(GL_TEXTURE_2D, met.textureID);
						glActiveTexture(GL_TEXTURE6);
						glBindTexture(GL_TEXTURE_2D, rough.textureID);
						glActiveTexture(GL_TEXTURE7);
						glBindTexture(GL_TEXTURE_2D, ao.textureID);
				}
				if (material == 2)
				{
						//active
						glActiveTexture(GL_TEXTURE3);
						glBindTexture(GL_TEXTURE_2D, alWood.textureID);
						glActiveTexture(GL_TEXTURE4);
						glBindTexture(GL_TEXTURE_2D, norWood.textureID);
						glActiveTexture(GL_TEXTURE5);
						glBindTexture(GL_TEXTURE_2D, metWood.textureID);
						glActiveTexture(GL_TEXTURE6);
						glBindTexture(GL_TEXTURE_2D, roughWood.textureID);
						glActiveTexture(GL_TEXTURE7);
						glBindTexture(GL_TEXTURE_2D, aoWood.textureID);
				}

				glm::mat4 view = cam.GetView();
				if (textured == 1)
				{
						//set the view and camera position
						pbrTex.useProg();
						pbrTex.setMat4("view", view);
						pbrTex.setVec3("camPos", cam.GetPos());
				}
				if (textured == 2)
				{
						//set the view and camera position
						pbr.useProg();
						pbr.setMat4("view", view);
						pbr.setVec3("camPos", cam.GetPos());
				}
				//set the model
				glm::mat4 mod = glm::mat4(1.0f);
				//create the spheres. set the spaces and pass through metalic and roughness clamps (min and max of value they can be)
				if (textured == 1)
				{
						for (int row = 0; row < nrRows; ++row)
						{
								pbrTex.setFloat("metallic", (float)row / (float)nrRows);
								for (int col = 0; col < nrColumns; ++col)
								{
										pbrTex.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.025f, 1.0f));
										mod = glm::mat4(1.0f);
										mod = glm::translate(mod, glm::vec3(
												(float)(col - (nrColumns / 2)) * spacing,
												(float)(row - (nrRows / 2)) * spacing,
												0.0f));
										pbrTex.setMat4("projection", projection);
										pbrTex.setMat4("model", mod);
										//load the spheres
										shape.loadSphere();
								}
						}
						//create the light sources
						for (unsigned int i = 0; i < sizeof(lightPos) / sizeof(lightPos[0]); ++i)
						{
								pbrTex.setVec3("lightPositions[" + std::to_string(i) + "]", lightPos[i]);
								pbrTex.setVec3("lightColors[" + std::to_string(i) + "]", lightColours[i]);

								mod = glm::mat4(1.0f);
								mod = glm::translate(mod, lightPos[i]);
								mod = glm::scale(mod, glm::vec3(0.01f));
								pbrTex.setMat4("model", mod);
						}
				}
				//create the spheres. set the spaces and pass through metalic and roughness clamps (min and max of value they can be)
				if (textured == 2)
				{
						for (int row = 0; row < nrRows; ++row)
						{
								pbr.setFloat("metallic", (float)row / (float)nrRows);
								for (int col = 0; col < nrColumns; ++col)
								{
										pbr.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.025f, 1.0f));
										mod = glm::mat4(1.0f);
										mod = glm::translate(mod, glm::vec3(
												(float)(col - (nrColumns / 2)) * spacing,
												(float)(row - (nrRows / 2)) * spacing,
												0.0f));
										pbr.setMat4("projection", projection);
										pbr.setMat4("model", mod);
										//load the spheres
										shape.loadSphere();
								}
						}
						//create the light sources
						for (unsigned int i = 0; i < sizeof(lightPos) / sizeof(lightPos[0]); ++i)
						{
								pbr.setVec3("lightPositions[" + std::to_string(i) + "]", lightPos[i]);
								pbr.setVec3("lightColors[" + std::to_string(i) + "]", lightColours[i]);

								mod = glm::mat4(1.0f);
								mod = glm::translate(mod, lightPos[i]);
								mod = glm::scale(mod, glm::vec3(0.01f));
								pbr.setMat4("model", mod);
						}
				}
				//based on user input, display or dont display the cubemap
				if (sky == 1)
				{
						//set up the program to create the cubemap/skybox
						glDepthFunc(GL_LEQUAL);
						background.useProg();
						background.setMat4("view", view);
						background.setMat4("projection", projection);
						//bind the cubemap texture and load the shape
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_CUBE_MAP, tex.envCubemap);
						shape.loadCube();
				}
				if (sky == 2) {}
				//update the camera. call the camera functions for movement and to display the camera
				cam.Update();

				//show the images
				SDL_GL_SwapWindow(window);
		}
		//clean up
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
}
