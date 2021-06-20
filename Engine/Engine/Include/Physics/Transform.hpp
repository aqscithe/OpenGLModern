#pragma once

#include <Physics/Physics.hpp>
#include <Core/Core.hpp>

class Physics::Transform
{
	public:
		Transform() = default;

		Transform(const Transform& other);
		void				operator=(const Transform& other);

		Core::Maths::vec3   translation{ 0.f, 0.f, 0.f };
		Core::Maths::vec3   rotation{ 0.f, 0.f, 0.f };
		float               scale = 1.f;

	private:

};
