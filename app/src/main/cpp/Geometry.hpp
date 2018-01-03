#ifndef CBPP_GEOMETRY_HPP
#define CBPP_GEOMETRY_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/glm.hpp>


namespace geometry {
  void initProjections();
  extern glm::mat4 leftEyeProj;
  extern glm::mat4 rightEyeProj;

  glm::mat4 MakeProjection(float l, float t, float r, float b, float n, float f);
  float distortInv(float radius);
  float distort(float r);
  void GetLeftEyeNoLensTanAngles(float* result);
}

#endif
