// VOXEL GRID
uniform sampler3D u_VoxelTexture;

uniform int u_VoxelGridResolution;
uniform float u_VoxelGridSize;

uniform vec3 u_VoxelGridCentroid;

// CONE TRACING
uniform float u_VoxelRayMaxDistance;
uniform float u_VoxelRayStepSize;
uniform int u_VoxelNumCones;
