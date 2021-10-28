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
  m_aliens.resize(55);

  float x = -0.6375;
  float y = 0.765;
  int i = 0;
  for (auto &alien : m_aliens) {
    alien = createAlien(x, y);
    x += 0.09 + 0.0375;
    i++;
    if (i == 11){
        i = 0;
        x = -0.6375;
        y -= 0.08 + 0.075;
    }
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

  // move troops for 10 ms 
  if (m_marchPace.elapsed() > 505.0 / 1000.0) m_marchPace.restart();

  auto min = std::min_element(m_aliens.begin(), m_aliens.end(), compareX);
  

  if ( min->m_translation.x < -0.85 ){
    m_direction = 1.0;
  }
  else {
    auto max = std::max_element(m_aliens.begin(), m_aliens.end(), compareX);
    if ( max->m_translation.x> 0.85 ){
        m_direction = -1.0;
    }
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
  // move troops every 500 ms
  if (m_marchPace.elapsed() > 500.0 / 1000.0) {
    for (auto &alien : m_aliens) {
        alien.m_translation += alien.m_velocity * deltaTime * m_direction;
    }
  }
}

Aliens::Alien Aliens::createAlien(float x, float y, float scale) {
  Alien alien;

  alien.m_color = glm::vec4{1};
  alien.m_rotation = 0.0f;
  alien.m_scale = scale;
  alien.m_translation = glm::vec2(x, y);
  alien.m_velocity = glm::vec2(3.0f, 0.0f);

  // clang-format off
  std::array<glm::vec2, 24> positions{
      // alien generic
      glm::vec2{-0.045f, -0.00f},
      glm::vec2{-0.045f, +0.08f},
      glm::vec2{+0.045f, -0.00f},
      glm::vec2{+0.045f, +0.08f},
      };

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

bool Aliens::compareX(const Aliens::Alien &left, const Aliens::Alien &right){
    return left.m_translation.x < right.m_translation.x;
}
