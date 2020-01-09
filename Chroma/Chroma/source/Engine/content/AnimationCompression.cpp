#include "AnimationCompression.h"


namespace Chroma
{
	void AnimationCompress::RemoveKeysNotInSkeleton(Take& take, Skeleton*& skeleton)
	{
		for (std::pair<std::string, KeyFrame> keyFrame : take.m_KeyFrames)
		{
			if (! skeleton->GetJointExists(keyFrame.first))
			{
				CHROMA_INFO("ANIMATION COMPRESSION :: NO JOINT FOR ANIMATION KEYFRAME {0} FOUND. REMOVING KEYFRAME.", keyFrame.first);
				take.m_KeyFrames.erase(keyFrame.first);
			}
		}

	}
}