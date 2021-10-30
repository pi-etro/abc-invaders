#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>

#include "abcg.hpp"
#include "cannon.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Bullets {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(Cannon &cannon, const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  struct Bullet {
    bool m_dead{};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  float m_scale{1};

  std::list<Bullet> m_bullets;
};

#endif
