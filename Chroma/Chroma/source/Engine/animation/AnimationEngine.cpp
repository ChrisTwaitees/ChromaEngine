#include "AnimationEngine.h"

void AnimationEngine::Update(ChromaTime& time)
{
	for (IEntity* animatedEntity : m_Scene->GetAnimatedEntities())
	{
		// Process Animated Components for each animated entity
		std::cout << "Animated Entity : "  <<  animatedEntity->GetName() <<  " : Processing Animations" << std::endl;
	}

	std::cout << "Animation Engine Reading Delta time : " << time.GetDeltaTime() << " Game time : " << time.GetGameTime() << std::endl;
}

AnimationEngine::AnimationEngine()
{
}

AnimationEngine::~AnimationEngine()
{
}

