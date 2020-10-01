#version 450 core

in GS_OUT{
	vec3 worldPositionFrag;
	vec3 normalFrag;
	vec2 texCoordsFrag;
	vec4 posLightSpaceFrag;
} gs_in;

// Artomic add emulation on 32-bit floating potin data type using a compare-and-swap operation
void imageAtomicFloatAdd( layout(r32ui) coherent volatile uimage3D imgUI , ivec3 coords , float val )
{
	uint newVal = floatBitsToUint( val );
	uint prevVal = 0; 
	uint curVal;
	// Loop as long as destination value gets changed by other threads
	while ( ( curVal = imageAtomicCompSwap( imgUI , coords , prevVal , newVal ) ) != prevVal )
	{
		prevVal = curVal ;
		newVal = floatBitsToUint(( val + uintBitsToFloat( curVal )));
	}
}


// UNIFORMS
uniform sampler2DArray shadowmap;

// VOXELS
#include "util/voxelUniforms.glsl"
#include "util/voxelFuncs.glsl"

// UNIFORMS
// Texture Checks
uniform bool UseAlbedoMap;

// LIGHTING
#include "util/lightingStructs.glsl"
// ubos
#include "util/uniformBufferLighting.glsl"
#include "util/uniformBufferCamera.glsl"
// Lighting Functions
#include "util/lightingDirectFuncsDeclaration.glsl"
// MATERIALS
#include "util/materialStruct.glsl"
uniform Material material;
// Material overrides if no maps provided
uniform vec3 color;


void main()
{
	// REMAP VOXEL COORDS
	// --------------------------------------
	// World Position -> Voxel Grid Space
	vec3 voxelUVW = (gs_in.worldPositionFrag - u_VoxelGridCentroid) * (1.0 / u_VoxelGridResolution) * (1.0 / u_VoxelGridSize) ;
	// Voxel Grid Space -> Clip Space (-1 : 1) -> Texture Space (0 : 1) 
	voxelUVW = remap1101(voxelUVW);

	// Check within 3D Texture bounds
	if(inVoxelGrid(voxelUVW, 0.0))
	{
		// --------------------------------------
		// LIGHTING
		// --------------------------------------
		// Albedo
		vec3 Albedo = UseAlbedoMap? vec3(texture(material.texture_albedo1, gs_in.texCoordsFrag)) : color;
		// Normals
		vec3 Normal = normalize(gs_in.normalFrag);
		// worldPosition
		vec4 worldPos = vec4(gs_in.worldPositionFrag,1.0);
		// view direction
		vec3 viewDir = normalize(cameraPosition - worldPos.xyz);

		// DIRECT LIGHTING
		//------------------------------------------------------------------------
		vec4 radiance;

		// Directional Lights
		for (uint i = 0 ; i < numDirectionalLights ; i++)
		{
			radiance += CalcDirLight( UBO_DirLights[i], Normal, viewDir, Albedo, gs_in.posLightSpaceFrag, shadowmap);
		}
		// Point Lights
		for (uint i = 0 ; i < numPointLights ; ++i)
		{
			if (length(UBO_PointLights[i].position - worldPos.xyz) < UBO_PointLights[i].radius)
				radiance += CalcPointLight(UBO_PointLights[i], Normal,  viewDir, worldPos.xyz, Albedo);
		}
		// Spot Lights
		for (uint i = 0 ; i < numSpotLights ; ++i)
		{
			radiance += CalcSpotLight(UBO_SpotLights[i], Normal,  viewDir, worldPos.xyz, Albedo);
		}

		// PREPARE OUTPUT
		//------------------------------------------------------------------------
		// encode Normal into radiance alpha
		//float normalOut = PackNormal(Normal);
		// TEMP IMPLEMENTATION : Packing HDR Color and Normals in the future
		float alpha = radiance.rgb == vec3(0.0f) ? 0.0 : 1.0;

		// OUTPUT TO 3D TEXTURE
		//------------------------------------------------------------------------
		vec4 writeData =  vec4(radiance.rgb, alpha);
		// Clip Space -> Voxel Grid Space
		ivec3 writeCoord = ivec3(floor(voxelUVW * u_VoxelGridResolution));
		imageStore(u_VoxelWriteTexture, writeCoord, writeData);

		// Attempting atomic operation
		//float testConvergence = (radiance.r) * 0.3 + (radiance.g) * 0.3 + (radiance.b) * 0.3;
		//imageAtomicFloatAdd( voxelTexture , writeCoord , testConvergence);

	}
	else
		return;
}

// Function Implementations
#include "util/lightingDirectFuncsImplementation.glsl"