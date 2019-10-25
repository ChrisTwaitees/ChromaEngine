#ifndef _CHROMA_RENDERER_CONFIG_
#define _CHROMA_RENDERER_CONFIG_

#include "../screenmanager/ChromaScreenManagerConfig.h"
#include <string>
// SHADOWS
static const unsigned int SHADOW_WIDTH{ SCREEN_WIDTH / 2 };
static const unsigned int SHADOW_HEIGHT{ SCREEN_HEIGHT / 2 };

// OPENGL CONFIG
static const std::string OPENGL_VERSION{"330 core"};

#endif