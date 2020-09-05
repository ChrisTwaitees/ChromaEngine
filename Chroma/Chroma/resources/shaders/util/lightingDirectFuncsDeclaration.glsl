vec4 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, vec3 albedo, vec4 FragPosLightSpace, sampler2DArray shadowmap);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo);