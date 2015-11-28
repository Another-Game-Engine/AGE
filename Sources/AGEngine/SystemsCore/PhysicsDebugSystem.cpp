#include <SystemsCore/PhysicsDebugSystem.hpp>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>
#include <Core/AScene.hh>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Render/GeometryManagement/DebugDrawManager.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Core/Engine.hh>

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
			if (GetEngine()->hasInstance<DebugDrawManager>() == false)
			{
				GetEngine()->setInstance<DebugDrawManager>();
			}
			return true;
		}

		void PhysicsDebugSystem::finalize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->disableDebug();
		}

		void PhysicsDebugSystem::mainUpdate(float elapsedTime)
		{
			// check if the render thread does not already have stuff to draw
			if (GetMainThread()->isRenderFrame() == false)
			{
				return;
			}

			const Physics::DebugInformation &information = _scene->getInstance<Physics::PhysicsInterface>()->getWorld()->getDebugInformation();

			auto debugManager = GetEngine()->getInstance<DebugDrawManager>();
			AGE_ASSERT(debugManager);

			for (const Physics::DebugInformation::Point &point : information.points)
			{
				debugManager->draw3DLine(point.position[0], point.color[0], point.position[0], point.color[0], false);
			}
			for (const Physics::DebugInformation::Line &line : information.lines)
			{
				debugManager->draw3DLine(line.position[0], line.color[0], line.position[1], line.color[1], false);
			}
			for (const Physics::DebugInformation::Triangle &triangle : information.triangles)
			{
				debugManager->draw3DLine(triangle.position[0], triangle.color[0], triangle.position[1], triangle.color[1], false);
				debugManager->draw3DLine(triangle.position[1], triangle.color[1], triangle.position[2], triangle.color[2], false);
				debugManager->draw3DLine(triangle.position[2], triangle.color[2], triangle.position[0], triangle.color[0], false);
			}
		}
	}
}