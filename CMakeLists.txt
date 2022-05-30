cmake_minimum_required(VERSION 3.10)

project(LearningOpenGL)

add_executable(app
  src/main.cpp
  src/shader.cpp
)

target_link_libraries(app glfw GL GLEW)
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
