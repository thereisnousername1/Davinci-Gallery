#pragma region include & namespace
#include <iostream>
// glew.h or glad.h right above every other OpenGL things

#include <glad/glad.h>

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
#include <windows.h>
#include <SDL.h>
#undef main
#include "SDL_mixer.h"
#include "SDL_opengl.h"

// SDL_ttf and freetype cannot exists together, freetype can only exists once
// somehow the SDL_ttf make use of freetype somewhere
// SDL_ttf has to be FULLY(linker, VC++ directories, both include and library) disabled in order to use freetype
// according to https://www.reddit.com/r/cpp_questions/comments/167r2ug/freetype_not_working_the_procedure_entry_point_ft/

//#include "SDL_ttf.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

using namespace glm;

using namespace std;

#pragma endregion

SDL_Renderer* renderer;

//TTF_Font* font;

#pragma region Global variables

SDL_GLContext context;
SDL_Texture* image;

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
mat4 sceneModel;
mat4 view;
mat4 projection;

// Line rendering mode
bool lineMode = false;

// models loading
bool renderRock = true;
bool render2, render3, render4, render5, render6, render7 = false;

bool movementLock = false;

bool isClayShadingOn, isNormalsShadingOn = false;

bool UpLightsOn = false;
bool DownLightsOn = true;

#pragma endregion

#pragma region Matrices for octagon room

// bind to VAO[0]
float ceilingVertices[] = { //      texture vertex           color             normals
    50.0f, 30.0f, 0.0f,             0.5f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    35.35534f, 30.0f, 35.35534f,    1.0f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 30.0f, 50.0f,             1.0f, 0.5f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -35.35534f, 30.0f, 35.35534f,   0.5f, 1.0f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -50.0f, 30.0f, 0.00f,           0.0f, 0.5f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -35.35534f, 30.0f, -35.35534f,  0.0f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 30.0f, -50.0f,            0.5f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    35.35534f, 30.0f, -35.35534f,   1.0f, 0.5f,              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 30.0f, 0.0f,              0.5f, 0.5f,  /*centre*/  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
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
    50.0f, -30.0f, 0.0f,             0.5f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    35.35534f, -30.0f, 35.35534f,    1.0f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.0f, -30.0f, 50.0f,             1.0f, 0.5f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -35.35534f, -30.0f, 35.35534f,   0.5f, 1.0f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -50.0f, -30.0f, 0.00f,           0.0f, 0.5f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -35.35534f, -30.0f, -35.35534f,  0.0f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.0f, -30.0f, -50.0f,            0.5f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    35.35534f, -30.0f, -35.35534f,   1.0f, 0.5f,              0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.0f, -30.0f, 0.0f,              0.5f, 0.5f,  /*centre*/  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f
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
    50.0f, 30.0f, 0.0f,             1.0f, 0.5f,              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    35.35534f, 30.0f, 35.35534f,    0.8536f, 0.8536f,        0.0f, 0.0f, 0.0f, 0.7071f, 0.0f, 0.7071f,
    0.0f, 30.0f, 50.0f,             0.5f, 1.0f,              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -35.35534f, 30.0f, 35.35534f,   0.1464f, 0.8536f,        0.0f, 0.0f, 0.0f, -0.7071f, 0.0f, 0.7071f,
    -50.0f, 30.0f, 0.0f,            0.0f, 1.0f,              0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -35.35534f, 30.0f, -35.35534f,  0.1464f, 0.1464f,        0.0f, 0.0f, 0.0f, -0.7071f, 0.0f, -0.7071f,
    0.0f, 30.0f, -50.0f,            0.5f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    35.35534f, 30.0f, -35.35534f,   0.8536f, 0.1464f,        0.0f, 0.0f, 0.0f, 0.7071f, 0.0f, -0.7071f,
    0.0f, 30.0f, 0.0f,              1.0f, 0.0f,              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    50.0f, -30.0f, 0.0f,            0.0f, 1.0f,              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    35.35534f, -30.0f, 35.35534f,   0.8536f, 0.1464f,        0.0f, 0.0f, 0.0f, 0.7071f, 0.0f, 0.7071f,
    0.0f, -30.0f, 50.0f,            0.5f, 0.0f,              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -35.35534f, -30.0f, 35.35534f,  0.1464f, 0.1464f,        0.0f, 0.0f, 0.0f, -0.7071f, 0.0f, 0.7071f,
    -50.0f, -30.0f, 0.0f,           1.0f, 0.0f,              0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -35.35534f, -30.0f, -35.35534f, 0.1464f, 0.8536f,        0.0f, 0.0f, 0.0f, -0.7071f, 0.0f, -0.7071f,
    0.0f, -30.0f, -50.0f,           0.5f, 1.0f,              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    35.35534f, -30.0f, -35.35534f,  0.8536f, 0.8536f,        0.0f, 0.0f, 0.0f, 0.7071f, 0.0f, -0.7071f,
    0.0f, -30.0f, 0.0f,             1.0f, 0.5f,              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
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

#pragma region freetype init

/*
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

map<char, Character> Characters;

unsigned int VAO, VBO;
*/

#pragma endregion

int main()
{
#pragma region InitLogic
    /* replace with SDL2 for better ui (at least I hope so although it didn't work, I mean text rendering part)
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

    */

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        // printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }

    window = SDL_CreateWindow("Davinci Gallery", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    // very important

    context = SDL_GL_CreateContext(window);
    if (!context) {
        cerr << "Failed to create OpenGL context: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    //Initialisation of GLAD with GLFW
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    
    //Initialisation of GLAD with SDL2
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        cerr << "GLAD failed to initialise\n";
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    else {
        cout << "GLAD successfully executed!" << endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //TTF_Init();
    //font = TTF_OpenFont("arial.ttf", 28);

    //Loading of shaders
    // CANNOT INITIALIZE OUTSIDE THE MAIN FUNCTION
    Shader Shader1("shaders/vertexShader.vert", "shaders/fragmentShader1.frag");
    //Shader Shader2("shaders/textVS.vert", "shaders/textFS.frag");

    // be careful of the order, the last line will be the one get use by the system
    Shader1.use();
    //Shader2.use();

    Model Rock("media/rock/Rock07-Base.obj");

    Model Tree("media/tree/yamaboushi_tan_6000_a_spr1.obj");

    Model Sushi("models/Crayfish_simplified.obj");
    Model Sushi2("models/sushi/sushi.obj");

    Model teapot_2("models/Teapot/Teapot_2.obj");
    Model teapot_4("models/Teapot/Teapot_4.obj");
    Model teapot_8("models/Teapot/Teapot_8.obj");

    //Sets the viewport size within the window to match the window size of 1280x720
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //// PLEASE ADD THIS LINE TO COMPLICATED 3D PROJECT!
    glEnable(GL_DEPTH_TEST);
    //// PLEASE ADD THIS LINE TO COMPLICATED 3D PROJECT!

#pragma endregion

#pragma region Models Logic

    //Model matrix
    model = mat4(1.0f);

    sceneModel = mat4(1.0f);

    //Scaling to zoom in
    //model = scale(model, vec3(4.0f, 4.0f, 4.0f));
    //Scaling to zoom in
    sceneModel = scale(sceneModel, vec3(1.0f, 1.0f, 1.0f));

    model = scale(model, vec3(1.0f, 1.0f, 1.0f));

    //Rotation to look down
    model = rotate(model, radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    //Movement to position further back
    model = translate(model, vec3(0.0f, 0.0f, 0.0f));

    sceneModel = translate(sceneModel, vec3(0.0f, 0.0f, 0.0f));

    //Projection matrix
    projection = perspective(radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Normals
    // use channel 3
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);

    //Textures
    // use channel 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Color
    // use channel 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Normals
    // use channel 3
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);

    //Textures
    // use channel 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Color
    // use channel 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Normals
    // use channel 3
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);

    //Textures
    // use channel 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Color
    // use channel 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

#pragma region init logic of freetype, sadly it won't work

    /* it won't work
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    */
#pragma endregion

#pragma region RenderLoop
    // Keep running, don't terminate so fast
    //Render loop
    // while (glfwWindowShouldClose(window) == false)
    while (true) {

        SDL_RenderClear(renderer);
        
        //Input
        // ProcessUserInput(window); //Takes user input
        Input();
        
        // I don't know what is the main issue, it kind of worked but maybe the world transform location or something else is wrong
        // I couldn't make text displaying on the screen
        // The tutorial on github is complete mess, I had to look up to tonnes of resources to solve issues
        // I had to work overnight everyday in my christmas holiday so that I can take good photos in the daytime
        // I want to sleep for eternity after the deadline 7/1/2025
        // I am too tired to find a solution so that text could be rendered
        // I don't even care anymore
        // ZZZZZZZZZZZZZZZZZZZZZZZZZ
        //RenderText(Shader2, "This is sample text", 25.0f, 25.0f, 1.0f, vec3(0.5f, 0.8f, 0.2f));
        //RenderText(Shader2, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, vec3(0.3f, 0.7f, 0.9f));
        //Shader1.use();
        SDL_Color color = { 255, 255, 255, 255 };
        //image = renderText("TTF fonts are cool!", color, 64, renderer);
        //glBindTexture(GL_TEXTURE_2D, image->id);

        if (UpLightsOn) {
            // lights at 4 ceiling corners
            Shader1.setVec3("lightPosition[0]", vec3(51.0f, 31.0f, 1.0f));
            Shader1.setVec3("lightPosition[1]", vec3(1.0f, 31.0f, 51.0f));
            Shader1.setVec3("lightPosition[2]", vec3(-51.0f, 31.0f, 1.0f));
            Shader1.setVec3("lightPosition[3]", vec3(1.0f, 31.0f, -51.0f));
        }
        if (!UpLightsOn) {
            // lights at 4 ceiling corners
            Shader1.setVec3("lightPosition[0]", vec3(0.0f, 0.0f, 0.0f));
            Shader1.setVec3("lightPosition[1]", vec3(0.0f, 0.0f, 0.0f));
            Shader1.setVec3("lightPosition[2]", vec3(0.0f, 0.0f, 0.0f));
            Shader1.setVec3("lightPosition[3]", vec3(0.0f, 0.0f, 0.0f));
        }
        

        if (DownLightsOn) {
            // lights at 4 floor corners
            Shader1.setVec3("lightPosition[4]", vec3(36.35534f, -31.0f, 36.35534f));
            Shader1.setVec3("lightPosition[5]", vec3(-36.35534f, -31.0f, 36.35534f));
            Shader1.setVec3("lightPosition[6]", vec3(-36.35534f, -31.0f, -36.35534f));
            Shader1.setVec3("lightPosition[7]", vec3(36.35534f, -31.0f, -36.35534f));
        }
        if (!DownLightsOn) {
            // lights at 4 floor corners
            Shader1.setVec3("lightPosition[4]", vec3(0.0f, 0.0f, 0.0f));
            Shader1.setVec3("lightPosition[5]", vec3(0.0f, 0.0f, 0.0f));
            Shader1.setVec3("lightPosition[6]", vec3(0.0f, 0.0f, 0.0f));
            Shader1.setVec3("lightPosition[7]", vec3(0.0f, 0.0f, 0.0f));
        }
        
        /*
        if (UpLightsOn == false && DownLightsOn == false) {
            for (int i = 0; i < 8; i++) {
                Shader1.setVec3("lightPosition[i]", vec3(1.0f, 31.0f, 1.0f));
            }
        }
        */

        Shader1.setVec3("lightColor", vec3(1.0f, 1.0f, 1.0f));

        Shader1.setVec3("materialAmbient", vec3(0.2f, 0.2f, 0.2f));
        Shader1.setVec3("materialDiffuse", vec3(0.8f, 0.8f, 0.8f));
        Shader1.setVec3("materialSpecular", vec3(5.0f, 5.0f, 5.0f));
        Shader1.setFloat("shininess", 10000.0f);
        Shader1.setVec3("viewPosition", cameraPosition);

        //Time
        // float currentFrame = static_cast<float>(glfwGetTime());
        // doesn't work anymore

        float currentFrame = SDL_GetTicks() / 1000.0f; // SDL_GetTicks() return ms
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Rendering
        glClearColor(0.25f, 0.0f, 0.0f, 1.0f); //Colour to display on cleared window
        // currently dark red

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the colour buffer

        //// ----- start of generating scene ----- ////
        //Drawing
        glBindTexture(GL_TEXTURE_2D, Buffers[Textures]);

        SetMatrices(Shader1, sceneModel);

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(Shader1.ID, ""), 1);

        glBindVertexArray(VAOs[0]); //Bind buffer object to render; VAOs[0] - ceiling
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAOs[1]); //Bind buffer object to render; VAOs[1] - floor
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glBindVertexArray(VAOs[2]); //Bind buffer object to render; VAOs[2] - walls
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // glEnable(GL_CULL_FACE); //Discards all back-facing triangles
        
        //// ----- end of generating scene ----- ////

        view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); //Sets the position of the viewer, the movement direction in relation to it & the world up direction

        if (isClayShadingOn) {
            Shader1.setInt("shadingMode", 2);
            
            // debug
            //cout << "clay shading mode is now active" << endl;
        }
        
        if (isNormalsShadingOn) {
            Shader1.setInt("shadingMode", 1);

            // debug
            //cout << "normals shading mode is now active" << endl;
        }

        if (isClayShadingOn == false && isNormalsShadingOn == false) {
            Shader1.setInt("shadingMode", 0);

            // debug
            //cout << "default shading mode is now active" << endl;
        }

        if (renderRock) {
            //Drawing
            model = mat4(1.0);
            model = scale(model, vec3(0.25f, 0.25f, 0.25f));
            SetMatrices(Shader1, model);
            Rock.Draw(Shader1);
            glActiveTexture(GL_TEXTURE0);
        }
        
        if (render2) {
            model = mat4(1.0);
            //Tree (changes MVP in relation to past values)
            model = scale(model, vec3(0.00125f, 0.00125f, 0.00125f));
            SetMatrices(Shader1, model);
            Tree.Draw(Shader1);
            glActiveTexture(GL_TEXTURE0);
        }

        //////// VERY IMPORTANT INFORMATION!!!!!!
        //////// THE VARIABLE MODEL MUST MAINTAIN THE SAME AFTER THE RENDERING OF THE LAST OBJECT
        //////// DOWNSCALING SOMETHING BY scale(model, vec3(0.05f, 0.05f, 0.05f));
        //////// MEANS YOU HAVE TO UPSCALE IT BACK TO 1.0f
        //////// WHICH MEANS ADDING THIS scale(model, vec3(20.0f, 20.0f, 20.0f)); AFTERWARDS
        
        //Rock (reorient MVP back to starting values)
        //model = scale(model, vec3(20.0f, 20.0f, 20.0f));
        //SetMatrices(Shader1, model);
        
        if (render3) {
            model = mat4(1.0);
            model = scale(model, vec3(0.125f, 0.125f, 0.125f));
            SetMatrices(Shader1, model);
            Sushi.Draw(Shader1);
        }

        if (render4) {
            model = mat4(1.0);
            model = scale(model, vec3(2.5f, 2.5f, 2.5f));
            SetMatrices(Shader1, model);
            Sushi2.Draw(Shader1);
            //glActiveTexture(GL_TEXTURE0);
        }

        if (render5) {
            model = mat4(1.0);
            model = scale(model, vec3(0.25f, 0.25f, 0.25f));
            SetMatrices(Shader1, model);
            teapot_2.Draw(Shader1);
        }
        
        if (render6) {
            model = mat4(1.0);
            model = scale(model, vec3(0.25f, 0.25f, 0.25f));
            SetMatrices(Shader1, model);
            teapot_4.Draw(Shader1);
        }
        
        if (render7) {
            model = mat4(1.0);
            model = scale(model, vec3(0.25f, 0.25f, 0.25f));
            SetMatrices(Shader1, model);
            teapot_8.Draw(Shader1);
        }

        model = scale(model, vec3(1.0f, 1.0f, 1.0f));
        SetMatrices(Shader1, model);

        //Refreshing
        //glfwSwapBuffers(window); //Swaps the colour buffer

        //replace with this
        SDL_GL_SwapWindow(window);

        // update the screen
        //SDL_RenderPresent(renderer);

        // glfwPollEvents(); //Queries all GLFW events
    }
#pragma endregion

    //Safely terminates GLFW
    // glfwTerminate();

    //Mix_FreeMusic(bgm1);

    //FT_Done_Face(face);
    //FT_Done_FreeType(ft);

    Mix_Quit();

    //replace with this
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/* replace with SDL2 logic instead
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
    if (glfwGetKey(WindowIn, GLFW_KEY_L) == GLFW_PRESS && lineMode == false)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        lineMode = true;
        Sleep(200);
    }
    else if (glfwGetKey(WindowIn, GLFW_KEY_L) == GLFW_PRESS && lineMode == true)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        lineMode = false;
        Sleep(200);
    }
}

// void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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
*/

// Control
void Input() {
    SDL_Event event;

    //Extent to which to move in one instance
    const float movementSpeed = 10.0f * deltaTime;

    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event)) {

        // Terminate the game
        if (state[SDL_SCANCODE_ESCAPE]) {
            SDL_Quit();
            exit(0);
        }

        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int newWidth = event.window.data1;
            int newHeight = event.window.data2;

            // update viewport
            glViewport(0, 0, newWidth, newHeight);
        }

        if (event.type == SDL_MOUSEMOTION) {
            int x, y;
            SDL_GetMouseState(&x, &y);

            if (mouseFirstEntry) {
                cameraLastXPos = (float)x;
                cameraLastYPos = (float)y;
                mouseFirstEntry = false;
            }

            //float xOffset = (float)x - cameraLastXPos;
            float xOffset = event.motion.x - centerX;
            //float yOffset = cameraLastYPos - (float)y;
            float yOffset = event.motion.y - centerY;

            cameraLastXPos = (float)x;
            cameraLastYPos = (float)y;

            const float sensitivity = 0.025f;
            xOffset *= sensitivity;

            // inverted y-axis control
            yOffset *= -sensitivity;

            cameraYaw += xOffset;
            cameraPitch += yOffset;

            cameraPitch = clamp(cameraPitch, -89.0f, 89.0f);

            vec3 direction;
            direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
            direction.y = sin(radians(cameraPitch));
            direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
            cameraFront = normalize(direction);

            // Reset cursor to center
            SDL_WarpMouseInWindow(window, centerX, centerY);
        }
    }

    // SDL_SCANCODE_COMMA = 54
    // SDL_SCANCODE_PERIOD = 55

    if (state[SDL_SCANCODE_COMMA]) {

    }

    if (state[SDL_SCANCODE_PERIOD]) {

    }

    if (movementLock == false) {
        if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
            cameraPosition += movementSpeed * cameraFront;
        }

        if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
            cameraPosition -= movementSpeed * cameraFront;
        }

        if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
            cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
        }

        if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
            cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
        }
    }

    // select 3d models
    if (state[SDL_SCANCODE_1]) {
        renderRock = !renderRock;
        Sleep(200);
    }

    if (state[SDL_SCANCODE_2]) {
        render2 = !render2;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_3]) {
        render3 = !render3;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_4]) {
        render4 = !render4;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_5]) {
        render5 = !render5;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_6]) {
        render6 = !render6;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_7]) {
        render7 = !render7;
        Sleep(200);
    }

    // reset camera position
    if (state[SDL_SCANCODE_R]) {
        cameraPosition = vec3(0, 0, 0);
        // ...
    }

    // orthographic view
    if (state[SDL_SCANCODE_O] && movementLock == false) {
        // max data -50.0f, 50.0f, -50.0f, 50.0f
        // projection = ortho(-50.0f, 50.0f, -50.0f, 50.0f, -1000.0f, 1000.0f);
        projection = ortho(-60.0f, 60.0f, -60.0f, 60.0f, -1000.0f, 1000.0f);
        cameraPosition = vec3(0, 0, 0);
        movementLock = true;
        Sleep(200);
    }
    else if (state[SDL_SCANCODE_O] && movementLock == true) {
        projection = perspective(radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
        movementLock = false;
        Sleep(200);
    }

    // line rendering mode
    if (state[SDL_SCANCODE_L] && lineMode == false) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        lineMode = true;
        Sleep(200);
    }
    else if (state[SDL_SCANCODE_L] && lineMode == true)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        lineMode = false;
        Sleep(200);
    }

    if (state[SDL_SCANCODE_I]) {
        UpLightsOn = !UpLightsOn;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_K]) {
        DownLightsOn = !DownLightsOn;
        Sleep(200);
    }

    if (state[SDL_SCANCODE_B]) {
        isClayShadingOn = !isClayShadingOn;
        Sleep(200);
    }
    
    if (state[SDL_SCANCODE_N]) {
        isNormalsShadingOn = !isNormalsShadingOn;
        Sleep(200);
    }
}

/*
void SetMatrices(Shader& ShaderProgramIn)
{
    mvp = projection * view * model; //Setting of MVP
    ShaderProgramIn.setMat4("mvpIn", mvp); //Setting of uniform with Shader class
}*/

void SetMatrices(Shader& ShaderProgramIn, mat4 model)
{
    ShaderProgramIn.setMat4("model", model);
    ShaderProgramIn.setMat4("view", view);
    ShaderProgramIn.setMat4("projection", projection);
}

/* neither do this
SDL_Texture* renderText(const std::string& message, 
    SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    return texture;
}
*/

/* it won't work anyway
void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    s.use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
*/