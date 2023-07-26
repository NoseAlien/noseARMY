#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(mat_mvp, pos);
	output.worldPosition = mul(mat_world, pos);
	output.normal = mul(mat_world_rot, normal);
	output.uv = uv;
	return output;
}