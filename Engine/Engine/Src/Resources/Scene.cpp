#include <iostream>
#include <fstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Core/Log.hpp>
#include <Resources/Scene.hpp>

using namespace Resources;


Scene::Scene(const std::string& scnName) : name(scnName)
{
	lightCounts[0] = sizeof(point) / sizeof(point[0]);
    lightCounts[1] = 1;
    lightCounts[2] = sizeof(spot) / sizeof(spot[0]);
}

void Scene::draw(GLFWwindow* window, float deltaTime, const LowRenderer::CameraInputs& inputs)
{
    clearBackground();
    bindShader();
    processCamera(deltaTime, inputs);
    setLights();

    for (LowRenderer::Model& model : models)
    {
        if (model.enabled)
        {
            setModel(model);

            // ASSERT textures > 0
            for (int i = 0; i < model.textures.size(); ++i)
            {
                drawModel(model, i);
            }
        }
        
    }
    time += deltaTime;
}

void    Scene::clearBackground()
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::bindShader()
{
    glUseProgram(shader.shaderProgram);
}

void	Scene::drawModel(LowRenderer::Model& model, int index)
{
    glBindTexture(GL_TEXTURE_2D, model.textures[index].texCount);
    glBindVertexArray(model.meshes[index].VAO);
    if(model.wireframe)
        glDrawElements(GL_LINES, model.textures[index].vertexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_TRIANGLES, model.textures[index].vertexCount, GL_UNSIGNED_INT, 0);
}

void Scene::processCamera(float deltaTime, const LowRenderer::CameraInputs& inputs)
{
    camera.update(deltaTime, inputs);
    shader.setVec3("camPos", camera.getCamPos());
}

void	Scene::setModel(LowRenderer::Model& model)
{
    Core::Maths::mat4 modelMat4 = calcModelMat4(model);
    Core::Maths::mat4 mvp = camera.getMVP(modelMat4);
    shader.setMat4("mvp", mvp);
    shader.setMat4("modelMat4", modelMat4);
    shader.setBool("textureEnabled", model.textureEnabled);
    shader.setVec3("modelColor", model.color);
}

void    Scene::setMesh(Resources::Mesh& mesh, int index)
{
    Core::Maths::mat4 meshMat4 = calcModelMat4(mesh);
    Core::Maths::mat4 mvp = camera.getMVP(meshMat4);
    std::string label = "submeshes[" + std::to_string(index) + "].";
    shader.setMat4(label + "meshMat4", meshMat4);
    shader.setMat4(label + "mvp", mvp);
  
}

Core::Maths::mat4	Scene::calcModelMat4(Resources::Mesh& mesh)
{
    if (constantTransformations)
    {
        return mesh.getMat4(time);
    }
    return mesh.getMat4();
}

Core::Maths::mat4	Scene::calcModelMat4(LowRenderer::Model& model)
{
    if (constantTransformations)
    {
        return model.getMat4(time); 
    }
    return model.getMat4(); 
}

void Scene::setLights()
{
    setDirectionalLights();
    setPointLights();
    setSpotLights();
}

void Scene::setSpotLights()
{
    for (int i = 0; i < lightCounts[2]; ++i)
    {
        std::string lightLabel = "spotLights[" + std::to_string(i) + "].";
        shader.setBool(lightLabel + "enabled", spot[i].enabled);
        shader.setVec3(lightLabel + "position", spot[i].position);
        shader.setVec3(lightLabel + "direction", spot[i].direction);
        shader.setFloat(lightLabel + "cutoff", spot[i].cutoff);
        shader.setFloat(lightLabel + "constant", spot[i].constant);
        shader.setFloat(lightLabel + "linear", spot[i].linear);
        shader.setFloat(lightLabel + "quadratic", spot[i].quadratic);
        shader.setVec3(lightLabel + "ambient", spot[i].ambient);
        shader.setVec3(lightLabel + "diffuse", spot[i].diffuse);
        shader.setVec3(lightLabel + "specular", spot[i].specular);
    }
    
}

void Scene::setDirectionalLights()
{
    std::string lightLabel = "dirLight.";
    shader.setBool(lightLabel + "enabled", directional.enabled);
    shader.setVec3(lightLabel + "direction", directional.direction);
    shader.setVec3(lightLabel + "ambient", directional.ambient);
    shader.setVec3(lightLabel + "diffuse", directional.diffuse);
    shader.setVec3(lightLabel + "specular", directional.specular);
}

void Scene::setPointLights()
{
    for (int i = 0; i < lightCounts[0]; ++i)
    {
        std::string lightLabel = "pointLights[" + std::to_string(i) + "].";
        shader.setBool(lightLabel + "enabled", point[i].enabled);
        shader.setVec3(lightLabel + "position", point[i].position);
        shader.setFloat(lightLabel + "constant", point[i].constant);
        shader.setFloat(lightLabel + "linear", point[i].linear);
        shader.setFloat(lightLabel + "quadratic", point[i].quadratic);
        shader.setVec3(lightLabel + "ambient", point[i].ambient);
        shader.setVec3(lightLabel + "diffuse", point[i].diffuse);
        shader.setVec3(lightLabel + "specular", point[i].specular);
    }
    
}

void Scene::showImGuiControls()
{
    if (ImGui::Begin(name.c_str()))
    {
        if (ImGui::CollapsingHeader("Scene Options", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //ImGui::Checkbox("Constant transformations", &constantTransformations);
            ImGui::ColorEdit3("Background", clearColor.e);
        }

        if (ImGui::TreeNode("Camera"))
        {
            camera.showImGuiControls();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Models"))
        {
            ImGui::SliderInt("Index", &currModel, 0, models.size() - 1);
            models[currModel].showImGuiControls();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Directional Light"))
        {
            directional.showImGuiControls(1);
            ImGui::TreePop();
        }


        if (ImGui::TreeNode("Point Lights"))
        {
            ImGui::SliderInt("Index", &currPoint, 0, lightCounts[0] - 1);
            ImGui::Text("Light %d", currPoint);
            point[currPoint].showImGuiControls(0);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Spot Lights"))
        {
            ImGui::SliderInt("Index", &currSpot, 0, lightCounts[2] - 1);
            ImGui::Text("Light %d", currSpot);
            spot[currSpot].showImGuiControls(2);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Scene::debug()
{
    std::string statement = name + " | Models: " + std::to_string(models.size()) + " | Point Lights: "
        + std::to_string(lightCounts[0]) + " | Directional Lights: " + std::to_string(lightCounts[1])
        + " | Spot Lights: " + std::to_string(lightCounts[2]);
    Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::INFO));
}