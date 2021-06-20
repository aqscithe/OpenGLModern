#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Graph.hpp>

class Application 
{
	public:
		Application(GLFWframebuffersizefun callback);
		
		Application(const Application& app) = delete;
		void	operator=(const Application& app) = delete;
		
		void						run();

	private:
		void						init(GLFWframebuffersizefun callback);
		void						initglfw();
		void						createWindow(GLFWframebuffersizefun callback);
		void						loadGlad();
		void						loadImGui();
		void						processVAO(Resources::Mesh& mesh);
		void						processInput();
		void						newFrame();
		void						endFrame();
		void						updateMousePos();
		void						verifyMouseCapture();
		void						updateInputs();
		void						createVAO();
		void						renderLoop();
		void						showImGuiControls();

		GLFWwindow*					window;

		LowRenderer::CameraInputs	inputs;

		Core::DataStructure::Graph	graph;

		const unsigned int			SCR_WIDTH = 1920;
		const unsigned int			SCR_HEIGHT = 1080;

		bool						logsEnabled = true;

		bool						mouseCaptured = false;
		double						mouseX = 0.0;
		double						mouseY = 0.0;
		float						mouseDeltaX = 0.0;
		float						mouseDeltaY = 0.0;

		int							currScene = 0;
};