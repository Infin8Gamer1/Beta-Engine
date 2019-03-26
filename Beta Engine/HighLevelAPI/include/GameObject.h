//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <BetaObject.h>
#include <Serializable.h>
#include <Component.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Component;
class Space;

// Function pointer for Damage event handlers.
// Params:
//   callingObject = The object whose handler is being called.
//   otherObject = The other object that the object collided with.
typedef void(*DamageEventHandler)(int ammount, std::string type, GameObject& damageCauser, GameObject& damageTaker);

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class GameObject : public BetaObject, public Serializable
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new game object.
	// Params:
	//	 name = The name of the game object being created.
	GameObject(const std::string& name);

	// Clone a game object from another game object.
	// Params:
	//	 other = A reference to the object being cloned.
	GameObject(const GameObject& other);

	// Free the memory associated with a game object.
	~GameObject();

	// Loads object data from a file.
	void Deserialize(Parser& parser) override;

	// Loads object data from a file. The B function is used to read the object in without readskiping the name of the object
	void DeserializeB(Parser& parser);

	// Saves object data to a file.
	void Serialize(Parser& parser) const override;

	TwBar* CreateTweakBar(TwBar* bar = nullptr);

	// Initialize this object's components and set it to active.
	void Initialize() override;

	// Update any components attached to the game object.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt) override;

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void FixedUpdate(float dt) override;

	// Draw any visible components attached to the game object.
	void Draw() override;

	size_t NumberOfComponents();
	
	// Adds a component to the object.
	void AddComponent(Component* component);

	// Retrieves the component with the given name if it exists.
	// Params:
	//   name = The name of the component to find.
	[[deprecated("Replaced by GetComponent<Type>(), which has an improved interface")]]
	Component* GetComponent(const std::string& name);

	template<class T>
	T * GetComponent()
	{
		//std::string Type = std::string(typeid(T).name());
		const char* Type = typeid(T).name();

		for (size_t i = 0; i < components.size(); i++)
		{
			//std::string compType(typeid(*components[i]).name());
			const char* compType = typeid(*components[i]).name();
			
			if (Type == compType)
			{
				return static_cast<T*>(components[i]);
			}

			//attempt dynamic cast
			try
			{
				T* output = dynamic_cast<T*>(components[i]);
				if (output != nullptr) {
					return output;
				}
			}
			catch (std::bad_cast e)
			{
				std::cout << "Caught: " << e.what() << std::endl;
			}
		}

		return nullptr;
	}

	// Retrieves the component with the given type if it exists.
	// Template params:
	//  ComponentType = The type of component to retrieve.
	// Returns:
	//  A pointer to the component if it exists, nullptr otherwise.
	//template<class ComponentType> 
	//ComponentType* GetComponent()
	
	// Mark an object for destruction.
	void Destroy();
	
	// Whether the object has been marked for destruction.
	// Returns:
	//		True if the object will be destroyed, false otherwise.
	bool IsDestroyed() const;

	// Get the space that contains this object.
	Space* GetSpace() const;

	// Sets the damage handler function for the Game Object
	void setDamageHandler(DamageEventHandler handler);

	// Deal Damage to this Gameobject
	static void DealDamage(GameObject& damegedGO, GameObject& damageCauser, int ammount, std::string damageType = "Default");

	// Get the health of this gameobject
	int getHealth();

	// Set the health of this gameobject
	void setHealth(int Health);

	std::string GetSavePath();

	void SetSavePath(std::string Path);

	const std::string& GetName() const;

	void SetName(const std::string& newName);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Components
	std::vector<Component*> components;

	DamageEventHandler damageHandler;

	// Whether the object has been marked for destruction.
	bool isDestroyed;

	TwBar* bar;

	std::string gameObjectName;

	// the health of this object
	int Health;

	std::string Path;

	std::vector<std::string> UnloadedComponentNames;

	std::vector<std::string> UnloadedComponentStrings;
};

//------------------------------------------------------------------------------
