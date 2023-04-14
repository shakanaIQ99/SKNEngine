#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv)*color;
    
    
    const float shininess = 4.0f;
    
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    
    float3 ambient = m_ambient;
    
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);

    for (int i = 0; i < DIR_LIGHT_NUM;i++)
    {
        if(dirLights[i].active)
        {
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
            
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
            
            float3 diffuse = dotlightnormal * m_diffuse;
            
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
            
        }
        
    }
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
            
            float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
            
            float d = length(lightv);
            
            float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
            
            
            
            float3 dotlightnormal = dot(lightv, input.normal);
            
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
            
            float3 diffuse = dotlightnormal * m_diffuse;
            
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb +=atten* (diffuse + specular) * pointLights[i].lightcolor;
            
        }
    }
    shadecolor.a = m_alpha;
    
    return shadecolor * texcolor;
    
}