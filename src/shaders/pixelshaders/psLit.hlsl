#include "light.hlsl"

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

Texture2D mainTexture : register(t0);

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
        
    float4 textureColor = mainTexture.Sample(mainSampler, float2(input.uv.x, 1 - input.uv.y));
    return textureColor * (ambientCol + diffuseCol + specularCol);
}