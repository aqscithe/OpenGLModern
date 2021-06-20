#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <LowRenderer/Light.hpp>
#include <Core/Log.hpp>

using namespace LowRenderer;
/*
Light::Light(const Light& light)
{
    enabled = light.enabled;

    cutoff = light.cutoff;
    constant = light.constant;
    linear = light.linear;
    quadratic = light.quadratic;

    position = light.position;
    ambient = light.ambient;
    diffuse = light.diffuse;
    specular = light.specular;
    direction = light.direction;
}

void Light::operator=(const Light& light)
{
    enabled = light.enabled;

    cutoff = light.cutoff;
    constant = light.constant;
    linear = light.linear;
    quadratic = light.quadratic;

    position = light.position;
    ambient = light.ambient;
    diffuse = light.diffuse;
    specular = light.specular;
    direction = light.direction;
}*/

void Light::showImGuiControls(int type)
{
    ImGui::Checkbox("enabled ", &enabled);

    switch (type)
    {
    case 0:
        ImGui::DragFloat3("pos", position.e);
        ImGui::SliderFloat("constant", &constant, 0.f, 1.f);
        ImGui::SliderFloat("linear", &linear, 0.f, 1.f);
        ImGui::SliderFloat("quadratic", &quadratic, 0.f, 1.f);
        break;
    case 1:
        ImGui::DragFloat3("direction", direction.e);
        break;
    case 2:
        ImGui::DragFloat3("pos", position.e);
        ImGui::DragFloat3("direction", direction.e);
        ImGui::SliderFloat("cut off", &cutoff, 0.1f, 14.f);
        ImGui::SliderFloat("constant", &constant, 0.f, 1.f);
        ImGui::SliderFloat("linear", &linear, 0.f, 1.f);
        ImGui::SliderFloat("quadratic", &quadratic, 0.f, 1.f);
        break;
    default:
        std::string statement = "Invalid light code: " + std::to_string(type);
        Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::ERROR));
        break;
    }

    ImGui::ColorEdit3("ambient", ambient.e);
    ImGui::ColorEdit3("diffuse", diffuse.e);
    ImGui::ColorEdit3("specular", specular.e);
    
}