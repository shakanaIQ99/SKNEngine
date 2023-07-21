#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv)*color;
    
    
    const float shininess = 50.0f;
    
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    
    float3 ambient = m_ambient;
    
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);

    for (unsigned int i = 0; i < DIR_LIGHT_NUM;i++)
    {
        if(dirLights[i].active)
        {
            float3 dotlightnormal = saturate(dot(dirLights[0].lightv, input.normal));
            
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
            
            float3 diffuse = dotlightnormal * m_diffuse;
            
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
            
        }
        
    }
    for (unsigned int j = 0; j < POINT_LIGHT_NUM; j++)
    {
        if (pointLights[j].active)
        {
            
            float3 lightv = pointLights[j].lightpos - input.worldpos.xyz;
            
            float d = length(lightv);
            
            float atten = 1.0f / (pointLights[j].lightatten.x + pointLights[j].lightatten.y * d + pointLights[j].lightatten.z * d * d);
            
            
            
            float3 dotlightnormal = saturate(dot(lightv, input.normal));
            
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
            
            float3 diffuse = dotlightnormal * m_diffuse;
            
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb +=atten* (diffuse + specular + ambient) * pointLights[j].lightcolor;
            
        }
    }

    
    return shadecolor * texcolor;
    
}