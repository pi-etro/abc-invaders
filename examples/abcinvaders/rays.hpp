#ifndef RAYS_HPP_
#define RAYS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "aliens.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Rays {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(Aliens &aliens, float deltaTime);

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

  struct Ray {
    bool m_dead{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  float m_scale{1};

  std::list<Ray> m_rays;

  std::default_random_engine m_randomEngine;

  glm::vec2 getRandomAlienTranslation(Aliens &aliens);
};

#endif
