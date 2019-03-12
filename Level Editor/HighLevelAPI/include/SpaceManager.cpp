#include "stdafx.h"
#include "SpaceManager.h"
#include "Space.h"
#include <GameObjectFactory.h>
#include <ResourceManager.h>

SpaceManager::SpaceManager() : BetaObject("SpaceManager")
{
}

SpaceManager::~SpaceManager()
{
	//Shutdown();
}

void SpaceManager::Update(float dt)
{
	for (size_t i = 0; i < spaceList.size(); i++)
	{
		spaceList[i]->Update(dt);
	}
}

void SpaceManager::Shutdown(void)
{
	std::vector<Space*>::iterator i;

	for (i = spaceList.begin(); i != spaceList.end(); ++i)
	{
		delete (*i);
		*i = nullptr;
	}

	spaceList.clear();
	spaceList.shrink_to_fit();

	GameObjectFactory* GOFactory = &GameObjectFactory::GetInstance();
	delete GOFactory;

	ResourceManager* ReManager = &ResourceManager::GetInstance();
	delete ReManager;
}

void SpaceManager::AddSpace(Space & space)
{
	spaceList.push_back(&space);
}

Space * SpaceManager::GetSpaceByName(const std::string & spaceName) const
{
	for (size_t i = 0; i < spaceList.size(); i++)
	{
		if (spaceList[i]->GetName() == spaceName) {
			return spaceList[i];
		}
	}

	std::cout << "Couldn't find space with name of " << spaceName << "." << std::endl;
	return nullptr;
}

unsigned SpaceManager::GetSpaceCount(const std::string & objectName) const
{
	return static_cast<unsigned>(spaceList.size());
}
