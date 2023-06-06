#include "hzpch.h"
#include "Hazel/Core/LayerStack.h"

namespace Hazel {

	LayerStack::~LayerStack()
	{
		for (auto &layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	void LayerStack::PushLayer(Ptr layer)
	{
		auto* ptr = layer.get();
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		ptr->OnAttach();
	}

	void LayerStack::PushOverlay(Ptr overlay)
	{
		auto* ptr = overlay.get();
		m_Layers.emplace_back(overlay);
		ptr->OnAttach();
	}

	void LayerStack::PopLayer(Layer &layer)
	{
	    auto it = std::find_if(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, [&layer](auto &it)
	            { return it.get() == &layer;
			});
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer.OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer & overlay)
	{
	    auto it = std::find_if(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, [&overlay](auto& it)
	            { return it.get() == &overlay; });
		if (it != m_Layers.end())
		{
			overlay.OnDetach();
			m_Layers.erase(it);
		}
	}

}
