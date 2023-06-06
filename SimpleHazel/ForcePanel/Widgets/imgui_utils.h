#pragma once

namespace imgui_utils
{
static ImVec2 parent_window_content_region()
{
	ImGuiWindow* curwin = ImGui::GetCurrentWindow();

	return curwin->Size - curwin->WindowPadding * 2.0f - ImVec2{ 0.0f,curwin->TitleBarHeight() };
}
}