/* Copyright (c) 2023 GameMart Inc. */
#pragma once
#include <vector>
#include <string>
#include <memory>

class ObjectData;
namespace force_panel
{
	class DataMarshall;
}

class TestScene
{
public:
  TestScene();
  void addForce();
  std::vector<std::string> getNames();
  std::unique_ptr<force_panel::DataMarshall> getObject(int i);
private:
  std::vector<std::shared_ptr<ObjectData>> m_objects;
};