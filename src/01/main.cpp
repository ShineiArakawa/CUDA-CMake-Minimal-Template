#include <00/functional.cuh>
#include <common/GUI.hpp>
#include <common/image.hpp>
#include <iostream>

static gui::FrameBuffer sceneBuffer(640, 480);

void window_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  sceneBuffer.RescaleFrameBuffer(width, height);
}

int main(int argc, char **argv) {
  std::cout << "Hello world!" << std::endl;
  utils::deviceInfo();

  if (glfwInit() == GLFW_FALSE) {
    fprintf(stderr, "Initialization failed!\n");
    return 1;
  }

  GLFWwindow *window = glfwCreateWindow(1280, 960, "test", NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    fprintf(stderr, "Window creation failed!\n");
    return 1;
  }

  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  glfwSwapInterval(1);

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
  std::string imgFilePath =
      "../data/image_1.jpg";
  GLuint textureID;
  common::loadTexture(imgFilePath, textureID);

  glfwSetWindowSizeCallback(window, window_size_callback);

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
      sceneBuffer.Bind();

      sceneBuffer.Unbind();
    }

    {
      // rendering our geometries
      ImGui::Begin("Scene");
      {
        ImGui::BeginChild("GameRender");

        ImGui::Image(
            (ImTextureID)sceneBuffer.getFrameTexture(),
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0));
      }
      ImGui::EndChild();
      ImGui::End();
    }

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