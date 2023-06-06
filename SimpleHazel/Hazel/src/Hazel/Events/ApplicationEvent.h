#pragma once

#include "Hazel/Events/Event.h"

namespace Hazel {

	class WindowResizeEvent : public Event
	{
	public:
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
	    WindowMovedEvent(int x, int y)
	        : m_x(x)
	        , m_y(y)
	    {
	    }

	    int GetX() const { return m_x; }
	    int GetY() const { return m_y; }

	    std::string ToString() const override
	    {
		    std::stringstream ss;
		    ss << "WindowMovedEvent: " << m_x << ", " << m_y;
		    return ss.str();
	    }

	    EVENT_CLASS_TYPE(WindowMoved)
	    EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
	    int m_x, m_y;
    };

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}