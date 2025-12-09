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

		if (!textureName.empty()) baseMap = Resources().Get<Texture>(textureName);

		//specular
		std::string specularName;
		SERIAL_READ_NAME(document, "specularMap", specularName);

		if (!specularName.empty()) specularMap = Resources().Get<Texture>(specularName);

		//emissive
		std::string emissiveName;
		SERIAL_READ_NAME(document, "emissiveMap", emissiveName);

		if (!emissiveName.empty()) emissiveMap = Resources().Get<Texture>(emissiveName);

		//normals
		std::string normalName;
		SERIAL_READ_NAME(document, "normalMap", normalName);

		if (!normalName.empty()) normalMap = Resources().Get<Texture>(normalName);

		//cubemap
		std::string cubeMapName = "";
		SERIAL_READ_NAME(document, "cubeMap", cubeMapName);

		if (!cubeMapName.empty()) cubeMap = Resources().Get<CubeMap>(cubeMapName);

		//shininess
		SERIAL_READ(document, shininess);

		//tiling
		SERIAL_READ(document, tiling);

		//offset
		SERIAL_READ(document, offset);

		//color
		SERIAL_READ(document, baseColor);

		//emissive color
		SERIAL_READ(document, emissiveColor);

		return true;
	}
	void Material::Bind() {
		parameters = Parameters::None;
		program->Use();

		if (baseMap) {
			baseMap->SetActive(GL_TEXTURE0);
			baseMap->Bind();
			program->SetUniform("u_baseMap", 0);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::BaseMap);
		}
		if (specularMap) {
			specularMap->SetActive(GL_TEXTURE1);
			specularMap->Bind();
			program->SetUniform("u_specularMap", 1);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::SpecularMap);
		}
		if (emissiveMap) {
			emissiveMap->SetActive(GL_TEXTURE2);
			emissiveMap->Bind();
			program->SetUniform("u_emissiveMap", 2);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::EmissiveMap);
		}
		if (normalMap) {
			normalMap->SetActive(GL_TEXTURE3);
			normalMap->Bind();
			program->SetUniform("u_normalMap", 3);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::NormalMap);
		}
		if (cubeMap) {
			cubeMap->SetActive(GL_TEXTURE4);
			cubeMap->Bind();
			program->SetUniform("u_cubeMap", 4);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::CubeMap);
		}

		program->SetUniform("u_material.shininess", shininess);
		program->SetUniform("u_material.tiling", tiling);
		program->SetUniform("u_material.offset", offset);
		program->SetUniform("u_material.baseColor", baseColor);
		program->SetUniform("u_material.emissiveColor", emissiveColor);
		program->SetUniform("u_material.parameters", (uint32_t)(parameters));
	}


	void Material::UpdateGUI()
	{
		if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::Text("Name: %s", name.c_str());
			ImGui::Text("Shader: %s", program->name.c_str());
			if (baseMap)ImGui::Text("BaseMap: %s", baseMap->name.c_str());
			if (specularMap)ImGui::Text("specularMap: %s", specularMap->name.c_str());
			if (emissiveMap)ImGui::Text("emissiveMap: %s", emissiveMap->name.c_str());
			//if (cubeMap)ImGui::Text("cubeMap: %s", cubeMap->name.c_str());

			ImGui::ColorEdit3("Emissive Color", glm::value_ptr(emissiveColor));
			ImGui::ColorEdit3("Color", glm::value_ptr(baseColor));
			ImGui::DragFloat("Shininess", &shininess, 1.0f, 1.0f, 256.0f);
			ImGui::DragFloat2("tiling", glm::value_ptr(tiling), 0.0);
			ImGui::DragFloat2("offset", glm::value_ptr(offset), 0.0);
		}
	}
}