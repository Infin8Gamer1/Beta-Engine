#include "stdafx.h"
#include "SaveLoadButton.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "Transform.h"
#include "Sprite.h"
#include "Physics.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Space.h"
#include "SpriteText.h"
#include "SpaceManager.h"
#include <Engine.h>
#include "SaveManager.h"
#include "Parser.h"
#include "glfw3.h"
#include <System.h>

SaveLoadButton::SaveLoadButton(state state) : Button("Button"), myState(state), saveManager(nullptr)
{
}

void SaveLoadButton::Initialize()
{
    int windowWidth, windowHeight;
    GLFWwindow* handle = System::GetInstance().GetWindowHandle();
    glfwGetWindowSize(handle, &windowWidth, &windowHeight);

    saveManager = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("EditorManager");

    float padding = 10.0f;
    Transform* t = GetOwner()->GetComponent<Transform>();
    Vector2D pos = Vector2D((windowWidth / 2) * -1 + padding + t->GetScale().x / 2, (windowHeight / 2) - padding - t->GetScale().y / 2);

    if (myState == Save)
    {
        t->SetTranslation(pos);
    }
    else //Load
    {
        
        t->SetTranslation(Vector2D(pos.x + t->GetScale().x + padding, pos.y));
    }
}

Component * SaveLoadButton::Clone() const
{
    return new SaveLoadButton(*this);
}

void SaveLoadButton::Serialize(Parser & parser) const
{
    parser.WriteVariable("state", myState);
}

void SaveLoadButton::Deserialize(Parser & parser)
{
    int i;
    parser.ReadVariable("state", i);
    myState = static_cast<state>(i);
}

void SaveLoadButton::Clicked()
{
    if (myState == Save)
    {
        saveManager->GetComponent<Behaviors::SaveManager>()->Save();
    }
    else //Load
    {
        saveManager->GetComponent<Behaviors::SaveManager>()->Load();
    }
}
