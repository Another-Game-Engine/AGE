#include <Components/MeshRenderer.hh>
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/AssetManager.hh>
#include <assert.h>
#include <Threads/ThreadManager.hpp>
#include <Threads/PrepareRenderThread.hpp>
namespace AGE
{

	namespace Component
	{
		MeshRenderer::MeshRenderer() :
			Component::ComponentBase<MeshRenderer>(),
			_scene(nullptr)
			, _serializationInfos(nullptr)
		{
		}

		MeshRenderer::~MeshRenderer(void)
		{
		}

		MeshRenderer::MeshRenderer(MeshRenderer &&o)
			: ComponentBase<MeshRenderer>(std::move(o)),
			_scene(o._scene),
			_key(o._key),
			_serializationInfos(std::move(o._serializationInfos)),
			_material(o._material),
			_mesh(o._mesh)
		{
			}

		MeshRenderer &MeshRenderer::operator=(MeshRenderer &&o)
		{
			_scene = o._scene;
			_key = o._key;
			_serializationInfos = std::move(o._serializationInfos);
			_material = o._material;
			_mesh = o._mesh;
			return *this;
		}

		void MeshRenderer::init(AScene *scene, std::shared_ptr<MeshInstance> r)
		{
			_scene = scene;
			setMesh(r);
		}

		void MeshRenderer::reset(AScene *scene)
		{
			assert(!_key.invalid());
			scene->getLink(entityId)->unregisterOctreeObject(_key);
			//scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
			_key = AGE::PrepareKey();
		}

		MeshRenderer &MeshRenderer::setMesh(const std::shared_ptr<MeshInstance> &mesh)
		{
			_mesh = mesh;
			_key = AGE::GetPrepareThread()->addMesh();
			_scene->getLink(entityId)->registerOctreeObject(_key);
			assert(!_key.invalid());
			updateGeometry();
			return (*this);
		}

		std::shared_ptr<MeshInstance> MeshRenderer::getMesh()
		{
			return _mesh;
		}

		MeshRenderer &MeshRenderer::setMaterial(const std::shared_ptr<MaterialSetInstance> &material)
		{
			_material = material;
			updateGeometry();
			return (*this);
		}

		std::shared_ptr<MaterialSetInstance> MeshRenderer::getMaterial()
		{
			return _material;
		}

		void MeshRenderer::updateGeometry()
		{
			assert(_scene != nullptr);
			if (this->_mesh == nullptr || this->_material == nullptr)
				return;
			AGE::GetPrepareThread()->updateGeometry(_key, _mesh->subMeshs);
		}

		void MeshRenderer::postUnserialization(AScene *scene)
		{
			_scene = scene;
			if (_serializationInfos)
			{
				if (!_serializationInfos->mesh.empty())
				{
					auto mesh = _scene->getInstance<AGE::AssetsManager>()->getMesh(_serializationInfos->mesh);
					if (mesh)
					{
						init(_scene, mesh);
					}
				}
				// todo with animations
			}
		}
	}

}