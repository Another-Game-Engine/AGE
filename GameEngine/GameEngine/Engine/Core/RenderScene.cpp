#include "RenderScene.hpp"

namespace AGE
{
	PrepareKey addMesh()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Drawable;
		if (!_freeMeshs.empty())
		{
			res.id = _freeMeshs.front();
			_freeMeshs.pop();
		}
		else
			res.id = PrepareKey::OctreeObjectId(_MeshCounter++);
		_prepareThread->getQueue()->emplace<PRTC::CreateDrawable>(res);
		return res;
	}

	PrepareKey OldPrepareRenderThread::addCamera()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Camera;
		if (!_freeCameras.empty())
		{
			res.id = _freeCameras.front();
			_freeCameras.pop();
		}
		else
			res.id = PrepareKey::OctreeObjectId(_cameraCounter++);
		_commandQueue.emplace<PRTC::CreateCamera>(res);

		return res;
	}

	PrepareKey OldPrepareRenderThread::addPointLight()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::PointLight;
		if (!_freePointLights.empty())
		{
			res.id = _freePointLights.front();
			_freePointLights.pop();
		}
		else
			res.id = PrepareKey::OctreeObjectId(_pointLightCounter++);
		_commandQueue.emplace<PRTC::CreatePointLight>(res);
		return res;
	}

	OldPrepareRenderThread &OldPrepareRenderThread::removeElement(const PrepareKey &key)
	{
		assert(!key.invalid());
		switch (key.type)
		{
		case PrepareKey::Type::Camera:
			_freeCameras.push(key.id);
			_commandQueue.emplace<PRTC::DeleteCamera>(key);
			break;
		case PrepareKey::Type::Drawable:
			_freeMeshs.push(key.id);
			_commandQueue.emplace<PRTC::DeleteDrawable>(key);
			break;
		case PrepareKey::Type::PointLight:
			_freePointLights.push(key.id);
			_commandQueue.emplace<PRTC::DeletePointLight>(key);
			break;
		default:
			break;
		}
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setPointLight(glm::vec3 const &color, glm::vec3 const &range, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::SetPointLight>(color, range, id);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setPosition(const glm::vec3 &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Position>(id, v);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setOrientation(const glm::quat &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Orientation>(id, v);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setScale(const glm::vec3 &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Scale>(id, v);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setCameraInfos(const PrepareKey &id
		, const glm::mat4 &projection)
	{
		_commandQueue.emplace<PRTC::CameraInfos>(id, projection);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setPosition(v, e);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setOrientation(v, e);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setScale(v, e);
		return (*this);
	}

	OldPrepareRenderThread &OldPrepareRenderThread::updateGeometry(
		const PrepareKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials
		, const gl::Key<AGE::AnimationInstance> &animation)
	{
		assert(!key.invalid() || key.type != PrepareKey::Type::Drawable);
		_commandQueue.emplace<PRTC::Geometry>(key, meshs, materials, animation);
		return (*this);
	}

	DRAWABLE_ID OldPrepareRenderThread::addDrawable(USER_OBJECT_ID uid)
	{
		DRAWABLE_ID res = DRAWABLE_ID(-1);
		Drawable *co = nullptr;
		if (!_freeDrawables.empty())
		{
			res = _freeDrawables.front();
			_freeDrawables.pop();
			co = &(_drawables[res]);
		}
		else
		{
			res = _drawables.size();
			_drawables.emplace_back(Drawable());
			co = &(_drawables.back());
		}
		co->id = res;
		co->active = true;
		return res;
	}

	void OldPrepareRenderThread::removeDrawableObject(DRAWABLE_ID id)
	{
		_freeDrawables.push(PrepareKey::OctreeObjectId(id));
		_drawables[id].reset();
#ifdef ACTIVATE_OCTREE_CULLING
		// remove drawable from octree
		if (_drawables[id].toAddInOctree == false)
			_octree = _octree->removeElement(&_drawables[id]);
#endif
		assert(id != (std::size_t)(-1));
	}


	bool OldPrepareRenderThread::_updateBegin()
	{
		auto returnValue = true;

		return returnValue;
	}

	bool OldPrepareRenderThread::_updateEnd()
	{
		auto returnValue = true;
		_next->_next->getTaskQueue()->emplace<AGE::MTC::FrameTime>(_threadId, std::chrono::duration_cast<std::chrono::milliseconds>(_elapsed).count());
		return returnValue;
	}
}