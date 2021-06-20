#pragma once

#include <vector>
#include <string>
#include <vector>

#include <Physics/Transform.hpp>
#include <Resources/Mesh.hpp>
#include <Resources/Texture.hpp>

namespace LowRenderer
{
    class Model
    {
    public:
        Model() = default;
        Model(std::vector<float>& transform);
        Model(const Model& other);

        void                                operator=(const Model& other);

        Core::Maths::mat4                   getMat4(float time = 1.f);
        
        void                                showImGuiControls();

        std::vector<Resources::Mesh>		meshes;
        std::vector<Resources::Texture>     textures;
        Physics::Transform                  transform;

        std::string							materialsFile;
        std::string                         name;

        Core::Maths::vec3                   color{ 1.f, 1.f, 1.f };

        bool                                textureEnabled = true;
        bool                                wireframe = false;
        bool                                enabled = true;

    private:
        void                                setSubmeshTransforms();

        int                                 currMesh = 0;

    };
}

