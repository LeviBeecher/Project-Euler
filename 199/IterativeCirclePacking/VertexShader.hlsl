struct VS_In
{
    float4 posIn : POSITION;
    float4 scaleIn : SCALE;
    float4 colorIn : COLOR;
};

struct VS_Out
{
    float4 posOut : SV_Position;
    float4 scaleOut : SCALE;
    float4 colorOut : COLOR;
};

VS_Out main(VS_In vin)
{
    VS_Out vout;

    vout.posOut = vin.posIn;
    vout.scaleOut = vin.scaleIn;
    vout.colorOut = vin.colorIn;

    return vout;
}