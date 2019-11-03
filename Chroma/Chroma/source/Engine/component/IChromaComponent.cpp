#include "IChromaComponent.h"



IChromaComponent::IChromaComponent()
{
	typeName = typeid(this).name();
}


IChromaComponent::~IChromaComponent()
{
}
