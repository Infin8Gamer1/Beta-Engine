#include "stdafx.h"
#include "SpaceManager.h"
#include "Space.h"
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <glfw3.h>
#include <System.h>
#include <CallbackInputManager.h>

#define GLFW_CDECL
#include <AntTweakBar.h>

SpaceManager::SpaceManager(bool createTwBar) : BetaObject("SpaceManager")
{
	CreateTwBar = createTwBar;
}

SpaceManager::~SpaceManager()
{
	//Shutdown();
}

static GLFWmousebuttonfun originalMouseButtonCallback = nullptr;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (!TwEventMouseButtonGLFW(button, action))
	{
		if (originalMouseButtonCallback != nullptr)
			originalMouseButtonCallback(window, button, action, mods);
	}
}

static GLFWcursorposfun originalCursorPosCallback = nullptr;

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if (!TwEventMousePosGLFW((int)xpos, (int)ypos))
	{
		if (originalCursorPosCallback != nullptr)
			originalCursorPosCallback(window, xpos, ypos);
	}
}

static GLFWscrollfun originalScrollCallback = nullptr;

void CursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!TwEventMouseWheelGLFW((int)yoffset))
	{
		if (originalScrollCallback != nullptr)
			originalScrollCallback(window, xoffset, yoffset);
	}
}

static GLFWkeyfun originalKeyCallback = nullptr;

void MyKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!TwEventKeyGLFW(key, action))
	{
		if (originalKeyCallback != nullptr)
			originalKeyCallback(window, key, scancode, action, mods);
	}
	else {
		std::cout << "Testing!" << std::endl;
	}
}

static GLFWcharfun originalCharCallback = nullptr;

void CharCallback(GLFWwindow* window, int codepoint) {
	if (!TwEventCharGLFW(codepoint, 0))
	{
		if (originalCharCallback != nullptr)
			originalCharCallback(window, codepoint);
	}
}

static GLFWwindowsizefun originalWindowSizeCallback = nullptr;

void WindowSizeCallback(GLFWwindow* window, int width, int height) {

	TwWindowSize(width, height);

	if(originalWindowSizeCallback != nullptr)
		originalWindowSizeCallback(window, width, height);
}

void SpaceManager::Initialize()
{
	if (CreateTwBar)
	{
		TwInit(TwGraphAPI::TW_OPENGL_CORE, NULL);

		int windowWidth, windowHeight;
		GLFWwindow* handle = System::GetInstance().GetWindowHandle();
		glfwGetWindowSize(handle, &windowWidth, &windowHeight);

		TwWindowSize(windowWidth, windowHeight);

		bar = TwNewBar("BAR");

		TwDefine(" BAR label='Properties' position='30 50' alpha=128 refresh=0.15 movable=true resizable=true contained=false color='28 183 255' "); // Message added to the help bar.

		//init input callback because we need to save these as the original callbacks
		CallbackInputManager::GetInstance();

		// pass GLFW events to AntTweakBar and then call the original handle
		originalMouseButtonCallback = glfwSetMouseButtonCallback(handle, (GLFWmousebuttonfun)MouseButtonCallback);
		originalCursorPosCallback = glfwSetCursorPosCallback(handle, (GLFWcursorposfun)CursorPosCallback);
		originalScrollCallback = glfwSetScrollCallback(handle, (GLFWscrollfun)CursorScrollCallback);
		originalKeyCallback = glfwSetKeyCallback(handle, (GLFWkeyfun)MyKeyCallback);
		originalCharCallback = glfwSetCharCallback(handle, (GLFWcharfun)CharCallback);
		originalWindowSizeCallback = glfwSetWindowSizeCallback(handle, (GLFWwindowsizefun)WindowSizeCallback);
	}
}

void SpaceManager::Update(float dt)
{
	for (size_t i = 0; i < spaceList.size(); i++)
	{
		spaceList[i]->Update(dt);
	}

	TwDraw();
}

void SpaceManager::Shutdown(void)
{
	TwTerminate();

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

Space * SpaceManager::GetSpaceByID(size_t spaceID) const
{
	return spaceList[spaceID];
}

unsigned SpaceManager::GetSpaceCount() const
{
	return static_cast<unsigned>(spaceList.size());
}

TwBar * SpaceManager::GetTwBar()
{
	return bar;
}
