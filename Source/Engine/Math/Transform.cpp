#include "Transform.h"

namespace neu {

    void Transform::Read(const serial_data_t& value) {
        SERIAL_READ(value, position);

        glm::vec3 rotation;
        SERIAL_READ(value, rotation);
        SetRotation(rotation);

        // Load scale factor with default 1.0 (original size) if missing
        // Scale of 1.0 maintains the object's original dimensions
        SERIAL_READ(value, scale);
    }
    void Transform::UpdateGUI() {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);

            glm::vec3 rotation = GetRotation();
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f)) {
                SetRotation(rotation);
            }
            ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
        }
    }
}