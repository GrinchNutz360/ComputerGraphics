#pragma once
#include <glm/glm.hpp>
#include "GUI.h"

namespace neu {
	class Program;
	class Texture;

	class Material : public Resource, GUI{
	public:
		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

		void UpdateGUI() override;

	public:
		float shininess{ 2 };
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };
		glm::vec3 baseColor{ 0,0,0 };

		res_t <Program> program;
		res_t <Texture> baseMap;
		res_t < Texture> specularMap;
	};



}
