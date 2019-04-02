#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <rendering/shader.hpp>

static const GLuint WIDTH = 800;
static const GLuint HEIGHT = 600;
/* ourColor is passed on to the fragment shader. */
static const GLchar* vertex_shader_source =
  "#version 330 core\n"
  "layout (location = 0) in vec3 position;\n"
  "layout (location = 1) in vec3 color;\n"
  "out vec3 ourColor;\n"
  "uniform mat4 transform;\n"
  "void main() {\n"
  "    gl_Position = transform * vec4(position, 1.0f);\n"
  "    ourColor = color;\n"
  "}\n";
static const GLchar* fragment_shader_source =
  "#version 330 core\n"
  "in vec3 ourColor;\n"
  "out vec4 color;\n"
  "void main() {\n"
  "    color = vec4(ourColor, 1.0f);\n"
  "}\n";

#define CIRCLE_SIDES 50
#define CIRCLE_VERTS (CIRCLE_SIDES + 2)
#define COLOR_LEN 3

class Circle {
public:

  Circle(float x, float y) : x(x), y(y) {
    update_verts(0.5f);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
  }

  float x;
  float y;

  float verts[3 * CIRCLE_VERTS];
  float color[COLOR_LEN] = {0.5, 0.5, 0.5};
  unsigned int vao; // vertex attribute object
  unsigned int vbo; // vertex buffer object (gpu memory)

  void draw(Shader &shader) {
    GLint transform_location = glGetUniformLocation(shader.program, "transform");

    GLfloat transform[] = {
      1.0f, 0.0f, 0.0f, x,
      0.0f, 1.0f, 0.0f, y,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f,
    };
    glUniformMatrix4fv(transform_location, 1, GL_FALSE, transform);

    shader.setVec3("ourColor", color[0], color[1], color[2]);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTS);
    glBindVertexArray(0);
  }

private:
  void update_verts(GLfloat radius) {
    verts[0] = 0;
    verts[1] = 0;
    verts[2] = 0;
    for (int i = 1; i < CIRCLE_VERTS; i++) {
      verts[i * 3] = static_cast<float> (0 + (radius * cos(i *  2 * M_PI / CIRCLE_SIDES)));
      verts[i * 3 + 1] = static_cast<float> (0 + (radius * sin(i * 2 * M_PI / CIRCLE_SIDES)));
      verts[i * 3 + 2] = 0;
    }
  }
};

int main(int argc, char *argv[]) {
  GLint transform_location;
  GLuint vbo;
  GLuint vao;
  GLFWwindow* window;
  double time;

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glViewport(0, 0, WIDTH, HEIGHT);

  Shader shader;
  shader.compile_shaders(vertex_shader_source, fragment_shader_source);

  Circle circle(0.5f, 0.5f);

  static GLfloat vertices[] = {
    /*   Positions          Colors */
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Position attribute */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  /* Color attribute */
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    circle.draw(shader);

//    transform_location = glGetUniformLocation(shader.program, "transform");
//    /* THIS is just a dummy transform. */
//    GLfloat transform[] = {
//      0.0f, 0.0f, 0.0f, 0.0f,
//      0.0f, 0.0f, 0.0f, 0.0f,
//      0.0f, 0.0f, 1.0f, 0.0f,
//      0.0f, 0.0f, 0.0f, 1.0f,
//    };
//    time = glfwGetTime();
//    transform[0] = 1.0f * sin(3.14 * time);
//    transform[5] = 1.0f * cos(time);
//    glUniformMatrix4fv(transform_location, 1, GL_FALSE, transform);
//
//    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glfwTerminate();
  return EXIT_SUCCESS;
}