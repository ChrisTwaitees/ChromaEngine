#ifndef CHROMA_EDITORPROPERTY_H
#define CHROMA_EDITORPROPERTY_H

#include <types/Types.h>

class EditorProperty
{
public:
	inline Chroma::Type::EditorProperty GetType() { return m_Type; };

	EditorProperty() {};
	EditorProperty(Chroma::Type::EditorProperty type) { m_Type = type; };
	~EditorProperty() {};
protected:
	Chroma::Type::EditorProperty m_Type{ Chroma::Type::EditorProperty::kNullEditorProperty };

};

#endif // CHROMA_EDITORPROPERTY_H