cbuffer SimpleCBuffer : register(b0)
{
	float4x4 objectTransform;
	float4x4 viewProjection;
	float4x4 objectInverseTranspose;
	float4 objectColor;

	float3 lightDirection;
	float gameTime;

	float3 cameraPosition; 
	float pad1;

	float2 textureScale;
	float pointLightRange;
	float pointLightIntensity;

	float3 pointLightPos;
	float pad2;

	float3 pointLightAttenuation;
}

struct VPosColNormInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
};

struct VSkyboxToPixel
{
    float4 position : SV_POSITION;
    float3 localPos : POSITION;
};

struct VPosColNormToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
};

struct VPosTexNormInput
{
	float3 position		: POSITION;
	float2 texCoords    : TEXCOORD0;
	float3 normal		: NORMAL;
};

struct VPosTexNormToPixel
{
	float4 position		: SV_POSITION;
	float2 texCoords    : TEXCOORD0;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
};

struct VPosTexNormTangentInput
{
	float3 position		: POSITION;
	float2 texCoords    : TEXCOORD0;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
};

struct VPosTexNormTangentToPixel
{
	float4 position		: SV_POSITION;
	float2 texCoords    : TEXCOORD0;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
};

typedef VPosTexNormTangentInput StandardVertInput;
typedef VPosTexNormTangentToPixel StandardVertToPixel;