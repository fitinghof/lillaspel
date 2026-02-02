struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 cameraPosition : CAMERA_POSITION;
};

cbuffer MaterialBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
};

struct Spotlight
{
    float3 position;
    float3 direction;

    float4 color;
    float intensity;

    float spotAngle;
};

float CalculateLightFalloff(float3 fragPosition, float3 lightPosition, float intensity)
{
    float3 vecToLightUnnormalized = lightPosition - fragPosition;
    return intensity * 1 / dot(vecToLightUnnormalized, vecToLightUnnormalized);
}

float4 BlinnPhongSpecularComponent(float3 fragPosition, float3 lightPosition, float3 cameraPosition, float3 normal, float4 specularCol, float shininess, float surfaceLightIntensity, float4 lightColor)
{
    float3 vecToLight = normalize(lightPosition - fragPosition);

    float3 vecToCam = normalize(cameraPosition - fragPosition);
    float3 halfVector = normalize(vecToCam + vecToLight);
    float spec = pow(max(dot(normal.xyz, halfVector), 0.0f), shininess);
    return specularCol * lightColor * surfaceLightIntensity * spec;
}

float4 DiffuseComponent(float3 fragPosition, float3 lightPosition, float3 normal, float4 diffuseCol, float surfaceLightIntensity, float4 lightColor)
{
    float3 vecToLight = normalize(lightPosition - fragPosition);
    
    return diffuseCol * lightColor * surfaceLightIntensity * max(dot(normal, vecToLight), 0);
}

StructuredBuffer<Spotlight> spotlightBuffer : register(t1);

SamplerState mainSampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    
    float4 ambientCol = ambient;  
    float4 diffuseCol = 0;
    float4 specularCol = 0;
    
    // Light falloff on distance
    float surfaceLightIntensity = CalculateLightFalloff(input.worldPosition.xyz, spotlightBuffer[0].position, spotlightBuffer[0].intensity); // light at surface
    
    // Diffuse component
    diffuseCol += DiffuseComponent(input.worldPosition.xyz, spotlightBuffer[0].position, normal, diffuse, surfaceLightIntensity, spotlightBuffer[0].color);
        
    // Specular component
    specularCol += BlinnPhongSpecularComponent(input.worldPosition.xyz, spotlightBuffer[0].position, input.cameraPosition, normal, specular, shininess, surfaceLightIntensity, spotlightBuffer[0].color);
    
    return ambientCol + diffuseCol + specularCol;
}