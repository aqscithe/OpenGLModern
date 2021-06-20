#pragma once

#include <string>

namespace Resources
{
	class Texture
	{
	public:
		Texture(int vertexCount);
		Texture(const Texture& other);

		void                                operator=(const Texture& other);
		void				bindTexture();
		void				processTexData(std::string textureFile);

		unsigned int		texCount;
		int					width;
		int					height;
		int					vertexCount;

	private:
		void				debug(std::string textureFile);
		unsigned char*      loadImage(std::string filename, int& width, int& height);

	};
}

