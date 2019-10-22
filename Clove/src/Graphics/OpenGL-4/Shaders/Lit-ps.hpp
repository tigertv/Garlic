//cmake generated header file

static const std::string shader_Lit_ps = R"(
#version 460 core

in vec2 vertTexCoord;
in vec3 vertPos;
in vec3 vertNormal;

layout(binding = 1) uniform sampler2D albedoSampler;
layout(binding = 2) uniform sampler2D specularSampler;

layout(std140, binding = 4) uniform Material{
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
	vec3 position;
   
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic; 
};

struct SpotLight{
	vec3 position;
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float cutOff;
	float outerCutOff;
};

layout (std140, binding = 1) uniform PointLightData{
	int numLights;
	PointLight lights[10]; //10 max for now
};

layout (std140, binding = 2) uniform ViewData{
	vec3 viewPos;
};

layout(location = 0) out vec4 fragmentColour;

vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
//vec3 CalculateSpotLight(SpotLight light,  vec3 normal, vec3 fragPos, vec3 viewDirection);

void main(){
	vec3 fragNorm		= normalize(vertNormal);
	vec3 viewDir		= normalize(viewPos - vertPos);

	vec3 lighting;

//	for(int i = 0; i < NUM_DIR_LIGHTS; i++){
//		lighting += CalculateDirectionalLighting(directionLights[i], fragNorm, viewDir);
//	}

	for(int i = 0; i < numLights; i++){
		lighting += CalculatePointLight(lights[i], fragNorm, vertPos, viewDir);
	}

	fragmentColour = vec4(lighting, 1.0);
};

vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDirection){
	vec3 lightDir			= normalize(-light.direction); //vec pointing away from light source

	//Ambient
	vec3 ambient			= light.ambient * vec3(texture(albedoSampler, vertTexCoord));
	
	//Diffuse
	float diff				= max(dot(normal, lightDir), 0.0);
	vec3 diffuse			= light.diffuse * diff * vec3(texture(albedoSampler, vertTexCoord));

	//Specular
	vec3 reflectDirection	= reflect(-lightDir, normal);
	float spec				= pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
	vec3 specular			= light.specular * spec * vec3(texture(specularSampler, vertTexCoord));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection){
	vec3 lightDir			= normalize(light.position - fragPos); //vec pointing towards light source

	//Ambient
	vec3 ambient			= light.ambient * vec3(texture(albedoSampler, vertTexCoord));
	
	//Diffuse
	float diff				= max(dot(normal, lightDir), 0.0);
	vec3 diffuse			= light.diffuse * diff * vec3(texture(albedoSampler, vertTexCoord));

	//Specular
	vec3 reflectDirection	= reflect(-lightDir, normal);
	float spec				= pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
	vec3 specular			= light.specular * spec * vec3(texture(specularSampler, vertTexCoord));

	//Attenuation
	float distance			= length(light.position - fragPos);
	float attenuation		= 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	return (ambient + diffuse + specular);
}

//vec3 CalculateSpotLight(SpotLight light,  vec3 normal, vec3 fragPos, vec3 viewDirection){
//	vec3 lightDir			= normalize(light.position - fragPos); //vec pointing towards light source
//
//	//Ambient
//	vec3 ambient			= light.ambient * vec3(texture(material.diffuse, vertTexCoord));
//	
//	//Diffuse
//	float diff				= max(dot(normal, lightDir), 0.0);
//	vec3 diffuse			= light.diffuse * diff * vec3(texture(material.diffuse, vertTexCoord));
//
//	//Specular
//	vec3 reflectDirection	= reflect(-lightDir, normal);
//	float spec				= pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
//	vec3 specular			= light.specular * spec * vec3(texture(material.specular, vertTexCoord));
//
//	//Spot light circle
//	float theta				= dot(lightDir, normalize(-light.direction));
//	float epsilon			= light.cutOff - light.outerCutOff;
//	float intensity			= clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
//
//  diffuse  *= intensity;
//  specular *= intensity;
//
//	return (ambient + diffuse + specular);
//}
)";