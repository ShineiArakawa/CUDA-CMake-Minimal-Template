#ifndef __INCLUDE_GUI__

// clang-format off
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_glut.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
// clang-format on

namespace gui {
class FrameBuffer {
 public:
  FrameBuffer(float width, float height);
  ~FrameBuffer();
  unsigned int getFrameTexture();
  void RescaleFrameBuffer(float width, float height);
  void Bind() const;
  void Unbind() const;
  float _width;
  float _height;

 private:
  GLuint _fbo;
  GLuint _texture;
  GLuint _rbo;
};
}  // namespace gui

#define __INCLUDE_GUI__
#endif