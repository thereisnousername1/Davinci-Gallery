#pragma once

//framebuffer_size_callback() needs GlFW, so include moved here
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>

// init before main
GLuint program;
int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

//Called on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Processes user input on a particular window
void ProcessUserInput(GLFWwindow* WindowIn);

//mouse motion as the camera rotates
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Sets the model-view-projection matrix
void SetMatrices(Shader& ShaderProgramIn);