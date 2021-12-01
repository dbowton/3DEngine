#pragma once
#include "Framework/System.h"
#include "Math/Transform.h"

#include "SDL.h"
#include <glad/glad.h>
#include <string>

namespace dwb
{
	class Renderer : public System
	{
	public:
		void StartUp() override;
		void ShutDown() override;
		void Update(float dt) override;

		void Create(const std::string& name, int width, int height);
		void BeginFrame();
		void EndFrame();

	private:
		SDL_GLContext context;
		SDL_Window* window = nullptr;
	};
}