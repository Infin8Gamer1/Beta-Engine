//------------------------------------------------------------------------------
//
// File Name:	ResourceManager.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "SpriteSource.h"
#include "MeshHelper.h"
#include <Mesh.h>
#include <Texture.h>
#include <Parser.h>

static ResourceManager* Instance;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	Shutdown();
}

Mesh * ResourceManager::GetMesh(const std::string & objectName, bool createIfNotFound, Vector2D textureSize, Vector2D textureExtents)
{
	for (size_t i = 0; i < Meshes.size(); i++)
	{
		std::string currentName = Meshes[i]->GetName();

		if (currentName == objectName) {
			return Meshes[i];
		}
	}

	if (createIfNotFound) {
		Mesh* mesh = CreateQuadMesh(textureSize, textureExtents);

		mesh->SetName(objectName);

		AddMesh(mesh);

		return mesh;
	}
	
	return nullptr;
}

bool ResourceManager::MeshExists(const Mesh* mesh) {

	std::string meshName = mesh->GetName();

	for (size_t i = 0; i < Meshes.size(); i++)
	{
		std::string currentName = Meshes[i]->GetName();

		if (currentName == meshName) {
			return true;
		}
	}

	return false;
}

void ResourceManager::AddMesh(Mesh * mesh)
{
	if (!MeshExists(mesh)) {
		Meshes.push_back(mesh);
	}
	else {
		delete mesh;
	}
}

Texture * ResourceManager::GetTexture(const std::string & objectName, bool createIfNotFound)
{
	for (size_t i = 0; i < Textures.size(); i++)
	{
		std::string currentName = Textures[i]->GetName();

		std::vector<std::string> tokens = explodeString(currentName, '/');

		if (tokens.back() == objectName) {
			return Textures[i];
		}
	}

	if (createIfNotFound) {
		Texture* texture = Texture::CreateTextureFromFile(objectName);

		AddTexture(texture);

		return texture;
	}

	return nullptr;
}

bool ResourceManager::TextureExists(const Texture * texture)
{
	std::string textureName = texture->GetName();

	for (size_t i = 0; i < Textures.size(); i++)
	{
		std::string currentName = Textures[i]->GetName();

		if (currentName == textureName) {
			return true;
		}
	}

	return false;
}

void ResourceManager::AddTexture(Texture * texture)
{
	if (!TextureExists(texture)) {
		Textures.push_back(texture);
	}
	else {
		delete texture;
	}
}

SpriteSource * ResourceManager::GetSpriteSource(const std::string & textureName, unsigned numCols, unsigned numRows, unsigned frameCount, unsigned frameStart, bool createIfNotFound)
{
	for (size_t i = 0; i < SpriteSources.size(); i++)
	{
		std::string currentName = SpriteSources[i]->GetTexture()->GetName();

		std::vector<std::string> tokens = explodeString(currentName, '/');

		if (tokens.back() == textureName && SpriteSources[i]->GetFrameCount() == frameCount && SpriteSources[i]->GetFrameStart() == frameStart) {
			return SpriteSources[i];
		}
	}

	if (createIfNotFound) {
		Texture* texture = GetTexture(textureName);
		SpriteSource* ss = new SpriteSource("", numCols, numRows, frameCount, frameStart, texture);

		AddSpriteSource(ss);

		return ss;
	}

	return nullptr;
}

SpriteSource * ResourceManager::GetSpriteSource(const std::string & Name, bool createIfNotFound)
{
	for (size_t i = 0; i < SpriteSources.size(); i++)
	{
		std::string currentName = SpriteSources[i]->GetName();

		if (currentName == Name)
		{
			return SpriteSources[i];
		}
	}

	if (createIfNotFound) {
		Parser* parser = new Parser(SpriteSourcesFilePath + Name + ".spriteSource", std::fstream::in);

		SpriteSource* ss = new SpriteSource(Name);

		ss->Deserialize(*parser);

		delete parser;
		parser = nullptr;

		AddSpriteSource(ss);

		return ss;
	}

	return nullptr;
}

bool ResourceManager::SpriteSourceExists(const SpriteSource * ss)
{
	std::string ssName = ss->GetName();

	for (size_t i = 0; i < SpriteSources.size(); i++)
	{
		std::string currentName = SpriteSources[i]->GetName();

		if (currentName == ssName) {
			return true;
		}
	}

	return false;
}

void ResourceManager::AddSpriteSource(SpriteSource * ss)
{
	if (!SpriteSourceExists(ss)) {
		SpriteSources.push_back(ss);
	}
	else {
		delete ss;
	}
}

void ResourceManager::SaveSpriteSourceToFile(SpriteSource * object)
{
	Parser* parser = new Parser(SpriteSourcesFilePath + object->GetName() + ".spriteSource", std::fstream::out);

	object->Serialize(*parser);

	delete parser;
	parser = nullptr;

	std::cout << "The Game Object: " << object->GetName() << " was saved to \"" << SpriteSourcesFilePath << object->GetName() << ".spriteSource\"" << std::endl;
}

Tilemap * ResourceManager::GetTilemap(const std::string & tilemapName, bool createIfNotFound, bool reload)
{
	if (reload) {
		for (size_t i = 0; i < Tilemaps.size(); i++)
		{
			std::string currentName = Tilemaps[i]->GetName();

			if (currentName == tilemapName) {
				Tilemap* map = LoadTileMapFromFile(tilemapName);

				if (map != nullptr)
				{
					//reload the tilemap and put it in tilemaps[i]
					Tilemaps[i] = map;
				}
				else {
					std::cout << "Error Reloading Tilemap!";

					delete map;
				}

				//return tilemaps[i]
				return Tilemaps[i];
			}
		}
	}
	

	for (size_t i = 0; i < Tilemaps.size(); i++)
	{
		std::string currentName = Tilemaps[i]->GetName();

		if (currentName == tilemapName) {
			return Tilemaps[i];
		}
	}

	if (createIfNotFound) {

		Tilemap* map = LoadTileMapFromFile(tilemapName);

		map->Print();

		if (map == nullptr)
		{
			std::cout << "Error Loading Tilemap!";

			delete map;
		}
		else {
			AddTilemap(map);

			return map;
		}
	}

	return nullptr;	
}

bool ResourceManager::TilemapExists(Tilemap * map)
{
	std::string tilemapName = map->GetName();
	
	for (size_t i = 0; i < Tilemaps.size(); i++)
	{
		std::string currentName = Tilemaps[i]->GetName();

		if (currentName == tilemapName) {
			return true;
		}
	}

	return false;
}

void ResourceManager::AddTilemap(Tilemap * map)
{
	if (!TilemapExists(map)) {
		Tilemaps.push_back(map);
	}
	else {
		delete map;
	}
}

void ResourceManager::SaveTilemapToFile(Tilemap * map)
{
	Parser* parser = new Parser(TileMapsFilePath + map->GetName() + ".tileMap", std::fstream::out);

	map->Serialize(*parser);

	delete parser;
	parser = nullptr;

	std::cout << "The tilemap: " << map->GetName() << " was saved to \"" << TileMapsFilePath << map->GetName() << ".tileMap\"" << std::endl;
}

Tilemap * ResourceManager::LoadTileMapFromFile(const std::string & tilemapName)
{
	Parser* parser = new Parser(TileMapsFilePath + tilemapName + ".tileMap", std::fstream::in);

	Tilemap* map = new Tilemap(tilemapName);

	map->Deserialize(*parser);

	delete parser;
	parser = nullptr;

	return map;
}

void ResourceManager::Shutdown()
{
	DeleteVector<Mesh>(Meshes);

	DeleteVector<SpriteSource>(SpriteSources);

	DeleteVector<Tilemap>(Tilemaps);

	DeleteVector<Texture>(Textures);
}

ResourceManager & ResourceManager::GetInstance()
{
	if (Instance == nullptr) {
		Instance = new ResourceManager();
	}

	return *Instance;
}

std::vector<std::string> ResourceManager::explodeString(const std::string& str, const char& ch) {
	std::string next;
	std::vector<std::string> result;

	// For each character in the string
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(next);
				next.clear();
			}
		}
		else {
			// Accumulate the next character into the sequence
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}
