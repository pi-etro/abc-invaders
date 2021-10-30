#ifndef ALIENS_HPP_
#define ALIENS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class Rays;

class Aliens {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(float deltaTime);

 private:
  friend OpenGLWindow;
  friend Rays;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  struct Alien {
    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    bool m_hit{false};
    glm::vec4 m_color{1};
    float m_rotation{};
    float m_scale{1};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  std::list<Alien> m_aliens;

  abcg::ElapsedTimer m_marchPace;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  Aliens::Alien createAlien(float x, float y, float scale = 1.0f);

  float m_direction = 1.0;
  float m_last_direction = 1.0;

  static bool compareX(const Aliens::Alien &left, const Aliens::Alien &right);

  abcg::ElapsedTimer m_rayCoolDownTimer;
};

#endif
