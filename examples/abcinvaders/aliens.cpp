#include "aliens.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Aliens::initializeGL(GLuint program) {
  terminateGL();

  // Start pseudo-random number generator
//   m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create aliens
  m_aliens.clear();
  m_aliens.resize(5);

  for (auto &alien : m_aliens) {
    alien = createAlien();
  }
}

void Aliens::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &alien : m_aliens) {
    abcg::glBindVertexArray(alien.m_vao);

    abcg::glUniform1f(m_scaleLoc, alien.m_scale);
    abcg::glUniform1f(m_rotationLoc, alien.m_rotation);
    abcg::glUniform2fv(m_translationLoc, 1, &alien.m_translation.x);

    abcg::glUniform4fv(m_colorLoc, 1, &alien.m_color.r);
    abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);
    // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, alien.m_polygonSides + 2);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Aliens::terminateGL() {
  for (auto alien : m_aliens) {
    abcg::glDeleteBuffers(1, &alien.m_vbo);
    abcg::glDeleteBuffers(1, &alien.m_ebo);
    abcg::glDeleteVertexArrays(1, &alien.m_vao);
  }
}

void Aliens::update(float deltaTime) {
  if (m_marchPace.elapsed() > 1) {
      m_marchPace.restart();
    for (auto &alien : m_aliens) {
        alien.m_translation += alien.m_velocity * deltaTime;
    }
  }
}

Aliens::Alien Aliens::createAlien(glm::vec2 translation,
                                              float scale) {
  Alien alien;

  alien.m_color = glm::vec4{1};
  alien.m_rotation = 0.0f;
  alien.m_scale = scale;
  alien.m_translation = translation;
  alien.m_velocity = glm::vec2(1.5f, 0.0f);

  // clang-format off
  std::array<glm::vec2, 24> positions{
      // alien
      glm::vec2{-10.5f, -04.0f},
      glm::vec2{-10.5f, +04.0f},
      glm::vec2{+10.5f, -04.0f},
      glm::vec2{+10.5f, +04.0f},
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  const std::array indices{0, 1, 2,
                           1, 2, 3};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &alien.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, alien.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &alien.m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alien.m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &alien.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(alien.m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, alien.m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alien.m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return alien;
}
