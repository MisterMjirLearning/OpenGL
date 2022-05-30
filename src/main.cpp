#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

static void framebuffer_size_callback(GLFWwindow* window, int w, int h);

int main()
{
  // Initialize glfw
  if (!glfwInit())
  {
    std::cout << "glfwInit() failed\n";
    return -1;
  }

  // Create a glfw window and assign it to the context
  GLFWwindow* window = glfwCreateWindow(800, 600, "Learning OpenGL", nullptr, nullptr);
  if (!window)
  {
    std::cout << "Could not create GLFW window" << "\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize glew
  if (glewInit() != GLEW_OK)
  {
    std::cout << "glewInit() error\n";
    return -1;
  }

  // GL coordinates are in between -1 and 1, the viewport will scale these
  glViewport(0, 0, 800, 600);

  // Rescale viewport when window is resized
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

  // Create vertices
  float vertices[] = {
    // Positions        // Colors         // Texture
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top Left
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f  // Bottom Left
  };

  unsigned int indices[] = {
    3, 0, 1,
    2, 3, 0
  };

  // Create a vertex buffer object
  unsigned int vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);

  // Create a vertex array object
  unsigned int vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);

  // Create element buffer object
  unsigned int elementBufferObject;
  glGenBuffers(1, &elementBufferObject);

  // Bind vertex array
  glBindVertexArray(vertexArrayObject);

  // Bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  /* Tell GL how to interpret the vertex data */
  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);
  // Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Create texture
  stbi_set_flip_vertically_on_load(true);
  // First texture
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  unsigned char* data = stbi_load("res/textures/king_louie.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD\n" << std::endl;
  }
  stbi_image_free(data);
  // Second texture
  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  data = stbi_load("res/textures/hbird_logo.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
  }
  stbi_image_free(data);

  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();

    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertexArrayObject);
  glDeleteBuffers(1, &vertexBufferObject);
  glDeleteBuffers(1, &elementBufferObject);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
  glViewport(0, 0, w, h);
}
