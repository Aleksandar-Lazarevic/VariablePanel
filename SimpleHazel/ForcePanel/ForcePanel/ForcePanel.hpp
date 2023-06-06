#pragma once

#include "ForcePanel_exports.hpp"

#include <Hazel.h>
#include <optional>

namespace force_panel
{

enum IPEC_ANCHOR
{
	IPEC_ANCHOR_NONE = 0,
	IPEC_ANCHOR_ON = 1,
	IPEC_ANCHOR_FIXED_LEFT = 2,
	IPEC_ANCHOR_FIXED_RIGHT = 4,
	IPEC_ANCHOR_FIXED_TOP = 8,
	IPEC_ANCHOR_FIXED_BOTTOM = 16,
	IPEC_ANCHOR_FIXED_WIDTH = 32,
	IPEC_ANCHOR_FIXED_HEIGHT = 64,
	IPEC_ANCHOR_FIXED_ASPECT = 128,
	IPEC_ANCHOR_NOTCH = 256,
	IPEC_ANCHOR_PERCENTAGE = 512
};

enum class IPEC_DRAWORDER
{
	NORMAL = 0,
	BEFORE,
	TOP,
	ZBUFFER,
	IPEC_ORDER_MAX
};

struct TransformData
{
	float w = 0.0f;
	float h = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	float sx = 0.0f;
	float sy = 0.0f;
	float rot = 0.0f;
	bool lockObject = 0;
	uint32_t anchoringMode = 0;
};

struct AnimationData
{
	float lifetime = 0.0f;
	bool clampTime = 0;
};

struct CommonData
{
	std::string m_name;
	IPEC_DRAWORDER draw_order = IPEC_DRAWORDER::NORMAL;
};

struct ForceData
{
	float m_strength = 0.0f;
	float m_attenuation = 0.0f;
};

struct Model
{
	CommonData m_common;
	AnimationData m_animation;
	TransformData m_transform;
	ForceData m_force;
};

class PropertyName
{
public:
	template <std::size_t N>
	constexpr PropertyName(const char (&str)[N])
	    : begin(str)
	    , end(str + N)
	{
	}

	// Accessors
	constexpr const char* data() const { return begin; }
	constexpr size_t size() const { return end - begin; }

private:
	const char* const begin;
	const char* const end;
};

namespace properties
{
#define STRING_CONST(name) constexpr auto name = PropertyName(#name)

	STRING_CONST(limit);
	STRING_CONST(col_compress);
	STRING_CONST(col_shift);
	STRING_CONST(sound_spacing);
	STRING_CONST(tex_speed);
	STRING_CONST(weight_min);
	STRING_CONST(weight_max);
	STRING_CONST(bounce_min);
	STRING_CONST(damp_min);
	STRING_CONST(bounce_max);
	STRING_CONST(damp_max);
	STRING_CONST(fric_min);
	STRING_CONST(fric_max);
	STRING_CONST(scale_min);
	STRING_CONST(scale_max);
	STRING_CONST(direction_min);
	STRING_CONST(direction_max);
	STRING_CONST(rate);
	STRING_CONST(period);
	STRING_CONST(emit_start_min);
	STRING_CONST(emit_start_max);
	STRING_CONST(start_ang_min);
	STRING_CONST(start_ang_max);

	STRING_CONST(life_min);
	STRING_CONST(life_max);
	STRING_CONST(vel_min);
	STRING_CONST(vel_max);
	STRING_CONST(spin_min);
	STRING_CONST(spin_max);
	STRING_CONST(x);
	STRING_CONST(y);
	STRING_CONST(width);
	STRING_CONST(clip0);
	STRING_CONST(clip1);
	STRING_CONST(clip2);
	STRING_CONST(clip3);
	STRING_CONST(clockx);
	STRING_CONST(clocky);
	STRING_CONST(clock0);
	STRING_CONST(clock1);
	STRING_CONST(slicedX1);
	STRING_CONST(slicedX2);
	STRING_CONST(slicedY1);
	STRING_CONST(slicedY2);
	STRING_CONST(height);
	STRING_CONST(angle);
	STRING_CONST(emitter_life);
	STRING_CONST(wander_min);
	STRING_CONST(wander_max);
	STRING_CONST(offx);
	STRING_CONST(offy);
	STRING_CONST(igravy_min);
	STRING_CONST(igravy_max);
	STRING_CONST(vis_min);
	STRING_CONST(vis_max);
	STRING_CONST(align_min);
	STRING_CONST(align_max);
	STRING_CONST(rot_min);
	STRING_CONST(rot_max);
	STRING_CONST(rot_start_min);
	STRING_CONST(rot_start_max);
	STRING_CONST(strength);
	STRING_CONST(blend_length);
	STRING_CONST(particle_kill);
	STRING_CONST(time_max);
	STRING_CONST(content_width);
	STRING_CONST(content_height);

	STRING_CONST(attenuation);

	STRING_CONST(disabled);
	STRING_CONST(step);
	STRING_CONST(yrange_min);
	STRING_CONST(yrange_max);
	STRING_CONST(xrange_min);
	STRING_CONST(xrange_max);
	STRING_CONST(slidermin);
	STRING_CONST(slidermax);
	STRING_CONST(delay);

	STRING_CONST(zoom_min);
	STRING_CONST(zoom_max);
	STRING_CONST(zoom_squeeze);
	STRING_CONST(extratime);
	STRING_CONST(local);
	STRING_CONST(rgb_children);
	STRING_CONST(a_children);
	STRING_CONST(blend_mode);
	STRING_CONST(scale_children_old);
	STRING_CONST(flip_mode);
	STRING_CONST(r_min);
	STRING_CONST(g_min);
	STRING_CONST(b_min);
	STRING_CONST(r_max);
	STRING_CONST(g_max);
	STRING_CONST(b_max);
	STRING_CONST(oncancel);
	STRING_CONST(onrelease);
	STRING_CONST(ondown);
	STRING_CONST(onmove);
	STRING_CONST(slidervar);
	STRING_CONST(textdata);
	STRING_CONST(fontname);
	STRING_CONST(horz_align);
	STRING_CONST(vert_align);
	STRING_CONST(ellipses);
	STRING_CONST(local_particles);
	STRING_CONST(particle_order);
	STRING_CONST(particle_assign);
	STRING_CONST(eject);
	STRING_CONST(tex_repeat);
	STRING_CONST(plane);
	STRING_CONST(newtons);
	STRING_CONST(freezone);
	STRING_CONST(solid_unused);
	STRING_CONST(sphere_unused);
	STRING_CONST(release_length);
	STRING_CONST(layercomps);
	STRING_CONST(containers_unused);
	STRING_CONST(texture_cur);
	STRING_CONST(sprite_name);
	STRING_CONST(sprite_anim);
	STRING_CONST(transitions);
	STRING_CONST(box_width);
	STRING_CONST(box_height);
	STRING_CONST(easing);
	STRING_CONST(vol_min);
	STRING_CONST(preroll);
	STRING_CONST(release_sound);
	STRING_CONST(fontstyle);
	STRING_CONST(shadow_r);
	STRING_CONST(shadow_g);
	STRING_CONST(shadow_b);
	STRING_CONST(shadow_a);
	STRING_CONST(stroke_r);
	STRING_CONST(stroke_g);
	STRING_CONST(stroke_b);
	STRING_CONST(stroke_a);
	STRING_CONST(advance);
	STRING_CONST(line_height);
	STRING_CONST(xrange_squeeze);
	STRING_CONST(yrange_squeeze);
	STRING_CONST(anchor);
	STRING_CONST(sprite_data);
	STRING_CONST(pivot_x);
	STRING_CONST(pivot_y);
	STRING_CONST(private_data);
	STRING_CONST(z_offset);
	STRING_CONST(scriptfile);
	STRING_CONST(fit_children);
	STRING_CONST(min_size);
	STRING_CONST(text_formatting);
	STRING_CONST(effect_link);
	STRING_CONST(particle_flags);
	STRING_CONST(force_mag_min);
	STRING_CONST(force_mag_max);
	STRING_CONST(screenflow);
	STRING_CONST(variables);

#undef STRING_CONST
}

class DataMarshall
{
public:
	struct KeyData
	{
		float time;
		float value;
	};

	struct Keyframe
	{
		PropertyName type;
		KeyData data;
	};

	virtual ~DataMarshall() = default;

	virtual std::string getName() = 0;
	virtual void setName(std::string const& newName) = 0;

	virtual IPEC_DRAWORDER getDrawOrder() = 0;
	virtual void setDrawOrder(IPEC_DRAWORDER newDrawOrder) = 0;

	virtual bool getClampTime() = 0;
	virtual void setClampTime(bool newClampTime) = 0;

	virtual bool getLocked() = 0;
	virtual void setLocked(bool newLocked) = 0;

	virtual std::vector<Keyframe> getAllKeyframes() = 0;
	virtual void setKeyframeOfType(PropertyName type, std::vector<KeyData> key) = 0;

	virtual bool property_valid(PropertyName type) = 0;
	virtual std::optional<float> property_single(PropertyName type) = 0;

	virtual void openKeyframer(std::vector<PropertyName> types) = 0;
};

enum class ForceIcons
{
	align_top = 0,
	align_middle,
	align_bottom,
	align_left,
	align_right,
	align_center,

	lock_on1,
	lock_off1,
	lock_on2,
	lock_off2,
	rotate,

	anchor_top_left_r,
	anchor_top_r,
	anchor_top_right_r,
	anchor_left_r,
	anchor_center_r,
	anchor_right_r,
	anchor_bottom_left_r,
	anchor_bottom_r,
	anchor_bottom_right_r,

	anchor_top_left_p,
	anchor_top_p,
	anchor_top_right_p,
	anchor_left_p,
	anchor_center_p,
	anchor_right_p,
	anchor_bottom_left_p,
	anchor_bottom_p,
	anchor_bottom_right_p,

	anchor_fixed_width_top,
	anchor_fixed_width_center,
	anchor_fixed_width_bottom,

	anchor_fixed_height_left,
	anchor_fixed_height_center,
	anchor_fixed_height_right,

	anchor_fixed,

	keyframed
};

struct ForceUIState
{
	bool m_show_window = true;
	float m_lifetime_c = 10.0f;
	float m_strength_c = 0.0f;
	bool m_lock_size = true;
	bool m_lock_scale = true;
	float m_size_ratio = 1.0f;
	float m_scale_ratio = 1.0f;
};

class FORCE_PANEL_API ForcePanel : public Hazel::Layer
{
public:
	using Textures = std::unordered_map<ForceIcons, Hazel::Ref<Hazel::Texture2D>>;
	ForcePanel();
	~ForcePanel() override = default;

	void attachDataSource(std::unique_ptr<DataMarshall> newdata);

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	void ShowName();

	ForceUIState m_gui;
	Textures m_icons;
	std::unique_ptr<DataMarshall> m_marshall;
};
}