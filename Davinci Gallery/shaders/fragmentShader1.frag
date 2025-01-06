#version 460

#define NUM_OF_LIGHTS 8
// retrieve data from main program
uniform int shadingMode; // 0: flat, 1: clay, 2: graphite

// I don't want too many lights
uniform vec3 lightPosition[NUM_OF_LIGHTS];
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

vec3 result;

// final output to viewport
out vec4 FragColor;

void main()
{
    vec3 ambient = 0.1 * lightColor * materialAmbient;

    vec3 normalizedNormal = normalize(normals);

    for (int i = 0; i < NUM_OF_LIGHTS; i++) {

        vec3 lightDir = normalize(lightPosition[i] - FragPos);
        float diff = max(dot(normalizedNormal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * materialDiffuse;

        vec3 viewDir = normalize(viewPosition - FragPos);
        vec3 reflectDir = reflect(-lightDir, normalizedNormal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * lightColor * materialSpecular;
    
        result += ambient + diffuse + specular;
    }

    if (shadingMode == 1) {

    /*
        // flat shading - testing
        vec3 result1;
        for (int i = 0; i < NUM_OF_LIGHTS; i++) {

            vec3 lightDir = normalize(lightPosition[i] - FragPos);
            float diff = max(dot(normalizedNormal, lightDir), 0.0);
            //                    dimmer light             color of clay
            vec3 diffuse = diff * vec3(0.8f, 0.9f, 1.0f) * vec3(0.355, 0.142, 0.106);

            vec3 viewDir = normalize(viewPosition - FragPos);
            vec3 reflectDir = reflect(-lightDir, normalizedNormal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = (0.1 * spec) * lightColor * (materialSpecular/5);
    
            result1 += specular;
        }

        //FragColor = vec4(diffuse, 1.0);
        FragColor = vec4(result1, 1.0);
    */
        
        // normals shading
        FragColor = vec4(normalizedNormal, 1.0);

    }
    else if (shadingMode == 2) {

        // clay shading
        vec3 result2;

        for (int i = 0; i < NUM_OF_LIGHTS; i++) {

            vec3 lightDir = normalize(lightPosition[i] - FragPos);
            float diff = max(dot(normalizedNormal, lightDir), 0.0);
            //                    dimmer light             color of clay
            vec3 diffuse = diff * vec3(0.8f, 0.9f, 1.0f) * vec3(0.455, 0.142, 0.106);

            vec3 viewDir = normalize(viewPosition - FragPos);
            vec3 reflectDir = reflect(-lightDir, normalizedNormal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = (0.01 * spec) * lightColor * (materialSpecular/5);
    
            result2 += ambient + diffuse + specular;
        }

        //FragColor = vec4(diffuse, 1.0);
        FragColor = vec4(result2, 1.0);

    }
    else if (shadingMode == 3) {

        // graphite shading - testing
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