#version 430

layout(location = 0) out vec3 FragColor;

// From Vertex Shader
in vec3 v_position;
in vec2 v_uv;
in vec3 v_normal;

// Texture
uniform sampler2D u_Texture;

// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

// Lights
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light; 

// Directional Light
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	FragColor = texture(u_Texture, v_uv).rgb;
}

// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // combine results
//     vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_uv));
//     vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_uv));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, v_uv));
//     return (ambient + diffuse + specular);
// }  