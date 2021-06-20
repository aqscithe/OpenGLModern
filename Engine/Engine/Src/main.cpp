#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Log.hpp>
#include <Application/Application.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	Core::Debug::Log::configureLogFiles();

	Application app(framebuffer_size_callback);

	app.run();
	
	return 0;
}


