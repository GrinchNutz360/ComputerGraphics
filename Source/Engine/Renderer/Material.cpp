#pragma once
#include "Material.h"
#include "Program.h"
#include "Resources/ResourceManager.h"

namespace neu {
	bool Material::Load(const std::string& filename) {
		// load material document
		serial::document_t document;
		if (!serial::Load(filename, document)) {
			LOG_WARNING("Could not load program file: {}", filename);
			return false;
		}
		//program
		std::string programName;
		SERIAL_READ_NAME(document, "program", programName);

		program = Resources().Get<Program>(programName);

		//texture
		std::string textureName;
		SERIAL_READ_NAME(document, "baseMap", textureName);

		baseMap = Resources().Get<Texture>(textureName);

		//specular
		std::string specularName;
		SERIAL_READ_NAME(document, "specularMap", specularName);

		if (!specularName.empty()) specularMap = Resources().Get<Texture>(specularName);

		//shininess
		SERIAL_READ(document, shininess);

		//tiling
		SERIAL_READ(document, tiling);

		//offset
		SERIAL_READ(document, offset);

		//color
		SERIAL_READ(document, baseColor);

		return true;
	}
	void Material::Bind() {
		program->Use();
		if (baseMap) {
			baseMap->SetActive(GL_TEXTURE0);
			baseMap->Bind();
		}
		if (baseMap) {
			specularMap->SetActive(GL_TEXTURE1);
			baseMap->Bind();
		}

		program->SetUniform("u_material.shininess", shininess);
		program->SetUniform("u_material.tiling", tiling);
		program->SetUniform("u_material.offset", offset);
		program->SetUniform("u_material.baseColor", baseColor);
	}

	void Material::UpdateGUI()
	{
		if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::Text("Name: %s", name.c_str());
			ImGui::Text("Shader: %s", program->name.c_str());
			if (baseMap)ImGui::Text("BaseMap: %s", baseMap->name.c_str());
			ImGui::ColorEdit3("Color", glm::value_ptr(baseColor));
			ImGui::DragFloat("Shininess", &shininess, 1.0f, 1.0f, 256.0f);
			ImGui::DragFloat2("tiling", glm::value_ptr(tiling), 0.0);
			ImGui::DragFloat2("offset", glm::value_ptr(offset), 0.0);
		}
	}
}