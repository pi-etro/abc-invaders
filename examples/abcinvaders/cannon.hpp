#ifndef CANNON_HPP_
#define CANNON_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class Bullets;

class Cannon {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;
  friend Bullets;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{0.00f, 1.00f, 0.00f, 1.0f};
  float m_rotation{};
  float m_scale{1};
  glm::vec2 m_translation{glm::vec2(0.0f, -0.90f)};
  glm::vec2 m_velocity{glm::vec2(1.5f, 0.0f)};

  abcg::ElapsedTimer m_bulletCoolDownTimer;
};

#endif