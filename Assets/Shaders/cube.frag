#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform bool unlit = false;

void main()
{
    if (unlit) {
        FragColor = vec4(objectColor, 1.0);
    } else {
        // Ambient
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
      	
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
                
        vec3 result = (ambient + diffuse) * objectColor;
        FragColor = vec4(result, 1.0);
    }
}
