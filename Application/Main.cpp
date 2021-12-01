#include "Engine.h"
#include <glad\glad.h>
#include <sdl.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

int main(int argc, char** argv)
{
	//create engine // unique ptr
	dwb::Engine engine;
	engine.StartUp();
	engine.Get<dwb::Renderer>()->Create("OpenGL", 800, 600);

	// create scene
	std::unique_ptr<dwb::Scene> scene = std::make_unique<dwb::Scene>();
	scene->engine = &engine;

	dwb::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	dwb::SetFilePath("../resources");

	//create shaders
	std::shared_ptr<dwb::Program> program = engine.Get<dwb::ResourceSystem>()->Get<dwb::Program>("basic_shader");
	std::shared_ptr<dwb::Shader> vshader = engine.Get<dwb::ResourceSystem>()->Get<dwb::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<dwb::Shader> fshader = engine.Get<dwb::ResourceSystem>()->Get<dwb::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);

	program->Link();
	program->Use();

	std::shared_ptr<dwb::VertexBuffer> vertexBuffer = engine.Get<dwb::ResourceSystem>()->Get<dwb::VertexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), (6 * sizeof(GLfloat)));

	//texture
	{
		auto texture = engine.Get<dwb::ResourceSystem>()->Get<dwb::Texture>("textures/llama.png");
		texture->Bind();

		texture = engine.Get<dwb::ResourceSystem>()->Get<dwb::Texture>("textures/rocks.bmp");
		texture->Bind();

		texture = engine.Get<dwb::ResourceSystem>()->Get<dwb::Texture>("textures/wood.png");
		texture->Bind();

		texture = engine.Get<dwb::ResourceSystem>()->Get<dwb::Texture>("textures/spot.png");
		texture->Bind();
	}

	//camera
	{
		auto actor = dwb::ObjectFactory::Instance().Create<dwb::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 10 };
		
		{
			auto component = dwb::ObjectFactory::Instance().Create<dwb::CameraComponent>("CameraComponent");
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}

		{
			auto component = dwb::ObjectFactory::Instance().Create<dwb::FreeCameraController>("FreeCameraController");
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}

		scene->addActor(std::move(actor));
	}

	//create cube
	{
		auto actor = dwb::ObjectFactory::Instance().Create<dwb::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		//auto component = dwb::ObjectFactory::Instance().Create<dwb::MeshComponent>("MeshComponent");
		//component->program = engine.Get<dwb::ResourceSystem>()->Get<dwb::Program>("basic_shader");
		//component->vertexBuffer = engine.Get<dwb::ResourceSystem>()->Get<dwb::VertexBuffer>("cube_mesh");

		auto component = dwb::ObjectFactory::Instance().Create<dwb::ModelComponent>("ModelComponent");
		component->program = engine.Get<dwb::ResourceSystem>()->Get<dwb::Program>("basic_shader");
		component->model = engine.Get<dwb::ResourceSystem>()->Get<dwb::Model>("models/spot.obj");

		actor->AddComponent(std::move(component));
		scene->addActor(std::move(actor));
	}
	
	glm::vec3 translate{ 0 };

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
		scene->Update(engine.time.deltaTime);
		engine.Update();

		engine.Get<dwb::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine.Get<dwb::Renderer>()->EndFrame();
	}

	return 0;
}