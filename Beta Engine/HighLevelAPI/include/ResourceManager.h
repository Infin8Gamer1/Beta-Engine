//------------------------------------------------------------------------------
//
// File Name:	ResourceManager.h
// Author(s):	Jeremy Kings (j.kings), Jacob Holyfield (j.holyfield)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <Vector2D.h>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class Texture;
class SpriteSource;
class Tilemap;
//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ResourceManager
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	ResourceManager();

	// Destructor
	~ResourceManager();

	// Retrieve an existing mesh required by a game object.
	// Params:
	//   objectName = The name of the mesh.
	//   createIfNotFound = Whether to create a default quad mesh if no mesh is found.
	Mesh* GetMesh(const std::string& objectName, bool createIfNotFound = true, Vector2D textureSize = Vector2D(1,1), Vector2D textureExtents = Vector2D(1,1));

	bool MeshExists(const Mesh* mesh);

	void AddMesh(Mesh* mesh);

	// Retrieve an existing mesh required by a game object.
	// Params:
	//   objectName = The name of the mesh.
	//   createIfNotFound = Whether to create a default quad mesh if no mesh is found.
	Texture* GetTexture(const std::string& objectName, bool createIfNotFound = true);

	bool TextureExists(const Texture* mesh);

	void AddTexture(Texture* mesh);

	// Retrieve a sprite source that uses a given texture, create it if not found.
	// Params:
	//	 textureName = Filename of the texture used by the sprite source.
	//	 numCols = The number of columns in the sprite sheet.
	//	 numRows = The number of rows in the sprite sheet.
	//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
	SpriteSource* GetSpriteSource(const std::string& textureName, unsigned numCols = 1, unsigned numRows = 1, unsigned frameCount = 1, unsigned frameStart = 0, bool createIfNotFound = true);

	// Retrieve a sprite source that uses a given texture, create it if not found.
	// Params:
	//	 textureName = Filename of the texture used by the sprite source.
	SpriteSource* GetSpriteSource(const std::string& Name, bool createIfNotFound = true);

	bool SpriteSourceExists(const SpriteSource* mesh);

	void AddSpriteSource(SpriteSource* mesh);

	void SaveSpriteSourceToFile(SpriteSource * object);

	// Retrieve an existing Tilemap required by a game object.
	// Params:
	//   tilemapName = The name of the tilemap.
	//   createIfNotFound = Whether to create a tilemap if no tilemap is found.
	Tilemap* GetTilemap(const std::string& tilemapName, bool createIfNotFound = true, bool reload = false);

	bool TilemapExists(Tilemap* map);

	void AddTilemap(Tilemap* map);

	void SaveTilemapToFile(Tilemap* map);

	Tilemap* LoadTileMapFromFile(const std::string& tilemapName);

	// Unloads all resources used by the resource manager.
	void Shutdown();

	// Returns an instance of the factory.
	static ResourceManager& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	template<class T>
	void DeleteVector(std::vector<T*>& vector)
	{
		class std::vector<T*>::iterator i;

		for (i = vector.begin(); i != vector.end(); ++i)
		{
			delete (*i);
			*i = nullptr;
		}

		vector.clear();
		vector.shrink_to_fit();
	}


	std::vector<std::string> explodeString(const std::string& str, const char& ch);
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	
	std::vector<Mesh*> Meshes;

	std::vector<Texture*> Textures;
	
	std::vector<SpriteSource*> SpriteSources;

	std::vector<Tilemap*> Tilemaps;

	std::string SpriteSourcesFilePath = "Assets/SpriteSources/";

	std::string TileMapsFilePath = "Assets/TileMaps/";

};

//------------------------------------------------------------------------------
