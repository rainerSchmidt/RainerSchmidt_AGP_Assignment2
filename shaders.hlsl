struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

Texture2D		texture0;
SamplerState	sampler0;

cbuffer CBufferTransformation
{
	matrix WVPMatrix; // 64 Bytes
};

cbuffer CBufferLight
{
	matrix LightWVPMatrix; // 64 Bytes
	float4 directional_light_vector; // 16 Bytes
	float4 directional_light_color; // 16 Bytes
	float4 ambient_light_color; // 16 Bytes
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	output.position = mul(WVPMatrix, position);
	//color.rgb = 0.7;

	output.color = color;
	output.texcoord = texcoord;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color;
}