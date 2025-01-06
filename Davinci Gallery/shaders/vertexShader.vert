#version 460

// use the structure below, it is good for your health
// who would calculate everything before you actually import into the vertex shader????
// you don't know all the parameters
//Model-View-Projection Matrix
//uniform mat4 mvpIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//// IMPORTANT!!
//// IN ORDER TO MAKE THE IMPORTED MODEL WORK IN THE WAY THEY MEANT TO BE
//// DATA MUST BE IMPORTED ACCORDING TO THIS ORDER
//// DUE TO THE DEFAULT DATA MANGEMENT ORDER IN THOSE .mtl FILE
//// POSITION, NORMALS, TEXTURE VERTEX/COORDINATES, COLOR

//Triangle position with values retrieved from main.cpp
layout (location = 0) in vec3 position;

layout (location = 1) in vec3 normal;

//Texture coordinates from last stage
layout (location = 2) in vec2 textureVertex;

layout (location = 3) in vec3 color;

//Texture to send
out vec2 textureFrag;
out vec3 vertexColor;
out vec3 normals;
out vec3 FragPos;

void main()
{
    
    //Sending texture coordinates to next stage
    textureFrag = textureVertex;
    vertexColor = color;

    // calculate normals
    normals = mat3(transpose(inverse(model))) * normal;

    FragPos = vec3(model * vec4(position, 1.0));
    
    //Transformation applied to vertices
    //gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}