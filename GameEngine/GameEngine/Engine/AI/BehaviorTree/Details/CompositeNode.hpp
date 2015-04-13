#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstddef>

#include <AI/BehaviorTree/Details/LeafNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class CompositeNode : public LeafNode < Entity, Args... >, public std::enable_shared_from_this < CompositeNode<Entity, Args...> >
			{
			protected:
				using EntityType = typename LeafNode<Entity, Args...>::EntityType;

				using EntityPtr = typename LeafNode<Entity, Args...>::EntityPtr;

				using Parent = typename LeafNode<Entity, Args...>::Parent;

				using ParentPtr = typename LeafNode<Entity, Args...>::ParentPtr;

				using Child = LeafNode < Entity, Args... > ;

				using ChildPtr = std::shared_ptr < Child > ;

				using ChildrenList = std::vector < ChildPtr > ;

				// Attributes
				ChildrenList children;

				// Constructors
				CompositeNode(void) = default;

				CompositeNode(CompositeNode const &) = default;

				// Assignment Operators
				CompositeNode &operator=(CompositeNode const &) = default;

				// Virtual Methods
				virtual void onChildAdded(ChildPtr)
				{
					return;
				}

				virtual void onChildRemoved(ChildPtr)
				{
					return;
				}

				virtual void initialize(EntityPtr entity) override = 0;

				virtual Status execute(EntityPtr entity, Args... args) override = 0;

			public:
				// Destructor
				virtual ~CompositeNode(void) = default;

				// Virtual Methods
				virtual ParentPtr addChild(ChildPtr child)
				{
					assert(child && "Invalid child");
					children.push_back(child);
					onChildAdded(child);
					child->onParentAdded(this->shared_from_this());
					return this->shared_from_this();
				}

				// Methods
				ChildPtr removeChild(ChildPtr child)
				{
					typename ChildrenList::iterator child_it = std::find(children.begin(), children.end(), child);
					assert(child_it != children.end() && "Invalid child");
					if (child_it != children.end())
					{
						onChildRemoved(child);
						child->onParentRemoved(this->shared_from_this());
						children.erase(child_it);
						return child;
					}
					else
					{
						return ChildPtr();
					}
				}

				ChildPtr removeChild(std::size_t position)
				{
					assert(position < children.size() && "Invalid position");
					if (position < children.size())
					{
						typename ChildrenList::iterator child_it = std::next(children.begin(), position);
						ChildPtr child = *child_it;
						onChildRemoved(child);
						child->onParentRemoved(this->shared_from_this());
						children.erase(child_it);
						return child;
					}
					else
					{
						return ChildPtr();
					}
				}
			};
		}
	}
}
