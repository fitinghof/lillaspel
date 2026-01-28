
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 vertexWorldPosition : VERTEX_WORLD_POSITION;
    float4 normal : NORMAL;
    float2 texcord : TEXCOORD;
};

float4 main(PixelShaderInput input) : SV_TARGET
{	
    return float4(0, 1, 0, 1);
}