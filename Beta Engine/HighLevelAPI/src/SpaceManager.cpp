#include "stdafx.h"
#include "SpaceManager.h"
#include "Space.h"
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <System.h>
#include <CallbackInputManager.h>
#include <Color.h>

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
	if (!TwEventMouseButtonGLFW3(window, button, action, mods))
	{
		if (originalMouseButtonCallback != nullptr)
			originalMouseButtonCallback(window, button, action, mods);
	}
}

static GLFWcursorposfun originalCursorPosCallback = nullptr;

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if (!TwEventMousePosGLFW3(window, xpos, ypos))
	{
		if (originalCursorPosCallback != nullptr)
			originalCursorPosCallback(window, xpos, ypos);
	}
}

static GLFWscrollfun originalScrollCallback = nullptr;

void CursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!TwEventMouseWheelGLFW3(window, xoffset, yoffset))
	{
		if (originalScrollCallback != nullptr)
			originalScrollCallback(window, xoffset, yoffset);
	}
}

static GLFWkeyfun originalKeyCallback = nullptr;

void MyKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (!TwEventKeyGLFW3(window, key, scancode, action, mods))
	{
		if (originalKeyCallback != nullptr)
			originalKeyCallback(window, key, scancode, action, mods);
	}
}

static GLFWcharfun originalCharCallback = nullptr;

void CharCallback(GLFWwindow* window, int codepoint) {
	if (!TwEventCharGLFW3(window, codepoint))
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
	TwInit(TwGraphAPI::TW_OPENGL_CORE, NULL);

	if (CreateTwBar)
	{
		int windowWidth, windowHeight;
		GLFWwindow* handle = System::GetInstance().GetWindowHandle();
		glfwGetWindowSize(handle, &windowWidth, &windowHeight);

		TwWindowSize(windowWidth, windowHeight);

		bar = TwNewBar("BAR");

		TwDefine(" BAR label='Properties' size='200 100' position='25 25' alpha=128 refresh=0.15 movable=true resizable=true contained=false color='28 183 255' "); // Message added to the help bar.

		//init input callback because we need to save these as the original callbacks
		CallbackInputManager::GetInstance();

		// pass GLFW events to AntTweakBar and then call the original handle
		originalMouseButtonCallback = glfwSetMouseButtonCallback(handle, (GLFWmousebuttonfun)MouseButtonCallback);
		originalCursorPosCallback = glfwSetCursorPosCallback(handle, (GLFWcursorposfun)CursorPosCallback);
		originalScrollCallback = glfwSetScrollCallback(handle, (GLFWscrollfun)CursorScrollCallback);
		originalKeyCallback = glfwSetKeyCallback(handle, (GLFWkeyfun)MyKeyCallback);
		originalCharCallback = glfwSetCharCallback(handle, (GLFWcharfun)CharCallback);
		originalWindowSizeCallback = glfwSetWindowSizeCallback(handle, (GLFWwindowsizefun)WindowSizeCallback);

		TwStructMember Vector2dMembers[] = {
		{ "x", TW_TYPE_FLOAT, offsetof(Vector2D, x), ""},
		{ "y", TW_TYPE_FLOAT, offsetof(Vector2D, y), ""},
		};

		TW_TYPE_VECTOR2D = TwDefineStruct("Vector2D", Vector2dMembers, 2, sizeof(Vector2D), NULL, NULL);

		Color myColor = Color();

		TwStructMember ColorMembers[] = {
		{ "a", TW_TYPE_FLOAT, offsetof(Color, a), "min=0 max=1 step=0.05"},
		{ "r", TW_TYPE_FLOAT, offsetof(Color, r), "min=0 max=1 step=0.05"},
		{ "g", TW_TYPE_FLOAT, offsetof(Color, g), "min=0 max=1 step=0.05"},
		{ "b", TW_TYPE_FLOAT, offsetof(Color, b), "min=0 max=1 step=0.05"},
		};

		TW_TYPE_BETA_COLOR = TwDefineStruct("Color", ColorMembers, 4, sizeof(Color), NULL, NULL);
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

TwType SpaceManager::GetVector2DTwType()
{
	return TW_TYPE_VECTOR2D;
}

TwType SpaceManager::GetColorTwType()
{
	return TW_TYPE_BETA_COLOR;
}
