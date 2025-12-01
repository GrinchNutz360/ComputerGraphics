#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Vector2.h"
#include "Core/Serializable.h"
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace neu {
    
    struct Transform : public ISerializable {
        glm::vec3 position{ 0,0,0 };
        glm::vec3 rotation{ 0,0,0 }; // in radians
        glm::vec3 scale{ 1,1,1 };

        Transform() = default;
        Transform(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0, 0, 0), const glm::vec3& scale = glm::vec3(1, 1, 1)) :
            position{ position },
            rotation{ rotation },
            scale{ scale }
        {
        }

        glm::mat4 GetMatrix() const {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
            model = glm::scale(model, scale);
            return model;
        }

        operator glm::mat4() const {
            return GetMatrix();
        }

        void Read(const serial_data_t& value) override;
    };
}