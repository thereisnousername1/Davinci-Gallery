#version 460

// retrieve data from main program
uniform int shadingMode; // 0: flat, 1: clay, 2: graphite

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;
uniform sampler2D texture_diffuse1;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float shininess;

// take from vertex shader's out
in vec2 textureFrag;
in vec3 vertexColor;
in vec3 normals;
in vec3 FragPos;

// final output to viewport
out vec4 FragColor;

void main()
{
    vec3 ambient = 1 * lightColor * materialAmbient;

    vec3 normalizedNormal = normalize(normals);
    vec3 lightDir = normalize(lightPosition - FragPos);

    float diff = max(dot(normalizedNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * materialDiffuse;

    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalizedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor * materialSpecular;
    
    vec3 result = ambient + diffuse + specular;

    if (shadingMode == 1) {

        // flat shading
        //FragColor = vec4(abs(normalizedNormal), 1.0);
        FragColor = vec4(vertexColor, 1.0);
    
    }
    else if (shadingMode == 2) {

        // clay shading
        float diff = max(dot(normalizedNormal, lightDir), 0.0);

        //                    dimmer light          color of clay
        vec3 diffuse = diff * vec3(1.0f, 0.9f, 0.8f) * vec3(0.8, 0.7, 0.6);
        //FragColor = vec4(diffuse, 1.0);
        FragColor = vec4(result, 1.0);

    }
    else if (shadingMode == 3) {

        // graphite shading
        //float intensity = dot(normalizedNormal, vec3(0.0, 0.0, 1.0));
        //vec4 textureColor = texture(graphiteTexture, textureFrag * 10.0);
        //FragColor = vec4(textureColor.rgb * intensity, 1.0);

    }
    else if (shadingMode == 0){

        // default
        vec4 texColor = texture(texture_diffuse1, textureFrag);
        FragColor = texColor * vec4(result, 1.0);

    }
}