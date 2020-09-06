// UTILS
const float PI = 3.14159265359;
const float	 SQRT2 = 1.41421356237309504880;
// Map index to resolution * resolution * resolution 
// Unpacking Texels of 3DTexture
ivec3 unflatten3D(int idx, int resolution)
{
	ivec3 dim = ivec3(resolution);
	const int z = idx / (dim.x * dim.y);
	idx -= (z * dim.x * dim.y);
	const int y = idx / dim.x;
	const int x = idx % dim.x;

	return ivec3(x,y,z);
}

// Remap from -1 : 1 to 0 : 1
vec3 remap1101(vec3 p) { return p * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f); }
// Check whether coordinate is in unit cube
bool inVoxelGrid(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }


const vec3 CONES[] = 
{
	vec3(0.57735, 0.57735, 0.57735),
	vec3(0.57735, -0.57735, -0.57735),
	vec3(-0.57735, 0.57735, -0.57735),
	vec3(-0.57735, -0.57735, 0.57735),
	vec3(-0.903007, -0.182696, -0.388844),
	vec3(-0.903007, 0.182696, 0.388844),
	vec3(0.903007, -0.182696, 0.388844),
	vec3(0.903007, 0.182696, -0.388844),
	vec3(-0.388844, -0.903007, -0.182696),
	vec3(0.388844, -0.903007, 0.182696),
	vec3(0.388844, 0.903007, -0.182696),
	vec3(-0.388844, 0.903007, 0.182696),
	vec3(-0.182696, -0.388844, -0.903007),
	vec3(0.182696, 0.388844, -0.903007),
	vec3(-0.182696, 0.388844, 0.903007),
	vec3(0.182696, -0.388844, 0.903007)
};

// Implementation in Wicked Engine originally written in HLSL by Turánszki János
// https://github.com/turanszkij/WickedEngine/blob/master/WickedEngine/voxelConeTracingHF.hlsli

// voxels:			3D Texture containing voxel scene with direct diffuse lighting (or direct + secondary indirect bounce)
// P:				world-space position of receiving surface
// N:				world-space normal vector of receiving surface
// coneDirection:	world-space cone direction in the direction to perform the trace
// coneAperture:	tan(coneHalfAngle)
vec4 ConeTrace(sampler3D voxels, vec3 P, vec3 N, vec3 coneDirection, float coneAperture)
{
	vec3 color;
	float alpha;
	
	// We need to offset the cone start position to avoid sampling its own voxel (self-occlusion):
	//	Unfortunately, it will result in disconnection between nearby surfaces :(
	float dist = u_VoxelGridSize; // offset by cone dir so that first sample of all cones are not the same
	vec3 startPos = P + N * u_VoxelGridSize * 2.0 * SQRT2; // sqrt2 is diagonal voxel half-extent

	// We will break off the loop if the sampling distance is too far for performance reasons:
	while (dist < u_VoxelRayMaxDistance && alpha < 1.0f)
	{
		float diameter = max(u_VoxelGridSize, 2.0 * coneAperture * dist);
		float mip = log2(diameter * (1.0 / u_VoxelGridSize));

		// Because we do the ray-marching in world space, we need to remap into 3d texture space before sampling:
		//	todo: optimization could be doing ray-marching in texture space
		vec3 tc = startPos + coneDirection * dist;
		// World Position -> Voxel Grid Space
		tc = (tc - u_VoxelGridCentroid) * (1.0 / u_VoxelGridResolution) * (1.0 / u_VoxelGridSize);
		// Voxel Grid Space -> Clip Space (-1 : 1) -> Texture Space (0 : 1) 
		tc = remap1101(tc);

		// break if the ray exits the voxel grid, or we sample from the last mip:
//		if (!inVoxelGrid(tc) || mip >= (float)g_xFrame_VoxelRadianceDataMIPs)
//			break;
		if (!inVoxelGrid(tc, 0.0))
			break;

		// sample voxel grid
		//float4 sam = voxels.SampleLevel(sampler_linear_clamp, tc, mip);
		vec4 sam = textureLod(voxels, tc, mip);

		// this is the correct blending to avoid black-staircase artifact (ray stepped front-to back, so blend front to back):
		float a = 1.0 - alpha;
		color += a * sam.rgb;
		alpha += a * sam.a;

		// step along ray:
		dist += diameter * u_VoxelRayStepSize;
	}

	return vec4(color, alpha);
}

// voxels:			3D Texture containing voxel scene with direct diffuse lighting (or direct + secondary indirect bounce)
// P:				world-space position of receiving surface
// N:				world-space normal vector of receiving surface
vec4 ConeTraceRadiance(sampler3D voxels, vec3 P, vec3 N)
{
	vec4 radiance;

	for (uint cone = 0; cone < u_VoxelNumCones; ++cone) // quality is between 1 and 16 cones
	{
		// approximate a hemisphere from random points inside a sphere:
		//  (and modulate cone with surface normal, no banding this way)
		vec3 coneDirection = normalize(CONES[cone] + N);
		// if point on sphere is facing below normal (so it's located on bottom hemisphere), put it on the opposite hemisphere instead:
		coneDirection *= dot(coneDirection, N) < 0 ? -1 : 1;

		radiance += ConeTrace(voxels, P, N, coneDirection, tan(PI * 0.5f * 0.33f));
	}

	// final radiance is average of all the cones radiances
	radiance *= (1.0 / float(u_VoxelNumCones));
	radiance.a = clamp(radiance.a, 0.0, 1.0);
	//radiance.a = 1.0;
	return radiance;
}

// voxels:			3D Texture containing voxel scene with direct diffuse lighting (or direct + secondary indirect bounce)
// P:				world-space position of receiving surface
// N:				world-space normal vector of receiving surface
// V:				world-space view-vector (cameraPosition - P)
vec4 ConeTraceReflection(sampler3D voxels, vec3 P, vec3 N, vec3 ViewDir, float roughness)
{
	float aperture = tan(roughness * PI * 0.5f * 0.1f);
	vec3 coneDirection = reflect(-ViewDir, N);

	vec4 reflection = ConeTrace(voxels, P, N, coneDirection, aperture);

	return vec4(max(vec3(0.0), reflection.rgb), clamp(reflection.a, 0.0, 1.0));
}