#ifndef CHROMA_TRANSLATEGIZMO_H
#define CHROMA_TRANSLATEGIZMO_H


#include <ui/uicomponents/IGizmo.h>

namespace Chroma
{
	class TranslateGizmo :	public IGizmo
	{
	public:
		TranslateGizmo();
		void Init() override;
	};
}

#endif // CHROMA_TRANSLATEGIZMO_H