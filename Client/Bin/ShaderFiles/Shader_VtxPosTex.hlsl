
/* float2 float3 float4 == vector */
/* float1x3, float3x3, float1x3, float4x4 == matrix */

/* VertexShader */
/* �����ϳ��� � ��ȯ�� ���İ��� �Ǵ����� ���� ������ ���� ����. */
/* ���� ���̴��� ���� �ϳ��� �޾ƿ���. ������ ��ģ�� ������ ��ģ ������ �������ش�. */
/* �簢�����۸� �׸���. 6��ȣ��. */
// VS_MAIN(/*����*/float3 vPosition  : POSITION, float2 vTexcoord : TEXCOORD0)
//{
//
//}

#include "Shader_Engine_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
bool g_IsChange[3] = { false, false, false };
float g_ChangeColor[3] = { 0.f, 0.f, 0.f };
bool g_Istransparency = false;
float g_TransAlpah = 1.f;
texture2D g_Texture;
texture2D g_DepthTexture;




struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
	
};

struct VS_OUT
{
	/* SV_ : ShaderValue */
	/* ���� �ؾ��� ������ ��� ���ĳ��� ���̹Ƿ� ���� dx�� �߰������� �ؾ��� �̸��� �ض�. */
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
	
};


struct VS_OUT_EFFECT
{
	/* SV_ : ShaderValue */
	/* ���� �ؾ��� ������ ��� ���ĳ��� ���̹Ƿ� ���� dx�� �߰������� �ؾ��� �̸��� �ض�. */
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

/* 1. ������ ��ȯ������ �����Ѵ�. */
/* 2. ������ ���� ������ �����Ѵ�. */
VS_OUT VS_MAIN( /*����*/VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	/* ������ ��ġ�� ���� �� ������ȯ�Ѵ�.*/		
	/* Ŭ���̾�Ʈ���� ���������� �����ϴ� TrnasformCoord�Լ��ʹ� �ٸ��� */
	/* mul�Լ��� ��쿡�� �����ϰ� ����� ���ϱ⸸ ������ �ϰ� w�����⿬����ü�� �������� �ʴ´�. */
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN In)
{
    VS_OUT_EFFECT Out = (VS_OUT_EFFECT) 0;

	/* ������ ��ġ�� ���� �� ������ȯ�Ѵ�.*/
	/* Ŭ���̾�Ʈ���� ���������� �����ϴ� TrnasformCoord�Լ��ʹ� �ٸ��� */
	/* mul�Լ��� ��쿡�� �����ϰ� ����� ���ϱ⸸ ������ �ϰ� w�����⿬����ü�� �������� �ʴ´�. */
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = vPosition;

    return Out;
}


/* Triangle : ���� ������ �� vs_main�� ����Ҷ����� ��� */
/* ������ ��δ� ����Ǹ�. ���� ������ ����. */
/* ���ϵ� ������ w�� ������ xyzw�� ������. ���� */
/* ������ ��ġ�� ����Ʈ�� ��ȯ��. (��������ǥ�� ��ȯ�Ѵ�)*/
/* �����Ͷ����� : ���������� ��������Ͽ� �ȼ��� ���������. */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_IN_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

vector Sample(float2 vTexcoord)
{
    return g_Texture.Sample(LinearSampler, vTexcoord);
}

/* 1. �ȼ��� �������� ������ �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
  
    
    if (g_IsChange[0])
        Out.vColor.r = g_ChangeColor[0];
    
    if (g_IsChange[1])
        Out.vColor.g = g_ChangeColor[1];
    
    if (g_IsChange[2])
        Out.vColor.b = g_ChangeColor[2];
    
    if (g_Istransparency)
        Out.vColor.a = g_TransAlpah;
    
    if (0.1 >= Out.vColor.a)
        discard;
    
    return Out;
}

PS_OUT PS_MAIN_EFFECT(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = (float2) 0.f;

    vTexcoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexcoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	/* ���� ����Ʈ �ȼ��� �׸������ϴ� ��ġ�� ȭ������� ���������� ���̸� ���´�. */
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexcoord);
    float fOldViewZ = vDepthDesc.y * 1000.f;

    float fViewZ = In.vProjPos.w;

    Out.vColor = Sample(In.vTexcoord);



    Out.vColor.a = Out.vColor.a * saturate(fOldViewZ - fViewZ);

	

    return Out;
}



technique11 DefaultTechnique
{
	/* ������ + ������Ʈ + ssao + ��ָ��� + pbr*/
    pass UI
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	/* ����� + ���� */
    pass Effect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
    }
}