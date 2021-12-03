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
		material->shader->SetUniform("model", owner->transform.matrix);
		auto actor = owner->scene->findActor("camera");
		
		if (actor != nullptr)
		{
			material->shader->SetUniform("view", actor->GetComponent<CameraComponent>()->view);
			material->shader->SetUniform("projection", actor->GetComponent<CameraComponent>()->projection);
		}

		material->Set();
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