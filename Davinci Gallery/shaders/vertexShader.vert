#version 460

//Model-View-Projection Matrix
uniform mat4 mvpIn;

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
    normals = mat3(transpose(inverse(model))) * normal;
    //normals = normalize(mat3(transpose(inverse(model))) * normal);

    FragPos = vec3(model * vec4(position, 1.0));
    
    //Transformation applied to vertices
    //gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0);
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