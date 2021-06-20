#include <fstream>

#include <Resources/ResourcesManager.hpp>
#include <Physics/Transform.hpp>
#include <Core/Log.hpp>
#include <Core/Assertion.hpp>

using namespace Resources;

void ResourcesManager::addResource(int resourceType, std::string& vert, std::string& frag)
{
	bool success = true;

	switch (resourceType)
	{
		case static_cast<int>(ResourceType::SHADER) :
			shaders.push_back(Shader(vert, frag));
			scenes.back().shader = shaders.back(); 
            Core::Debug::Log::print("Added shader", static_cast<int>(Core::Debug::LogType::INFO));
			break;
		default:
			success = false;
			std::string statement = "ResourcesManager::addResource() 1 - Invalid resource type: " 
				+ std::to_string(resourceType);
			Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::ERROR));
			break;
	}

	if (success)
		ResourcesManager::increaseResourceCount();
}

void ResourcesManager::addResource(int resourceType, std::string& resourceInfo, Physics::Transform& modelTransform)
{
	bool success = true;

	switch (resourceType)
	{
		case static_cast<int>(ResourceType::MESH) :
			meshes.push_back(Resources::Mesh(resourceInfo, modelTransform));
            scenes.back().models.back().meshes.push_back(meshes.back()); 
            Core::Debug::Log::print("Added mesh", static_cast<int>(Core::Debug::LogType::INFO));
			break;
		default:
			success = false;
			std::string statement = "ResourcesManager::addResource() 1 - Invalid resource type: "
				+ std::to_string(resourceType);
			Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::ERROR));
			break;
	}

	if (success)
		ResourcesManager::increaseResourceCount();
}

void ResourcesManager::addResource(int resourceType, std::string& resourceInfo)
{
    bool success = true;

    switch (resourceType)
    {
        case static_cast<int>(ResourceType::SCENE) :
            scenes.push_back(Resources::Scene(resourceInfo));
            Core::Debug::Log::print("Added scene", static_cast<int>(Core::Debug::LogType::INFO));
            break;
        default:
            success = false;
            std::string statement = "ResourcesManager::addResource() 1 - Invalid resource type: "
                + std::to_string(resourceType);
            Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::ERROR));
            break;
    }

    if (success)
        ResourcesManager::increaseResourceCount();
}

void ResourcesManager::addResource(int resourceType, int value)
{
	bool success = true;

	switch (resourceType)
	{
		case static_cast<int>(ResourceType::TEXTURE) :
			textures.push_back(Texture(value));
            scenes.back().models.back().textures.push_back(textures.back()); 
            Core::Debug::Log::print("Added texture", static_cast<int>(Core::Debug::LogType::INFO));
			break;
		default:
			success = false;
			std::string statement = "ResourcesManager::addResource() 2 - Invalid resource type: "
				+ std::to_string(resourceType);
			Core::Debug::Log::print(statement, static_cast<int>(Core::Debug::LogType::ERROR));
			break;
	}

	if (success)
		ResourcesManager::increaseResourceCount();
}

void ResourcesManager::loadObj(std::string& modelName, std::vector<float>& transform)
{
    std::string filepath = "Assets/models/" + modelName + ".obj";
    const char* path = filepath.c_str();

    scenes.back().models.push_back(LowRenderer::Model(transform));
    if (loadObj(path))
    {
        for (Resources::Mesh& mesh : scenes.back().models.back().meshes)
        {
            mesh.setIndices();
        }
    }
    loadMaterials();
    
}

bool ResourcesManager::loadObj(const char* filename)
{
    using namespace Core::Maths;

    // ASSERT
    Core::Debug::Assertion::assertTest(!std::string(filename).empty());


    std::ifstream readFile;
    readFile.open(filename, std::ios::in);
    int meshesStart = meshes.size();
    int meshesEnd = meshesStart;

    if (readFile.is_open())
    {
        std::string line;

        std::vector<vec3> vertices;
        std::vector<vec2> texCoords;
        std::vector<vec3> normals;
        bool addedTexture = false;
        int faceCount = 0;

        while (std::getline(readFile, line))
        {
            if (line[0] != 'v' && line[0] != 'f' && line[0] != 'u' && line[0] != 'm')
                continue;

            std::string category;
            std::string text;
            bool categoryFound = false;
            bool faceCountIncremented = false;
            bool addedVertexCount = false;
            vec3 in;
            int count = 0;
            int specs[12];

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
                    if (faceCount > 0 && category == "v")
                    {
                        addResource(static_cast<int>(Resources::ResourceType::TEXTURE), faceCount * 3);
                        faceCount = 0;
                    }

                    if (category == "v" && addedTexture)
                    {
                        addedTexture = false;
                    }

                    if (category == "mtllib" && c != '/')
                    {
                        text += c;
                    }
                    else if (c != ' ' && c != '/')
                    {
                        text += c;
                    }

                    if (category == "v" || category == "vt" || category == "vn")
                    {


                        if ((i + 1) == (int)line.length() || line[i + 1] == ' ')
                        {
                            count++;
                            if (count == 1)
                                in.x = std::stof(text);
                            else if (count == 2)
                                in.y = std::stof(text);
                            else if (count == 3)
                                in.z = std::stof(text);
                            text = "";
                        }

                        if (category == "v" && count == 3)
                        {
                            vertices.push_back(in);
                            in = { 0.f, 0.f, 0.f };
                            count = 0;
                        }
                        else if (category == "vt" && count == 2)
                        {
                            texCoords.push_back(in.xy);
                            in = { 0.f, 0.f, 0.f };
                            count = 0;
                        }
                        else if (category == "vn" && count == 3)
                        {
                            normals.push_back(in);
                            in = { 0.f, 0.f, 0.f };
                            count = 0;
                        }
                        continue;
                    }
                    else if (category == "f")
                    {

                        if (!faceCountIncremented)
                        {
                            ++faceCount;
                            faceCountIncremented = true;
                        }

                        if (((i + 1) == (int)line.length() && text != "") || line[i + 1] == ' ' || line[i + 1] == '/')
                        {
                            specs[count] = std::stoi(text) - 1;
                            count++;
                            text = "";
                        }
                        if (count == 9 && (i + 1) == (int)line.length()) // triangle face
                        {
                            vec3 vertex[3] = { vertices.at(specs[0]), vertices.at(specs[3]), vertices.at(specs[6]) };
                            vec2 texCoord[3] = { texCoords.at(specs[1]), texCoords.at(specs[4]), texCoords.at(specs[7]) };
                            vec3 normal[3] = { normals.at(specs[2]), normals.at(specs[5]), normals.at(specs[8]) };
                            meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                vertex[0].x, vertex[0].y, vertex[0].z,
                                1.f, 0.f, 0.f, 1.f,
                                normal[0].x, normal[0].y, normal[0].z,
                                texCoord[0].x, texCoord[0].y,
                                });
                            meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                vertex[1].x, vertex[1].y, vertex[1].z,
                                0.f, 1.f, 0.f, 1.f,
                                normal[1].x, normal[1].y, normal[1].z,
                                texCoord[1].x, texCoord[1].y,
                                });
                            meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                vertex[2].x, vertex[2].y, vertex[2].z,
                                0.f, 0.f, 1.f, 1.f,
                                normal[2].x, normal[2].y, normal[2].z,
                                texCoord[2].x, texCoord[2].y,
                                });
                            count = 0;
                        }
                        else if (count == 12) // 4-vertex face
                        {
                            vec3 vertex[4] = { vertices.at(specs[0]), vertices.at(specs[3]), vertices.at(specs[6]), vertices.at(specs[9]) };
                            vec2 texCoord[4] = { texCoords.at(specs[1]), texCoords.at(specs[4]), texCoords.at(specs[7]), texCoords.at(specs[10]) };
                            vec3 normal[4] = { normals.at(specs[2]), normals.at(specs[5]), normals.at(specs[8]), normals.at(specs[11]) };
                            meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                vertex[0].x, vertex[0].y, vertex[0].z,
                                1.f, 0.f, 0.f, 1.f,
                                normal[0].x, normal[0].y, normal[0].z,
                                texCoord[0].x, texCoord[0].y,
                                });
                            meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                vertex[1].x, vertex[1].y, vertex[1].z,
                                0.f, 1.f, 0.f, 1.f,
                                normal[1].x, normal[1].y, normal[1].z,
                                texCoord[1].x, texCoord[1].y,
                                });
                            meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                vertex[2].x, vertex[2].y, vertex[2].z,
                                0.f, 0.f, 1.f, 1.f,
                                normal[2].x, normal[2].y, normal[2].z,
                                texCoord[2].x, texCoord[2].y,
                                });
                            
                            count = 0;
                        }
                    }
                    else if ((i + 1) == (int)line.length())
                    {
                        if (category == "mtllib")
                        {
                            scenes.back().models.back().materialsFile = text;
                        }
                        else if (category == "usemtl")
                        {
                            addResource(static_cast<int>(Resources::ResourceType::MESH), text, scenes.back().models.back().transform);
                            ++meshesEnd;
                        }
                    }
                }
            }
        }
        readFile.close();
        if (faceCount > 0)
        {
            addResource(static_cast<int>(Resources::ResourceType::TEXTURE), faceCount * 3);
            faceCount = 0;
        }
    }
    else
    {
        Core::Debug::Log::print("Unable to open file: " + std::string(filename), static_cast<int>(Core::Debug::LogType::ERROR));
        return false;
    }
    scenes.back().models.back().meshes = std::vector<Resources::Mesh>(meshes.begin() + meshesStart, meshes.begin() + meshesEnd);
    return true;
}


void ResourcesManager::loadMaterials()
{
    std::vector<std::string> materials;
    std::vector<std::string> texFiles;
    parseMtl(materials, texFiles);
    loadTextures(materials, texFiles);
}


void ResourcesManager::parseMtl(std::vector<std::string>& materials, std::vector<std::string>& texFiles)
{
    // ASSERT
    {
        Core::Debug::Assertion::assertTest(!scenes.back().models.back().materialsFile.empty());
    }

    std::string path = "Assets/mtls/" + scenes.back().models.back().materialsFile;
    const char* filename = path.c_str();

    {
        Core::Debug::Log::print("Materials file: " + path, 0);
    }

    std::ifstream readFile;
    readFile.open(filename, std::ios::in);
    if (readFile.is_open())
    {
        std::string line;

        while (std::getline(readFile, line))
        {
            if (line[0] != 'n' && line[0] != 'm')
                continue;

            std::string category;
            std::string text;
            bool categoryFound = false;
            bool atIndex0 = true;
            int count = 0;
            int specs[9];

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
                    if (c != ' ' && atIndex0)
                    {
                        text += c;
                        atIndex0 = false;
                    }
                    else
                    {
                        text += c;
                    }

                    if ((i + 1) == (int)line.length())
                    {
                        if (category == "newmtl")
                        {
                            materials.push_back(text);
                        }
                        else if (category == "map_Kd")
                        {
                            for (int i = 0; i < text.length(); ++i)
                            {
                                if (text[i] == '\\')
                                {
                                    text = text.substr(i + 1, text.length() - (i + 1));
                                    i = 0;
                                }
                            }
                            texFiles.push_back(text);
                        }
                    }
                }
            }
        }
        readFile.close();
    }
    else
    {
        Core::Debug::Log::print("Unable to open file: " + std::string(filename), 3);
    }
}


void	ResourcesManager::loadTextures(std::vector<std::string>& materials, std::vector<std::string>& texFiles)
{
    int index = textures.size() - scenes.back().models.back().textures.size();
    int vertexCount = 0;
    int textureStart = index;
    int textureEnd = textureStart;
    for (Resources::Mesh& mesh : scenes.back().models.back().meshes)
    {
        for (int i = 0; i < materials.size(); ++i)
        {
            if (mesh.materialsInfo == materials.at(i))
            {
               textures[index].bindTexture();

                // ASSERT 
                Core::Debug::Assertion::assertTest(!texFiles[i].empty());

                // URGENT! Need a way to check if a texture file has already been loaded
                textures[index].processTexData(texFiles[i]);

                ++textureEnd;
                ++index;
            }
        }
        vertexCount += mesh.rdrVertices.size();
    }
    {
        Core::Debug::Log::print("Total mesh vertices: " + std::to_string(vertexCount), 0);
    }
    scenes.back().models.back().textures = 
        std::vector<Resources::Texture>(textures.begin() + textureStart, textures.begin() + textureEnd);
}



