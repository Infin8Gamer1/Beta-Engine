//------------------------------------------------------------------------------
//
// File Name:	SoundManager.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SoundManager.h"
#include <stdio.h>
#include <fmod_errors.h>

SoundManager::SoundManager() : BetaObject("SoundManager")
{

	// Individual sound files
	numSounds = 0;
	//FMOD::Sound* soundList[maxNumSounds];

	// Sound banks
	numBanks = 0;
	//FMOD::Studio::Bank* bankList[maxNumBanks];

	// Systems
	studioSystem = nullptr;			// The internal FMOD studio system.
	FMOD_Assert(FMOD::Studio::System::create(&studioSystem)); //create studio system
	FMOD_Assert(studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0)); //init FMOD system

	system = nullptr;	// The internal FMOD low-level system
	FMOD_Assert(studioSystem->getLowLevelSystem(&system)); //set low level system from high level system

	// Channels
	musicChannel = nullptr;				// The channel most recently used to play music

	effectsChannelGroup = nullptr;	// The channel group used for SFX.
	FMOD_Assert(system->createChannelGroup("SoundEffects", &effectsChannelGroup));
	

	// Volume
	musicVolume = 1.0f;	 // The current volume of the music channel (0.0 to 1.0).
	effectsVolume = 1.0f; // The current volume of the sound effects channel group (0.0 to 1.0).

	// Directories
	audioFilePath = "Audio/"; // Directory of audio assets
	bankFilePath = "Banks/"; // Subdirectory for FMOD bank files
	eventPrefix = "event:/"; // All events start with "event:/"
}

SoundManager::~SoundManager()
{
	FMOD_Assert(studioSystem->release());
}

void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	studioSystem->update();
}

void SoundManager::Shutdown(void)
{
	for (size_t i = 0; i < numSounds; i++)
	{
		FMOD_Assert(soundList[i]->release());
		soundList[i] = nullptr;
	}

	numSounds = 0;

	for (size_t i = 0; i < numBanks; i++)
	{
		FMOD_Assert(bankList[i]->unload());
		bankList[i] = nullptr;
	}

	numBanks = 0;
}

void SoundManager::AddEffect(const std::string & filename)
{
	AddSound(filename, FMOD_DEFAULT);
}

void SoundManager::AddMusic(const std::string & filename)
{
	AddSound(filename, FMOD_DEFAULT | FMOD_LOOP_NORMAL);
}

void SoundManager::AddBank(const std::string & filename)
{
	//make the path to the bank
	std::string fullFilePath = "Assets/" + audioFilePath + bankFilePath + filename;
	//create and load bank
	FMOD::Studio::Bank *bank;
	FMOD_Assert(studioSystem->loadBankFile(fullFilePath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
	//add bank to banks list
	bankList[numBanks] = bank;
	numBanks += 1;
}

FMOD::Channel * SoundManager::PlaySound(const std::string & _name)
{
	//Create a C-style string (character pointer) of length 100 or so.
	char soundName[100];
	//Create a variable to store the mode of the sound (FMOD_MODE).
	FMOD_MODE *soundMode = new FMOD_MODE();

	//Loop through all sounds in the sound list
	for (size_t i = 0; i < numSounds; i++)
	{
		//Get the name of the sound from the sound object
		FMOD_Assert(soundList[i]->getName(soundName, 100));
		//If the name of the sound is not equal to the sound name that was passed in, skip this sound
		if (strcmp(soundName, _name.c_str()) == 0) {
			//get the mode of the sound object
			FMOD_Assert(soundList[i]->getMode(soundMode));
			//If the mode indicates it's a stream
			if (*soundMode & FMOD_CREATESTREAM) {
				//Return the result of using the PlayMusic helper function with that sound object
				delete soundMode;
				soundMode = nullptr;
				return PlayMusic(soundList[i]);
			} else {
				//Return the result of using the PlayEffect helper function with that sound object.
				delete soundMode;
				soundMode = nullptr;
				return PlayEffect(soundList[i]);
			}
		}
	}

	
	
	//If no sound with that name was found, return nullptr.
	std::cout << "Couldn't find sound to play with name : " + _name;
	return nullptr;
}

FMOD::Studio::EventInstance * SoundManager::PlayEvent(const std::string & _name)
{
	//Create a string that contains the full event name (eventPrefix + eventName)
	std::string eventPath = eventPrefix + _name;

	//Create pointer variables for an FMOD::Studio::EventDescription and an FMOD::Studio::EventInstance
	FMOD::Studio::EventDescription *eventDescription;
	FMOD::Studio::EventInstance *eventInstance;

	//Get an event with the given name from the studio system
	FMOD_Assert(studioSystem->getEvent(eventPath.c_str(), &eventDescription));
	//Create an instance of that event using the event description
	FMOD_Assert(eventDescription->createInstance(&eventInstance));
	//Start the instance
	eventInstance->start();
	//Release the instance (this doesn't release it immediately; it waits until the event is finished to destroy it)
	eventInstance->release();
	//Return the instance so that it can be modified later
	return eventInstance;
}

void SoundManager::SetMusicVolume(float volume)
{
	musicVolume = volume;
}

void SoundManager::SetEffectsVolume(float volume)
{
	effectsVolume = volume;
}

float SoundManager::GetMusicVolume() const
{
	return musicVolume;
}

float SoundManager::GetEffectsVolume() const
{
	return effectsVolume;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

FMOD::Channel * SoundManager::PlayEffect(FMOD::Sound * sound) const
{
	//Create an FMOD::Channel pointer
	FMOD::Channel *channel;
	//Use the low - level system to play a sound using the effects channel group
	system->playSound(sound, effectsChannelGroup, false, &channel);
	//Return the channel that was used so it can be modified later if needed
	return channel;
}

FMOD::Channel * SoundManager::PlayMusic(FMOD::Sound * sound)
{
	//If the music channel exists(isn't null), stop whatever sound is currently playing on that channel
	if (musicChannel != nullptr) {
		musicChannel->stop();
	}
	//get the music chanel group
	FMOD::ChannelGroup *musicChanelGroup;
	musicChannel->getChannelGroup(&musicChanelGroup);
	//Use the low - level system to play a sound using the music channel
	FMOD_Assert(system->playSound(sound, musicChanelGroup, false, &musicChannel));
	//Return the music channel so it can be modified later if needed
	return musicChannel;
}

void SoundManager::AddSound(const std::string & filename, FMOD_MODE mode)
{
	//Create a string that contains the full file path(Assets / +audioFilePath + filename)
	std::string filePath;
	filePath.append("Assets/").append(audioFilePath).append(filename);

	//Create an FMOD::Sound pointer variable
	FMOD::Sound *sound;
	//Use the low - level system to create a sound using the given path and mode
	FMOD_Assert(system->createSound(filePath.c_str(), mode, nullptr, &sound));

	//Put the sound in the sound list at the next available index
	soundList[numSounds] = sound;
	//Increase the number of sounds
	numSounds += 1;
}

void SoundManager::FMOD_Assert(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
	}
}
