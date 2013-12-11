
#ifndef ACAMERA_HPP_
#define ACAMERA_HPP_

#include "Input.hh"
#include "Timer.hh"

#include "glm/glm.hpp"
#include "Utils/SmartPointer.hh"
#include "OpenGL/Shader.hh"

class Engine
{};

namespace Resources
{
	class CubeMap;
};

class ACamera
{
private:
  glm::mat4       _transform;
  glm::mat4       _projection;

  bool            _hasMoved;

  SmartPointer<Resources::CubeMap> _skybox;
  std::string _cubeMapShader;

  virtual void customUpdate() = 0;
protected:
	Engine          &_engine;
public:
  ACamera(Engine &engine);
  virtual ~ACamera() { }

  glm::mat4         &setTransform();
  glm::mat4         &getTransform();
  glm::mat4         &setProjection();
  glm::mat4         &getProjection();

  bool              hasMoved() const;
  void              resetMoved();

  void attachSkybox(const std::string &name, const std::string &cubeMapShader);
  void dettachSkybox();

  void      update();
};

#endif