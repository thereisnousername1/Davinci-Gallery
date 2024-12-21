#pragma region include & namespace
#include <iostream>
// glew.h and glad.h right above every other OpenGL things

// lab 9 task 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//LEARNOPENGL
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>
#include "main.h"
#include <vector>
#include <cmath>

using namespace glm;

using namespace std;

#pragma endregion

#pragma region Global variables

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 3 };
//VAOs
GLuint VAOs[NumVAOs];

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];

//Transformations
mat4 transform;

// also Transformations
//Relative position within world space
vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
//The direction of travel
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//Up position within world space
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

//Time
//Time change
float deltaTime = 0.0f;
//Last value of time change
float lastFrame = 0.0f;

//Camera sideways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = -30.0f;
//Determines if first entry of mouse into window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
// float cameraLastXPos = 800.0f / 2.0f;
float cameraLastXPos = 0.0f;
// float cameraLastYPos = 600.0f / 2.0f;
float cameraLastYPos = 0.0f;

//Model-View-Projection Matrix
mat4 mvp;
mat4 model;
mat4 view;
mat4 projection;

#pragma endregion

#pragma region Matrices for octagon room

// bind to VAO[0]
float ceilingVertices[] = {
    50.0f, 30.0f, 0.0f,             0.5f, 0.0f,
    35.35534f, 30.0f, 35.35534f,    1.0f, 0.0f,
    0.0f, 30.0f, 50.0f,             1.0f, 0.5f,
    -35.35534f, 30.0f, 35.35534f,   0.5f, 1.0f,
    -50.0f, 30.0f, 0.00f,           0.0f, 0.5f,
    -35.35534f, 30.0f, -35.35534f,  0.0f, 0.0f,
    0.0f, 30.0f, -50.0f,            0.5f, 0.0f,
    35.35534f, 30.0f, -35.35534f,   1.0f, 0.5f,
    0.0f, 30.0f, 0.0f,              0.5f, 0.5f   // centre
};


unsigned int ceilingIndices[] = {
    0, 1, 8,
    1, 2, 8,
    2, 3, 8,
    3, 4, 8,
    4, 5, 8,
    5, 6, 8,
    6, 7, 8,
    7, 0, 8
};
// bind to VAO[0]

// bind to VAO[1]
float floorVertices[] = {
    50.0f, -30.0f, 0.0f,             0.5f, 0.0f,
    35.35534f, -30.0f, 35.35534f,    1.0f, 0.0f,
    0.0f, -30.0f, 50.0f,             1.0f, 0.5f,
    -35.35534f, -30.0f, 35.35534f,   0.5f, 1.0f,
    -50.0f, -30.0f, 0.00f,           0.0f, 0.5f,
    -35.35534f, -30.0f, -35.35534f,  0.0f, 0.0f,
    0.0f, -30.0f, -50.0f,            0.5f, 0.0f,
    35.35534f, -30.0f, -35.35534f,   1.0f, 0.5f,
    0.0f, -30.0f, 0.0f,              0.5f, 0.5f   // centre
};

unsigned int floorIndices[] = {
    0, 1, 8,
    1, 2, 8,
    2, 3, 8,
    3, 4, 8,
    4, 5, 8,
    5, 6, 8,
    6, 7, 8,
    7, 0, 8
};
// bind to VAO[1]

float wallVertices[] = {
    50.0f, 30.0f, 0.0f,             1.0f, 0.5f,
    35.35534f, 30.0f, 35.35534f,    0.8536f, 0.8536f,
    0.0f, 30.0f, 50.0f,             0.5f, 1.0f,
    -35.35534f, 30.0f, 35.35534f,   0.1464f, 0.8536f,
    -50.0f, 30.0f, 0.0f,            0.0f, 1.0f,
    -35.35534f, 30.0f, -35.35534f,  0.1464f, 0.1464f,
    0.0f, 30.0f, -50.0f,            0.5f, 0.0f,
    35.35534f, 30.0f, -35.35534f,   0.8536f, 0.1464f,
    0.0f, 30.0f, 0.0f,              1.0f, 0.0f,
    50.0f, -30.0f, 0.0f,            0.0f, 1.0f,
    35.35534f, -30.0f, 35.35534f,   0.8536f, 0.1464f,
    0.0f, -30.0f, 50.0f,            0.5f, 0.0f,
    -35.35534f, -30.0f, 35.35534f,  0.1464f, 0.1464f,
    -50.0f, -30.0f, 0.0f,           1.0f, 0.0f,
    -35.35534f, -30.0f, -35.35534f, 0.1464f, 0.8536f,
    0.0f, -30.0f, -50.0f,           0.5f, 1.0f,
    35.35534f, -30.0f, -35.35534f,  0.8536f, 0.8536f,
    0.0f, -30.0f, 0.0f,             1.0f, 0.5f
};

unsigned int wallIndices[] = {
    0, 1, 9,
    1, 10, 9,
    1, 2, 10,
    2, 11, 10,
    2, 3, 11,
    3, 12, 11,
    3, 4, 12,
    4, 13, 12,
    4, 5, 13,
    5, 14, 13,
    5, 6, 14,
    6, 15, 14,
    6, 7, 15,
    7, 16, 15,
    7, 0, 16,
    0, 9, 16
};

#pragma endregion

int main()
{
#pragma region InitLogic
    //Initialisation of GLFW
    glfwInit();
    //Initialisation of 'GLFWwindow' object
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Davinci Gallery", NULL, NULL);

    //Checks if window has been successfully instantiated
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }
    else {
        cout << "GLFW Window successfully generated!" << endl;
    }

    //Sets cursor to automatically bind to window & hides cursor pointer
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Binds OpenGL to window
    glfwMakeContextCurrent(window);

    //Sets the framebuffer_size_callback() function as the callback for the window resizing event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Sets the mouse_callback() function as the callback for the mouse movement event
    glfwSetCursorPosCallback(window, mouse_callback);

    //Initialisation of GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "GLAD failed to initialise\n";
        return -1;
    }
    else {
        cout << "GLAD successfully executed!" << endl;
    }

    //Loading of shaders
    Shader Shaders("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
    //Model Rock("media/rock/Rock07-Base.obj");

    //Model Tree("media/tree/yamaboushi_tan_6000_a_spr1.obj");

    Shaders.use();

    //Sets the viewport size within the window to match the window size of 1280x720
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //// PLEASE ADD THIS LINE TO COMPLICATED 3D PROJECT!
    glEnable(GL_DEPTH_TEST);
    //// PLEASE ADD THIS LINE TO COMPLICATED 3D PROJECT!

#pragma endregion

#pragma region Models Logic

    //Model matrix
    model = mat4(1.0f);

    //Scaling to zoom in
    //model = scale(model, vec3(4.0f, 4.0f, 4.0f));
    //Scaling to zoom in
    model = scale(model, vec3(0.05f, 0.05f, 0.05f));

    //Rotation to look down
    model = rotate(model, radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    //Movement to position further back
    model = translate(model, vec3(0.0f, 1.f, -1.5f));

    //Projection matrix
    projection = perspective(radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

#pragma endregion

#pragma region VAOs for Gallery Environment
        
    //// VAO[0] - ceiling
    //Sets index of VAO
    glGenVertexArrays(NumVAOs, VAOs); //NumVAOs, VAOs
    //Binds VAO to a buffer
    glBindVertexArray(VAOs[0]); //VAOs[0]
    //Sets indexes of all required buffer objects
    glGenBuffers(NumBuffers, Buffers); //NumBuffers, Buffers
    //glGenBuffers(1, &EBO);

    //Binds vertex object to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
    //Allocates buffer memory for the vertices of the 'Triangles' buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);

    //Binding & allocation for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ceilingIndices), ceilingIndices, GL_STATIC_DRAW);

    //Allocation & indexing of vertex attribute memory for vertex shader
    //Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Textures
    // use channel 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Unbinding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //// VAO[1] - floor
    //Binds VAO to a buffer
    glBindVertexArray(VAOs[1]); //VAOs[1]
    //Sets indexes of all required buffer objects
    glGenBuffers(NumBuffers, Buffers); //NumBuffers, Buffers
    //glGenBuffers(1, &EBO);
    
    //Binds vertex object to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
    //Allocates buffer memory for the vertices of the 'Triangles' buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    //Binding & allocation for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    //Allocation & indexing of vertex attribute memory for vertex shader
    //Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Textures
    // use channel 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Unbinding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //// VAO[2] - walls
    //Binds VAO to a buffer
    glBindVertexArray(VAOs[2]); //VAOs[2]
    //Sets indexes of all required buffer objects
    glGenBuffers(NumBuffers, Buffers); //NumBuffers, Buffers
    //glGenBuffers(1, &EBO);

    //Binds vertex object to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
    //Allocates buffer memory for the vertices of the 'Triangles' buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    //Binding & allocation for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    //Allocation & indexing of vertex attribute memory for vertex shader
    //Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Textures
    // use channel 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Unbinding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Textures to generate
    glGenTextures(NumBuffers, Buffers);

    //Binding texture to type 2D texture
    glBindTexture(GL_TEXTURE_2D, Buffers[Textures]);

    //Sets to use linear interpolation between adjacent mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //Sets to use linear interpolation upscaling (past largest mipmap texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Parameters that will be sent & set based on retrieved texture
    int width, height, colourChannels;
    //Retrieves texture data
    unsigned char* data = stbi_load("media/marble.jpg", &width, &height, &colourChannels, 0);

    if (data) //If retrieval successful
    {
        //Generation of texture from retrieved texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //Automatically generates all required mipmaps on bound texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else //If retrieval unsuccessful
    {
        cout << "Failed to load texture.\n";
        return -1;
    }

    //Clears retrieved texture from memory
    stbi_image_free(data);

    //Unbinding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#pragma endregion

#pragma region RenderLoop
    // Keep running, don't terminate so fast
    //Render loop
    while (glfwWindowShouldClose(window) == false)
    {

        //Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Input
        ProcessUserInput(window); //Takes user input

        //Rendering
        glClearColor(0.25f, 0.0f, 0.0f, 1.0f); //Colour to display on cleared window
        // currently dark red

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the colour buffer

        // lab 9 task 1

        // lab 6 task 4
        // the lab solution swap Buffers[Textures] with a variable texture, use Buffers[Textures] instead
        //Drawing
        //glBindTexture(GL_TEXTURE_2D, Buffers[Textures]);

        glBindVertexArray(VAOs[0]); //Bind buffer object to render; VAOs[0]
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);
        // lab 6 task 4

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(VAOs[1]); //Bind buffer object to render; VAOs[0]
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(VAOs[2]); //Bind buffer object to render; VAOs[0]
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

        // glEnable(GL_CULL_FACE); //Discards all back-facing triangles

        view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); //Sets the position of the viewer, the movement direction in relation to it & the world up direction
        
        SetMatrices(Shaders);

        //Drawing
        //Rock.Draw(Shaders);
        
        //Tree (changes MVP in relation to past values)
        // model = scale(model, vec3(0.05f, 0.05f, 0.05f));
        SetMatrices(Shaders);
        //Tree.Draw(Shaders);

        //Rock (reorient MVP back to starting values)
        // model = scale(model, vec3(20.0f, 20.0f, 20.0f));
        SetMatrices(Shaders);

        //Refreshing
        glfwSwapBuffers(window); //Swaps the colour buffer
        glfwPollEvents(); //Queries all GLFW events
    }
#pragma endregion

    //Safely terminates GLFW
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Resizes window based on contemporary width & height values
    glViewport(0, 0, width, height);
}

// define input method, keys, etc.
void ProcessUserInput(GLFWwindow* WindowIn)
{
    //Closes window on 'exit' key press
    if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowIn, true);
    }

    //Extent to which to move in one instance
    const float movementSpeed = 1.0f * deltaTime;
    //WASD controls
    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Initially no last positions, so sets last positions to current positions
    if (mouseFirstEntry)
    {
        cameraLastXPos = (float)xpos;
        cameraLastYPos = (float)ypos;
        mouseFirstEntry = false;
    }

    //Sets values for change in position since last frame to current frame
    float xOffset = (float)xpos - cameraLastXPos;
    float yOffset = cameraLastYPos - (float)ypos;

    //Sets last positions to current positions for next frame
    cameraLastXPos = (float)xpos;
    cameraLastYPos = (float)ypos;

    //Moderates the change in position based on sensitivity value
    const float sensitivity = 0.025f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    //Adjusts yaw & pitch values against changes in positions
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    // angle limiter
    //Prevents turning up & down beyond 90 degrees to look backwards
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }

    //Modification of direction vector based on mouse turning
    vec3 direction;
    direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
    direction.y = sin(radians(cameraPitch));
    direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
    cameraFront = normalize(direction);
}

void SetMatrices(Shader& ShaderProgramIn)
{
    mvp = projection * view * model; //Setting of MVP
    ShaderProgramIn.setMat4("mvpIn", mvp); //Setting of uniform with Shader class
}