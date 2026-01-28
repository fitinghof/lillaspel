struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcord : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 vertexWorldPosition : VERTEX_WORLD_POSITION;
    float4 normal : NORMAL;
    float2 texcord : TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.position = float4(input.position, 1);
    output.vertexWorldPosition = float4(input.position, 1);
    output.normal = float4(input.normal, 1);
    output.texcord = input.texcord;
    
    return output;
}