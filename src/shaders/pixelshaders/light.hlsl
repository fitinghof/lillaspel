float CalculateLightFalloff(float3 fragmentPosition, float3 lightPosition, float intensity)
{
    float3 vectorToLightUnnormalized = lightPosition - fragmentPosition;
    return intensity * 1 / (dot(vectorToLightUnnormalized, vectorToLightUnnormalized));
}

float4 BlinnPhongSpecularComponent(float3 fragmentPosition, float3 lightPosition, float3 cameraPosition, float3 normal, float4 specularColor, float shininess, float surfaceLightIntensity, float4 lightColor)
{
    float3 vectorToLight = normalize(lightPosition - fragmentPosition);

    float3 vectorToCamera = normalize(cameraPosition - fragmentPosition);
    float3 halfVector = normalize(vectorToCamera + vectorToLight);
    float spec = pow(max(dot(normal.xyz, halfVector), 0.0f), shininess);
    return specularColor * lightColor * surfaceLightIntensity * spec;
}

float4 DiffuseComponent(float3 fragmentPosition, float3 lightPosition, float3 normal, float4 diffuseColor, float surfaceLightIntensity, float4 lightColor)
{
    float3 vectorToLight = normalize(lightPosition - fragmentPosition);
    
    return diffuseColor * lightColor * surfaceLightIntensity * max(dot(normal, vectorToLight), 0);
}