#pragma once

#include "Mesh.h"
#include "Utils.h"
#include "Vertex.h"
#include "Vec.h"
#include "Texture.h"

#include <regex>
#include <list>
#include <string>

namespace Crystal
{
	class WavefrontModel : public Mesh
	{
	public:
		WavefrontModel();
		WavefrontModel(std::string filename);
		WavefrontModel(std::string filenameObj, std::string filenameTexture);
		void loadFromString(std::string source);
		void loadTextureFromString(std::string source);
		virtual void render(Renderer* renderer);

	private:
		std::vector<std::string> lines;
		std::vector<Vec> positions;
		std::vector<Vec> normals;
		std::vector<Vec> uvs;
		std::vector<Vertex> faces;

		Texture texture;

		char* posRegex = "^v\\s(.*)\\s(.*)\\s(.*)$";
		char* normalRegex = "^vn\\s(.*)\\s(.*)\\s(.*)$";
		char* faceRegex = "^f\\s(.*)\/\/(.*)\\s(.*)\/\/(.*)\\s(.*)\/\/(.*)$";
	};
}