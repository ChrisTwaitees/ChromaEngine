#ifndef _CHROMA_CONFIG_
#define _CHROMA_CONFIG_

#include <string>
#include <glm/glm.hpp>


// ANIMATION
static const unsigned int MAX_JOINTS{ 100 };
static const unsigned int MAX_VERT_INFLUENCES{ 4 };

// RENDER
// Screen
static const unsigned int SCREEN_WIDTH{ 1500 };
static const unsigned int SCREEN_HEIGHT{ 1500 };
static const unsigned int MSAA_SAMPLES{ 4 };

// Background Color
static const glm::vec4 SCREEN_DEFAULT_COLOR(0.0f, 0.0f, 0.0f, 0.0f);

// SHADOWS
static const unsigned int SHADOW_WIDTH{ SCREEN_WIDTH };
static const unsigned int SHADOW_HEIGHT{ SCREEN_HEIGHT };

// OPENGL CONFIG
static const std::string OPENGL_VERSION{ "330 core" };

// INPUT
static double MOUSE_XOFFSET, MOUSE_YOFFSET;

#endif