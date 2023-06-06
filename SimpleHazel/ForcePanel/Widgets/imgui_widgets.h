#pragma once

#include <imgui/imgui.h>

namespace imgui_widgets
{
	bool BeginComboFast(const char* label, const char* preview_value, ImGuiComboFlags flags);
}