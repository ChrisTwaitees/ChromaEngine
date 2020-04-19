#ifndef CHROMA_EDITORPROPERTY_H
#define CHROMA_EDITORPROPERTY_H

#include <types/Types.h>

using namespace Chroma;

class EditorProperty
{
public:
	inline Type::EditorProperty GetType() { return m_Type; };

	EditorProperty() {};
	EditorProperty(Type::EditorProperty type) { m_Type = type; };
	~EditorProperty() {};
protected:
	Type::EditorProperty m_Type = Type::EditorProperty{ Type::EditorProperty::kNullEditorProperty };

};

#endif // CHROMA_EDITORPROPERTY_H