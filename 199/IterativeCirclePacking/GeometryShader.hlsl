struct GS_In
{
    float4 posIn : SV_Position;
    float4 scaleIn : SCALE;
    float4 colorIn : COLOR;
};

struct GS_Out
{
	float4 posOut : SV_POSITION;
    float4 uvOut  : TEXCOORD;
    float4 colorOut : COLOR;
};

// Create squares out of each given point
[maxvertexcount(4)]
void main(point GS_In input[1], inout TriangleStream<GS_Out> OutputStream)
{
    GS_Out output = (GS_Out)0;
    output.colorOut = input[0].colorIn;
    output.colorOut.w = input[0].scaleIn.x;
    
    output.posOut = input[0].posIn + float4(-input[0].scaleIn.x, input[0].scaleIn.y, 0, 1);
    output.uvOut = float4(-1, 1, 0, 0);
    OutputStream.Append(output);
    
    output.posOut = input[0].posIn + float4(input[0].scaleIn.x, input[0].scaleIn.y, 0, 1);
    output.uvOut = float4(1, 1, 0, 0);
    OutputStream.Append(output);
    
    output.posOut = input[0].posIn + float4(-input[0].scaleIn.x, -input[0].scaleIn.y, 0, 1);
    output.uvOut = float4(-1, -1, 0, 0);
    OutputStream.Append(output);
    
    output.posOut = input[0].posIn + float4(input[0].scaleIn.x, -input[0].scaleIn.y, 0, 1);
    output.uvOut = float4(1, -1, 0, 0);
    OutputStream.Append(output);
}