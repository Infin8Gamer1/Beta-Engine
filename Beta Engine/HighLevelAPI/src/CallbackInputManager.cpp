#include "stdafx.h"
#include "CallbackInputManager.h"
#include <Vector2D.h>
#include <System.h>

CallbackInputManager & CallbackInputManager::GetInstance()
{
	static CallbackInputManager instance;

	return instance;
}

void CallbackInputManager::InputCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	GetInstance().CursorPosition = Vector2D((float)xpos, (float)ypos);
}

void CallbackInputManager::InputCursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	GetInstance().scroll = yoffset;
}

void CallbackInputManager::InputMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		GetInstance().onKeyPress(button);
	}
	else if (action == GLFW_RELEASE) {
		GetInstance().onKeyRelease(button);
	}
}

void CallbackInputManager::InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		GetInstance().onKeyPress(key);
	}
	else if (action == GLFW_RELEASE) {
		GetInstance().onKeyRelease(key);
	}
}

CallbackInputManager::CallbackInputManager()
{
	GLFWwindow* handle = System::GetInstance().GetWindowHandle();

	glfwSetCursorPosCallback(handle, (GLFWcursorposfun)InputCursorPosCallback);
	glfwSetScrollCallback(handle, (GLFWscrollfun)InputCursorScrollCallback);
	glfwSetMouseButtonCallback(handle, (GLFWmousebuttonfun)InputMouseButtonCallback);
	glfwSetKeyCallback(handle, (GLFWkeyfun)InputKeyCallback);
}

void CallbackInputManager::addKeyPressBinding(int key, const Callback callback)
{
	m_KeyPressCallbacks[key].push_back(callback);
}

void CallbackInputManager::onKeyPress(int key)
{
	for (Callback& call : m_KeyPressCallbacks[key])
	{
		call(key);
	}
}

void CallbackInputManager::addKeyReleaseBinding(int key, const Callback callback)
{
	m_KeyReleaseCallbacks[key].push_back(callback);
}

void CallbackInputManager::onKeyRelease(int key)
{
	for (Callback& call : m_KeyReleaseCallbacks[key])
	{
		call(key);
	}
}

Vector2D CallbackInputManager::GetCursorPosition() const
{
	return CursorPosition;
}

double CallbackInputManager::GetCursorScroll() const
{
	return scroll;
}