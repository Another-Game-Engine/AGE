#pragma once

#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Core/Commands/MainToPrepare.hpp>
#include <Core/PrepareKey.hpp>
#include <Entities/EntityTypedef.hpp>

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
		RenderScene(PrepareRenderThread *prepareThread);
		~RenderScene(void);
		bool RenderScene::init();
	private:
		void _setCameraInfos(AGE::Commands::MainToPrepare::CameraInfos &msg);
		void _createCamera(AGE::Commands::MainToPrepare::CreateCamera &msg);
		void _createPointLight(AGE::Commands::MainToPrepare::CreatePointLight &msg);
		void _createDrawable(AGE::Commands::MainToPrepare::CreateDrawable &msg);
		void _setPointLight(AGE::Commands::MainToPrepare::SetPointLight &msg);
		void _deleteCamera(AGE::Commands::MainToPrepare::DeleteCamera &msg);
		void _deletePointLight(AGE::Commands::MainToPrepare::DeletePointLight &msg);
		void _deleteDrawable(AGE::Commands::MainToPrepare::DeleteDrawable &msg);
		void _setGeometry(AGE::Commands::MainToPrepare::SetGeometry &msg);
		void _setPosition(AGE::Commands::MainToPrepare::SetPosition &msg);
		void _setScale(AGE::Commands::MainToPrepare::SetScale &msg);
		void _setOrientation(AGE::Commands::MainToPrepare::SetOrientation &msg);
		void _prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg);

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
			return true;
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