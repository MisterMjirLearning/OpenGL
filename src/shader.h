#ifndef SHADER
#define SHADER

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  unsigned int get() {return ID;}
  void use();

  // Uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string & name, int value) const;
  void setFloat(const std::string &name, float value) const;
private:
  unsigned int ID;
};

#endif
