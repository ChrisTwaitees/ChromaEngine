#ifndef CHROMA_APPLICATION_EVENT_H
#define CHROMA_APPLICATION_EVENT_H

#include <event/Event.h>

namespace Chroma
{

	class WindowResizeEvent : public Event
	{
	public : 
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};


	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(unsigned int xPos, unsigned int yPos)
			: m_XPos(xPos), m_YPos(xPos) {}

		unsigned int GetWidth() const { return m_XPos; }
		unsigned int GetHeight() const { return m_YPos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		unsigned int m_XPos, m_YPos;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};




}


#endif //CHROMA_APPLICATION_EVENT_H
