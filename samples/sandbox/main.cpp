#include <iostream>
#include <vdtecs/ecs.h>

#include "time.h"

using namespace std;
using namespace ecs;

struct Position
{
	int x, y;
};

class PositionSystem : public System<PositionSystem, Position>
{
public:
	virtual void update(const float delta_time) override
	{
		for (Component& component : components())
		{
			component.data().x += 1;
		}
	}
};

int main()
{
	Engine engine;
	engine.update(0.1f);

	Entity a = Entity::create();
	Entity b = Entity::create();
	a.addComponent<PositionSystem::Component>(1, 3);
	a.addComponent<PositionSystem::Component>(2, 5);
	a.addComponent<PositionSystem::Component>(3, 3);
	
	const auto& c = a.getComponent<PositionSystem::Component>();
	const auto& component = a.getComponents<PositionSystem::Component>();

	const auto& f = PositionSystem::instance().getComponentById(2);
	
	for (auto entity : Entity::all())
	{
		cout << entity.id() << endl;
	}

	auto& manager = ISystem::manager();
	manager.add<PositionSystem>();

	const auto& systems = manager.all();

	return getchar();
}