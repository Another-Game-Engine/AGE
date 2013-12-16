#ifndef    __GRAPH_NODE_HPP__
# define   __GRAPH_NODE_HPP__

#include "Component.hh"
#include <Core/Engine.hh>
#include <Entities/Entity.hh>

namespace Component
{
	class GraphNode : public ComponentBase<GraphNode>
	{
	public:
		typedef std::list<Handle> t_EntityList;

		GraphNode(Engine &engine, Handle &entity)
			: ComponentBase<GraphNode>(engine, entity, "GraphNodeComponent")
		{}

		virtual ~GraphNode()
		{}

		const Handle	    	&getParent() const
		{
			return _parent;
		}

		void 					removeSon(Handle &son, bool notify = true)
		{
			_childs.erase(son);
			if (notify && son->hasComponent<Component::GraphNode>())
			{
				son->getComponent<Component::GraphNode>()->removeParent(false);
			}
		}

		void 					setParent(Handle &father, bool notify = true)
		{
			if (_parent.get() && _parent->hasComponent<Component::GraphNode>())
			{
				_parent->getComponent<Component::GraphNode>()->removeSon(_entity, false);
			}
			_parent = father;
			if (notify && father->hasComponent<Component::GraphNode>())
				father->getComponent<Component::GraphNode>()->addSon(_entity, false);
		}

		void 					addSon(Handle &son, bool notify = true)
		{
			_childs.insert(son);
			if (notify && son->hasComponent<Component::GraphNode>())
			{
				son->getComponent<Component::GraphNode>()->setParent(_entity, false);
			}
		}

		void                    removeParent(bool notify = true)
		{
			if (notify && _parent.get() && _parent->hasComponent<Component::GraphNode>())
			{
				_parent->getComponent<Component::GraphNode>()->removeSon(_entity);
			}
			auto key = PubSubKey("graphNodeSetAsRoot");
			_entity->pub(key, _entity);
			_parent = Handle(std::numeric_limits<unsigned int>::max(), nullptr);
		}
	private:
		Handle _parent;
		std::set<Handle> _childs;
	};
}

#endif     //__GRAPH_NODE_HPP__