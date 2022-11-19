#version 430
#extension GL_ARB_bindless_texture : enable

layout(location = 0) out vec3 FragColor;

// From Vertex Shader
in vec3 v_position;
in vec2 v_uv;
in vec3 v_normal;

// Texture
uniform sampler2D u_texture;
uniform sampler2D u_normalMap;
uniform int u_useNormalMap;

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

// Point Light
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    float intensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isActive;
};  
#define NR_POINT_LIGHTS 100
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Camera
uniform vec3 u_viewPos;

/***********
    Main
************/
void main()
{
    vec3 result = vec3(0.3f);
    vec3 viewDir = normalize(u_viewPos - v_position);

    vec3 norm = normalize(v_normal);
    // normal map
    if(u_useNormalMap == 1)
    {
        norm = texture(u_normalMap, v_uv).rgb;
        norm = normalize(norm * 2.0 - 1.0);
    }

    // Point Lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if (pointLights[i].isActive != 0)
            result += CalcPointLight(pointLights[i], norm, v_position, viewDir);
    }

    result = material.ambient * texture(u_texture, v_uv).rgb * result;

    if(result == vec3(0.f))
    {
	    FragColor = material.ambient * texture(u_texture, v_uv).rgb * vec3(.3f);
    }
    else
    {
        FragColor = result;
    }
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
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  = ambient * attenuation * light.intensity;
    diffuse  = diffuse * attenuation * light.intensity;
    specular = specular * attenuation * light.intensity;
    return (ambient + diffuse + specular);
} 