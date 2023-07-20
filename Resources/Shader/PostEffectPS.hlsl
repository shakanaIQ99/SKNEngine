#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_Target
{
    //float4 texcolor = tex.Sample(smp, input.uv);
    
    //return float4(texcolor.rgb,1.0f);
    
    //float shiftU = 1.0f / 1280.0f;
    //float shiftV = 1.0f / 720.0f;
    //float4 col = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //int num = 0;
    
    //[loop]
    //for (float i = -2.0f; i <= 2.0f; i++)
    //{
    //    [loop]
    //    for (float j = -2.0f; j <= 2.0f; j++)
    //    {
    //        float2 pickUV = input.uv + float2(shiftU * i, shiftV * j);
    //        pickUV = clamp(pickUV, 0.0f, 0.9999f);
    //        col += tex.Sample(smp, pickUV);
    //        num++;
    //    }
    //}
    
    //return float4(col.rgb / num, 1);
    
    float4 colortex0 = tex0.Sample(smp, input.uv);
    float4 colortex1 = tex1.Sample(smp, input.uv);
    
    float4 color = colortex0;
    if(fmod(input.uv.y,0.1f)<0.05f)
    {
        color = colortex1;
    }
    
    return float4(color.rgb, 1);
    
}