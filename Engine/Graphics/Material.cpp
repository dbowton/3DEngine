#include "Material.h"

namespace dwb
{
	void Material::Bind()
	{
		for (auto& texture : textures)
		{
			texture->Bind();
		}
	}
	void Material::SetProgram(Program& program)
	{
		program.Use();
	}
}