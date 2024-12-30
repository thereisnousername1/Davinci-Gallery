#pragma once

//framebuffer_size_callback() needs GlFW, so include moved here
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>
#include <SDL.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"

using namespace glm;

// init before main
GLuint program;
int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

SDL_Window* window;

//Called on window resize
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Processes user input on a particular window
//void ProcessUserInput(GLFWwindow* WindowIn);

//mouse motion as the camera rotates
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void Input();

//Sets the model-view-projection matrix
void SetMatrices(Shader& ShaderProgramIn, mat4 model);