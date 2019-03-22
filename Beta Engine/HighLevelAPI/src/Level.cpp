//------------------------------------------------------------------------------
//
// File Name:	Level.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Level.h"
#include <Space.h>
#include <Parser.h>
#include <GameObjectFactory.h>
#include <Transform.h>
#include <sstream>

Level::Level(const std::string & name) : BetaObject(name)
{
	fileLocation = "";
}

Space * Level::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}

void Level::Deserialize(Parser & parser)
{
	parser.ReadSkip(GetName());
	parser.ReadSkip("{");

	unsigned numGameObjects = 0;
	parser.ReadVar(numGameObjects);

	parser.ReadSkip("{");

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		/*std::string GameObjectName;
		parser.ReadVar(GameObjectName);

		GameObject* object = GameObjectFactory::GetInstance().CreateObject(GameObjectName);

		std::string TranslationString;
		parser.ReadVariable("Translation", TranslationString);

		Transform* transform = object->GetComponent<Transform>();

		if (transform != nullptr && (TranslationString != "" && TranslationString != "Null")) {
			std::stringstream ss;
			ss.str(TranslationString);

			Vector2D Translation;
			ss >> Translation;

			transform->SetTranslation(Translation);

			object->SetSaveTranslation(true);
		}
		else {
			object->SetSaveTranslation(false);
		}*/

		std::string name;
		name = parser.ReadLine();

		GameObject* object = new GameObject(name);

		object->DeserializeB(parser);

		GetSpace()->GetObjectManager().AddObject(*object);
	}

	parser.ReadSkip("}");

	parser.ReadSkip("}");
}

void Level::Serialize(Parser & parser) const
{
	parser.WriteValue(GetName());

	parser.BeginScope();

	std::vector<GameObject*> activeGOs = GetSpace()->GetObjectManager().GetGameObjectActiveList();

	unsigned numGameObjects = (unsigned)activeGOs.size();
	parser.WriteVar(numGameObjects);

	parser.BeginScope();

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		/*std::string GameObjectName = activeGOs[i]->GetSavePath();
		parser.WriteVar(GameObjectName);

		Transform* transform = activeGOs[i]->GetComponent<Transform>();

		if (transform != nullptr && activeGOs[i]->GetSaveTranslation())
		{
			parser.WriteVariable("Translation", transform->GetTranslation());
		}
		else {
			parser.WriteVariable("Translation", "Null");
		}

		GameObjectFactory::GetInstance().SaveObjectToFile(activeGOs[i]);*/

		activeGOs[i]->Serialize(parser);
	}

	parser.EndScope();

	parser.EndScope();
}

void Level::SaveLevel()
{
	if (GetFileLocation() != "") {
		Parser* parser = new Parser(GetFileLocation(), std::fstream::out);

		try
		{
			Serialize(*parser);
		}
		catch (ParseException e)
		{
			std::cout << e.what() << std::endl;

			DisplayMessage(e.what());
		}

		delete parser;
		parser = nullptr;
	}
}

void Level::LoadLevel()
{
	if (GetFileLocation() != "") {
		Parser* parser = new Parser(GetFileLocation(), std::fstream::in);

		try
		{
			Deserialize(*parser);
		}
		catch (ParseException e)
		{
			std::cout << e.what() << std::endl;

			DisplayMessage(e.what());
		}

		delete parser;
		parser = nullptr;
	}
}

void Level::SetFileLocation(std::string _fileLocation)
{
	fileLocation = _fileLocation;
}

std::string Level::GetFileLocation()
{
	return fileLocation;
}

void Level::DisplayMessage(std::string what)
{
	MessageBox(
		NULL,
		what.c_str(),
		"There Was An ERROR!",
		MB_ICONHAND | MB_OK | MB_APPLMODAL
	);
}


