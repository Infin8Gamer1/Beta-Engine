//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"
#include <Vector2D.h>
#include <Texture.h>
#include <Parser.h>
#include <ResourceManager.h>
#include <Texture.h>

SpriteSource::SpriteSource(std::string _name, unsigned numColsInput, unsigned numRowsInput, unsigned frameCountInput, unsigned frameStartInput, Texture * textureInput) : Serializable()
{
	numRows = numRowsInput;
	numCols = numColsInput;
	frameCount = frameCountInput;
	frameStart = frameStartInput;
	texture = textureInput;

	if (_name == "") {
		name = textureInput->GetName() + "_" + std::to_string(frameStart) + "_" + std::to_string(frameCount);
	}
	else {
		name = _name;
	}
	
}

void SpriteSource::Deserialize(Parser & parser)
{
	std::string mName = this->GetName();
	parser.ReadSkip(mName);
	parser.ReadSkip("{");

	std::string textureName;
	parser.ReadVariable("textureName", textureName);
	parser.ReadVariable("textureRows", numRows);
	parser.ReadVariable("textureColumns", numCols);
	parser.ReadVariable("frameCount", frameCount);
	parser.ReadVariable("frameStart", frameStart);

	if (textureName != "null" && textureName != "none") {
		texture = ResourceManager::GetInstance().GetTexture(textureName);
	}

	parser.ReadSkip("}");
}

void SpriteSource::Serialize(Parser & parser) const
{
	parser.WriteValue(GetName());

	parser.BeginScope();

	parser.WriteVariable("textureName", GetTexture()->GetName());
	parser.WriteVariable("textureRows", numRows);
	parser.WriteVariable("textureColumns", numCols);
	parser.WriteVariable("frameCount", frameCount);
	parser.WriteVariable("frameStart", frameStart);

	parser.EndScope();
}

Texture * SpriteSource::GetTexture() const
{
	return texture;
}

void SpriteSource::SetTexture(Texture * _texture)
{
	this->texture = _texture;
}

unsigned SpriteSource::GetFrameCountTexture() const
{
	return numRows * numCols;
}

unsigned SpriteSource::GetFrameCount() const
{
	return frameCount;
}

unsigned SpriteSource::GetFrameStart() const
{
	return frameStart;
}

std::string SpriteSource::GetName() const
{
	return name;
}

void SpriteSource::SetName(const std::string &Name)
{
	name = Name;
}

const Vector2D SpriteSource::GetUV(unsigned int frameIndex) const
{
	
	float k_X = 1.0f / static_cast<float>(numCols);
	float k_Y = 1.0f / static_cast<float>(numRows);

	float xOffset = k_X * (frameIndex % numCols);
	float yOffset = k_Y * (frameIndex / numCols);

	return Vector2D(xOffset, yOffset);
}

const std::string & SpriteSource::GetTextureName() const
{
	return texture->GetName();
}

const Vector2D SpriteSource::GetTextureDimensions() const
{
	return Vector2D((float)numCols, (float)numRows);
}
