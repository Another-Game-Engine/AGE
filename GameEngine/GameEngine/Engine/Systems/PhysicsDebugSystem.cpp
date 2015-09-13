#include <Systems/PhysicsDebugSystem.hpp>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>
#include <Core/AScene.hh>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>

namespace AGE
{
	namespace Private
	{
		// Constructors
		PhysicsDebugSystem::PhysicsDebugSystem(AScene *scene)
			: System(scene)
		{
			_name = "PhysicsDebugSystem";
		}

		// Inherited Methods
		bool PhysicsDebugSystem::initialize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->enableDebug();
			return true;
		}

		void PhysicsDebugSystem::finalize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->disableDebug();
		}

		void PhysicsDebugSystem::mainUpdate(float elapsedTime)
		{
			const Physics::DebugInformation &information = _scene->getInstance<Physics::PhysicsInterface>()->getWorld()->getDebugInformation();
			TMQ::HybridQueue *queue = GetRenderThread()->getQueue();
			for (const Physics::DebugInformation::Point &point : information.points)
			{
				queue->emplaceTask<Commands::ToRender::Draw3DLine>(point.position[0], point.color[0], point.position[0], point.color[0], false);
			}
			for (const Physics::DebugInformation::Line &line : information.lines)
			{
				queue->emplaceTask<Commands::ToRender::Draw3DLine>(line.position[0], line.color[0], line.position[1], line.color[1], false);
			}
			for (const Physics::DebugInformation::Triangle &triangle : information.triangles)
			{
				queue->emplaceTask<Commands::ToRender::Draw3DLine>(triangle.position[0], triangle.color[0], triangle.position[1], triangle.color[1], false);
				queue->emplaceTask<Commands::ToRender::Draw3DLine>(triangle.position[1], triangle.color[1], triangle.position[2], triangle.color[2], false);
				queue->emplaceTask<Commands::ToRender::Draw3DLine>(triangle.position[2], triangle.color[2], triangle.position[0], triangle.color[0], false);
			}
		}
	}
}