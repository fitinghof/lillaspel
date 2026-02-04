
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 cameraPosition : CAMERA_POSITION;
};

float4 main(PixelShaderInput input) : SV_TARGET
{	
    return float4(abs(input.normal), 1);
    //return float4(input.uv, 0, 1);
}