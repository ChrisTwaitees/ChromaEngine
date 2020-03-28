#include "AnimationCompression.h"


namespace Chroma
{
	void AnimationCompress::RemoveKeysNotInSkeleton(Take& take, Skeleton*& skeleton)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("ANIMATION COMPRESSION :: Reading : {0}. Removing KeyFrameArray if not found in Skeleton.", take.m_Name);
		CHROMA_TRACE_UNDERLINE;
		for (std::pair<std::string, KeyFrame> keyFrame : take.m_KeyFrames)
		{
			if (! skeleton->GetJointExists(keyFrame.first))
			{
				CHROMA_TRACE("ANIMATION COMPRESSION :: No Joint found for KeyFrame : {0}. Removing KeyFrame.", keyFrame.first);
				take.m_KeyFrames.erase(keyFrame.first);
			}
		}
		CHROMA_TRACE_UNDERLINE;
	}
}