//High Level Shader Language - HLSL
cbuffer SimpleCBuffer : register(b0)
{
	float4x4 objectTransform;//Where the object is
	float4x4 viewProjection;//Where the camera is
	float4	 objectColor;
}

//vertex position color input
//represents each individual passed in
struct VPosColInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
};

//this reperesents the data we output from this shader
//and recieve in the pixel shader
struct VPosColToPixel
{
	float4 position		: SV_POSITION;//System Value Position
	float4 color		: COLOR;
};

VPosColToPixel main(VPosColInput input)
{
	VPosColToPixel output;
		
	output.position = mul(float4(input.position, 1), mul(objectTransform, viewProjection));

	output.color = input.color * objectColor;
	output.color.a = 1;
	//output.color.rgba = 1;//swizzle (using more than one value in one line) 
	//output.color.xyzw = 1; 
	
	return output;
}