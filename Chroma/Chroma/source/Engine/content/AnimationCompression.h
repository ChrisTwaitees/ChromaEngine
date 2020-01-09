#ifndef _CHROMA_ANIM_COMPRESS_
#define _CHROMA_ANIM_COMPRESS_

#include <common/CoreCommon.h>
#include <animation/Animator.h>
#include <animation/Skeleton.h>

namespace Chroma
{
	class AnimationCompress
	{
	public:
		static void RemoveKeysNotInSkeleton(Take& take, Skeleton*& skeleton);
	};
}


#endif