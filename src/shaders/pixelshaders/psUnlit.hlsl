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
    int textureCount;
};

SamplerState mainSampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    return diffuse;
    //return float4(abs(input.normal), 1);
    //return float4(input.uv, 0, 1);
}