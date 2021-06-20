#include <Physics/Transform.hpp>

using namespace Physics;

Transform::Transform(const Transform& other)
{
	scale = other.scale;
	rotation = other.rotation;
	translation = other.translation;
}

void Transform::operator=(const Transform& other)
{
	scale = other.scale;
	rotation = other.rotation;
	translation = other.translation;
}