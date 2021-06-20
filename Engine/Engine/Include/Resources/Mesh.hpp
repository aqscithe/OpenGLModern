#pragma once

#include <string>
#include <vector>
#include <string>

#include <glad/glad.h>

#include <Resources/Resource.hpp>
#include <Core/Core.hpp>
#include <Physics/Transform.hpp>

namespace Resources
{
    class Mesh
    {
    public:
        Mesh(std::string materialsInfo, Physics::Transform& modelTransform);
        Mesh(const Mesh& other);

        void                            operator=(const Mesh& other);
        void	                        setIndices();
        void                            updateWorldTransform(const Physics::Transform& modelTransform);
        void                            showImGuiControls();

        Core::Maths::mat4               getMat4(float time = 1.f);

        std::vector<Core::rdrVertex>    rdrVertices;
        std::vector<int>                indices;

        std::string	                    materialsInfo;

        Physics::Transform              worldTransform;
        Physics::Transform              localTransform;

        GLuint                          VAO = 0;
        GLuint                          VBO = 0;
        GLuint                          EBO = 0;

    private:
    };
}
