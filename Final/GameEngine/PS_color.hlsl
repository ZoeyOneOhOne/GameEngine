//This is what the vertex shader outputs
//and the pixel shader receives from there
struct VPosColToPixel
{
	float4 position : SV_POSITION;//System Value Position
	float4 color	: COLOR;
};

//This calculates the final color for this pixel
//based on all the data we've taken in 
float4 main(VPosColToPixel input) : SV_TARGET
{
	return input.color;
}