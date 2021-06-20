#pragma once

#include <Core/Core.hpp>

namespace LowRenderer
{
    class Light
    {
        public:
            Light() = default;

            void                showImGuiControls(int type);

            bool                enabled = false;

            float               cutoff = 6.f;
            float               constant = 0.f;
            float               linear = 1.f;
            float               quadratic = 0.f;

            Core::Maths::vec3   position{ 3.f, 0.3f, 0.35f}; 
            Core::Maths::vec3   ambient{ 1.0, 1.0, 1.0 };
            Core::Maths::vec3   diffuse{ 0.1, 0.4, 0.7 };
            Core::Maths::vec3   specular{ 1.0, 0.4, 0.5 };
            Core::Maths::vec3   direction{ -1.f, 0.f, 0.f };

    private:

    };
}