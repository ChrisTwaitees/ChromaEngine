#version 330 core

// IMPLEMENTATION : 
//http://imanolfotia.com/blog/update/2017/03/11/ScreenSpaceReflections.html
// more resources : 
// https://lettier.github.io/3d-game-shaders-for-beginners/screen-space-reflection.html
// TODO : Investigate cone roughness
// http://roar11.com/2015/07/screen-space-glossy-reflections/

// out
out vec4 FragColor;

// in
in vec2 TexCoords;

// uniforms
uniform sampler2D vsNormals;
uniform sampler2D vsPositions;
uniform sampler2D vAlbedo;

uniform mat4 vProjection;

// consts
const float step = 0.1;
const float minRayStep = 0.1;
const float maxSteps = 30;
const int numBinarySearchSteps = 5;
const float maxDistance = 10;
const float maxCameraDistance = 25;

// funcs
vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
 
vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth);

vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);


void main()
{

    vec3 viewNormal = texture2D(vsNormals, TexCoords).xyz;
    vec3 viewPos = texture2D(vsPositions, TexCoords).xyz;
    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));

    vec3 hitPos = viewPos;
	float dDepth;
    vec4 ReflectedCoords = RayMarch((reflected * maxDistance), hitPos, dDepth);

	// Masking
	vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - TexCoords.xy));
    float maskScreenEdge = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

	//float depthThicknessMask = 1.0 - clamp(hitPos.z / step , 0.0, 1.0);
	float maskCamFacing = 1.0 - max(dot(-normalize(viewPos), reflected), 0.0);
	float maskFromDist =  1.0 - clamp( length(viewPos - hitPos) / maxDistance, 0.0,1.0 );
	float maskFromCameraDist =  1.0 - smoothstep(maxCameraDistance, maxCameraDistance +  maxCameraDistance / 2.0, length(viewPos));
	float uvOutOfBoundsMask =  (ReflectedCoords.x < 0 || ReflectedCoords.x > 1 ? 0 : 1)  * (ReflectedCoords.y < 0 || ReflectedCoords.y > 1 ? 0 : 1);
	float mask =  maskScreenEdge *  -reflected.z * maskFromDist * uvOutOfBoundsMask * maskCamFacing * maskFromCameraDist;


    //FragColor = texture(vAlbedo, TexCoords) + vec4(texture(vAlbedo, ReflectedCoords.xy)) * vec4(mask);
	//FragColor = vec4(mask);
	// FINAL
	FragColor = vec4(ReflectedCoords.xy, mask, mask);

}


vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
{
    float depth;

    vec4 projectedCoord;
 
    for(int i = 0; i < numBinarySearchSteps; i++)
    {

        projectedCoord = vProjection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(vsPositions, projectedCoord.xy, 2).z;

 
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }

        projectedCoord = vProjection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}

vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{

    dir *= step;
 
 
    float depth;
    int steps;
    vec4 projectedCoord;

 
    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;
 
        projectedCoord = vProjection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = texture2D(vsPositions, projectedCoord.xy, 2).z;
        if(depth > maxDistance)
            continue;
 
        dDepth = hitCoord.z - depth;

        if((dir.z - dDepth) < 1.2)
        {
            if(dDepth <= 0.0)
            {   
                vec4 Result;
                Result = vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);

                return Result;
            }
        }
        
        steps++;
    }
 
    
    return vec4(projectedCoord.xy, depth, 0.0);
}


