//------------------------------------------------------------------------------
//
// File Name:	CallbackInputManager.h
// Author(s):	Jacob Holyfield
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <stdafx.h>
#include <Vector2D.h>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

//typedef std::function<void()> Callback;

//class Vector2D;
struct GLFWwindow;

// Function pointer for Damage event handlers.
// Params:
//   callingObject = The object whose handler is being called.
//   otherObject = The other object that the object collided with.
typedef void(*Callback)(int key);

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class CallbackInputManager
{
public:

	//using Callback = std::function<void()>;

	// Retrieve the instance of the Input singleton.
	static CallbackInputManager& GetInstance();

	void addKeyPressBinding(int key, const Callback callback);

	//void removeKeyPressBinding(int key, const Callback& callback);

	void onKeyPress(int key);

	void addKeyReleaseBinding(int key, const Callback callback);

	//void removeKeyReleaseBinding(int key, const Callback& callback);

	void onKeyRelease(int key);

	// Get the current location of the mouse in screen coordinates
	Vector2D GetCursorPosition() const;

	double GetCursorScroll() const;

private:
	// Constructor/destructor private to prevent accidental instantiation/destruction.
	CallbackInputManager();

	// Disable copy constructor and assignment operator
	CallbackInputManager(const CallbackInputManager&) = delete;
	CallbackInputManager& operator=(const CallbackInputManager&) = delete;

	std::map<int, std::vector<Callback>> m_KeyPressCallbacks;
	std::map<int, std::vector<Callback>> m_KeyReleaseCallbacks;

	static void InputCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	static void InputCursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static void InputMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	double scroll;
	Vector2D CursorPosition;
};

//------------------------------------------------------------------------------
