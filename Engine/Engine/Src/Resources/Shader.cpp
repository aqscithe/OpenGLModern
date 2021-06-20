#include <fstream>
#include <sstream>

#include <Resources/Shader.hpp>
#include <Core/Log.hpp>

using namespace Resources;

Shader::Shader(std::string& vertexFilename, std::string& fragFilename)
{
	vertexShaderString = "Bin/shaders/" + vertexFilename;
	fragShaderString = "Bin/shaders/" + fragFilename;

	initShader();
	getShaderSources();
	initShaderProgram();
}

Shader::Shader(const Shader& other)
{
	vertexShader = other.vertexShader;
	fragmentShader = other.fragmentShader;
	shaderProgram = other.shaderProgram;
	fragShaderString = other.fragShaderString;
	vertexShaderString = other.vertexShaderString;
}

void Shader::operator=(const Shader& other)
{
	vertexShader = other.vertexShader;
	fragmentShader = other.fragmentShader;
	shaderProgram = other.shaderProgram;
	fragShaderString = other.fragShaderString;
	vertexShaderString = other.vertexShaderString;
}

void	Shader::initShader()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
}


void	Shader::initShaderProgram()
{
	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragShaderSource = fragShaderString.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Core::Debug::Log::print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog), 
			static_cast<int>(Core::Debug::LogType::ERROR));
	}

	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		Core::Debug::Log::print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog), 
			static_cast<int>(Core::Debug::LogType::ERROR));
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		Core::Debug::Log::print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog), 
			static_cast<int>(Core::Debug::LogType::ERROR));
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void	Shader::getShaderSources()
{
	std::ifstream vertexFile(vertexShaderString);
	std::ifstream fragmentFile(fragShaderString);

	{
		Core::Debug::Log::print("Vertex shader: " + vertexShaderString, static_cast<int>(Core::Debug::LogType::DEBUG));
		Core::Debug::Log::print("Fragment shader: " + fragShaderString, static_cast<int>(Core::Debug::LogType::DEBUG));
	}

	if (!vertexFile.is_open() || !fragmentFile.is_open())
	{
		Core::Debug::Log::print("A Shader file failed to open", 3);
	}

	std::string content((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
	std::string contentB((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());

	vertexShaderString = content;
	fragShaderString = contentB;
}

void	Shader::setMat4(const std::string& name, const Core::Maths::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_TRUE, (GLfloat*)value.e);
}

void	Shader::setVec3(const std::string& name, const Core::Maths::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, (GLfloat*)value.e);
}

void	Shader::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), (GLfloat)value);
}

void	Shader::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (GLboolean)value);
}