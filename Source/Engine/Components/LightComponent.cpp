#include "EnginePCH.h"


namespace neu {
	FACTORY_REGISTER(LightComponent)
		void LightComponent::Update(float dt)
	{
	}
	void LightComponent::SetProgram(Program& program, const std::string& name, const glm::mat4& view)
	{
		glm::vec3 position = glm::vec3(view * glm::vec4(owner->transform.position, 1));
		glm::vec3 direction = glm::normalize(glm::mat3(view) * owner->transform.Forward());

		program.SetUniform(name + ".type", (int)lightType);
		program.SetUniform(name + ".position", position);
		program.SetUniform(name + ".direction", direction);
		program.SetUniform(name + ".color", color);
		program.SetUniform(name + ".intensity", intensity);
		program.SetUniform(name + ".range", range);
		program.SetUniform(name + ".outerSpotAngle", glm::radians(outerSpotAngle));
		program.SetUniform(name + ".innerSpotAngle", glm::radians(innerSpotAngle));
	}
	void LightComponent::Read(const serial_data_t& value)
	{
		std::string typeName;
		SERIAL_READ_NAME(value, "lightType", typeName);
		if (equalsIgnoreCase(typeName, "point")) lightType = LightType::Point;
		else if (equalsIgnoreCase(typeName, "directional")) lightType = LightType::Directional;
		else if (equalsIgnoreCase(typeName, "spot")) lightType = LightType::Spot;

		SERIAL_READ(value, color);
		SERIAL_READ(value, intensity);
		SERIAL_READ(value, range);
		SERIAL_READ(value, range);
		SERIAL_READ(value, outerSpotAngle);
		SERIAL_READ(value, innerSpotAngle);
	}
	void LightComponent::UpdateGUI()
	{
		const char* types[] = { "Point", "Directional", "Spot" };
		ImGui::Combo("Type", (int*)&lightType, types, 3);
		ImGui::ColorEdit3("Color", glm::value_ptr(color));
		ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f);
		if (lightType != LightType::Directional) {
			ImGui::DragFloat("Range", &range, 0.1, 0.0f);
		}

		if (lightType == LightType::Spot) {
			ImGui::DragFloat("OuterSpotAngle", &outerSpotAngle, 0.1f, 0.0f, innerSpotAngle);
			ImGui::DragFloat("InnerSpotAngle", &innerSpotAngle, 0.1f, 0.0f, outerSpotAngle);

			outerSpotAngle = math::max(innerSpotAngle, outerSpotAngle);
		}
	}
}