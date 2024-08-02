#pragma once

#include "Onyx/Core/Base.h"
#include "Onyx/Events/Event.h"

namespace Onyx
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") 
			: m_DebugName(name) 
		{}
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(float ts) = 0;
		virtual void OnEvent(Event& event) = 0;

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}