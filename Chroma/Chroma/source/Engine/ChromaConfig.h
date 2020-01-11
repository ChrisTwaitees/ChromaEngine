#ifndef _CHROMA_CONFIG_
#define _CHROMA_CONFIG_

#include <string>
#include <glm/glm.hpp>

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
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1500 
#define MSAA_SAMPLES 4 

// Background Color
#define SCREEN_DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)

// SHADOWS
#define SHADOW_WIDTH SCREEN_WIDTH
#define SHADOW_HEIGHT SCREEN_HEIGHT 

// OPENGL CONFIG
#define OPENGL_VERSION std::string( "330 core" )

// INPUT
static double MOUSE_XOFFSET, MOUSE_YOFFSET;

#endif