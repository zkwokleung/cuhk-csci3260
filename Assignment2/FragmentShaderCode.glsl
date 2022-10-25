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
    float intensity;
  
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

    float intensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isActive;
};  
#define NR_POINT_LIGHTS 10
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Spot light
struct SpotLight {
    vec3 position;

    vec3 direction;
    float cutOff;
    float innerCutOff;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_SPOT_LIGHTS 1
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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
    {
        if (pointLights[i].isActive != 0)
            result += CalcPointLight(pointLights[i], norm, v_position, viewDir);
    }

    // Spot Lights
    // for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    //     result += CalcSpotLight(spotLights[i], norm, v_position, viewDir);
    // result += CalcSpotLight(spotLight, norm, v_position, viewDir);

	FragColor = material.ambient * texture(u_Texture, v_uv).rgb * result;
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
    vec3 ambient  = light.intensity * light.ambient  * material.diffuse;
    vec3 diffuse  = light.intensity * light.diffuse  * material.diffuse;
    vec3 specular = light.intensity * light.specular * spec * material.specular;
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
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  = ambient * attenuation * light.intensity;
    diffuse  = diffuse * attenuation * light.intensity;
    specular = specular * attenuation * light.intensity;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float nDotVP;       // normal * light direction
	float nDotR;        // normal * light reflection vector
	float pf=0;           // power factor
	float spotDot;      // cosine of angle between spotlight
	float spotAttenuation;     // spotlight attenuation factor;
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light position
	vec3 VP;            // direction from surface to light position
	vec3 reflection;    // direction of maximum highlights

	// Compute vector from surface to light position
	VP = vec3 (light.position) - fragPos;

	// Compute distance between surface and light position
	d = length (VP);

	// Normalize the vector from surface to light position
	VP = normalize (VP);

	// Compute attenuation
    attenuation = 1.f / (light.constant +
                        light.linear * d +
                        light.quadratic * d * d);

    // See if point on surface is inside cone of illumination
	spotDot = dot (-VP, normalize (light.direction));

	if (spotDot < light.cutOff)
	{
		spotAttenuation = 0.f;
	}
	else
	{
		// we are going to ramp from the outer cone value to the inner using
		// smoothstep to create a smooth value for the falloff
		float spotValue=smoothstep(light.cutOff,light.innerCutOff,spotDot);
		spotAttenuation = pow(spotValue, 2);
	}

	// Combine the spot and distance attenuation
    attenuation *= spotAttenuation;
    // calc the reflection for the highlight
	reflection = normalize (reflect (-normalize (VP), normalize(v_normal)));

	nDotVP = max (0.f, dot (v_normal, VP));
	nDotR = max (0.f, dot (normalize (v_normal), reflection));
	pf=clamp(nDotVP,0.0,pow (nDotR, material.shininess));
	// combine the light / material values
    vec3 ambient = material.diffuse * light.ambient * attenuation;
    vec3 diffuse = material.diffuse * light.diffuse * nDotVP * attenuation;
    vec3 specular = material.specular * light.specular * pf * attenuation;

	return ambient + diffuse + specular;
}