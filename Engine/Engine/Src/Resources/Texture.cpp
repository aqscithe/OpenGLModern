#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB_IMAGE/stb_image.h>

#include <Resources/Texture.hpp>
#include <Core/Log.hpp>

using namespace Resources;

Texture::Texture(int vertexCount) : vertexCount(vertexCount)
{
    texCount = 0;
    width = 0;
    height = 0;
}

Texture::Texture(const Texture& other)
{
    texCount = other.texCount;
    width = other.width;
    height = other.height;
    vertexCount = other.vertexCount;
}

void Texture::operator=(const Texture& other)
{
    texCount = other.texCount;
    width = other.width;
    height = other.height;
    vertexCount = other.vertexCount;
}

void    Texture::bindTexture()
{
    glGenTextures(1, &texCount);
    glBindTexture(GL_TEXTURE_2D, texCount);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void	Texture::processTexData(std::string textureFile)
{
    std::string filename = "Assets/textures/" + textureFile;
    unsigned char* data = loadImage(filename, width, height);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        Core::Debug::Log::print("Failed to load texture", static_cast<int>(Core::Debug::LogType::ERROR));
    }
    stbi_image_free(data);

    debug(textureFile);
}

unsigned char* Texture::loadImage(std::string filename, int& width, int& height)
{
    stbi_set_flip_vertically_on_load(1);
    return stbi_load(filename.c_str(), &width, &height, nullptr, 4);
}

void Texture::debug(std::string textureFile)
{
    std::string statement = "Texture ID: " + std::to_string(texCount)
        + " | File: " + textureFile + " | width: " + std::to_string(width)
        + " | height: " + std::to_string(height);
    Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::DEBUG));
    
}