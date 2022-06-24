//���� ���̴� ����� �޾Ƽ� �Է°��� ó��
//�ȼ� ���̴� 1�ȼ����� ȣ���
//SV_TARGET = ��������(�����), vertex �� �ٸ�


Texture2D		g_txColor : register(t0); //register(t0) - �ؽ��� ��������
Texture2D		g_txMask : register(t1);
SamplerState	g_Sample : register(s0);  //g_txColor �ؽ��Ŀ��� �ȼ��� �����ͼ� �ȼ� ���̴����� �۾�

//�ؽ��� ��ǥ
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{	
	//�ؽ�ó���� t��ǥ�� �ش��ϴ� �÷���(�ȼ�) ��ȯ
	float4 color = g_txColor.Sample(g_Sample, input.t);
	float4 mask = g_txMask.Sample(g_Sample, input.t);
	float4 final = color;	
	// �ҽ�����(1) = ����ũ�̹����� �������κ��� �������ȴ�.������ ����
	// �ҽ�����(0) = ����ũ�̹����� ����κ���   �����ȴ�.
	final.a = 1.0f-mask.r;
	final = final * input.c;
	return final;
}