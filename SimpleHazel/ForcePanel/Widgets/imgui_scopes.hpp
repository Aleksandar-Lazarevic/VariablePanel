#pragma once

#include <imgui/imgui.h>

enum class ImGuiScopeType
{
	ALWAYS_END,
	END_ON_SUCCESS
};

template <auto BeginFunc, void(*EndFunc)(), ImGuiScopeType defaultAction=ImGuiScopeType::END_ON_SUCCESS>
class ImGuiScope {
public:
	ImGuiScope(const ImGuiScope&) = delete; // disable copy constructor
	ImGuiScope& operator=(const ImGuiScope&) = delete; // disable copy assignment


	template <typename... Args>
	static auto Begin(Args&&... args) -> std::enable_if_t<!std::is_same_v<decltype(BeginFunc(std::forward<Args>(args)...)), void>, ImGuiScope>
	{
		return ImGuiScope(BeginFunc(std::forward<Args>(args)...));
	}

	template <typename... Args>
	static auto Begin(Args&&... args) -> std::enable_if_t<std::is_same_v<decltype(BeginFunc(std::forward<Args>(args)...)), void>, ImGuiScope>
	{
		BeginFunc(std::forward<Args>(args)...);
		return ImGuiScope(true);
	}

	ImGuiScope& operator=(ImGuiScope&& other) noexcept = delete;

	ImGuiScope(ImGuiScope&& other) noexcept
		: m_should_end(other.m_should_end), m_defaultAction(other.m_defaultAction){
		other.m_should_end = false;
		other.m_defaultAction = ImGuiScopeType::END_ON_SUCCESS;
	}


	~ImGuiScope() {
		if (m_should_end || m_defaultAction== ImGuiScopeType::ALWAYS_END) {
			EndFunc();
		}
	}

	bool is_enabled() const {
		return m_should_end;
	}

	operator bool() const {
		return is_enabled();
	}

private:
	explicit ImGuiScope(bool should_end)
		: m_should_end(should_end), m_defaultAction(defaultAction)
	{
	}

	ImGuiScope() = delete;

	bool m_should_end;
	ImGuiScopeType m_defaultAction;
};


namespace imgui_scopes
{
	static void PushStyleVec2(ImGuiStyleVar idx, const ImVec2& val)
	{
		ImGui::PushStyleVar(idx, val);
	}
    static void PopSingleStyle()
    {
	    ImGui::PopStyleVar();
    }
	using Window = ImGuiScope<ImGui::Begin, ImGui::End, ImGuiScopeType::ALWAYS_END>;
	using StyleVec2 = ImGuiScope<PushStyleVec2, PopSingleStyle, ImGuiScopeType::ALWAYS_END>;
	using Group = ImGuiScope<ImGui::BeginGroup, ImGui::EndGroup>;
	using ListBox = ImGuiScope< ImGui::BeginListBox, ImGui::EndListBox>;
	using MenuBar = ImGuiScope< ImGui::BeginMenuBar, ImGui::EndMenuBar>;
	using Menu = ImGuiScope< ImGui::BeginMenu, ImGui::EndMenu>;
}
