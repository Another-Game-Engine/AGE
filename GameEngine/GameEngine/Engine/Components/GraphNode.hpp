#ifndef    __GRAPH_NODE_HPP__
# define   __GRAPH_NODE_HPP__

#include "Component.hpp"
#include <Core/Engine.hh>
#include <Entities/Entity.hh>

namespace Component
{
	class GraphNode : public ComponentBase<GraphNode>
	{
	public:
		typedef std::list<Handle> t_EntityList;

		GraphNode(Engine &engine)
			: ComponentBase<GraphNode>(engine)
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
			// TODO CHECK IF FATHER != NULL IF NOT SO, REMOVE FROM CHILDRENS
			if (_parent.get() && _parent->hasComponent<Component::GraphNode>())
			{
				_parent->getComponent<Component::GraphNode>()->removeSon(_father, false);
			}
			_parent = father;
			if (notify && father->hasComponent<Component::GraphNode>())
				father->getComponent<Component::GraphNode>()->addSon(_father, false);
		}

		void 					addSon(Handle &son, bool notify = true)
		{
			_childs.insert(son);
			if (notify && son->hasComponent<Component::GraphNode>())
			{
				son->getComponent<Component::GraphNode>()->setParent(_father, false);
			}
		}

		void                    removeParent(bool notify = true)
		{
			if (notify && _parent.get() && _parent->hasComponent<Component::GraphNode>())
			{
				_parent->getComponent<Component::GraphNode>()->removeSon(_father);
			}
			auto key = PubSubKey("graphNodeSetAsRoot");
			_father->pub(key, _father);
			_parent = Handle(std::numeric_limits<unsigned int>::max(), nullptr);
		}

	//SmartPointer<t_EntityList> 	getSonsByFlags(size_t flags, Entity::GetFlags op);

	//t_sonsList::iterator 		getSonsBegin();
	//t_sonsList::iterator 		getSonsEnd();
	private:
		Handle _parent;
		std::set<Handle> _childs;


	};
}

#endif     //__GRAPH_NODE_HPP__