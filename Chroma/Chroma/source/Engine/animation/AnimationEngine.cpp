#include "AnimationEngine.h"

void AnimationEngine::Update(Time& time)
{
	for ( std::string const& UID : m_Scene->GetAnimatedEntityUIDs())
	{
		// Process Animated Components for each animated entity
		for (IComponent* const& component : m_Scene->GetEntity(UID)->getAnimationComponents())
		{
			((AnimationComponent*)component)->ProcessAnimators(time);
		}
	}
}

AnimationEngine::AnimationEngine()
{
}

AnimationEngine::~AnimationEngine()
{
}

