#include <00/functional.cuh>
#include <common/GUI.hpp>
#include <common/image.hpp>
#include <iostream>
#include <memory>

GLuint indexBufferId;
GLuint vertexBufferId;

// Vertex class
struct Vertex {
  Vertex(const glm::vec3& position_, const glm::vec3& color_)
      : position(position_), color(color_) {
  }

  glm::vec3 position;
  glm::vec3 color;
};

// clang-format off
static const glm::vec3 positions[8] = {
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3( 1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3( 1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3( 1.0f, -1.0f,  1.0f),
    glm::vec3( 1.0f,  1.0f,  1.0f)
};

static const glm::vec3 colors[6] = {
    glm::vec3(1.0f, 0.0f, 0.0f),  // 赤
    glm::vec3(0.0f, 1.0f, 0.0f),  // 緑
    glm::vec3(0.0f, 0.0f, 1.0f),  // 青
    glm::vec3(1.0f, 1.0f, 0.0f),  // イエロー
    glm::vec3(0.0f, 1.0f, 1.0f),  // シアン
    glm::vec3(1.0f, 0.0f, 1.0f),  // マゼンタ
};

static const unsigned int faces[12][3] = {
    { 7, 4, 1 }, { 7, 1, 6 },
    { 2, 4, 7 }, { 2, 7, 5 },
    { 5, 7, 6 }, { 5, 6, 3 },
    { 4, 2, 0 }, { 4, 0, 1 },
    { 3, 6, 1 }, { 3, 1, 0 },
    { 2, 5, 3 }, { 2, 3, 0 }
};
// clang-format on

class SceneBuffer {
 public:
  inline static std::shared_ptr<gui::FrameBuffer> frameBuffer;
  inline static void setFrameBuffer(float width, float height) { SceneBuffer::frameBuffer = std::make_shared<gui::FrameBuffer>(width, height); };
  inline static void window_size_callback(GLFWwindow* window, int width, int height) {
    SceneBuffer::frameBuffer->RescaleFrameBuffer(width, height);
    glViewport(0, 0, width, height);
  };
};

void initializeGL() {
  // 深度テストの有効化
  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // 背景色の設定 (黒)
  // Background color (black)
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // 頂点配列の作成
  // Prepare vertex array
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  int idx = 0;
  for (int face = 0; face < 6; face++) {
    for (int i = 0; i < 3; i++) {
      vertices.push_back(Vertex(positions[faces[face * 2 + 0][i]], colors[face]));
      indices.push_back(idx++);
    }

    for (int i = 0; i < 3; i++) {
      vertices.push_back(Vertex(positions[faces[face * 2 + 1][i]], colors[face]));
      indices.push_back(idx++);
    }
  }

  // 頂点バッファオブジェクトの作成
  // Create vertex buffer object
  glGenBuffers(1, &vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  // 頂点番号バッファオブジェクトの作成
  // Create index buffer object
  glGenBuffers(1, &indexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               indices.data(), GL_STATIC_DRAW);
}

int main(int argc, char** argv) {
  std::cout << "Hello world!" << std::endl;
  utils::deviceInfo();

  if (glfwInit() == GLFW_FALSE) {
    fprintf(stderr, "Initialization failed!\n");
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow(1200, 900, "test", NULL, NULL);

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
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = nullptr;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  initializeGL();

  SceneBuffer::setFrameBuffer(640, 480);
  // glfwSetWindowSizeCallback(window, SceneBuffer::window_size_callback);

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
      // Side Bar
      if (ImGui::BeginViewportSideBar("Demo window", ImGui::GetWindowViewport(), ImGuiDir_::ImGuiDir_Left, 200.0, true)) {
        ImGui::Button("Hello!");
        ImGui::End();
      }
    }

    {
      // Render OpenGL buffer
      ImVec2 viewportPos = ImGui::GetWindowViewport()->Pos;
      ImGui::SetNextWindowPos(ImVec2(viewportPos.x + 200.0, viewportPos.y));
      ImVec2 windowSize = ImVec2(ImGui::GetWindowViewport()->Size.x - 200.0, ImGui::GetWindowViewport()->Size.y);
      ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);

      ImGui::Begin("My Scene");
      const float window_width = ImGui::GetContentRegionAvail().x;
      const float window_height = ImGui::GetContentRegionAvail().y;

      SceneBuffer::window_size_callback(window, window_width, window_height);
      glViewport(0, 0, window_width, window_height);

      ImVec2 pos = ImGui::GetCursorScreenPos();

      ImGui::GetWindowDrawList()->AddImage(
          (void*)SceneBuffer::frameBuffer->getFrameTexture(),
          ImVec2(pos.x, pos.y),
          ImVec2(pos.x + window_width, pos.y + window_height),
          ImVec2(0, 1),
          ImVec2(1, 0));

      ImGui::End();
    }

    // Render dear imgui into screen
    ImGui::Render();

    {
      // OpenGL Rendering
      SceneBuffer::frameBuffer->Bind();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // 座標の変換
      // Coordinate transformation
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(45.0f, SceneBuffer::frameBuffer->_width / SceneBuffer::frameBuffer->_height, 0.1f, 1000.0f);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(3.0f, 4.0f, 5.0f,   // 視点の位置 / Eye position
                0.0f, 0.0f, 0.0f,   // 見ている先 / Looking position
                0.0f, 1.0f, 0.0f);  // 視界の上方向 / Upward direction

      // 回転行列の設定
      // Setup rotation matrix
      glPushMatrix();

      // 頂点バッファの有効化
      // Enable vertex buffer object
      glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

      // 頂点情報の詳細を設定
      // Setup details for vertex array
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));

      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));

      // 三角形の描画
      // Draw triangles
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

      // 頂点バッファの無効化
      // Disable vertex buffer object
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      // 回転行列の破棄
      // Dispose of rotation matrix
      glPopMatrix();
      SceneBuffer::frameBuffer->Unbind();
    }

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