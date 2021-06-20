#include <iostream>
#include <fstream>

#include <Core/Graph.hpp>
#include <Core/Log.hpp>
#include <Core/Assertion.hpp>

using namespace Core::DataStructure;

void Graph::loadScenes()
{
	std::vector<std::string> sceneList;
	parseSceneList(sceneList, "Bin/scenes/scene_list.txt");
	for (std::string& scene : sceneList)
	{
        rm.addResource(static_cast<int>(Resources::ResourceType::SCENE), scene);
        loadScene();
	}

    {
        std::string statement = "Scenes Loaded: " + std::to_string(rm.scenes.size());
        Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::INFO));
        for (Resources::Scene& scene : rm.scenes)
        {
            scene.debug();
        }
    }
    
}

void Graph::setScenes()
{
    scenes = rm.scenes;
}

void Graph::parseSceneList(std::vector<std::string>& sceneList, const char* filePath)
{
	std::string path = std::string(filePath);
	std::ifstream readFile;
	readFile.open(path, std::ios::in);

    if (readFile.is_open())
    {
        std::string line;

        while (std::getline(readFile, line))
        {
            if(line[0] != '#')
                sceneList.push_back(line);
        }
    }
    else
    {
        Core::Debug::Log::print("Unable to open file: " + std::string(path), static_cast<int>(Core::Debug::LogType::ERROR));
    }
}

void Graph::loadScene()
{
    std::string path = "Bin/scenes/" + rm.scenes.back().name;
    std::ifstream readFile;
    readFile.open(path, std::ios::in);

    std::map<std::string, std::vector<float>> models;

    std::vector<std::string> shaderInfo;
    std::vector<std::string> modelNames;
    std::vector<float> transform;
    std::vector<float> cameraInfo;

    bool modelNameAdded = false;

    if (readFile.is_open())
    {
        std::string line;

        while (std::getline(readFile, line))
        {
            bool categoryFound = false;
            std::string category;
            std::string text;

            for (int i = 0; i < line.length(); ++i)
            {
                char c = line[i];
                if (!categoryFound)
                {
                    if (c == ' ')
                    {
                        categoryFound = true;
                    }
                    else
                    {
                        category += c;
                        continue;
                    }
                }
                else
                {
                    if (category == "s")
                    {
                        if (c != ' ')
                            text += c;
                        if ((i + 1) == (int)line.length() || line[i + 1] == ' ')
                        {
                            shaderInfo.push_back(text);
                            text.erase();
                        }
                    }
                    else if (category == "m")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if (!modelNameAdded && line[i + 1] == ' ')
                        {
                            modelNames.push_back(text);
                            text.erase();
                            modelNameAdded = true;
                        }
                        else if ( line[i + 1] == ' ' || line[i + 1] == '/')
                        {
                            transform.push_back(std::stof(text));
                            text.erase();
                        }
                        else if ((i + 1) == (int)line.length())
                        {
                            transform.push_back(std::stof(text));
                            models.emplace(modelNames.back(), transform);
                            text.erase();
                            transform.clear();
                            modelNameAdded = false;
                        }
                    }
                    else if (category == "c")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if ((i + 1) == (int)line.length() || line[i + 1] == ' ' || line[i + 1] == '/')
                        {
                            cameraInfo.push_back(std::stof(text));
                            text.erase();
                        }
                    }
                    else if (category == "ld")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if ((i + 1) == (int)line.length() || line[i + 1] == ' ' || line[i + 1] == '/')
                        {
                            if (std::stoi(text))
                                rm.scenes.back().directional.enabled = true;
                            text.erase();
                        }
                    }
                }
            }
        }
    }
    else
    {
        Core::Debug::Log::print("Unable to open file: " + std::string(path), static_cast<int>(Core::Debug::LogType::ERROR));
    }

    rm.addResource(static_cast<int>(Resources::ResourceType::SHADER), shaderInfo[0], shaderInfo[1]);
    loadModels(models, modelNames);
    rm.scenes.back().camera = LowRenderer::Camera(1280, 720, cameraInfo);
}

void	Graph::loadModels(std::map<std::string, std::vector<float>>& models, std::vector<std::string>& names)
{
    // ASSERT
    Core::Debug::Assertion::assertTest(models.size() == names.size());

    for (std::string& name : names)
    {
        rm.loadObj(name, models[name]);
    }
}