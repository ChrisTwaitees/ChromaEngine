#ifndef _CHROMA_CONFIG_
#define _CHROMA_CONFIG_

//common
#include <common/PrecompiledHeader.h>

// DATA
#define CHROMA_UID_LENGTH 8
//static const unsigned int CHROMA_UID_LENGTH{ 8 };

// TIME
#define CHROMA_MAX_FRAME_RATE 60

// ANIMATION
#define MAX_JOINTS 100
#define MAX_VERT_INFLUENCES 4

// RENDER
// Screen
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768 
#define MSAA_SAMPLES 4 

// Background Color
#define SCREEN_DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)

// SHADOWS
#define SHADOW_NUMCASCADES 3

// LIGHTING
#define MAX_POINT_LIGHTS 20
#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_SPOT_LIGHTS 20

// OPENGL CONFIG
#define OPENGL_VERSION std::string( "330 core" )


#endif