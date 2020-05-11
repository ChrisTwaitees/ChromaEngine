#include "Light.h"
#include <ui/uicomponents/Icon.h>
#include <scene/Scene.h>


void Light::UpdatePointRadius()
{
	if (m_LightType == POINT)
	{
		glm::vec3 brightness = m_Diffuse * m_Intensity;
		m_LightMax = std::fmaxf(std::fmaxf(brightness.r, brightness.g), brightness.b);
		m_Radius = (-m_Linear + std::sqrtf(m_Linear * m_Linear - 4 * m_Quadratic * (m_Constant - (256.0 / 5.0) * m_LightMax)))	/ (2 * m_Quadratic);
		CHROMA_INFO("PointLightRadius Updated");
	}
}

void Light::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kLightComponent;	

#ifdef EDITOR
	{
		IComponent* lightIconComponent = new Icon();
		Chroma::Scene::AddUIComponent(lightIconComponent);
		m_IconUID = lightIconComponent->GetUID();
	}
#endif
	// Set Point Radius
	UpdatePointRadius();

	CMPNT_INITIALIZED
}

void Light::Update()
{
#ifdef EDITOR
	{
		UpdatePointRadius();
	}
#endif
}

void Light::Destroy()
{
#ifdef EDITOR
	{

	}
#endif
	CMPNT_DESTROYED

}

void Light::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN
	EditorProperty editorProperty(Chroma::Type::EditorProperty::kNullEditorProperty);
	editorProperty.m_Vec3MinMax = std::make_pair(glm::vec3(-20.0), glm::vec3(20.0));
	serializer->AddProperty("m_Position", &m_Position, editorProperty);
	editorProperty.m_FloatMinMax = std::make_pair(0.0,5.0);
	serializer->AddProperty("m_Intensity", &m_Intensity, editorProperty);
	editorProperty.m_Vec3MinMax = std::make_pair(glm::vec3(-1.0), glm::vec3(1.0));
	serializer->AddProperty("m_Direction", &m_Direction, editorProperty);
	editorProperty.m_FloatMinMax = std::make_pair(0.0, 1.0);
	serializer->AddProperty("m_Linear", &m_Linear);
	serializer->AddProperty("m_Quadratic", &m_Quadratic);
	serializer->AddProperty("m_Constant", &m_Constant);

	editorProperty.m_Type = Chroma::Type::EditorProperty::kColorProperty;
	serializer->AddProperty("m_Diffuse", &m_Diffuse, editorProperty);
	// m_Type
	serializer->AddProperty("m_Type", &m_LightType, editorProperty);
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
	switch (m_LightType)
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
	UpdatePointRadius();
	Init();
}


Light::~Light()
{
}
