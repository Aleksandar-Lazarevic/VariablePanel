#include "hzpch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"


#include <FileWatch/FileWatch.hpp>

#include "Hazel/Core/Application.h"
#include "Hazel/Core/Timer.h"
#include "Hazel/Core/Buffer.h"
#include "Hazel/Core/FileSystem.h"

#include "Hazel/Project/Project.h"

class MonoDomain;
class MonoType;

namespace Hazel {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },

		{ "Hazel.Vector2", ScriptFieldType::Vector2 },
		{ "Hazel.Vector3", ScriptFieldType::Vector3 },
		{ "Hazel.Vector4", ScriptFieldType::Vector4 },

		{ "Hazel.Entity", ScriptFieldType::Entity },
	};

	namespace Utils {

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false)
		{
			return nullptr;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			return ScriptFieldType::None;
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		bool AssemblyReloadPending = false;

#ifdef HZ_DEBUG
		bool EnableDebugging = true;
#else
		bool EnableDebugging = false;
#endif
		// Runtime

		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type)
	{
		if (!s_Data->AssemblyReloadPending && change_type == filewatch::Event::modified)
		{
			s_Data->AssemblyReloadPending = true;

			Application::Get().SubmitToMainThread([]()
			{
				s_Data->AppAssemblyFileWatcher.reset();
				ScriptEngine::ReloadAssembly();
			});
		}
	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
		ScriptGlue::RegisterFunctions();

		bool status = LoadAssembly("Resources/Scripts/Hazel-ScriptCore.dll");
		if (!status)
		{
			HZ_CORE_ERROR("[ScriptEngine] Could not load Hazel-ScriptCore assembly.");
			return;
		}
		
		auto scriptModulePath = Project::GetAssetDirectory() / Project::GetActive()->GetConfig().ScriptModulePath;
		status = LoadAppAssembly(scriptModulePath);
		if (!status)
		{
			HZ_CORE_ERROR("[ScriptEngine] Could not load app assembly.");
			return;
		}

		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		// Retrieve and instantiate class
		s_Data->EntityClass = ScriptClass("Hazel", "Entity", true);
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::InitMono()
	{
	}

	void ScriptEngine::ShutdownMono()
	{
	}

	bool ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		return false;
	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		return false;
	}

	void ScriptEngine::ReloadAssembly()
	{
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName))
		{
			UUID entityID = entity.GetUUID();

			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity);
			s_Data->EntityInstances[entityID] = instance;

			// Copy field values
			if (s_Data->EntityScriptFields.find(entityID) != s_Data->EntityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at(entityID);
				for (const auto& [name, fieldInstance] : fieldMap)
					instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
			}

			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityUUID = entity.GetUUID();
		if (s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end())
		{
			Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
			instance->InvokeOnUpdate((float)ts);
		}
		else
		{
			HZ_CORE_ERROR("Could not find ScriptInstance for entity {}",  entityUUID);
		}
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto it = s_Data->EntityInstances.find(entityID);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}


	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;

		return s_Data->EntityClasses.at(name);
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;

		s_Data->EntityInstances.clear();
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		HZ_CORE_ASSERT(entity);

		UUID entityID = entity.GetUUID();
		return s_Data->EntityScriptFields[entityID];
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();


	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}


	MonoObject* ScriptEngine::GetManagedInstance(UUID uuid)
	{
		HZ_CORE_ASSERT(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end());
		return s_Data->EntityInstances.at(uuid)->GetManagedObject();
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		return nullptr;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return nullptr;
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return nullptr;
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		// Call Entity constructor
		{
			UUID entityID = entity.GetUUID();
			void* param = &entityID;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		return false;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		return false;
	}

}
