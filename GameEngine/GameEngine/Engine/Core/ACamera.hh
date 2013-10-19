
#ifndef ACAMERA_HPP_
#define ACAMERA_HPP_

#include "Input.hh"
#include "Timer.hh"

#include "glm/glm.hpp"

class ACamera
{
private:
  glm::mat4       _transform;
  glm::mat4       _projection;

  bool            _hasMoved;

public:
  ACamera();
  virtual ~ACamera() { }

  glm::mat4         &setTransform();
  glm::mat4         &getTransform();
  glm::mat4         &setProjection();
  glm::mat4         &getProjection();

  bool              hasMoved() const;
  void              resetMoved();

  virtual void      update() = 0;
};

#endif