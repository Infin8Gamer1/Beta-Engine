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

Sprite::Sprite() : Component("Sprite")
{
	transform = nullptr;
	frameIndex = 0;
	spriteSource = nullptr;
	mesh = nullptr;
	color = Color();
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
