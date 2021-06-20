#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Resources/Shader.hpp>
#include <LowRenderer/Camera.hpp>
#include <LowRenderer/Light.hpp>
#include <LowRenderer/Model.hpp>


namespace Resources
{
	class Scene
	{
	public:
		Scene() = default;
		Scene(const std::string& scnFile);

		void								draw(
												GLFWwindow* window, float deltaTime, 
												const LowRenderer::CameraInputs& inputs
											);
		void								showImGuiControls();
		void								debug();

		std::vector<LowRenderer::Model>&	getModels() { return models; }

		std::vector<LowRenderer::Model>		models;
		LowRenderer::Light					point[3];
		LowRenderer::Light					directional;
		LowRenderer::Light					spot[2];
		LowRenderer::Camera					camera;
		Resources::Shader					shader;

		std::string name;

	private:
		void								bindShader();
		void								clearBackground();
		void								drawModel(LowRenderer::Model& model, int index);
		void								setModel(LowRenderer::Model& model);
		void								setMesh(Resources::Mesh& mesh, int index);
		void								processCamera(
												float deltaTime, const LowRenderer::CameraInputs& inputs
											);
		void								setLights();
		void								setDirectionalLights();
		void								setPointLights();
		void								setSpotLights();

		Core::Maths::mat4					calcModelMat4(LowRenderer::Model& model);
		Core::Maths::mat4					calcModelMat4(Resources::Mesh& mesh);

		Core::Maths::vec3					clearColor{ 0.f, 0.f, 0.f };

		int									lightCounts[3];
		int									currModel = 0;
		int									currPoint = 0;
		int									currSpot = 0;
		float								time = 0.f;

		bool								constantTransformations = false;
	};
}
