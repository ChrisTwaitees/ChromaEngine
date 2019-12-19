#include "AnimationEngine.h"

void AnimationEngine::Update(ChromaTime& time)
{
	for ( std::string const& UID : m_Scene->GetAnimatedEntityUIDs())
	{
		// Process Animated Components for each animated entity
		std::cout << "Animated Entity : "  << m_Scene->GetEntity(UID)->GetName() <<  " : Processing Animations" << std::endl;
	}

	std::cout << "Animation Engine Reading Delta time : " << time.GetDeltaTime() << " Game time : " << time.GetGameTime() << std::endl;
}

AnimationEngine::AnimationEngine()
{
}

AnimationEngine::~AnimationEngine()
{
}

