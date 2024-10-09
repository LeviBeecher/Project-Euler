struct PS_Out
{
    float4 posIn : SV_POSITION;
    float4 uvIn : TEXCOORD;
    float4 colorIn : COLOR;
};

float4 main(PS_Out pin) : SV_TARGET
{
    // Thanks stashio
    //float2 adjUV = (pin.uvIn * 2) - 1;
    
    float sqrDist = pin.uvIn.x * pin.uvIn.x + pin.uvIn.y * pin.uvIn.y;
    if (sqrDist > 1)
        discard;
    
    //if (sqrDist > 0.98f)
        //return float4(0, 0, 0, 1);
    
    return pin.colorIn;
}