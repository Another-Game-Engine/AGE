#ifndef    __GRAPH_NODE_SYSTEM_HPP__
#define    __GRAPH_NODE_SYSTEM_HPP__

#include "System.h"
#include <Components/GraphNode.hpp>
#include <Entities/EntityData.hh>

class GraphNodeSystem : public System
{
public:
	GraphNodeSystem(AScene *scene) : System(scene)
	{}
	virtual ~GraphNodeSystem(){}
private:
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float t = time;
		for (auto e : _roots)
		{
			if (e->getFlags() & EntityData::HAS_MOVED)
			{
				e->computeGlobalTransform(glm::mat4(1));
				recomputePositions(e, true);
			}
			else
			{
				recomputePositions(e, false);
			}
		}
	}

	void recomputePositions(Entity &e, bool hasMoved)
	{
		auto node = e->getComponent<Component::GraphNode>();
		auto it = node->getSonsBegin();

		while (it != node->getSonsEnd())
		{
			if ((it->get())->getFlags() & EntityData::HAS_MOVED)
				hasMoved = true;
			if (hasMoved)
				it->get()->computeGlobalTransform(e->getGlobalTransform());
			recomputePositions(it->get()->getHandle(), hasMoved);
			++it;
		}
}

	virtual void initialize()
	{
		// DOES NOT REQUIRE COMPONENTS BECAUSE GraphNode set as root  will directly communicate with it
		//require<Component::GraphNode>();

		globalSub(PubSubKey("graphNodeSetAsRoot"), [&](Entity entity){
			_roots.insert(entity);
		});

		globalSub(PubSubKey("graphNodeNotARoot"), [&](Entity entity){
			_roots.erase(entity);
		});
	}

	std::set<Entity> _roots;
};

#endif     //__GRAPH_NODE_SYSTEM_HPP__