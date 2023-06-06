#pragma once

#include <vector>
#include <entt/entt.hpp>
#include <memory>

class TestScene;

struct ActiveScene
{
	std::weak_ptr<TestScene> m_scene;
	int m_selected{};
};

struct ScenesList
{
	std::vector<std::shared_ptr<TestScene>> m_scenes;
};

struct EditorData
{
	entt::registry m_registry;
	entt::entity m_entity;
};
