#pragma once

#include <Resources/Shader.hpp>
#include <Resources/Mesh.hpp>
#include <Resources/Scene.hpp>
#include <Resources/Texture.hpp>

namespace Resources
{
	class ResourcesManager
	{
	public:
		ResourcesManager() = default;

		ResourcesManager(const ResourcesManager& other) = delete;
		void					operator=(const ResourcesManager& other) = delete;

		void					addResource(
									int resourceType, std::string& vert, 
									std::string& frag
								);
		void					addResource(
									int resourceType, std::string& resourceInfo, 
									Physics::Transform& modelTransform
								);
		void					addResource(
									int resourceType, std::string& resourceInfo
								);
		void					addResource(int resourceType, int value);
		void					loadObj(std::string& modelName, std::vector<float>& transform);

		inline unsigned int		getResourceCount() { return count; }
		inline void				increaseResourceCount() { ++count; }

		unsigned int			count = 0;

		std::vector<Shader>		shaders;
		std::vector<Mesh>		meshes;
		std::vector<Texture>	textures;
		std::vector<Scene>		scenes;

	private:
		bool					loadObj(const char* filename);

		void					loadMaterials();
		void					loadTextures(
									std::vector<std::string>& materials, 
									std::vector<std::string>& texFiles
								);
		void					parseMtl(
									std::vector<std::string>& materials, 
									std::vector<std::string>& texFiles
								);
	};
}
