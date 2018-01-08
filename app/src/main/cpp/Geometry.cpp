#include "Geometry.hpp"
#include <android/log.h>
#include <glm/glm/gtx/string_cast.hpp>
#include <cmath>

namespace geometry {
  float coefx = 0.34f;
  float coefy = 0.55f;
  float width = 0.110f;
  float height = 0.062f;
  float border = 0.004f;
  float outerFOV = 60.0f;
  float innerFOV = 60.0f;
  float upperFOV = 60.0f;
  float lowerFOV = 60.0f;
  float separation = 0.064f;
  float offset = 0.035f;
  float screenDistance = 0.039f;
  glm::mat4 leftEyeProj;
  glm::mat4 rightEyeProj;


  void initProjections(){
    float *rect = new float[4];
    GetLeftEyeNoLensTanAngles(rect);
    leftEyeProj = MakeProjection(rect[0], rect[1], rect[2], rect[3], 10.0f, 1000.0f);
    rightEyeProj = glm::transpose(glm::transpose(leftEyeProj)); // Roundabout copy
    rightEyeProj[0][2] *= -1.0f;
    leftEyeProj[1][1] *= -1.0f;
    rightEyeProj[1][1] *= -1.0f;
  }

  glm::mat4 MakeProjection(float l, float t, float r, float b, float n, float f){
    __android_log_print(ANDROID_LOG_INFO, "CBPP ", "l : %f", l);
    __android_log_print(ANDROID_LOG_INFO, "CBPP ", "t : %f", t);
    __android_log_print(ANDROID_LOG_INFO, "CBPP ", "r : %f", r);
    __android_log_print(ANDROID_LOG_INFO, "CBPP ", "b : %f", b);
    glm::mat4 m = glm::mat4(0.0f);
    m[0][0] = 2.0f * n / (r - l);
    m[1][1] = 2.0f * n / (t - b);
    m[0][2] = (r + l) / (r - l);
    m[1][2] = (t + b) / (t - b);
    m[2][2] = (n + f) / (n - f);
    m[2][3] = 2.0f * n * f / (n - f);
    m[3][2] = -1.0f;
    return m;
  }

  float distortInv(float radius){
    float r0 = 0.0f;
    float r1 = 1.0f;
    float dr0 = radius - distort(r0);
    while (std::fabs(r1 - r0) > 0.0001f) {
      float dr1 = radius - distort(r1);
      float r2 = r1 - dr1 * ((r1 - r0) / (dr1 - dr0));
      r0 = r1;
      r1 = r2;
      dr0 = dr1;
    }
    return r1;
  }

  float distort(float r){
    float r2 = r * r;
    float ret = 0;
    ret = r2 * (ret + coefx);
    ret = r2 * (ret + coefy);
    return (ret + 1) * r;
  }

  void GetLeftEyeNoLensTanAngles(float* result){
    float deg2Rad = std::acos(-1.0f) / 180.0f;
    float fovLeft = distortInv(std::tan(-outerFOV * deg2Rad));
    float fovTop = distortInv(std::tan(upperFOV * deg2Rad));
    float fovRight = distortInv(std::tan(innerFOV * deg2Rad));
    float fovBottom = distortInv(std::tan(-lowerFOV * deg2Rad));
    // Viewport size.
    float halfWidth = width / 4.0f;
    float halfHeight = height / 2.0f;
    // Viewport center, measured from left lens position.
    float centerX = separation / 2.0f - halfWidth;
    float centerY = -1.0f * (offset - border - height / 2.0f);
    float centerZ = screenDistance;
    // Tan-angles of the viewport edges, as seen through the lens.
    float screenLeft = (centerX - halfWidth) / centerZ;
    float screenTop = (centerY + halfHeight) / centerZ;
    float screenRight = (centerX + halfWidth) / centerZ;
    float screenBottom = (centerY - halfHeight) / centerZ;
    // Compare the two sets of tan-angles and take the value closer to zero on each side.
    result[0] = std::fmax(fovLeft, screenLeft);
    result[1] = std::fmin(fovTop, screenTop);
    result[2] = std::fmin(fovRight, screenRight);
    result[3] = std::fmax(fovBottom, screenBottom);
    // result[0] = screenLeft;
    // result[1] = screenTop;
    // result[2] = screenRight;
    // result[3] = screenBottom;
  }
}
