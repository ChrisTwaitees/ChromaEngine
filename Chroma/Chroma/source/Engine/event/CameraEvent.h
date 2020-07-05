#ifndef CHROMA_CAMERA_EVENT_H
#define CHROMA_CAMERA_EVENT_H

#include <event/Event.h>
#include <input/Input.h>

namespace Chroma
{
	class CameraEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryCamera)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Camera Event";
			return ss.str();
		}

		EVENT_CLASS_TYPE(CameraEvent);

	protected:
		CameraEvent() {};
	};

	class CameraMovedEvent : public CameraEvent
	{
	public:
		CameraMovedEvent(const glm::vec3& newPosition)
			: m_NewPosition(newPosition) {}

		glm::vec3 GetPosition() const { return m_NewPosition; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CameraMovedEvent: New Position : " << (float)m_NewPosition.x << ", " << (float)m_NewPosition.y << ", " << (float)m_NewPosition.z;
			return ss.str();
		}

		EVENT_CLASS_TYPE(CameraMoved);
	private :
		glm::vec3 m_NewPosition;
	};

}



#endif
