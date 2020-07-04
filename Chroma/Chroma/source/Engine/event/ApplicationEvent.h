#ifndef CHROMA_APPLICATION_EVENT_H
#define CHROMA_APPLICATION_EVENT_H

#include <event/Event.h>

namespace Chroma
{

	class WindowResizeEvent : public Event
	{
	public : 
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
	private:
		unsigned int m_Width, m_Height;
	};


}


#endif //CHROMA_APPLICATION_EVENT_H
