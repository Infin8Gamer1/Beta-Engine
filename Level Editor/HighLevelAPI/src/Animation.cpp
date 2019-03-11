//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Sprite.h"
#include "GameObject.h"
#include <SpriteSource.h>

Animation::Animation() : Component("Animation")
{
	frameIndex = 0;

	frameStart = 0;
	frameEnd = 0;
	
	frameDelay = 0;

	frameDuration = 20;

	isRunning = false;
	isLooping = false;
	isDone = false;
	playInReverse = false;

	sprite = nullptr;

	lastSpriteSource = nullptr;
}

Component * Animation::Clone() const
{
	Animation* out = new Animation();

	//out->sprite = sprite;

	return out;
}

void Animation::Initialize()
{
	sprite = GetOwner()->GetComponent<Sprite>();

	lastSpriteSource = sprite->GetSpriteSource();
}

void Animation::Play(float frameDurationInput, bool isLoopingInput, bool playInReverseInput)
{
	//std::cout << "Animation::Play" << std::endl;

	lastSpriteSource = sprite->GetSpriteSource();

	isLooping = isLoopingInput;
	playInReverse = playInReverseInput;


	if (!playInReverse) {
		frameStart = sprite->GetSpriteSource()->GetFrameStart();
		frameEnd = sprite->GetSpriteSource()->GetFrameStart() + sprite->GetSpriteSource()->GetFrameCount() - 1;
	}
	else {
		frameStart = sprite->GetSpriteSource()->GetFrameStart() + sprite->GetSpriteSource()->GetFrameCount() - 1;
		frameEnd = sprite->GetSpriteSource()->GetFrameStart();
	}
	
	frameIndex = frameStart;

	//std::cout << "Start : " << frameStart << ", End : " << frameEnd << std::endl;

	GetOwner()->GetComponent<Sprite>()->SetFrame(frameIndex);

	frameDuration = frameDurationInput;
	frameDelay = frameDuration;

	isRunning = true;
	isDone = false;
}

void Animation::FixedUpdate(float dt)
{
	//std::cout << "Animation::FixedUpdate" << std::endl;

	//check if sprite source has been updated
	if (lastSpriteSource != sprite->GetSpriteSource()) {
		lastSpriteSource = sprite->GetSpriteSource();
		Play(frameDuration, isLooping, playInReverse);
	}
	
	isDone = false;

	if (!isRunning) {
		return;
	}

	frameDelay -= dt;

	//move on to the next frame if delay has been reached
	if (frameDelay <= 0.0f) {
		

		//move frame up or down
		if (playInReverse) {
			frameIndex--;
		} else {
			frameIndex++;
		}

		if ((frameIndex > frameEnd && !playInReverse) || (frameIndex < frameEnd && playInReverse))
		{
			if (isLooping)
			{
				frameIndex = frameStart;
				isDone = true;
				isRunning = true;
			} else {
				frameIndex = frameStart;
				isDone = true;
				isRunning = false;
			}
		}

		sprite->SetFrame(frameIndex);

		frameDelay = frameDuration;
	}
}

bool Animation::IsDone() const
{
	return isDone;
}

bool Animation::IsRunning() const {
	return isRunning;
}

void Animation::SetFrameDuration(float duration)
{
	frameDuration = duration;
}
