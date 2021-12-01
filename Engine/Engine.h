#pragma once

#define REGISTER_CLASS(class) dwb::ObjectFactory::Instance().Register<class>(#class)

//core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Json.h"
#include "Core/Timer.h"
#include "Core/Serailizable.h"

//Framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

//Audio
#include "Audio/AudioSystem.h"
#include "Component/AudioComponet.h"

//Graphics
#include "Graphics/Material.h"
#include "Graphics/VertexBuffer.h"

//physics system

//Objects
#include "Object/Actor.h"
#include "Object/Object.h"
#include "Framework/System.h"

//Components
#include "Component/PhysicsComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/LightComponent.h"
#include "Component/ModelComponent.h"

#include "Component/FreeCameraController.h"
//#include "Component/AudioComponent.h"

//Systems
#include "Input/InputSystem.h"

//Resource
#include "Resource/ResourceSystem.h"

#include "Object/Scene.h"

//Math
#include "Math/MathUtils.h"
#include "Math/Random.h"
#include "Math/Transform.h"
#include "Math/MathTypes.h"

#include <vector>
#include <algorithm>
#include <memory>

namespace dwb
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine
	{
	public:
		void StartUp();
		void ShutDown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};
	
	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}

		return nullptr;
	}
}