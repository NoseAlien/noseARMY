cbuffer cbuff0 : register(b0)
{
	matrix mat_world;
	matrix mat_world_rot;
	matrix mat_mvp;
	float3 cameraworldpos;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
	float m_lightswitch : packoffset(c3.x);
	float m_toonswitch : packoffset(c3.y);
	float m_limswitch : packoffset(c3.z);
};

//頂点シェーダーの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	float4 svpos : SV_POSITION; //システム用頂点座標
	float3 worldPosition : TEXCOORD1;
	float3 normal : NORMAL; //法線ベクトル
	float3 worldNormal : TEXCOORD2;
	float2 uv : TEXCOORD; //uv値
};