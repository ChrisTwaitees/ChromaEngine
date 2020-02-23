#include "Light.h"
#include <ui/uicomponents/Icon.h>
#include <scene/Scene.h>


void Light::updatePointRadius()
{
	m_Diffuse *= m_Intensity;
	m_LightMax = std::fmaxf(std::fmaxf(m_Diffuse.r, m_Diffuse.g), m_Diffuse.b);
	m_Radius = (-m_Linear + std::sqrtf(m_Linear * m_Linear - 4 * m_Quadratic * (m_Constant - (256.0 / 5.0) * m_LightMax))) / (2 * m_Quadratic);
}

void Light::Init()
{
#ifdef EDITOR
	{
		CHROMA_INFO("Initializing Light");
		IComponent* lightIconComponent = new Icon();
		Chroma::Scene::AddUIComponent(lightIconComponent);
		m_IconUID = lightIconComponent->GetUID();
		//delete lightIconComponent;
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
	serializer->StartObject("LightComponent", m_UID);

	serializer->AddProperty("m_Position", &m_Position);
	serializer->AddProperty("m_Intensity", &m_Intensity);
	serializer->AddProperty("m_Diffuse", &m_Diffuse);


	//CHROMA_INFO("m_Diffuse {0}", m_Diffuse.x);

	serializer->AddProperty("m_Direction", &m_Direction);
	serializer->AddProperty("m_Linear", &m_Linear);
	serializer->AddProperty("m_Quadratic", &m_Quadratic);
	serializer->AddProperty("m_Constant", &m_Constant);

}

#ifdef EDITOR
void Light::DrawIcon(Texture& iconTexture)
{
	static_cast<Icon*>(Chroma::Scene::GetComponent(m_IconUID))->DrawWithIconTexture(iconTexture);
	static_cast<Icon*>(Chroma::Scene::GetComponent(m_IconUID))->SetPosition(m_Position);
}
#endif

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
	Init();
}


Light::~Light()
{
}
