#pragma once

#include <GL/glew.h>
#include <string>


namespace JEngine {
class GLSLProgram {
public:
  GLSLProgram();
  ~GLSLProgram();

  void compileShaders(const std::string &vertexShaderFilePath,
                      const std::string &fragmentShaderFile);

  void linkShaders();

  void addAttribute(const std::string &attributeName);

  GLint getUniformLocation(const std::string &uniformName);

  void use();
  void unuse();

private:
  int _numAttribute;

  void compileShader(const std::string &filePath, GLuint id);

  GLuint _programID;

  GLuint _vertexShaderID;
  GLuint _fragmentShaderID;
};

} // namespace JEngine