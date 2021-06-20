#pragma once

#include <string>

#include <glad/glad.h>

#include <Core/Core.hpp>
#include <LowRenderer/Light.hpp>

namespace Resources
{
	class Shader
	{
	public:
		Shader() = default;
		Shader(std::string& vertexFilename, std::string& fragFilename);
		Shader(const Shader& other);

		void            operator=(const Shader& other);

		void			setMat4(const std::string& name, const Core::Maths::mat4& value) const;
		void			setVec3(const std::string& name, const Core::Maths::vec3& value) const;
		void			setFloat(const std::string& name, const float value) const;
		void			setBool(const std::string& name, const bool value) const;

		GLuint			vertexShader = 0;
		GLuint			fragmentShader = 0;
		GLuint			shaderProgram = 0;

	private:
		void			initShader();
		void			getShaderSources();
		void			initShaderProgram();

		std::string		fragShaderString = "";
		std::string		vertexShaderString = "";
	};
}
