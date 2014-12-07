#pragma once

#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Core/Commands/MainToPrepare.hpp>
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
	class PrepareRenderThread;

	class RenderScene
	{
	public:
		RenderScene(PrepareRenderThread *prepareThread)
		{
			_drawables.reserve(65536);
			_octree = new OctreeNode();
			_prepareThread = prepareThread;
		}

		~RenderScene(void)
		{
			if (_octree)
				delete _octree;
		}

		bool RenderScene::init()
		{
			return true;
		}

	private:
		void _setCameraInfos(AGE::Commands::MainToPrepare::CameraInfos &msg)
		{
			Camera *co = nullptr;
			co = &_cameras[msg.key.id];
			co->hasMoved = true;
			co->projection = msg.projection;
		}

		void _createCamera(AGE::Commands::MainToPrepare::CreateCamera &msg)
		{
			Camera *co = nullptr;
			if (msg.key.id >= _cameras.size())
			{
				_cameras.push_back(Camera());
				co = &_cameras.back();
			}
			else
				co = &_cameras[msg.key.id];
			co->key.id = msg.key.id;
			co->active = true;
		}

		void _createPointLight(AGE::Commands::MainToPrepare::CreatePointLight &msg)
		{
			PointLight *co = nullptr;
			if (msg.key.id >= _pointLights.size())
			{
				_pointLights.push_back(PointLight());
				co = &_pointLights.back();
			}
			else
				co = &_pointLights.back();
			co->key.id = msg.key.id;
			co->active = true;
		}

		void _createDrawable(AGE::Commands::MainToPrepare::CreateDrawable &msg)
		{
			Mesh *uo = nullptr;
			if (msg.key.id >= _meshs.size())
			{
				_meshs.push_back(Mesh());
				uo = &_meshs.back();
			}
			else
				uo = &_meshs[msg.key.id];
		}

		void _setPointLight(AGE::Commands::MainToPrepare::SetPointLight &msg)
		{
			PointLight *l = nullptr;
			l = &_pointLights[msg.key.id];
			l->color = msg.color;
			l->range = msg.range;
		}

		void _deleteCamera(AGE::Commands::MainToPrepare::DeleteCamera &msg)
		{
			Camera *co = nullptr;
			co = &_cameras[msg.key.id];
			co->active = false;
		}

		void _deletePointLight(AGE::Commands::MainToPrepare::DeletePointLight &msg)
		{
			PointLight *co = nullptr;
			co = &_pointLights[msg.key.id];
			co->active = false;
		}
		
		void _deleteDrawable(AGE::Commands::MainToPrepare::DeleteDrawable &msg)
		{
			Mesh *uo = nullptr;
			uo = &this->_meshs[msg.key.id];
			for (auto &e : uo->drawableCollection)
			{
				removeDrawableObject(e);
			}
			uo->drawableCollection.clear();
			uo->active = false;
		}

		void _setGeometry(AGE::Commands::MainToPrepare::SetGeometry &msg)
		{
			Mesh *uo = nullptr;
			uo = &_meshs[msg.key.id];
			for (auto &e : uo->drawableCollection)
				removeDrawableObject(e);
			uo->drawableCollection.clear();
			for (std::size_t i = 0; i < msg.submeshInstances.size(); ++i)
			{
				auto id = addDrawable(msg.key.id);
				uo->drawableCollection.push_back(id);
				_drawables[id].mesh = msg.submeshInstances[i];
				_drawables[id].material = msg.materialInstances[i];
				_drawables[id].position = uo->position;
				_drawables[id].orientation = uo->orientation;
				_drawables[id].scale = uo->scale;
				_drawables[id].meshAABB = msg.submeshInstances[i].boundingBox;
				_drawables[id].toAddInOctree = true;
				_drawables[id].animation = msg.animation;
			}
		}

		void _setPosition(AGE::Commands::MainToPrepare::SetPosition &msg)
		{
			Camera *co = nullptr;
			Mesh *uo = nullptr;
			PointLight *l = nullptr;

			switch (msg.key.type)
			{

			case(PrepareKey::Type::Camera) :
				co = &_cameras[msg.key.id];
				co->position = msg.position;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs[msg.key.id];
				uo->position = msg.position;
				for (auto &e : uo->drawableCollection)
				{
					_drawables[e].position = uo->position;
					_drawables[e].hasMoved = true;
				}
				break;
			case(PrepareKey::Type::PointLight) :
				l = &_pointLights[msg.key.id];
				l->position = msg.position;
				break;
			default:
				break;
			}
		}

		void _setScale(AGE::Commands::MainToPrepare::SetScale &msg)
		{
			Mesh *uo = nullptr;
			Camera *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameras[msg.key.id];
				co->scale = msg.scale;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs[msg.key.id];
				uo->scale = msg.scale;
				for (auto &e : uo->drawableCollection)
				{
					_drawables[e].scale = uo->scale;
					_drawables[e].hasMoved = true;
				}
				break;
			default:
				break;
			}
		}

		void _setOrientation(AGE::Commands::MainToPrepare::SetOrientation &msg)
		{
			Mesh *uo = nullptr;
			Camera *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameras[msg.key.id];
				co->orientation = msg.orientation;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs[msg.key.id];
				uo->orientation = msg.orientation;
				for (auto &e : uo->drawableCollection)
				{
					_drawables[e].orientation = uo->orientation;
					_drawables[e].hasMoved = true;
				}
				break;
			default:
				break;
			}
		}
		
		void _prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg)
		{
			AGE::Vector<CullableObject*> toDraw;


			// we update animation instances
			auto animationManager = getDependencyManager().lock()->getInstance<AGE::AnimationManager>();
			animationManager->update(0.1f);


			// Update drawable positions in Octree
			for (auto &e : _drawables)
			{
				if (e.hasMoved && e.toAddInOctree == false)
				{
					e.hasMoved = false;
					e.previousAABB = e.currentAABB;
					e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
					e.currentAABB.fromTransformedBox(e.meshAABB, e.transformation);
#ifdef  ACTIVATE_OCTREE_CULLING
					_octree = _octree->moveElement(&e);
#endif
				}
#ifdef  ACTIVATE_OCTREE_CULLING
				if (e.toAddInOctree)
				{
					e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
					e.currentAABB.fromTransformedBox(e.meshAABB, e.transformation);
					e.previousAABB = e.currentAABB;
					e.toAddInOctree = false;
					_octree = _octree->addElement(&e);
				}
#endif
			}
			// Do culling for each camera
			_octreeDrawList.clear();
			for (auto &camera : _cameras)
			{
				if (!camera.active)
					continue;
				
				auto view = glm::inverse(glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale));

				// update frustum infos for culling
				camera.currentFrustum.setMatrix(camera.projection * view);

				_octreeDrawList.emplace_back();
				auto &drawList = _octreeDrawList.back();
				drawList.transformation = view;
				drawList.projection = camera.projection;

				// no culling for the lights for the moment (TODO)
				for (size_t index = 0; index < _pointLights.size(); ++index)
				{
					auto &p = _pointLights[index];
					if (p.active)
						drawList.lights.emplace_back(p.position, p.color, p.range);
				}

#ifdef ACTIVATE_OCTREE_CULLING

				// Do the culling
				_octree->getElementsCollide(&camera, toDraw);

				// iter on element to draw
				for (CullableObject *e : toDraw)
				{
					// mandatory if you want the object to be found again
					e->hasBeenFound = false;
					// all the elements are drawable for the moment (TODO)
					Drawable *currentDrawable = dynamic_cast<Drawable*>(e);
					//if (!currentDrawable->animation.empty())
					//{
					//	drawList.drawables.emplace_back(currentDrawable->mesh, currentDrawable->material, currentDrawable->transformation, animationManager->getBones(currentDrawable->animation));
					//	drawList.drawables.back().animation = currentDrawable->animation;
					//}
					//else
					//{
						drawList.drawables.emplace_back(currentDrawable->mesh, currentDrawable->material, currentDrawable->transformation);
					//}
				}
#else
				for (auto &e : _drawables)
				{
					if (e.active)
					{
						drawList.drawables.emplace_back(e.mesh, e.material, e.transformation);
					}
				}
#endif
			}
			getDependencyManager().lock()->getInstance<AGE::AnimationManager>()->update(0.1f);
			getCommandQueue()->emplace<RendCtxCommand::CopyDrawLists>(std::move(this->_octreeDrawList));
		}

public:
	PrepareKey addMesh();
	PrepareKey addCamera();
	PrepareKey addPointLight();
	RenderScene &removeElement(const PrepareKey &key);
	RenderScene &setPointLight(glm::vec3 const &color, glm::vec3 const &range, const PrepareKey &id);
	RenderScene &setPosition(const glm::vec3 &v, const PrepareKey &id);
	RenderScene &setOrientation(const glm::quat &v, const PrepareKey &id);
	RenderScene &setScale(const glm::vec3 &v, const PrepareKey &id);
	RenderScene &setCameraInfos(const PrepareKey &id
		, const glm::mat4 &projection);
	RenderScene &setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
	RenderScene &setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
	RenderScene &setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
	RenderScene &updateGeometry(
		const PrepareKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials
		, const gl::Key<AGE::AnimationInstance> &animation);
	DRAWABLE_ID addDrawable(USER_OBJECT_ID uid);
	void removeDrawableObject(DRAWABLE_ID id);


	bool _updateEnd()
	{
		//auto returnValue = true;
		//_next->_next->getTaskQueue()->emplace<AGE::MTC::FrameTime>(_threadId, std::chrono::duration_cast<std::chrono::milliseconds>(_elapsed).count());
		//return returnValue;
		return truel
	}
	private:
		AScene *_scene;
		OctreeNode *_octree;
		PrepareRenderThread *_prepareThread;

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