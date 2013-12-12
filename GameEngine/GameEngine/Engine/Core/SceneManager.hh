#ifndef   __SCENE_MANAGER_HH__
# define  __SCENE_MANAGER_HH__

#include <string>
#include <map>

class AScene;

class SceneManager
{
private:
	AScene							*_sceneBinded;
	std::map<std::string, AScene*>	_scenes;
	SceneManager(const SceneManager &o);
	SceneManager &operator=(const SceneManager &o);
public:
	SceneManager();
	virtual ~SceneManager();
	void		addScene(AScene *scene, std::string const &name);
	void		removeScene(std::string const &name);
	void		bindScene(std::string const &name);
	AScene		*getCurrentScene() const;
	bool        startScene() const;
	bool        userUpdate(double time) const;
	void        update(double time);
};

#endif    //__SCENE_MANAGER_HH__