struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 cameraPosition : CAMERA_POSITION;
};

cbuffer ViewProjMatrixBuffer : register(b0)
{
    row_major float4x4 viewProjectionMatrix;
    float4 cameraPos;
};

cbuffer WorldMatrixBuffer : register(b1)
{
    row_major float4x4 worldMatrix;
    row_major float4x4 worldMatrixInversedTransposed;
};

VertexShaderOutput main(VertexShaderInput input)
{    
    VertexShaderOutput output;
    
    float4 pos = float4(input.position, 1.0f);
    output.position = mul(mul(pos, worldMatrix), viewProjectionMatrix);
    output.worldPosition = mul(pos, worldMatrix);
    output.normal = mul(input.normal, (float3x3) worldMatrixInversedTransposed); 
    output.uv = input.uv;
    output.cameraPosition = cameraPos.xyz;
    
    return output;
}