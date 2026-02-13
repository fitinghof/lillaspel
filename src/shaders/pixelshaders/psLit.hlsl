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

    float spotCosAngle;
    float4x4 vpMatrix;
};

Texture2D diffuseTexture : register(t1);
Texture2D ambientTexture : register(t2);
Texture2D specularTexture : register(t3);
Texture2D normalTexture : register(t4);

StructuredBuffer<Spotlight> spotlightBuffer : register(t0);
Texture2DArray<unorm float> shadowMaps : register(t5);

SamplerState mainSampler : register(s0);
SamplerState shadowSampler : register(s1);

float4 main(PixelShaderInput input) : SV_TARGET
{    
    float3 normal = normalize(input.normal);
    
    float4 ambientColor = ambient;  
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    float3 camToPixel = input.worldPosition.xyz - input.cameraPosition;
    
    uint numLights, stride;
    spotlightBuffer.GetDimensions(numLights, stride);
        
    // Seems structured buffer might be made larger than the number of lights, as such we do need to use ugly constant buffer
    for (int i = 0; i < spotlightCount; i++)
    {
        
        Spotlight lightdata = spotlightBuffer[i];
                
        float4 lightClip = mul(float4(input.worldPosition.xyz, 1), lightdata.vpMatrix);
        float3 ndc = lightClip.xyz / lightClip.w;
        
        float2 uv = float2(ndc.x * 0.5f + 0.5f, ndc.y * -0.5f + 0.5f);
        
        float sceneDepth = ndc.z;
        float mapDepth = shadowMaps.SampleLevel(shadowSampler, float3(uv, i), 0.f).r;
        
        const float bias = 0.000001f;
        bool islit = (mapDepth + bias) >= sceneDepth;
        
        
        float3 LightToHit = input.worldPosition.xyz - lightdata.position;
        float3 lightDir = normalize(LightToHit);
        float lightCosAngle = dot(lightDir.xyz, normalize(lightdata.direction));
        
        if (lightCosAngle > lightdata.spotCosAngle && islit)
        {
            float intensity = (1 / dot(LightToHit, LightToHit)) * max(0.0f, dot(-lightDir, normal));
    
            float3 halfWayVector = normalize(lightDir + normalize(camToPixel));
            float specularDot = max(dot(normal, -halfWayVector), 0);
            float4 lighting = lit(intensity, specularDot, shininess);
            
            diffuseColor += lighting.y * lightdata.color;
            specularColor += lighting.z * lightdata.color;
        }
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