#version 460
//Triangle position with values retrieved from main.cpp
layout (location = 0) in vec3 position;

// This file and the fragment file seems no difference from previous lab(lab 7)
// but actually the layout (location = 1) must be set to 2(In this example)
// this way the correct image could be load, using location = 1 will load a wrong image
//Texture coordinates from last stage
layout (location = 2) in vec2 textureVertex;

//Model-View-Projection Matrix
uniform mat4 mvpIn;

//Texture to send
out vec2 textureFrag;

void main()
{
    //Transformation applied to vertices
    gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
    //Sending texture coordinates to next stage
    textureFrag = textureVertex;
}

/* up to lab 9 task 1
#version 460
//Triangle position with values retrieved from main.cpp
layout (location = 0) in vec3 position;
//Texture coordinates from last stage
layout (location = 1) in vec2 textureCoordinatesVertex;

//Texture coordinates to send
out vec2 textureCoordinatesFrag;

//Model-View-Projection Matrix
uniform mat4 mvpIn;

// lab 7 task 4 - disabled in lab 7 task 5
//Rotational transformation
// uniform mat4 transformIn;

void main()
{
    //Transformation applied to vertices

    // lab 7 task 3 - reuse in lab 7 task 5
    gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);

    // lab 7 task 4 - disabled in lab 7 task 5
    // gl_Position = mvpIn * transformIn * vec4(position.x, position.y, position.z, 1.0);

    //Sending texture coordinates to next stage
    textureCoordinatesFrag = textureCoordinatesVertex;
}

// up to lab 7 task 2
#version 460
//Triangle position with values retrieved from main.cpp
layout (location = 0) in vec3 position;
//Texture coordinates from last stage
layout (location = 1) in vec2 textureCoordinatesVertex;

//Texture coordinates to send
out vec2 textureCoordinatesFrag;

//Transformation
uniform mat4 transformIn;

void main()
{
    //Transformation applied to vertices
    gl_Position = transformIn * vec4(position.x, position.y, position.z, 1.0);
    //Sending texture coordinates to next stage
    textureCoordinatesFrag = textureCoordinatesVertex;
}
*/

/* up to lab 6
void main()
{
    //Triangle vertices sent through gl_Position to next stage
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    //Sending texture coordinates to next stage
    textureCoordinatesFrag = textureCoordinatesVertex;
}
*/