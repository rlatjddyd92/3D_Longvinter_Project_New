
/* float2 float3 float4 == vector */

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
bool g_Istransparency = true;
float g_TransAlpah = 1.f;
//texture2D g_DiffuseTexture;

sampler LinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    vector Color : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    vector Color : COLOR;
};

VS_OUT VS_MAIN( /*Á¤Á¡*/VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.Color = In.Color;

    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In, VS_OUT VsOut)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = VsOut.Color;
    
    //Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    Out.vColor.a = g_TransAlpah;
    
    if (0.3f >= Out.vColor.a)
        discard;

    return Out;
}



technique11 DefaultTechnique
{
    pass Terrain
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}