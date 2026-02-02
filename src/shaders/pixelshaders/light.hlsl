float CalculateLightFalloff(float3 fragPosition, float3 lightPosition, float intensity)
{
    float3 vecToLightUnnormalized = lightPosition - fragPosition;
    return intensity * 1 / (dot(vecToLightUnnormalized, vecToLightUnnormalized) * 0.1f); // Reduced light falloff
}

float4 BlinnPhongSpecularComponent(float3 fragPosition, float3 lightPosition, float3 cameraPosition, float3 normal, float4 specularCol, float shininess, float surfaceLightIntensity, float4 lightColor)
{
    float3 vecToLight = normalize(lightPosition - fragPosition);

    float3 vecToCam = normalize(cameraPosition - fragPosition);
    float3 halfVector = normalize(vecToCam + vecToLight);
    float spec = pow(max(dot(normal.xyz, halfVector), 0.0f), shininess);
    return specularCol * lightColor * surfaceLightIntensity * spec;
}

float4 DiffuseComponent(float3 fragPosition, float3 lightPosition, float3 normal, float4 diffuseCol, float surfaceLightIntensity, float4 lightColor)
{
    float3 vecToLight = normalize(lightPosition - fragPosition);
    
    return diffuseCol * lightColor * surfaceLightIntensity * max(dot(normal, vecToLight), 0);
}