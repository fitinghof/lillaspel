#include "light.hlsl"

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 cameraPosition : CAMERA_POSITION;
};

cbuffer SpotlightCountBuffer : register(b0)
{
    int spotlightCount;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    int textureSlots;
};

struct Spotlight
{
    float3 position;
    float3 direction;

    float4 color;
    float intensity;

    float spotAngle;
};

Texture2D diffuseTexture : register(t1);
Texture2D ambientTexture : register(t2);
Texture2D specularTexture : register(t3);
Texture2D normalTexture : register(t4);

StructuredBuffer<Spotlight> spotlightBuffer : register(t0);

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
    
    
    // Read textures
    // It's done in a way to avoid if-statements
    
    float2 uv = float2(input.uv.x, 1 - input.uv.y);
    
    float hasDiffuse = (textureSlots & 1) != 0;
    float hasAmbient = (textureSlots & 2) != 0;
    float hasSpecular = (textureSlots & 4) != 0;
    //float hasNormal = (textureSlots & 8) != 0;

    float4 diffuseSample = diffuseTexture.Sample(mainSampler, uv);
    float4 ambientSample = ambientTexture.Sample(mainSampler, uv);
    float4 specularSample = specularTexture.Sample(mainSampler, uv);
    //float4 normalSample = normalTexture.Sample(mainSampler, uv);

    float4 diffuseTextureColor =
    lerp(float4(1, 1, 1, 1), diffuseSample, hasDiffuse);

    // Ambient falls back to diffuse if there is no dedicated ambient texture
    float4 ambientTextureColor =
    lerp(diffuseTextureColor, ambientSample, hasAmbient); 

    float4 specularTextureColor =
    lerp(float4(1, 1, 1, 1), specularSample, hasSpecular);

    float4 color = diffuseTextureColor * diffuseColor + ambientTextureColor * ambientColor + specularTextureColor * specularColor;
    
    return color;
}