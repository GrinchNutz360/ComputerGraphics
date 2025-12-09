#pragma once

namespace neu {
	class LightComponent : public Component
	{
	public:
		enum LightType {
			Point,
			Directional,
			Spot
		};
	public:
		CLASS_PROTOTYPE(LightComponent)

			void Update(float dt) override;
		void SetProgram(Program& program, const std::string& name, const glm::mat4& view);

		void Read(const serial_data_t& value) override;

	public:
		glm::vec3 color{ 1, 1, 1 };
		float intensity{ 1 };
		float range{ 10 };
		LightType lightType = LightType::Point;
		float innerSpotAngle = 30.0f;
		float outerSpotAngle = 0.0f;

		// Inherited via GUI
		void UpdateGUI() override;
	};
}