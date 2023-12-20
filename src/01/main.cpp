#include <iostream>

#include <00/functional.cuh>
#include <DearImGUI.hpp>
#include <image.hpp>

int main(int argc, char **argv) {
  std::cout << "Hello world!" << std::endl;
  utils::deviceInfo();

  if (glfwInit() == GLFW_FALSE) {
    fprintf(stderr, "Initialization failed!\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow *window = glfwCreateWindow(640, 480, "test", NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    fprintf(stderr, "Window creation failed!\n");
    return 1;
  }

  glfwMakeContextCurrent(window);
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

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  std::string imgFilePath =
      "/home/shinaraka/Projects/CUDA_Training/data/image_1.jpg";
  GLuint textureID;
  common::loadTexture(imgFilePath, textureID);

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
      // rendering our geometries
      //   ImGui::BeginChild("GameRender");
      //   ImVec2 wsize = ImGui::GetWindowSize();
      //   ImGui::Image((ImTextureID)textureID, wsize, ImVec2(0, 1), ImVec2(1,
      //   0)); ImGui::EndChild();
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