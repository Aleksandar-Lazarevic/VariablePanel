/* Copyright (c) 2023 GameMart Inc. */
#include "TestScene.h"
#include <ForcePanel/ForcePanel.hpp>

using namespace force_panel;

struct PropertyNameHash {
	std::size_t operator()(const PropertyName& name) const {
		std::size_t h = 0;
		for (const char* p = name.data(); p != name.data() + name.size(); ++p) {
			h = (h * 31) + static_cast<unsigned char>(*p);
		}
		return h;
	}
};

struct PropertyNameEqual {
	bool operator()(const PropertyName& a, const PropertyName& b) const {
		return (a.size() == b.size()) && std::equal(a.data(), a.data() + a.size(), b.data());
	}
};

struct ObjectData
{
	std::string m_name;
	bool m_locked = false;
	bool m_clampTime = false;
	IPEC_DRAWORDER m_drawOrder = IPEC_DRAWORDER::NORMAL;
	std::unordered_map<PropertyName, DataMarshall::KeyData, PropertyNameHash, PropertyNameEqual> m_properties;
};

class TempMarshall :public DataMarshall
{
public:
	TempMarshall() = delete;
	TempMarshall(std::shared_ptr<ObjectData> thedata) :m_data(std::move(thedata))
	{

	}
	~TempMarshall() override = default;


	std::string getName()override
	{
		return m_data->m_name;
	}

	void setName(std::string const& newName)override
	{
		m_data->m_name = newName;
	}

	IPEC_DRAWORDER getDrawOrder()override
	{
		return m_data->m_drawOrder;
	}

	void setDrawOrder(IPEC_DRAWORDER newDrawOrder)override
	{
		m_data->m_drawOrder = newDrawOrder;
	}

	bool getClampTime()override
	{
		return m_data->m_clampTime;
	}

	void setClampTime(bool newClampTime)override
	{
		m_data->m_clampTime = newClampTime;
	}

	bool getLocked()override
	{
		return m_data->m_locked;
	}

	void setLocked(bool newLocked)override
	{
		m_data->m_locked = newLocked;
	}

	std::vector<Keyframe> getAllKeyframes() override
	{
		return {};

	}

	void setKeyframeOfType(PropertyName type, std::vector<KeyData> key) override
	{
		m_data->m_properties[type] = key[0];
	}

	bool property_valid(PropertyName type) override
	{
		return true;
	}

	std::optional<float> property_single(PropertyName type) override
	{
		auto found = m_data->m_properties.find(type);
		if (found == m_data->m_properties.end())
			return {};
		return found->second.value;
	}

	void openKeyframer(std::vector<PropertyName> types) override
	{

	}
private:
	std::shared_ptr<ObjectData> m_data;
};

TestScene::TestScene()
{

}

using KeyData = DataMarshall::KeyData;

static constexpr KeyData simple_keyframe(float value)
{
	return { 0.0f,value };
}

static std::vector<std::pair<PropertyName, KeyData>> g_default_force_props = {
		{ properties::box_width, simple_keyframe(100.0f) },
		{ properties::box_height, simple_keyframe(100.0f) },
		{ properties::width, simple_keyframe(1.0f) },
		{ properties::height, simple_keyframe(1.0f) },
		{ properties::pivot_x, simple_keyframe(0.5f) },
		{ properties::pivot_y, simple_keyframe(0.5f) },
		{ properties::angle, simple_keyframe(0.0f) },
		{ properties::anchor, simple_keyframe(0.0f) },
		{ properties::local, simple_keyframe(1.0f) },
		{ properties::emitter_life, simple_keyframe(1.0f) },
		{ properties::attenuation, simple_keyframe(1.0f) },
		{ properties::strength, simple_keyframe(100.0f) },
		{ properties::plane, simple_keyframe(1.0f) },
		{ properties::newtons, simple_keyframe(0.0f) }
};

void TestScene::addForce()
{
	auto newObject = std::make_shared<ObjectData>();
	
	for (const auto& prop : g_default_force_props)
	{
		newObject->m_properties.insert(prop);
	}

	newObject->m_name = "new force";

	m_objects.emplace_back(std::move(newObject));
}

std::vector<std::string> TestScene::getNames()
{
	std::vector<std::string> result;
	for (auto object : m_objects)
		result.push_back(object->m_name);
	return result;
}

std::unique_ptr<DataMarshall>  TestScene::getObject(int i)
{
	if (i >= m_objects.size())
		return {};
	return std::make_unique<TempMarshall>(m_objects[i]);
}
