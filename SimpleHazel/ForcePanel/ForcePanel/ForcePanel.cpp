#include "ForcePanel.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h> 
#include <optional>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <Widgets/imgui_widgets.h>
#include <Widgets/imgui_scopes.hpp>


static std::vector<const char*> g_items = { "Default", "Before parent", "Always On Top", "Z-Buffered" };
using namespace force_panel;

ForcePanel::ForcePanel() :Layer("ForcePanel")
{
}

using namespace Hazel;

void ForcePanel::attachDataSource(std::unique_ptr<DataMarshall> newdata)
{
	m_marshall = std::move(newdata);
}

void ForcePanel::OnAttach()
{
	m_icons.emplace(std::make_pair(ForceIcons::align_top, Texture2D::Create("Resources/Icons/Alignment/alignment.top.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::align_middle, Texture2D::Create("Resources/Icons/Alignment/alignment.middle.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::align_bottom, Texture2D::Create("Resources/Icons/Alignment/alignment.bottom.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::align_left, Texture2D::Create("Resources/Icons/Alignment/alignment.left.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::align_right, Texture2D::Create("Resources/Icons/Alignment/alignment.right.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::align_center, Texture2D::Create("Resources/Icons/Alignment/alignment.center.icon.png")));

	m_icons.emplace(std::make_pair(ForceIcons::lock_on1, Texture2D::Create("Resources/Icons/Widgets/lock.on.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::lock_off1, Texture2D::Create("Resources/Icons/Widgets/lock.off.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::lock_on2, Texture2D::Create("Resources/Icons/Widgets/lock.on.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::lock_off2, Texture2D::Create("Resources/Icons/Widgets/lock.off.icon.png")));
	m_icons.emplace(std::make_pair(ForceIcons::rotate, Texture2D::Create("Resources/Icons/Widgets/rotate.icon.png")));

	m_icons.emplace(std::make_pair(ForceIcons::anchor_top_left_r, Texture2D::Create("Resources/Icons/Anchor/top_left_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_top_r, Texture2D::Create("Resources/Icons/Anchor/top_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_top_right_r, Texture2D::Create("Resources/Icons/Anchor/top_right_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_left_r, Texture2D::Create("Resources/Icons/Anchor/left_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_center_r, Texture2D::Create("Resources/Icons/Anchor/center_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_right_r, Texture2D::Create("Resources/Icons/Anchor/right_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_bottom_left_r, Texture2D::Create("Resources/Icons/Anchor/bottom_left_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_bottom_r, Texture2D::Create("Resources/Icons/Anchor/bottom_r.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_bottom_right_r, Texture2D::Create("Resources/Icons/Anchor/bottom_right_r.png")));

	m_icons.emplace(std::make_pair(ForceIcons::anchor_top_left_p, Texture2D::Create("Resources/Icons/Anchor/top_left_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_top_p, Texture2D::Create("Resources/Icons/Anchor/top_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_top_right_p, Texture2D::Create("Resources/Icons/Anchor/top_right_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_left_p, Texture2D::Create("Resources/Icons/Anchor/left_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_center_p, Texture2D::Create("Resources/Icons/Anchor/center_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_right_p, Texture2D::Create("Resources/Icons/Anchor/right_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_bottom_left_p, Texture2D::Create("Resources/Icons/Anchor/bottom_left_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_bottom_p, Texture2D::Create("Resources/Icons/Anchor/bottom_p.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_bottom_right_p, Texture2D::Create("Resources/Icons/Anchor/bottom_right_p.png")));

	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed_width_top, Texture2D::Create("Resources/Icons/Anchor/fixed_width_top.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed_width_center, Texture2D::Create("Resources/Icons/Anchor/fixed_width_center.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed_width_bottom, Texture2D::Create("Resources/Icons/Anchor/fixed_width_bottom.png")));

	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed_height_left, Texture2D::Create("Resources/Icons/Anchor/fixed_height_left.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed_height_center, Texture2D::Create("Resources/Icons/Anchor/fixed_height_center.png")));
	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed_height_right, Texture2D::Create("Resources/Icons/Anchor/fixed_height_right.png")));

	m_icons.emplace(std::make_pair(ForceIcons::anchor_fixed, Texture2D::Create("Resources/Icons/Anchor/fixed.png")));

	m_icons.emplace(std::make_pair(ForceIcons::keyframed, Texture2D::Create("Resources/Icons/Widgets/keyframed.icon.png")));
}

void ForcePanel::OnDetach()
{
}

void ForcePanel::OnUpdate(Hazel::Timestep ts)
{
}


static ImTextureID get_imtexture(ForcePanel::Textures const& textures, ForceIcons icon)
{
	auto found = textures.find(icon);
	if (found == textures.end())
		return 0;
	return (ImTextureID)(uint64_t)found->second->GetRendererID();

}

static ForceIcons force_icon_as_percent(ForceIcons ratioIcon, bool toPercent)
{
	if (!toPercent)
		return ratioIcon;
	switch (ratioIcon)
	{
	case ForceIcons::anchor_top_left_r:
		return ForceIcons::anchor_top_left_p;
	case ForceIcons::anchor_top_r:
		return ForceIcons::anchor_top_p;
	case ForceIcons::anchor_top_right_r:
		return ForceIcons::anchor_top_right_p;
	case ForceIcons::anchor_left_r:
		return ForceIcons::anchor_left_p;
	case ForceIcons::anchor_center_r:
		return ForceIcons::anchor_center_p;
	case ForceIcons::anchor_right_r:
		return ForceIcons::anchor_right_p;
	case ForceIcons::anchor_bottom_left_r:
		return ForceIcons::anchor_bottom_left_p;
	case ForceIcons::anchor_bottom_r:
		return ForceIcons::anchor_bottom_p;
	case ForceIcons::anchor_bottom_right_r:
		return ForceIcons::anchor_bottom_right_p;
	default:
		throw std::runtime_error("not a ratio anchoring icon");
	}
}

static std::string to_string(ForceIcons icon)
{
	return "anchor_button_"+std::to_string((int)icon);
}

static void make_table1(ForcePanel::Textures const& textures, bool isPercent, ImVec2& uv0, ImVec2& uv1, bool  states[16], ImVec4& bg_hg, ImVec4& bg_col)
{
	if (ImGui::BeginTable("#table1", 3, ImGuiTableFlags_BordersOuter, ImVec2(24 * 4 + 16, 0.0f))) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		using namespace ImGui;
		ImGuiWindow* window = GetCurrentWindow();
		auto tlicon = force_icon_as_percent(ForceIcons::anchor_top_left_r, isPercent);
		if (ImGui::ImageButtonEx(window->GetID(to_string(tlicon).c_str()), get_imtexture(textures, tlicon),
			ImVec2(24, 24), uv0, uv1,
			states[0] ? bg_hg : bg_col, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImGuiButtonFlags_PressedOnClick)) {
		}

		ImGui::EndTable();
	}
}

void ForcePanel::ShowName()
{
	auto namescope = imgui_scopes::Group::Begin();

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(101, 101, 101, 255));
	ImGui::Text("Name");
	ImGui::PopStyleColor();
	ImGui::SameLine(35);

	std::string name = m_marshall->getName();
	if (ImGui::InputText("##Name", &name))
		m_marshall->setName(name);
}

static auto g_all_props = std::vector { 
	properties::limit,
	properties::col_compress,
	properties::col_shift,
	properties::sound_spacing,
	properties::tex_speed,
	properties::weight_min,
	properties::weight_max,
	properties::bounce_min,
	properties::damp_min,
	properties::bounce_max,
	properties::damp_max,
	properties::fric_min,
	properties::fric_max,
	properties::scale_min,
	properties::scale_max,
	properties::direction_min,
	properties::direction_max,
	properties::rate,
	properties::period,
	properties::emit_start_min,
	properties::emit_start_max,
	properties::start_ang_min,
	properties::start_ang_max,

	properties::life_min,
	properties::life_max,
	properties::vel_min,
	properties::vel_max,
	properties::spin_min,
	properties::spin_max,
	properties::x,
	properties::y,
	properties::width,
	properties::clip0,
	properties::clip1,
	properties::clip2,
	properties::clip3,
	properties::clockx,
	properties::clocky,
	properties::clock0,
	properties::clock1,
	properties::slicedX1,
	properties::slicedX2,
	properties::slicedY1,
	properties::slicedY2,
	properties::height,
	properties::angle,
	properties::emitter_life,
	properties::wander_min,
	properties::wander_max,
	properties::offx,
	properties::offy,
	properties::igravy_min,
	properties::igravy_max,
	properties::vis_min,
	properties::vis_max,
	properties::align_min,
	properties::align_max,
	properties::rot_min,
	properties::rot_max,
	properties::rot_start_min,
	properties::rot_start_max,
	properties::strength,
	properties::blend_length,
	properties::particle_kill,
	properties::time_max,
	properties::content_width,
	properties::content_height,

	properties::attenuation,

	properties::disabled,
	properties::step,
	properties::yrange_min,
	properties::yrange_max,
	properties::xrange_min,
	properties::xrange_max,
	properties::slidermin,
	properties::slidermax,
	properties::delay,

	properties::zoom_min,
	properties::zoom_max,
	properties::zoom_squeeze,
	properties::extratime,
	properties::local,
	properties::rgb_children,
	properties::a_children,
	properties::blend_mode,
	properties::scale_children_old,
	properties::flip_mode,
	properties::r_min,
	properties::g_min,
	properties::b_min,
	properties::r_max,
	properties::g_max,
	properties::b_max,
	properties::oncancel,
	properties::onrelease,
	properties::ondown,
	properties::onmove,
	properties::slidervar,
	properties::textdata,
	properties::fontname,
	properties::horz_align,
	properties::vert_align,
	properties::ellipses,
	properties::local_particles,
	properties::particle_order,
	properties::particle_assign,
	properties::eject,
	properties::tex_repeat,
	properties::plane,
	properties::newtons,
	properties::freezone,
	properties::solid_unused,
	properties::sphere_unused,
	properties::release_length,
	properties::layercomps,
	properties::containers_unused,
	properties::texture_cur,
	properties::sprite_name,
	properties::sprite_anim,
	properties::transitions,
	properties::box_width,
	properties::box_height,
	properties::easing,
	properties::vol_min,
	properties::preroll,
	properties::release_sound,
	properties::fontstyle,
	properties::shadow_r,
	properties::shadow_g,
	properties::shadow_b,
	properties::shadow_a,
	properties::stroke_r,
	properties::stroke_g,
	properties::stroke_b,
	properties::stroke_a,
	properties::advance,
	properties::line_height,
	properties::xrange_squeeze,
	properties::yrange_squeeze,
	properties::anchor,
	properties::sprite_data,
	properties::pivot_x,
	properties::pivot_y,
	properties::private_data,
	properties::z_offset,
	properties::scriptfile,
	properties::fit_children,
	properties::min_size,
	properties::text_formatting,
	properties::effect_link,
	properties::particle_flags,
	properties::force_mag_min,
	properties::force_mag_max,
	properties::screenflow,
	properties::variables
};

void ForcePanel::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
	auto forceWindow = imgui_scopes::Window::Begin("Example Inspector", &m_gui.m_show_window, ImGuiWindowFlags_NoCollapse);
	if(!forceWindow)
		return;
	if (!m_marshall)
	{
		ImGui::Text("No Object Selected");
		return;
	}
	ImGui::GetStyle().FramePadding.y = 3.0f;

	ShowName();

	//--------------------------
	//		lifetime
	//--------------------------
	using KeyFrame = DataMarshall::Keyframe;
	using KeyData = DataMarshall::KeyData;
	using KeyVec = std::vector<KeyData>;

	auto optlifetime = m_marshall->property_single(properties::emitter_life);
	if (optlifetime)
	{
		auto lifetime = *optlifetime;

		ImGui::BeginGroup();
		ImGui::PushItemWidth(100);

		if (ImGui::DragFloat("Lifetime", &lifetime, 0.2f, 0.0f, m_gui.m_lifetime_c * 2.0f, "%.f")) {
			m_gui.m_lifetime_c = lifetime;
		}

		ImGui::PopItemWidth();
		ImGui::SameLine(130);
		ImGui::PushItemWidth(90);
		ImGui::SliderFloat("##L2", &lifetime, 0, m_gui.m_lifetime_c * 2.0f);
		ImGui::PopItemWidth();
		ImGui::EndGroup();
		if (lifetime != *optlifetime)
			m_marshall->setKeyframeOfType(properties::emitter_life, KeyVec{ KeyData{0.0f,lifetime} });
	}
	else
	{// its keyframed or the lifetime isnt valid

	}
	ImGui::Dummy(ImVec2(0.0f, 2.0f));

	//--------------------------------
	//		Draw order + check boxes
	//--------------------------------																
	ImGui::BeginGroup();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(101, 101, 101, 255));
	ImGui::Text("Draw Order	");
	ImGui::PopStyleColor();
	ImGui::PushItemWidth(128);

	auto draw_order = static_cast<int>(m_marshall->getDrawOrder());
	if (imgui_widgets::BeginComboFast("##DO", g_items[draw_order], 0))
	{
		for (int n = 0; n < g_items.size(); n++)
		{
			const bool is_selected = (draw_order == n);
			if (ImGui::Selectable(g_items[n], is_selected))
				m_marshall->setDrawOrder(static_cast<IPEC_DRAWORDER>(n));

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::EndGroup();
	ImGui::SameLine(140);
	ImGui::BeginGroup();
	auto clampTime = m_marshall->getClampTime();
	if (ImGui::Checkbox("Clamp Time", &clampTime))
		m_marshall->setClampTime(clampTime);

	auto lockObject = m_marshall->getLocked();
	if (ImGui::Checkbox("Lock Object", &lockObject))
		m_marshall->setLocked(lockObject);
	ImGui::EndGroup();



	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	//--------------------------------
	//		TODO
	//--------------------------------																
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec4 bg_hg = ImVec4(0.2f, 0.3f, 0.6f, 0.7f);
	ImGui::ImageButton("top", get_imtexture(m_icons, ForceIcons::align_top), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

	ImGui::SameLine(40);
	ImGui::ImageButton("middle", get_imtexture(m_icons, ForceIcons::align_middle), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

	ImGui::SameLine(70);
	ImGui::ImageButton("bottom", get_imtexture(m_icons, ForceIcons::align_bottom), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

	ImGui::SameLine(100);
	ImGui::ImageButton("left", get_imtexture(m_icons, ForceIcons::align_left), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::SameLine(130);
	ImGui::ImageButton("center", get_imtexture(m_icons, ForceIcons::align_center), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::SameLine(160);
	ImGui::ImageButton("right", get_imtexture(m_icons, ForceIcons::align_right), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	//--------------------------------
	//		transform
	//--------------------------------					
	ImVec4 bg_col_lock1 = ImVec4{ 0.94f, 0.94f, 0.94f, 1.0f };
	ImVec4 tint_col_lock1 = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

	ImVec4 bg_col_lock2 = ImVec4{ 0.94f, 0.94f, 0.94f, 1.0f };
	ImVec4 tint_col_lock2 = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

	if (m_gui.m_lock_size) {
		bg_col_lock1 = ImVec4(0.12f, 0.39f, 0.9f, 1.0f);
		tint_col_lock1 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (m_gui.m_lock_scale) {
		bg_col_lock2 = ImVec4(0.12f, 0.39f, 0.9f, 1.0f);
		tint_col_lock2 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 2.0f));
	//W				
	ImGui::PushItemWidth(82);
	auto box_width_opt = m_marshall->property_single(properties::box_width);
	if (box_width_opt)
	{
		auto box_width = *box_width_opt;
		if (ImGui::DragFloat("W", &box_width, 0.1f, 0.0f, 1000.0f, "%.f px"))
		{
			m_marshall->setKeyframeOfType(properties::box_width, KeyVec{ KeyData{0.0f,box_width} });
		}
		//if (m_gui.m_size_ratio) { transform.h = transform.w / m_gui.m_size_ratio; }
		ImGui::PopItemWidth();
		ImGui::SameLine(120);
	}
	//H
	/*ImGui::PushItemWidth(82);
	ImGui::DragFloat("H", &transform.h, 0.1f, 0.0f, 1000.0f, "%.f px");
	if (m_gui.m_size_ratio) { transform.w = m_gui.m_size_ratio * transform.h; }
	ImGui::PopItemWidth();*/

	//LOCK
	ImGui::SameLine(215);
	if (ImGui::ImageButton("lock_off1", get_imtexture(m_icons, ForceIcons::lock_off1), ImVec2(20, 20), uv0, uv1,
		bg_col_lock1, tint_col_lock1)) {
		m_gui.m_lock_size = !m_gui.m_lock_size;
		if (m_gui.m_lock_size) {
			//if (transform.h <= 0.0f) m_scale_ratio = 1.0f;
			//else m_gui.m_scale_ratio = transform.w / transform.h;
		}
	}

	for (auto& prop : g_all_props)
	{
		auto valueopt = m_marshall->property_single(prop);
		if (!valueopt)
			continue;
		auto value = *valueopt;
		ImGui::InputFloat(prop.data(), &value, 1, 10);
		if (value != *valueopt)
			m_marshall->setKeyframeOfType(prop, KeyVec { KeyData { 0.0f, value } });
	}
	//----------------				
	//X
	/*ImGui::PushItemWidth(82);
	ImGui::DragFloat("X", &transform.x, 0.1f, -1000.0f, 1000.0f, "%.f");
	ImGui::PopItemWidth();
	ImGui::SameLine(120);
	//Y
	ImGui::PushItemWidth(82);
	ImGui::DragFloat("Y", &transform.y, 0.1f, -1000.0f, 1000.0f, "%.f");
	ImGui::PopItemWidth();
	//KeyFrame
	ImGui::SameLine(240);
	if (ImGui::ImageButton("keyframed", get_imtexture(m_icons, ForceIcons::keyframed), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f))) {
	}

	//----------------
	ImGui::Dummy(ImVec2(0.0f, 2.0f));
	//SX
	ImGui::PushItemWidth(82);
	ImGui::DragFloat("SX", &transform.sx, 0.01f, -1000.0f, 1000.0f, "%.2f");
	if (m_lock_scale) { transform.sy = transform.sx / m_scale_ratio; }
	ImGui::PopItemWidth();
	ImGui::SameLine(120);
	//SY						
	ImGui::PushItemWidth(82);
	ImGui::DragFloat("SY", &transform.sy, 0.01f, -1000.0f, 1000.0f, "%.2f");
	if (m_lock_scale) { transform.sx = m_scale_ratio * transform.sy; }
	ImGui::PopItemWidth();
	ImGui::SameLine(215);
	//LOCK
	if (ImGui::ImageButton("lock_off2", get_imtexture(m_icons, ForceIcons::lock_off2), ImVec2(20, 20), uv0, uv1,
		bg_col_lock2, tint_col_lock2)) {
		m_lock_scale = !m_lock_scale;
		if (m_lock_scale) {
			if (transform.sy <= 0.0f) m_scale_ratio = 1.0f;
			else m_scale_ratio = transform.sx / transform.sy;
		}
	}
	//KeyFrame
	ImGui::SameLine(240);
	if (ImGui::ImageButton("keyframed", get_imtexture(m_icons, ForceIcons::keyframed), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f))) {
	}
	//----------------
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	//ROT
	ImGui::Image(get_imtexture(m_icons, ForceIcons::rotate), ImVec2(16, 16));
	ImGui::SameLine(40);
	ImGui::PushItemWidth(40);
	ImGui::DragFloat("##ROT", &transform.rot, 0.36f, 0.0f, 360.0f, "%.f°");
	ImGui::PopItemWidth();
	//KeyFrame
	ImGui::SameLine(85);
	if (ImGui::ImageButton("keyframed2", get_imtexture(m_icons, ForceIcons::keyframed), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f))) {
	}

	//--------------------------------
	//		positioning
	//--------------------------------							
	static int positioning = 0;
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(101, 101, 101, 255));
	ImGui::Text("Positioning");
	ImGui::PopStyleColor();

	ImGui::RadioButton("Local", &positioning, 0);
	ImGui::SameLine(100);
	ImGui::RadioButton("Global", &positioning, 1);
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	//--------------------------------
	//			force shape
	//--------------------------------							
	static int forceShape = 0;
	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(101, 101, 101, 255));
	ImGui::Text("Force Shape");
	ImGui::PopStyleColor();

	ImGui::RadioButton("Oval", &forceShape, 0);
	ImGui::SameLine(100);
	ImGui::RadioButton("Line", &forceShape, 1);

	//--------------------------
	//--------------------------
	//force type
	static int forceType = 0;
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(101, 101, 101, 255));
	ImGui::Text("Force Type");
	ImGui::PopStyleColor();

	ImGui::RadioButton("px/s (like gravity)", &forceType, 0);
	ImGui::SameLine(130);
	ImGui::RadioButton("Newtons (like wind)", &forceType, 1);
	//--------------------------
	//--------------------------			
	//Strength
	ImGui::Dummy(ImVec2(0.0f, 2.0f));

	ImGui::BeginGroup();
	ImGui::PushItemWidth(110);
	if (ImGui::DragFloat("Strength", &m_force.strength, 0.1f, 0.0f, m_strength_c * 2.0f, "%.f px/s")) {
		m_strength_c = m_force.strength;
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(130);
	ImGui::PushItemWidth(85);
	ImGui::SliderFloat("##Strength", &m_force.strength, 0.0f, m_strength_c * 2.0f);
	ImGui::PopItemWidth();
	ImGui::SameLine(225);
	if (ImGui::ImageButton("keyframed4", get_imtexture(m_icons, ForceIcons::keyframed), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f))) {
	}
	ImGui::EndGroup();

	//Attenuation				
	ImGui::BeginGroup();
	ImGui::PushItemWidth(110);
	ImGui::DragFloat("Attenuation", &m_force.attenuation, 0.1f, 0.0f, 100.0f, "%.f%%");
	ImGui::PopItemWidth();
	ImGui::SameLine(130);
	ImGui::PushItemWidth(85);
	ImGui::SliderFloat("##Attenuation", &m_force.attenuation, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::SameLine(225);
	if (ImGui::ImageButton("keyframed3", get_imtexture(m_icons, ForceIcons::keyframed), ImVec2(16, 16), uv0, uv1,
		bg_col, ImVec4(0.4f, 0.4f, 0.4f, 1.0f))) {
	}
	ImGui::EndGroup();
	ImGui::Separator();
	//--------------------------
	//--------------------------
	//anchoring			
	if (!m_force.isAnchoring) {
		ImGuiContext& g = *GImGui;
		ImGuiLastItemData last_item_backup = g.LastItemData;
		float button_size = g.FontSize;
		float button_x = ImMax(g.LastItemData.Rect.Min.x, g.LastItemData.Rect.Max.x - g.Style.FramePadding.x * 2.0f - button_size);
		float button_y = g.LastItemData.Rect.Min.y;

		if (ImGui::plusButton(-99, ImVec2(button_x, button_y))) {
			isAnchoring = true;
		}

	}*/
	/*if (ImGui::CollapsingHeader("Anchoring", &m_force.isAnchoring, ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(101, 101, 101, 255));
		ImGui::Text("Spacing");
		ImGui::PopStyleColor();

		static int spacing = 0;
		static bool isPercent = true;
		ImGui::RadioButton("Ratio", &spacing, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Percent", &spacing, 1);
		if (spacing) {
			isPercent = false;
		}
		else {
			isPercent = true;
		}
		static bool states[16]{ false, false, false,
			false, true, false,
			false, false, false,
			false, true, false,
			false, true, false };
		//---------------------------------------
		//		table 1
		//---------------------------------------
		make_table1(m_icons, isPercent, uv0, uv1, states, bg_hg, bg_col);

		//-----------------------
		static bool notch = false;
		ImGui::Checkbox("Ignore Notch", &notch);

		//ImGui::End();
	}//End		*/
}

void ForcePanel::OnEvent(Hazel::Event& e)
{
}
