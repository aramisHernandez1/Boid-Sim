#version 330 core

struct Material  {
    sampler2D diffuse; //Diffuse and ambience are the same so dont include ambience
    sampler2D specular;
    float shine;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform Material material ;
uniform Light light;

out vec4 FragColor;

uniform vec3 viewPos;



void main()
{
    //Ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //Diffuse Lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    //Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords))

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}
