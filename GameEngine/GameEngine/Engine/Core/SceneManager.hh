#pragma once

#include <string>
#include <map>
#include <Utils/Dependency.hpp>
#include <memory>
#include <vector>

namespace AGE
{
	class AScene;
	class SceneManager
	{
	private:
		std::map<std::string, std::shared_ptr<AScene>>	_scenes;
		std::map < unsigned int, std::shared_ptr<AScene>> _actives;
		SceneManager(const SceneManager &o);
		SceneManager &operator=(const SceneManager &o);
	public:

		// TO IMPLEMENT
		// enableScene(const std::string &name, unsigned int priority);
		// disableScene(const std::string &name);
		// resetScene(const std::string &name);
		// initScene(const std::string &name);
		//
		// Each scene should have a unique ID.
		// Each scene should have an EntityCreator link to EntityManager
		// EntityCreator can access EntityManager private member functions such as createEntity, deleteEntity ...
		// EntityCreator are passed to Systems contained in scene.
		// EntityCreator should be the unique way to create / delete ... entities
		// When a scene is destroyed or reset the EntityCreator flush all created entities.
		SceneManager();
		virtual ~SceneManager();

		void		addScene(std::shared_ptr<AScene> scene, std::string const &name);
		void		removeScene(std::string const &name);
		void		enableScene(std::string const &name, unsigned int priority);
		void		disableScene(std::string const &name);
		void        resetScene(std::string const &name);
		bool        initScene(std::string const &name);
		void        getSceneList(std::vector<std::string> &list) const;
		std::shared_ptr<AScene> getScene(const std::string &name);

		bool        updateScenes(float time);
	};
}