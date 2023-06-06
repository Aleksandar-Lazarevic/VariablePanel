#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Core/Layer.h"

#include <vector>

namespace Hazel {

	class LayerStack
	{
	public:
		using Ptr = std::shared_ptr<Layer>;
		using LayerVec = std::vector<Ptr>;
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Ptr layer);
		void PushOverlay(Ptr overlay);
	    void PopLayer(Layer &layer);
	    void PopOverlay(Layer &overlay);

		LayerVec::iterator begin() { return m_Layers.begin(); }
		LayerVec::iterator end() { return m_Layers.end(); }
		LayerVec::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		LayerVec::reverse_iterator rend() { return m_Layers.rend(); }

		LayerVec::const_iterator begin() const { return m_Layers.begin(); }
		LayerVec::const_iterator end()	const { return m_Layers.end(); }
		LayerVec::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		LayerVec::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		LayerVec m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

}