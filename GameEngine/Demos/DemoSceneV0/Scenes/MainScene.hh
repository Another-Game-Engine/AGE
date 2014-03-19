#include <Core/AScene.hh>

class MainScene : public AScene	
{
public:
	MainScene(std::weak_ptr<Engine> engine);
	virtual ~MainScene(void);
	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
private:
};