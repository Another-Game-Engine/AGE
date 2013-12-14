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

		const Handle	    	&getFather() const
		{
			return _father;
		}

		void 					setFather(Handle &father)
		{
			// TODO CHECK IF FATHER != NULL IF NOT SO, REMOVE FROM CHILDRENS
			if ()
		}

	void 					addSon(Handle &son);
	void 					removeSon(Handle &son);

	SmartPointer<t_EntityList> 	getSonsByFlags(size_t flags, GetFlags op);

	t_sonsList::iterator 		getSonsBegin();
	t_sonsList::iterator 		getSonsEnd();
	private:
		bool _isRoot;
		Handle _father;
	};
}

#endif     //__GRAPH_NODE_HPP__