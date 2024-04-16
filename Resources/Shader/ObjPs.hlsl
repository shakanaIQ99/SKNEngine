#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);

    //光沢度
    const float shininess = 4.0f;
    //頂点から始点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    //環境反射光
    float3 ambient = m_ambient;
    //シェーディングによる色
    float4 shadecolor = float4(ambientColor * ambient * texcolor.rgb * color.rgb, color.a);

    if (dirLights.active)
    {
        //ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(dirLights.lightv, input.normal);
            //反射光ベクトル
        float3 reflect = normalize(-dirLights.lightv + 2 * dotlightnormal * input.normal);
             //拡散反射光
        float3 diffuse = saturate(dotlightnormal) * m_diffuse * texcolor.rgb * color.rgb;
            //鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
    
            //全て加算する
        shadecolor.rgb += saturate((diffuse + specular) * dirLights.lightcolor);
        
    }
    if (pointLights.active)
    {
        
        float3 lightv = pointLights.lightpos - input.worldpos.xyz;
        
        float d = length(lightv);
        
        float atten = 1.0f / (pointLights.lightatten.x + pointLights.lightatten.y * d + pointLights.lightatten.z * d * d);
        
        
        
        float3 dotlightnormal = saturate(dot(lightv, input.normal));
        
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
        
        float3 diffuse = dotlightnormal * m_diffuse;
        
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
        
        shadecolor.rgb +=atten* (diffuse + specular + ambient) * pointLights.lightcolor;
        
    }
    

    
    return shadecolor;
    
}