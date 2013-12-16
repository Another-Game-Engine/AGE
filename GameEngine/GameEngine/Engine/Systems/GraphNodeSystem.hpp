#ifndef    __GRAPH_NODE_SYSTEM_HPP__
#define    __GRAPH_NODE_SYSTEM_HPP__

#include "System.h"
#include <Components/GraphNode.hpp>
#include <Entities/Entity.hh>

class GraphNodeSystem : public System
{
public:
	GraphNodeSystem(Engine &engine) : System(engine)
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
	}

	virtual void initialize()
	{
		require<Component::GraphNode>();
	}
};

#endif     //__GRAPH_NODE_SYSTEM_HPP__