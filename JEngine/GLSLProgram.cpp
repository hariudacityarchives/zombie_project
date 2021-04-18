#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <iostream>
#include <vector>


namespace JEngine {
// set everything to zero, the fast way!
GLSLProgram::GLSLProgram()
    : _numAttribute(0), _programID(0), _vertexShaderID(0),
      _fragmentShaderID(0) {}

GLSLProgram::~GLSLProgram() {}

void GLSLProgram::compileShaders(const std::string &vertexShaderFilePath,
                                 const std::string &fragmentShaderFile) {
  // Create the holy program
  _programID = glCreateProgram();

  _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

  if (_vertexShaderID == 0) {
    fatalError("Vertex shader failed to be created!");
  }

  _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  if (_fragmentShaderID == 0) {
    fatalError("Fragment shader failed to be created!");
  }

  // std::cout << "Created program " << _programID << ", with vertex shader " <<
  // _vertexShaderID << ", and fragment shader " << _fragmentShaderID << "." <<
  // std::endl;

  compileShader(vertexShaderFilePath, _vertexShaderID);
  compileShader(fragmentShaderFile, _fragmentShaderID);
}

void GLSLProgram::compileShader(const std::string &filePath, GLuint id) {
  // allocate a stream
  std::ifstream vertexFile(filePath);
  if (vertexFile.fail()) {
    perror(filePath.c_str());
    fatalError("Failed to open shader file " + filePath + "!");
  }

  std::string fileContent = "";
  std::string line;

  while (std::getline(vertexFile, line)) {
    fileContent += line + '\n';
  }

  vertexFile.close();

  const char *contentsPtr = fileContent.c_str();
  // give it to the shader compiler
  glShaderSource(
      id, 1, &contentsPtr,
      nullptr); // we only have one shader, so we don't need length stuff

  glCompileShader(id);

  GLint succes = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &succes);

  if (succes == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

    // the maxlength contains the NULL char
    std::vector<char> errorLog(maxLength);
    glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

    glDeleteShader(id);

    std::printf("%s\n", &(errorLog[0]));
    fatalError("Shader \"" + filePath + "\" failed to compile!");
  }
}

void GLSLProgram::linkShaders() {
  // Attach our shaders to our program
  glAttachShader(_programID, _vertexShaderID);
  glAttachShader(_programID, _fragmentShaderID);

  // Link our program
  glLinkProgram(_programID);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(_programID);
    // Don't leak shaders either.
    glDeleteShader(_vertexShaderID);
    glDeleteShader(_fragmentShaderID);

    // Use the infoLog as you see fit.

    std::printf("%s\n", &(errorLog[0]));
    fatalError("Shaders failed to link!");
  }
  // std::cout << "Linked shaders to program " << _programID << "!" <<
  // std::endl;

  // Always detach shaders after a successful link.
  glDetachShader(_programID, _vertexShaderID);
  glDetachShader(_programID, _fragmentShaderID);
  glDeleteShader(_vertexShaderID);
  glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::addAttribute(const std::string &attributeName) {
  // bind attributes, how many inputs do you have, name
  glBindAttribLocation(_programID, _numAttribute++, attributeName.c_str());
  // after that stuff is done, ++.
}

GLint GLSLProgram::getUniformLocation(const std::string &uniformName) {
  GLint loc = glGetUniformLocation(_programID, uniformName.c_str());

  if (loc == GL_INVALID_INDEX) {
    fatalError("Uniform " + uniformName +
               " not found in shader(invalid index)! :(");
  }

  return loc;
}

void GLSLProgram::use() {
  glUseProgram(_programID);
  for (int i = 0; i < _numAttribute; i++) {
    glEnableVertexAttribArray(i);
  }
}

void GLSLProgram::unuse() {
  glUseProgram(0); // don't use program plz
  for (int i = 0; i < _numAttribute; i++) {
    glDisableVertexAttribArray(i);
  }
}
} // namespace JEngine