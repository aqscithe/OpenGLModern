#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <LowRenderer/Model.hpp>


using namespace LowRenderer;
using namespace Core::Maths;

Model::Model(std::vector<float>& _transform)
{
    transform.translation = vec3{ _transform[0], _transform[1], _transform[2] };
    transform.rotation = vec3{ _transform[3], _transform[4], _transform[5] };
    transform.scale = _transform[6];
}

Model::Model(const Model& model)
{
    meshes = model.meshes;
    textures = model.textures;
    transform = model.transform;

    materialsFile = model.materialsFile;
    name = model.name;

    color = model.color;

    textureEnabled = model.textureEnabled;
    wireframe = model.wireframe;
    enabled = model.enabled;
    currMesh = model.currMesh;
}

void Model::operator=(const Model& model)
{
    meshes = model.meshes;
    textures = model.textures;
    transform = model.transform;

    materialsFile = model.materialsFile;
    name = model.name;

    color = model.color;

    textureEnabled = model.textureEnabled;
    wireframe = model.wireframe;
    enabled = model.enabled;
    currMesh = model.currMesh;
}

mat4       Model::getMat4(float time)
{
    //setSubmeshTransforms();

    return translate(transform.translation * time) * rotateX(time * transform.rotation.x)
        * rotateY(time * transform.rotation.y) * rotateZ(time * transform.rotation.z)
        * Core::Maths::scale(transform.scale);
}

void Model::setSubmeshTransforms()
{
    for (Resources::Mesh& mesh : meshes)
    {
        mesh.updateWorldTransform(transform);
    }
}

void Model::showImGuiControls()
{
    ImGui::Checkbox("enabled", &enabled);
    ImGui::Checkbox("texture enabled", &textureEnabled);
    if (!textureEnabled)
        ImGui::ColorEdit3("model color", color.e);
    ImGui::Checkbox("wireframe", &wireframe);
    ImGui::DragFloat3("translation", transform.translation.e);
    ImGui::DragFloat3("rotation", transform.rotation.e);
    ImGui::SliderFloat("scale", &transform.scale, 0.01f, 5.f);
}