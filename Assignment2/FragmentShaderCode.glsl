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

// Point Light
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Camera
uniform vec3 u_viewPos;

/***********
    Main
************/
void main()
{
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(u_viewPos - v_position);

    // Dir light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // Point Lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, v_position, viewDir);

	FragColor = texture(u_Texture, v_uv).rgb * result;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    // vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_uv));
    // vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_uv));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, v_uv));
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    // vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_uv));
    // vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_uv));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, v_uv));
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 