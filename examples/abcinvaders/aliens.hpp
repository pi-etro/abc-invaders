#ifndef ALIENS_HPP_
#define ALIENS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Aliens {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(float deltaTime);

 private:
  friend OpenGLWindow;

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
    float m_scale{};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  std::list<Alien> m_aliens;

  abcg::ElapsedTimer m_marchPace;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  Aliens::Alien createAlien(glm::vec2 translation = glm::vec2(0),
                                     float scale = 0.25f);

};

#endif