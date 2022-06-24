//HLSL->c��� �����ϴ�.
//�������̴��� �ݵ�� float4:SV_POSITION(��������)�� ��ȯ�Ѵ�.
//���������� ����1������ ȣ��ȴ�.
//POSITION(�ø�ƽ:�ṉ̀���)
//SV_POSITION = ��������(�����)

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15
};

// �������(����:�������� ����(float4)�� �Ҵ�Ǿ�� �Ѵ�.)
cbuffer cb0 : register(b0)
{
	float4   Color0 : packoffset(c0);
	float    TimerX : packoffset(c1.x); // Timer.x, Timer.y, Timer.z, Timer.w	
};
cbuffer cb1 : register(b1)
{
	float4   Color1 : packoffset(c0);
	float4   Timer1 : packoffset(c1);
};
VS_OUTPUT VS(float2 p: POSITION, float2 t : TEXCOORD) 
{
	VS_OUTPUT pOut = (VS_OUTPUT)0;
	float fScaleX = cos(TimerX)*0.5f+0.5f;
	float fScaleY = sin(TimerX)*0.5f+0.5f;
	pOut.p = float4(p.x,p.y,0.0f,1.0f);
	pOut.t = t;
	pOut.c = Color0;
	return pOut;
}