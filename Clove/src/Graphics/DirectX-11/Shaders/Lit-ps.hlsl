Texture2D diffuseTexture : register(t1);
SamplerState diffuseSampler : register(s1);

Texture2D specularTexture : register(t2);
SamplerState specularSampler : register(s2);

struct Light{
	float3 position;

	float3 ambient;
	float3 diffuse;
	float3 specular;

	float constant;
	float linearV;
	float quadratic;
};
cbuffer PointLightBuffer : register(b1){
	Light lights[100]; //100 max for now
	int numLights;
};

cbuffer ViewBuffer : register(b2){
    float3 viewPos;
}

cbuffer MaterialBuffer : register(b4){
    float shininess;
}

float4 main(float2 texCoord : TexCoord, float3 vertPos : VertPos, float3 vertNormal : VertNormal) : SV_Target{
    float3 normal       = normalize(vertNormal);
    float3 viewDir      = normalize(viewPos - vertPos);
    
	float3 lighting;
	for(int i = 0; i < numLights; ++i){
		float3 lightDir = normalize(lights[i].position - vertPos);

		//Ambient
		float3 lambient = lights[i].ambient * (float3)diffuseTexture.Sample(diffuseSampler, texCoord);

		//Diffuse
		float3 diff = max(dot(normal, lightDir), 0.0f);
		float3 ldiffuse = lights[i].diffuse * diff * (float3)diffuseTexture.Sample(diffuseSampler, texCoord);

		//Specular
		float3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
		float3 lspecular = lights[i].specular * spec * (float3)specularTexture.Sample(specularSampler, texCoord);

		//Attenuation
		float distance = length(lights[i].position - vertPos);
		float attenuation = 1.0f / (lights[i].constant + (lights[i].linearV * distance) + (lights[i].quadratic * (distance * distance)));

		lambient *= attenuation;
		ldiffuse *= attenuation;
		lspecular *= attenuation;

		lighting += (lambient + ldiffuse + lspecular);
	}

    return float4(lighting, 1.0f);
}