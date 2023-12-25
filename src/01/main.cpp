#include <00/functional.cuh>
#include <common/GUI.hpp>
#include <common/image.hpp>
#include <iostream>
#include <memory>

class SceneBuffer {
 public:
  inline static std::shared_ptr<gui::FrameBuffer> frameBuffer;
  inline static void setFrameBuffer(float width, float height) { SceneBuffer::frameBuffer = std::make_shared<gui::FrameBuffer>(width, height); };
  inline static void window_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SceneBuffer::frameBuffer->RescaleFrameBuffer(width, height);
  };
};

int main(int argc, char **argv) {
  std::cout << "Hello world!" << std::endl;
  utils::deviceInfo();

  if (glfwInit() == GLFW_FALSE) {
    fprintf(stderr, "Initialization failed!\n");
    return 1;
  }

  GLFWwindow *window = glfwCreateWindow(1200, 900, "test", NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    fprintf(stderr, "Window creation failed!\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // OpenGL 3.x/4.xの関数をロードする (glfwMakeContextCurrentの後でないといけない)
  const int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    fprintf(stderr, "Failed to load OpenGL 3.x/4.x libraries!\n");
    return 1;
  }

  // バージョンを出力する
  printf("Load OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  // std::string imgFilePath = "../data/image_1.jpg";
  // GLuint textureID;
  // gui::loadTexture(imgFilePath, textureID);

  SceneBuffer::setFrameBuffer(640, 480);
  glfwSetWindowSizeCallback(window, SceneBuffer::window_size_callback);

  std::cout << "Start window loop !" << std::endl;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      SceneBuffer::frameBuffer->Bind();

      SceneBuffer::frameBuffer->Unbind();
    }

    // ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    // ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    // ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Appearing);
    {
      // rendering our geometries
      ImGui::Begin("Scene");
      {
        ImGui::BeginChild("GameRender");

        ImGui::Image(
            (ImTextureID)SceneBuffer::frameBuffer->getFrameTexture(),
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0));

        ImGui::EndChild();
      }
      ImGui::End();
    }

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Appearing);
    {
      // render your GUI
      ImGui::Begin("Demo window");
      ImGui::Button("Hello!");
      ImGui::End();
    }

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}