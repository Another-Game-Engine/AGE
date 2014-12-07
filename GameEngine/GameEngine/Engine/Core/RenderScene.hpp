#pragma once

#include <Threads/CommandQueuePusher.hpp>
#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>

#include <Core/PrepareKey.hpp>

class AScene;

namespace AGE
{
	struct Drawable;
	struct Mesh;
	struct PointLight;
	struct Camera;
	struct DrawableCollection;
	struct SubMeshInstance;
	class OctreeNode;

	class RenderScene : public QueuePusher
	{
	public:
	private:
		AScene *_scene;
		OctreeNode *_octree;

		AGE::Vector<Mesh> _meshs;
		AGE::Vector<Drawable> _drawables;
		AGE::Vector<Camera> _cameras;
		AGE::Vector<PointLight> _pointLights;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeDrawables;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeCameras;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeMeshs;
		AGE::Queue<PrepareKey::OctreeObjectId> _freePointLights;
		std::size_t _MeshCounter = 0;
		std::size_t _cameraCounter = 0;
		std::size_t _pointLightCounter = 0;
		AGE::Vector<DrawableCollection> _octreeDrawList;
	};
}