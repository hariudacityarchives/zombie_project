#pragma once

#include <GL/glew.h>

namespace JEngine {
struct Position {
  Position() : x(0), y(0) {}

  Position(float X, float Y) : x(X), y(Y) {}

  float x, y;
};

struct ColorRGBA8 {
  ColorRGBA8() : r(0), g(0), b(0), a(0) {}

  ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A = 255)
      : r(R), g(G), b(B), a(A) {}

  void setColor(GLubyte R, GLubyte G, GLubyte B, GLubyte A = 255) {
    r = R;
    g = G;
    b = B;
    a = A;
  }

  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
};

struct UV {
  float u, v;
};

struct Vertex {
  Position position;
  ColorRGBA8 color;
  UV uv;

  void setPosition(float x, float y) {
    position.x = x;
    position.y = y;
  }

  void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
  }

  void setUV(float u, float v) {
    // wow.
    uv.u = u;
    uv.v = v;
  }
};
} // namespace JEngine