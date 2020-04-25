#ifndef CHROMA_EDITORPROPERTY_H
#define CHROMA_EDITORPROPERTY_H

#include <common/PrecompiledHeader.h>

class EditorProperty
{
public:
	// MinMax

	std::pair<float, float> m_FloatMinMax{ std::make_pair(0.0f, 1.0f) };
	std::pair<int, int> m_IntMinMax{ std::make_pair(0, 10) };

	std::pair<glm::vec2, glm::vec2> m_Vec2MinMax{ std::make_pair(glm::vec2(0.0), glm::vec2(1.0)) };
	std::pair<glm::vec3, glm::vec3> m_Vec3MinMax{ std::make_pair(glm::vec3(0.0), glm::vec3(1.0)) };
	std::pair<glm::vec4, glm::vec4> m_Vec4MinMax{ std::make_pair(glm::vec4(0.0), glm::vec4(1.0)) };
	
	Chroma::Type::EditorProperty m_Type{ Chroma::Type::EditorProperty::kNullEditorProperty };

	EditorProperty() {};
	EditorProperty(Chroma::Type::EditorProperty type) { m_Type = type; };
	~EditorProperty() {};
protected:

};

#endif // CHROMA_EDITORPROPERTY_H