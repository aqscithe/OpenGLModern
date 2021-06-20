#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <LowRenderer/Camera.hpp>
#include <Core/Log.hpp>

using namespace LowRenderer;
using namespace Core::Maths;

Camera::Camera(int width, int height, std::vector<float>& camInfo)
{
    aspect = (float)width / (float)height;
    position = { camInfo[0], camInfo[1], camInfo[2] };
    pitch = camInfo[3];
    yaw = camInfo[4];
    fovY = Core::Maths::toRadians(camInfo[5]);
    near = camInfo[6];
    far = camInfo[7];
    MOUSE_SENSITIVITY = camInfo[8];
    SPEED = camInfo[9];

    debug();

}

Camera::Camera(const Camera& camera)
{
    MOUSE_SENSITIVITY = camera.MOUSE_SENSITIVITY;
    SPEED = camera.SPEED;
    pitch = camera.pitch;
    yaw = camera.yaw;
    aspect = camera.aspect;
    fovY = camera.fovY;
    near = camera.near;
    far = camera.far;
    orthoView = camera.orthoView;
    position = camera.position;
}

void Camera::operator=(const Camera& camera)
{
    MOUSE_SENSITIVITY = camera.MOUSE_SENSITIVITY;
    SPEED = camera.SPEED;
    pitch = camera.pitch;
    yaw = camera.yaw;
    aspect = camera.aspect;
    fovY = camera.fovY;
    near = camera.near;
    far = camera.far;
    orthoView = camera.orthoView;
    position = camera.position;
}

void Camera::debug()
{
    std::string statement1 = "Aspect Ratio: " + std::to_string(aspect);
    std::string statement2 = "Mouse sensitivity: " + std::to_string(MOUSE_SENSITIVITY);
    std::string statement3 = "Mouse speed: " + std::to_string(SPEED);
    Core::Debug::Log::print(statement1, 0);
    Core::Debug::Log::print(statement2, 0);
    Core::Debug::Log::print(statement3, 0);
}

void Camera::update(float deltaTime, const CameraInputs& inputs)
{
    yaw += inputs.deltaX * MOUSE_SENSITIVITY;
    pitch += inputs.deltaY * MOUSE_SENSITIVITY;

    float forwardMovement = 0.f;
    if (inputs.moveForward)
        forwardMovement += SPEED * deltaTime;
    if (inputs.moveBackward)
        forwardMovement -= SPEED * deltaTime;

    float strafeMovement = 0.f;
    if (inputs.strafeLeft)
        strafeMovement -= SPEED * deltaTime;
    if (inputs.strafeRight)
        strafeMovement += SPEED * deltaTime;

    float verticalMovement = 0.f;
    if (inputs.moveUp)
        verticalMovement += SPEED * deltaTime;
    if (inputs.moveDown)
        verticalMovement -= SPEED * deltaTime;

    position.x += Core::Maths::sin(yaw) * forwardMovement;
    position.z -= Core::Maths::cos(yaw) * forwardMovement;
                  
    position.x += Core::Maths::cos(yaw) * strafeMovement;
    position.z += Core::Maths::sin(yaw) * strafeMovement;

    position.y += verticalMovement;
}

mat4 Camera::getViewMatrix()
{
    return rotateX(pitch) * rotateY(yaw) * translate(-position);
}

mat4 Camera::getProjection()
{
    return perspective(fovY, aspect, near, far, orthoView);
}

mat4 Camera::getMVP(mat4 model)
{
    return getProjection() * getViewMatrix() * model;
}

vec3 Camera::getCamPos()
{
    return position;
}

void Camera::showImGuiControls()
{
    if (ImGui::Checkbox("Orthographic View", &orthoView))
    {
        if (orthoView)
            fovY = 3.127f;
        else
            fovY = 1.047f;
    }
    ImGui::DragFloat3("pos", position.e);
    ImGui::DragFloat("fovY", &fovY);
    ImGui::DragFloat("near", &near);
    ImGui::DragFloat("far", &far);
}