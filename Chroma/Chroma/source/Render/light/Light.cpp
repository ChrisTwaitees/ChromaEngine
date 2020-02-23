#include "Light.h"



void Light::updatePointRadius()
{
	diffuse *= intensity;
	lightMax = std::fmaxf(std::fmaxf(diffuse.r, diffuse.g), diffuse.b);
	radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic);
}

void Light::Init()
{
#ifdef EDITOR
	{

	}
#endif
}

void Light::Update()
{
#ifdef EDITOR
	{

	}
#endif
}

void Light::Destroy()
{
	CHROMA_INFO("Destroying Light");
#ifdef EDITOR
	{

	}
#endif

}

void Light::Serialize(ISerializer*& serializer)
{
	CHROMA_INFO("Serializing Light Component : {0}", m_UID.data);
	serializer->StartObject("LightComponent", m_UID.data.c_str());
}

std::string Light::GetTypeString() const
{
	switch (type)
	{
	case(TYPE::DIRECTIONAL) :
	{
		return "Directional";
		break;
	}
	case(TYPE::POINT):
	{
		return "Point";
		break;
	}
	case(TYPE::SPOT):
	{
		return "Spot";
		break;
	case(TYPE::SUNLIGHT):
	{
		return "Sunlight";
		break;
	}
	}
	}
}

Light::Light()
{
	updatePointRadius();
}


Light::~Light()
{
}
