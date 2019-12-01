#ifndef _CHROMA_ANIMATION_
#define _CHROMA_ANIMATION_

#include <vector>

#include <animation/KeyFrame.h>

class Animation
{
	std::vector<KeyFrame> m_keyFrames;
public:
	Animation();
	~Animation();
};

#endif