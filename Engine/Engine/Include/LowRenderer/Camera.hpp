#pragma once

#include <vector>

#include <Core/Core.hpp>

namespace LowRenderer
{
    struct CameraInputs
    {
        float deltaX;
        float deltaY;
        bool moveForward;
        bool moveBackward;
        bool strafeLeft;
        bool strafeRight;
        bool moveUp;
        bool moveDown;
    };

    class Camera
    {

        public:
            Camera() = default;
            Camera(int width, int height, std::vector<float>& camInfo);

            Camera(const Camera& camera);
            void operator=(const Camera& camera);

            void                update(float deltaTime, const CameraInputs& inputs);
            void                showImGuiControls();

            Core::Maths::mat4   getViewMatrix();
            Core::Maths::mat4   getProjection();
            Core::Maths::mat4   getMVP(Core::Maths::mat4 model);
            Core::Maths::vec3   getCamPos();

        private:
            void                debug();

            float               MOUSE_SENSITIVITY = 0.002f;
            float               SPEED = 1.f;
            float               pitch = 0.f;
            float               yaw = 0.f;
            float               aspect = 1.f; 
            float               fovY = Core::Maths::toRadians(60.f);
            float               near = 0.01f;
            float               far = 10.f;

            bool                orthoView = false;

            Core::Maths::vec3   position = { 0.175f, 0.474f, 1.773f };
            
    };
}