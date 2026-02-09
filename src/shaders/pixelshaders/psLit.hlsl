#include "light.hlsl"

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 cameraPosition : CAMERA_POSITION;
};

cbuffer SptolightCountBuffer : register(b0)
{
    int spotlightCount;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    int textureCount;
};

struct Spotlight
{
    float3 position;
    float3 direction;

    float4 color;
    float intensity;

    float spotAngle;
};

Texture2D diffuseTexture : register(t0);

StructuredBuffer<Spotlight> spotlightBuffer : register(t1);

SamplerState mainSampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    
    float4 ambientColor = ambient;  
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    
    for (int i = 0; i < spotlightCount; i++)
    {
        // Light falloff on distance
        float surfaceLightIntensity = CalculateLightFalloff(input.worldPosition.xyz, spotlightBuffer[i].position, spotlightBuffer[i].intensity); // light at surface
    
        // Diffuse component
        diffuseColor += DiffuseComponent(input.worldPosition.xyz, spotlightBuffer[i].position, normal, diffuse, surfaceLightIntensity, spotlightBuffer[i].color);
        
        // Specular component
        specularColor += BlinnPhongSpecularComponent(input.worldPosition.xyz, spotlightBuffer[i].position, input.cameraPosition, normal, specular, shininess, surfaceLightIntensity, spotlightBuffer[i].color);
    }
    
    // Goofy way to allow for textureless materials without using an if-statement
    float4 textureColor = float4(1, 1, 1, 1);
    textureColor += min(textureCount, 1) * (diffuseTexture.Sample(mainSampler, float2(input.uv.x, 1 - input.uv.y)) - float4(1, 1, 1, 1));
        
    // Clamp color between 0 and 1
    return min(textureColor * (ambientColor + diffuseColor) + specularColor, 1);
}