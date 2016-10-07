#include "WavefrontModel.h"

namespace Crystal
{
	WavefrontModel::WavefrontModel()
	{
	}

	WavefrontModel::WavefrontModel(std::string filename)
	{
		loadFromString(loadFile(filename));
	}

	WavefrontModel::WavefrontModel(std::string filenameObj, std::string filenameTexture)
	{
		loadFromString(loadFile(filenameObj));
	}

	void WavefrontModel::loadTextureFromString(std::string source)
	{
		texture.load(source);
	}

	void WavefrontModel::render(Renderer* renderer)
	{
		texture.bind(0);
		renderer->render(this);
	}

	void WavefrontModel::loadFromString(std::string source)
	{
		lines = split(source, "\n");

		std::regex pos(posRegex);
		std::regex normal(normalRegex);
		std::regex face(faceRegex);

		//Reporter::report((pos));

		for (const std::string line : lines)
		{
			std::string normalPrefix = "vn ";
			std::string vectorPrefix = "v ";
			std::string uvPrefix = "vt ";
			std::string facePrefix = "f ";

			if (line.substr(0, normalPrefix.size()) == normalPrefix)
			{
				std::string argumentValue = line.substr(normalPrefix.size());
				std::vector<std::string> values = split(argumentValue, " ");
				normals.push_back(Vec(stringToFloat(values[0]), stringToFloat(values[1]), stringToFloat(values[2])));
			}
			else if (line.substr(0, uvPrefix.size()) == uvPrefix)
			{
				std::string argumentValue = line.substr(uvPrefix.size());
				std::vector<std::string> values = split(argumentValue, " ");
				uvs.push_back(Vec(stringToFloat(values[0]), stringToFloat(values[1]), 0));
			}
			else if (line.substr(0, vectorPrefix.size()) == vectorPrefix)
			{
				std::string argumentValue = line.substr(vectorPrefix.size());
				std::vector<std::string> values = split(argumentValue, " ");
				positions.push_back(Vec(stringToFloat(values[0]), stringToFloat(values[1]), stringToFloat(values[2])));
			}
			else if (line.substr(0, facePrefix.size()) == facePrefix)
			{
				std::string argumentValue = line.substr(facePrefix.size());

				std::vector<std::string> values = split(argumentValue, " ");
				std::vector<std::string> value1 = split(values[0], "/");
				//value1.erase(std::remove(value1.begin(), value1.end(), ""), value1.end());
				std::vector<std::string> value2 = split(values[1], "/");
				//value2.erase(std::remove(value2.begin(), value2.end(), ""), value2.end());
				std::vector<std::string> value3 = split(values[2], "/");
				//value3.erase(std::remove(value3.begin(), value3.end(), ""), value3.end());

				faces.push_back(Vertex(
					Vec(stringToFloat(value1[0]), stringToFloat(value2[0]), stringToFloat(value3[0])),
					Vec(stringToFloat(value1[2]), stringToFloat(value2[2]), stringToFloat(value3[2])),
					Vec(stringToFloat(value1[1]), stringToFloat(value2[1]), stringToFloat(value3[1]))
					));
			}

			//char* string = (char*)line.c_str();

			//int res = fscanf(file, "%s", string);

			/*std::smatch match;

			if (std::regex_search(line.begin(), line.end(), match, pos))
			{
				positions.push_back(Vec(stringToFloat(match[1]), stringToFloat(match[2]), stringToFloat(match[3])));
			}

			if (std::regex_search(line.begin(), line.end(), match, normal))
			{
				normals.push_back(Vec(stringToFloat(match[1]), stringToFloat(match[2]), stringToFloat(match[3])));
			}

			if (std::regex_search(line.begin(), line.end(), match, face))
			{
				faces.push_back(Vertex(
					Vec(stringToFloat(match[1]), stringToFloat(match[3]), stringToFloat(match[5])),
					Vec(stringToFloat(match[2]), stringToFloat(match[4]), stringToFloat(match[6]))
					));
			}*/
		}

		for (Vertex face : faces)
		{
			vertices.push_back(Vertex(
				positions[face.pos.x() - 1],
				normals[face.normal.x() - 1],
				uvs[face.uvCoord.x() - 1]
				));
			vertices.push_back(Vertex(
				positions[face.pos.y() - 1],
				normals[face.normal.y() - 1],
				uvs[face.uvCoord.y() - 1]
				));
			vertices.push_back(Vertex(
				positions[face.pos.z() - 1],
				normals[face.normal.z() - 1],
				uvs[face.uvCoord.z() - 1]
				));
		}
	}
} 