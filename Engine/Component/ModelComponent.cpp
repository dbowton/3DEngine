#include "ModelComponent.h"
#include "CameraComponent.h"
#include "Object/Actor.h"

namespace dwb
{
	void ModelComponent::Update()
	{
	}

	void ModelComponent::Draw(Renderer* renderer)
	{
		program->SetUniform("model", owner->transform.matrix);
		auto actor = owner->scene->findActor("camera");
		if (actor != nullptr)
		{
			program->SetUniform("view", actor->GetComponent<CameraComponent>()->view);
			program->SetUniform("projection", actor->GetComponent<CameraComponent>()->projection);
		}
		model->Draw();
	}

	bool ModelComponent::Write(const rapidjson::Value& value) const
	{
		return true;
	}
	
	bool ModelComponent::Read(const rapidjson::Value& value)
	{
		return true;
	}
}