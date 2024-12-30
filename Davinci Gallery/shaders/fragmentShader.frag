#version 460

out vec4 FragColor;

in vec2 textureFrag;
in vec3 vertexColor;
in vec3 normals;
in vec3 FragPos;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 viewPosition;

uniform sampler2D texture_diffuse1;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float shininess;

void main()
{
    vec3 ambient = lightColor * materialAmbient;

    //vec3 norm = normalize(normals);
    vec3 lightDir = normalize(lightPosition - FragPos);

    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * lightColor * materialDiffuse;

    vec3 viewDir = normalize(viewPosition - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //vec3 specular = spec * lightColor * materialSpecular;

    //vec3 result = ambient + diffuse + specular;
    vec3 result = ambient;

    vec4 texColor = texture(texture_diffuse1, textureFrag);
    //FragColor = texColor * vec4(result, 1.0);
    FragColor = texColor;
}

/*
#version 460
//Colour value to send to next stage
out vec4 FragColor;

//Texture coordinates from last stage
in vec2 textureFrag;    // for gallery environment
//in vec3 vertexColor;

in vec3 normals;

uniform sampler2D texture_diffuse1;

void main()
{
    //Setting of colour coordinates to colour map
    
    //FragColor = texture(texture_diffuse1, textureFrag) + vec4(vertexColor, 1.0);
    FragColor = texture(texture_diffuse1, textureFrag);

    
}

#version 460
//Colour value to send to next stage
out vec4 FragColor;

//Texture coordinates from last stage
in vec2 textureCoordinatesFrag;
//Texture (colours)
uniform sampler2D textureIn;

void main()
{
    //Setting of texture & its coordinates as colour map
    FragColor = texture(textureIn, textureCoordinatesFrag);
}
*/