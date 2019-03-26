//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "GameObject.h"
#include <Mesh.h>
#include <Vertex.h>
#include <Graphics.h>
#include <Parser.h>
#include <ResourceManager.h>
#include <Engine.h>
#include <SpaceManager.h>
#include <FileOpenHelper.h>

Sprite::Sprite() : Component("Sprite")
{
	transform = nullptr;
	frameIndex = 0;
	spriteSource = nullptr;
	mesh = nullptr;
	color = Color();
	bar = nullptr;
	ssName = "";
}

Sprite::~Sprite()
{
	if (bar != nullptr)
	{
		TwRemoveVar(bar, "Frame Index");
		TwRemoveVar(bar, "Color");
		TwRemoveVar(bar, "Sprite Source Name");
		TwRemoveVar(bar, "Open Sprite Source");
	}
}

Component * Sprite::Clone() const
{
	return new Sprite(*this);
}

void Sprite::Deserialize(Parser & parser)
{
	//get frame index
	parser.ReadVariable("frameIndex", frameIndex);
	//get color
	parser.ReadVariable("color", color);
	//get spritesource
	std::string ssName;
	parser.ReadVariable("spriteSourceName", ssName);

	SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource(ssName, true));

	//get mesh
	SetMesh(ResourceManager::GetInstance().GetMesh(GetOwner()->GetName() + "_AutoMesh", true, Vector2D(1.0f / spriteSource->GetTextureDimensions().x, 1.0f / spriteSource->GetTextureDimensions().y)));
}

void Sprite::Serialize(Parser & parser) const
{
	//set frame index
	parser.WriteVariable("frameIndex", frameIndex);
	//set color
	parser.WriteVariable("color", color);
	//set spriteSourceName
	parser.WriteVariable("spriteSourceName", spriteSource->GetName());
}

void TW_CALL OpenSpriteSourceCB(void * clientData)
{
	std::string fileName = FileOpenHelper::BasicFileOpen();

	if (fileName == "")
	{
		return;
	}

	std::string ObjectPath = "Assets\\SpriteSources\\";

	size_t position = fileName.rfind(ObjectPath);

	fileName = fileName.substr(position + ObjectPath.length());

	fileName = fileName.substr(0, fileName.find_first_of("."));

	Sprite* sprite = static_cast<Sprite*>(clientData);

	sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource(fileName, true));

	sprite->RefreshAutoMesh();
}

void Sprite::AddVarsToTweakBar(TwBar * bar_)
{
	bar = bar_;

	Component::AddVarsToTweakBar(bar);
	std::string params = " group='" + GetName() + "' ";

	TwAddVarRW(bar, "Frame Index", TW_TYPE_INT32, (void*)&frameIndex, std::string(params + " min=0 ").c_str());
	TwAddVarRW(bar, "Color", Engine::GetInstance().GetModule<SpaceManager>()->GetColorTwType(), (void*)&color, params.c_str());
	TwAddVarRO(bar, "Sprite Source Name", TW_TYPE_STDSTRING, &ssName, params.c_str());
	TwAddButton(bar, "Open Sprite Source", OpenSpriteSourceCB, this, std::string(params + " label='Open Sprite Source' ").c_str());
}

void Sprite::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
}

void Sprite::Draw()
{
	Draw(Vector2D(0, 0));
}

void Sprite::Draw(const Vector2D& offset)
{
	if (mesh == nullptr || transform == nullptr) {
		return;
	}

	if (spriteSource != nullptr) {
		if (frameIndex == 0) {
			std::cout << "";
		}
		if (spriteSource->GetTexture() != nullptr) {
			Graphics::GetInstance().SetTexture(spriteSource->GetTexture(), spriteSource->GetUV(frameIndex));
		}
		else {
			Graphics::GetInstance().SetTexture(nullptr);
		}
	}
	else {
		Graphics::GetInstance().SetTexture(nullptr);
	}

	Graphics::GetInstance().SetSpriteBlendColor(color);

	Matrix2D matrix = reinterpret_cast<const Matrix2D&>(transform->GetMatrix());
	Matrix2D offsetMatrix = Matrix2D().TranslationMatrix(offset.x, offset.y);
	Graphics::GetInstance().SetTransform(offsetMatrix * matrix);

	mesh->Draw();
}

void Sprite::SetAlpha(float alpha)
{
	color.a = alpha;
}

float Sprite::GetAlpha() const
{
	return color.a;
}

void Sprite::SetFrame(unsigned int frameIndexInput)
{
	if (spriteSource == nullptr) {
		return;
	}
	//std::cout << "FrameIndex : " << frameIndexInput << std::endl;

	//std::cout << "Sprite::SetFrame(" << frameIndexInput << ")" << std::endl;

	if (frameIndexInput < spriteSource->GetFrameCountTexture()) {
		frameIndex = frameIndexInput;
	}
}

unsigned Sprite::GetFrame() const
{
	return frameIndex;
}

void Sprite::SetMesh(Mesh * meshInput)
{
	mesh = meshInput;
}

void Sprite::SetSpriteSource(SpriteSource * spriteSourceInput)
{
	spriteSource = spriteSourceInput;
	ssName = spriteSource->GetName();
}

SpriteSource * Sprite::GetSpriteSource()
{
	return spriteSource;
}

void Sprite::SetColor(Color colorInput)
{
	color = colorInput;
}

const Color & Sprite::GetColor() const
{
	return color;
}

void Sprite::RefreshAutoMesh()
{
	// TODO: Make auto refresh mesh not use a random string to make the name as there is a slight chance that it will break

	char alphanum[] =
		"0123456789"
		"!@#$%^&*"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	int stringLength = sizeof(alphanum) - 1;

	std::string random;

	for (unsigned i = 0; i < 20; i++)
	{
		random += alphanum[rand() % stringLength];
	}

	//get mesh
	SetMesh(ResourceManager::GetInstance().GetMesh(GetOwner()->GetName() + "_AutoMesh_" + random, true, Vector2D(1.0f / spriteSource->GetTextureDimensions().x, 1.0f / spriteSource->GetTextureDimensions().y)));
}


