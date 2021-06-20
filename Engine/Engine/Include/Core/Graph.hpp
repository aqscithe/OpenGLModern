#pragma once

#include <vector>
#include <map>
#include <string>

#include <Resources/ResourcesManager.hpp>
#include <Resources/Scene.hpp>

namespace Core
{
	namespace DataStructure
	{
		class Graph
		{
		public:
			Graph() = default;
			
			Graph(const Graph& graph) = delete;
			void operator=(const Graph& graph) = delete;

			inline Resources::Scene&		getScene(int index) { return scenes[index]; }

			void							loadScenes();
			void							setScenes();

			Resources::ResourcesManager		rm;

		private:
			void							parseSceneList(std::vector<std::string>& sceneList, const char* filePath);
			void							loadScene();
			void							loadModels(std::map<std::string, std::vector<float>>& models, 
												std::vector<std::string>& names);

			std::vector<Resources::Scene>	scenes;
		};
	}
}