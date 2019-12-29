#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath)
{
  std::ifstream stream(filepath);

  enum class ShaderType {NONE = -1, VERTEX = 0, FRAGMENT = 1};

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (std::getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
      {
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        type = ShaderType::FRAGMENT;
      }
    }
    else
    {
      ss[(int) type] << line << "\n";
    }
  }

  return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*) alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile shader\n";
    std::cout << message << "\n";
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragementShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragementShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(int argc, char* args[])
{
  GLFWwindow* window;

  if (!glfwInit())
  {
    std::cout << "glfwInit() error\n";
    return -1;
  }

  window = glfwCreateWindow(640, 480, "What's popping?", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
  {
    std::cout << "glewInit() error\n";
    return -1;
  }

  float positions[] =
  {
    -0.5f, -0.5f,
     0.0f,  0.5f,
     0.5f, -0.5f
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}
