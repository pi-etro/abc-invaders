#include "cannon.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Cannon::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_color = glm::vec4{0.00f, 1.00f, 0.00f, 1.0f};
  m_rotation = 0.0f;
  m_translation = glm::vec2(0, -0.90);
  m_velocity = glm::vec2(0.5f, 0.0f);

  // clang-format off
  std::array<glm::vec2, 16> positions{
      glm::vec2{-0.05625f, +0.00f},
      glm::vec2{-0.05625f, +0.04f},
      glm::vec2{+0.05625f, +0.00f},
      glm::vec2{+0.05625f, +0.04f},

      glm::vec2{-0.04875f, +0.00f},
      glm::vec2{-0.04875f, +0.05f},
      glm::vec2{+0.04875f, +0.00f},
      glm::vec2{+0.04875f, +0.05f},

      glm::vec2{-0.01125f, +0.00f},
      glm::vec2{-0.01125f, +0.07f},
      glm::vec2{+0.01125f, +0.00f},
      glm::vec2{+0.01125f, +0.07f},

      glm::vec2{-0.00375f, +0.00f},
      glm::vec2{-0.00375f, +0.08f},
      glm::vec2{+0.00375f, +0.00f},
      glm::vec2{+0.00375f, +0.08f},
      };

  const std::array indices{0, 1, 2,
                           1, 2, 3,

                           4, 5, 6,
                           5, 6, 7,
                           
                           8, 9, 10,
                           9, 10, 11,
                           
                           12, 13, 14,
                           13, 14, 15};
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
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

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

void Cannon::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Cannon::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Cannon::update(const GameData &gameData, float deltaTime) {
  // move cannon on left/right
  if (gameData.m_input[static_cast<size_t>(Input::Left)] &&
      m_translation.x > -0.85)
    m_translation -= m_velocity * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Right)] &&
      m_translation.x < 0.85)
    m_translation += m_velocity * deltaTime;
}
