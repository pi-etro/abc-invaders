#include "rays.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Rays::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rays.clear();

  // clang-format off
  std::array<glm::vec2, 28> positions{
      glm::vec2{+0.00375f, +0.00f},
      glm::vec2{+0.00375f, +0.01f},
      glm::vec2{+0.0075+0.00375f, +0.00f},
      glm::vec2{+0.0075+0.00375f, +0.01f},
      
      glm::vec2{-0.00375f, +0.01f},
      glm::vec2{-0.00375f, +0.02f},
      glm::vec2{+0.00375f, +0.01f},
      glm::vec2{+0.00375f, +0.02f},

      glm::vec2{-0.0075-0.00375f, +0.02f},
      glm::vec2{-0.0075-0.00375f, +0.03f},
      glm::vec2{-0.00375f, +0.02f},
      glm::vec2{-0.00375f, +0.03f},

      glm::vec2{-0.00375f, +0.03f},
      glm::vec2{-0.00375f, +0.04f},
      glm::vec2{+0.00375f, +0.03f},
      glm::vec2{+0.00375f, +0.04f},

      glm::vec2{+0.00375f, +0.04f},
      glm::vec2{+0.00375f, +0.05f},
      glm::vec2{+0.0075+0.00375f, +0.04f},
      glm::vec2{+0.0075+0.00375f, +0.05f},

      glm::vec2{-0.00375f, +0.05f},
      glm::vec2{-0.00375f, +0.06f},
      glm::vec2{+0.00375f, +0.05f},
      glm::vec2{+0.00375f, +0.06f},

      glm::vec2{-0.0075-0.00375f, +0.06f},
      glm::vec2{-0.0075-0.00375f, +0.07f},
      glm::vec2{-0.00375f, +0.06f},
      glm::vec2{-0.00375f, +0.07f},
      };

  const std::array indices{0, 1, 2,
                           1, 2, 3,

                           4, 5, 6,
                           5, 6, 7,

                           8, 9, 10,
                           9, 10, 11,

                           12, 13, 14,
                           13, 14, 15,

                           16, 17, 18,
                           17, 18, 19,

                           20, 21, 22,
                           21, 22, 23,

                           24, 25, 26,
                           25, 26, 27
                           };
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Rays::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform4f(m_colorLoc, 1, 1, 1, 1);
  abcg::glUniform1f(m_rotationLoc, 0);
  abcg::glUniform1f(m_scaleLoc, m_scale);

  for (const auto &ray : m_rays) {
    abcg::glUniform2f(m_translationLoc, ray.m_translation.x,
                      ray.m_translation.y);

    abcg::glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Rays::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Rays::update(Aliens &aliens, float deltaTime) {
  // Create ray
  if (m_rays.size() < 3 ){
    // At least 250 ms must have passed since the last bullets
    if (aliens.m_rayCoolDownTimer.elapsed() > 300.0 / 1000.0) {
      aliens.m_rayCoolDownTimer.restart();

      const auto raySpeed = glm::vec2{0.0f, -0.8f};

      auto rayTranslation = getRandomAlienTranslation(aliens);

      if (rayTranslation.x != 0 && rayTranslation.y != 0) {
        Ray ray{.m_dead = false,
                        .m_translation = rayTranslation,
                        .m_velocity = raySpeed};
        m_rays.push_back(ray);
      }
    }
  }

  for (auto &ray : m_rays) {
    ray.m_translation += ray.m_velocity * deltaTime;
    
    // Kill bullet if it goes off screen
    if (ray.m_translation.x < -1.1f) ray.m_dead = true; // TODO remove
    if (ray.m_translation.x > +1.1f) ray.m_dead = true; // TODO remove
    if (ray.m_translation.y < -1.1f) ray.m_dead = true;
    if (ray.m_translation.y > +1.1f) ray.m_dead = true;
  }

  // Remove dead bullets
  m_rays.remove_if([](const Ray &p) { return p.m_dead; });
}

glm::vec2 Rays::getRandomAlienTranslation(Aliens &aliens){
    if (!aliens.m_aliens.empty()) {
        auto iterator = aliens.m_aliens.begin();
        std::uniform_int_distribution<int> randomIndex{0, (int) aliens.m_aliens.size()};
        const auto random(randomIndex(m_randomEngine));
        // int random = rand() % aliens.m_aliens.size();
        std::advance(iterator, random);
        return iterator->m_translation;
    }
    return glm::vec2{0};
}
