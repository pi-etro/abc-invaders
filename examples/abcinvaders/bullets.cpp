#include "bullets.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Bullets::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_bullets.clear();

  // clang-format off
  std::array<glm::vec2, 4> positions{
      glm::vec2{-0.00375f, -0.00f},
      glm::vec2{-0.00375f, +0.06f},
      glm::vec2{+0.00375f, -0.00f},
      glm::vec2{+0.00375f, +0.06f},
      };

  const std::array indices{0, 1, 2,
                           1, 2, 3};
  // clang-format on

  // generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // end of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Bullets::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform4f(m_colorLoc, 0, 1, 0, 1);
  abcg::glUniform1f(m_rotationLoc, 0);
  abcg::glUniform1f(m_scaleLoc, m_scale);

  for (const auto &bullet : m_bullets) {
    abcg::glUniform2f(m_translationLoc, bullet.m_translation.x,
                      bullet.m_translation.y);

    abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Bullets::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Bullets::update(Cannon &cannon, const GameData &gameData,
                     float deltaTime) {
  // create bullet
  if (gameData.m_input[static_cast<size_t>(Input::Fire)] &&
      gameData.m_state == State::Playing) {
    // at least 500 ms must have passed since the last bullet
    if (cannon.m_bulletCoolDownTimer.elapsed() > 500.0 / 1000.0) {
      cannon.m_bulletCoolDownTimer.restart();

      const auto bulletSpeed = glm::vec2{0.0f, 2.5f};

      Bullet bullet{.m_dead = false,
                    .m_translation = cannon.m_translation,
                    .m_velocity = bulletSpeed};
      m_bullets.push_back(bullet);
    }
  }

  for (auto &bullet : m_bullets) {
    bullet.m_translation += bullet.m_velocity * deltaTime;

    // kill bullet if it goes off screen
    if (bullet.m_translation.y < -1.1f) bullet.m_dead = true;
    if (bullet.m_translation.y > +1.1f) bullet.m_dead = true;
  }

  // remove dead bullets
  m_bullets.remove_if([](const Bullet &p) { return p.m_dead; });
}
