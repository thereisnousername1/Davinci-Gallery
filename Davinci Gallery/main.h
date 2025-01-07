#pragma once

//framebuffer_size_callback() needs GlFW, so include moved here
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>
#include <SDL.h>
#include "SDL_mixer.h"

using namespace glm;

// init before main
GLuint program;

// declared as float for easy and duty-free coding experience
float WINDOW_WIDTH = 800.0f;
float WINDOW_HEIGHT = 600.0f;

// to be honest OpenGL window is better than SDL one, at least it is easy to resize the border
SDL_Window* window;

int currentBGM;
Mix_Music* bgm[3];

//Called on window resize
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Processes user input on a particular window
//void ProcessUserInput(GLFWwindow* WindowIn);

//mouse motion as the camera rotates
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void Input();

//Sets the model-view-projection matrix
void SetMatrices(Shader& ShaderProgramIn, mat4 model);

// SDL_Texture* renderText(const std::string& message, SDL_Color color, int fontSize, SDL_Renderer* renderer);

// void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);

void PlayMusic();