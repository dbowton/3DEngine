#include "Engine.h"

int main(int argc, char** argv)
{
	// create engine
	std::unique_ptr<dwb::Engine> engine = std::make_unique<dwb::Engine>();
	engine->StartUp();
	engine->Get<dwb::Renderer>()->Create("OpenGL", 800, 600);

	dwb::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	dwb::SetFilePath("../resources");
	
	// create scene
	std::unique_ptr<dwb::Scene> scene = std::make_unique<dwb::Scene>();
	scene->engine = engine.get();
	
	// load scene
	rapidjson::Document document;
	bool success = dwb::json::Load("scenes/main.scn", document);
	scene->Read(document);

	glm::vec3 translate{ 0 };
	float angle = 0;

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();
		engine->Update();
		scene->Update(engine->time.deltaTime);

		//// update actor
		//auto actor = scene->findActor("model");
		//if (actor != nullptr)
		//{
		//	actor->transform.rotation.y += engine->time.deltaTime;
		//}

		// update actor
		auto actor = scene->findActor("light");
		if (actor != nullptr)
		{
			glm::mat3 rotation = glm::rotate(engine->time.deltaTime, glm::vec3{ 0, 0, 1 });
			actor->transform.position = actor->transform.position * rotation;
		}

		engine->Get<dwb::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<dwb::Renderer>()->EndFrame();
	}

	return 0;
}
