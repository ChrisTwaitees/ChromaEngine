#include "AnimationEngine.h"

void AnimationEngine::Update()
{
	for ( std::string const& UID : m_Scene->GetAnimatedEntityUIDs())
	{
		// Process Animated Components for each animated entity
		for (IComponent* const& component : m_Scene->GetEntity(UID)->getAnimationComponents())
		{
			((AnimationComponent*)component)->ProcessAnimators();
		}
	}
}

void AnimationEngine::UpdateDebug(std::string const& debugAnimClipName, float const& debugTime)
{
	for (std::string const& UID : m_Scene->GetAnimatedEntityUIDs())
	{
		// Process Animated Components for each animated entity
		for (IComponent* const& component : m_Scene->GetEntity(UID)->getAnimationComponents())
		{
			((AnimationComponent*)component)->ProcessAnimatorsDebug(debugAnimClipName, debugTime);
		}
	}
}

AnimationEngine::AnimationEngine()
{
}

AnimationEngine::~AnimationEngine()
{
}

