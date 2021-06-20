#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Resources/Mesh.hpp>
#include <Core/Core.hpp>

using namespace Resources;
using namespace Core::Maths;

Mesh::Mesh(std::string materialsInfo, Physics::Transform& modelTransform) : materialsInfo(materialsInfo), worldTransform(modelTransform)
{
    localTransform.rotation = { 0.f, 0.f, 0.f };
    localTransform.translation = { 0.f, 0.f, 0.f };
    localTransform.scale = 1.f;
}

Mesh::Mesh(const Mesh& other)
{
    rdrVertices = other.rdrVertices;
    indices = other.indices;

    materialsInfo = other.materialsInfo;

    worldTransform = other.worldTransform;
    localTransform = other.localTransform;

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
}

void Mesh::operator=(const Mesh& other)
{
    rdrVertices = other.rdrVertices;
    indices = other.indices;

    materialsInfo = other.materialsInfo;

    worldTransform = other.worldTransform;
    localTransform = other.localTransform;

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
}

void Mesh::setIndices()
{
    int vertexCount = rdrVertices.size();
    for (int i = 0; i < vertexCount; ++i)
        indices.push_back(i);
}

void Mesh::updateWorldTransform(const Physics::Transform& modelTransform)
{
    worldTransform = modelTransform;
}

mat4       Mesh::getMat4(float time)
{
    return translate(localTransform.translation * time) * rotateX(time * localTransform.rotation.x)
        * rotateY(time * localTransform.rotation.y) * rotateZ(time * localTransform.rotation.z)
        * Core::Maths::scale(localTransform.scale);
}

void Mesh::showImGuiControls()
{
    ImGui::SliderFloat3("translation", localTransform.translation.e, -5.f, 5.f);
    ImGui::SliderFloat3("rotation", localTransform.rotation.e, -Core::Maths::TAU, Core::Maths::TAU);
    ImGui::SliderFloat("scale", &localTransform.scale, 0.01f, 3.f);
}
