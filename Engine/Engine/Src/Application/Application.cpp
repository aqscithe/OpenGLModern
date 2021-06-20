#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Application/Application.hpp>
#include <Core/Log.hpp>
#include <Core/Assertion.hpp>

Application::Application(GLFWframebuffersizefun callback)
{
	// ASSERT
	Core::Debug::Assertion::assertTest(SCR_HEIGHT > 0 && SCR_WIDTH > 0);

	std::string statement = "screen width: " + std::to_string(SCR_WIDTH) + " | screen height: " + 
		std::to_string(SCR_HEIGHT);
	Core::Debug::Log::print(statement, 0);

    init(callback);
}

void Application::init(GLFWframebuffersizefun callback)
{
    initglfw();
    createWindow(callback);
    loadGlad();
    loadImGui();
}

void Application::initglfw()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

void Application::loadGlad()
{
    // glad: load all OpenGL function pointers
    // --------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Core::Debug::Log::print("Failed to initialize GLAD", 3);
    }
    else
    {
        Core::Debug::Log::print("GLAD successfully initialized", 1);
    }
}

void Application::createWindow(GLFWframebuffersizefun framebuffer_size_callback)
{
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloGlWindow", NULL, NULL);
    if (window == NULL)
    {
        Core::Debug::Log::print("Failed to create GLFW window", 3);
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Application::loadImGui()
{
    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Application::run()
{
    graph.loadScenes();
	createVAO();
	graph.setScenes();
	renderLoop();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

void Application::createVAO()
{
    for (Resources::Scene& scene : graph.rm.scenes)
    {
        for (LowRenderer::Model& model : scene.models)
        {
            for (int i = 0; i < model.textures.size(); ++i)
            {
				processVAO(model.meshes[i]);
            }
        }
    }
	
}

void Application::processVAO(Resources::Mesh& mesh)
{
	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);
	glGenBuffers(1, &mesh.VBO);

	glGenBuffers(1, &mesh.EBO);

	//definir vbo/ebo
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);

	//remplir vbo/ebo
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(mesh.rdrVertices.front()) * mesh.rdrVertices.size(),
		mesh.rdrVertices.data(),
		GL_STATIC_DRAW
	);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(mesh.indices.front()) * mesh.indices.size(),
		mesh.indices.data(),
		GL_STATIC_DRAW
	);

	//binder les attributs (la position, etc) dans le vao

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// texture coordinate attribute
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(3);
}

void Application::newFrame()
{
	glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    if (mouseCaptured)
        ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    ImGui::NewFrame();
}

void Application::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Application::renderLoop()
{
	glEnable(GL_DEPTH_TEST);

	bool lastLogStatus = logsEnabled;

	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window))
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		newFrame();

		updateMousePos();

		verifyMouseCapture();
		updateInputs();

        Resources::Scene& scene = graph.getScene(currScene);
        scene.draw(window, std::chrono::duration<float>(elapsedTime).count(), inputs);

        processInput();

		showImGuiControls();
		scene.showImGuiControls();
        
		verifyMouseCapture();

		endFrame();
	}
}

void Application::showImGuiControls()
{
	if (ImGui::Begin("Application"))
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Scenes"))
		{
			ImGui::SliderInt("Index", &currScene, 0, graph.rm.scenes.size() - 1);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Info"))
		{
			if (ImGui::CollapsingHeader("Mouse", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("MousePos: %f, %f", mouseX, mouseY);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Settings"))
		{
			if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("Logs Enabled", &Core::Debug::Log::enabled);
				ImGui::Checkbox("Asserts Enabled", &Core::Debug::Assertion::enabled);
			}
			ImGui::TreePop();
		}
		
	}
	ImGui::End();
}

void Application::updateMousePos()
{
	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);
	mouseDeltaX = (float)(newMouseX - mouseX);
	mouseDeltaY = (float)(newMouseY - mouseY);
	mouseX = newMouseX;
	mouseY = newMouseY;
}

void Application::verifyMouseCapture()
{
	if (glfwGetKey(window, GLFW_KEY_0))
	{
		mouseCaptured = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (glfwGetKey(window, GLFW_KEY_9))
	{
		mouseCaptured = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Application::updateInputs()
{
	if (mouseCaptured)
	{
		inputs.deltaX = mouseDeltaX;
		inputs.deltaY = mouseDeltaY;
		inputs.moveForward = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		inputs.moveBackward = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		inputs.strafeLeft = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		inputs.strafeRight = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		inputs.moveUp = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		inputs.moveDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
