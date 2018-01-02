struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D		texture0;
SamplerState	sampler0;

cbuffer CBufferTransformation
{
	matrix WVPMatrix; // 64 Bytes
};

cbuffer CBufferLight
{
	float4 directional_light_vector; // 16 Bytes
	float4 directional_light_color; // 16 Bytes
	float4 ambient_light_color; // 16 Bytes
};

VOut VShader(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	output.position = mul(WVPMatrix, position);

	float diffuseAmount = dot(directional_light_vector, normal);
	diffuseAmount = saturate(diffuseAmount);

	output.color = ambient_light_color + (directional_light_color * diffuseAmount);
	
	//output.color = ambient_light_color;

	output.texcoord = texcoord;

	output.normal = normal;
	

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL) : SV_TARGET
{
	/*float diffuseAmount = dot(directional_light_vector, normal);
	diffuseAmount = saturate(diffuseAmount);

	return (color + (directional_light_color * diffuseAmount))*texture0.Sample(sampler0, texcoord);*/

	return color * texture0.Sample(sampler0, texcoord);
}