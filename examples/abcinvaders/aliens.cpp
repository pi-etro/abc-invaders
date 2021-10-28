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
    x += 0.0825 + 0.0375;
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
    abcg::glDrawElements(GL_TRIANGLES, 30 * 3, GL_UNSIGNED_INT, nullptr); // TODO revisar numero para todos
    // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, alien.m_polygonSides + 2);

    abcg::glBindVertexArray(0);
  }

  // move troops for 10 ms 
  if (m_marchPace.elapsed() > 505.0 / 1000.0) m_marchPace.restart();

  auto min = std::min_element(m_aliens.begin(), m_aliens.end(), compareX);

  if ( min->m_translation.x < -0.86 ){
    m_direction = 1.0;
  }
  else {
    auto max = std::max_element(m_aliens.begin(), m_aliens.end(), compareX);
    if ( max->m_translation.x > 0.86 ){
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
        if (m_last_direction != m_direction) {
            alien.m_translation.y -= 0.0375f;
        }
        else alien.m_translation.x += 1.5 * deltaTime* m_direction;
    }
    m_last_direction = m_direction;
  }
}

Aliens::Alien Aliens::createAlien(float x, float y, float scale) {
  Alien alien;

  alien.m_color = glm::vec4{1};
  alien.m_rotation = 0.0f;
  alien.m_scale = scale;
  alien.m_translation = glm::vec2(x, y);
  alien.m_velocity = glm::vec2(0);

  // clang-format off
  std::array<glm::vec2, 60> positions{
      // crab
      // left vertical squares
      glm::vec2{-0.0375f-0.00375f, +0.01f},
      glm::vec2{-0.0375f-0.00375f, +0.04f},
      glm::vec2{-0.0300f-0.00375f, +0.01f},
      glm::vec2{-0.0300f-0.00375f, +0.04f},

      glm::vec2{-0.0300f-0.00375f, +0.03f},
      glm::vec2{-0.0300f-0.00375f, +0.05f},
      glm::vec2{-0.0225f-0.00375f, +0.03f},
      glm::vec2{-0.0225f-0.00375f, +0.05f},

      glm::vec2{-0.0225f-0.00375f, +0.01f},
      glm::vec2{-0.0225f-0.00375f, +0.06f},
      glm::vec2{-0.0150f-0.00375f, +0.01f},
      glm::vec2{-0.0150f-0.00375f, +0.06f},

      glm::vec2{-0.0150f-0.00375f, +0.02f},
      glm::vec2{-0.0150f-0.00375f, +0.04f},
      glm::vec2{-0.0075f-0.00375f, +0.02f},
      glm::vec2{-0.0075f-0.00375f, +0.04f},

      glm::vec2{-0.0150f-0.00375f, +0.05f},
      glm::vec2{-0.0150f-0.00375f, +0.07f},
      glm::vec2{-0.0075f-0.00375f, +0.05f},
      glm::vec2{-0.0075f-0.00375f, +0.07f},

      // middle square
      glm::vec2{-0.0075f-0.00375f, +0.02f},
      glm::vec2{-0.0075f-0.00375f, +0.06f},
      glm::vec2{+0.0075f+0.00375f, +0.02f},
      glm::vec2{+0.0075f+0.00375f, +0.06f},

      // right vertical squares
      glm::vec2{+0.0300f+0.00375f, +0.01f},
      glm::vec2{+0.0300f+0.00375f, +0.04f},
      glm::vec2{+0.0375f+0.00375f, +0.01f},
      glm::vec2{+0.0375f+0.00375f, +0.04f},

      glm::vec2{+0.0225f+0.00375f, +0.03f},
      glm::vec2{+0.0225f+0.00375f, +0.05f},
      glm::vec2{+0.0300f+0.00375f, +0.03f},
      glm::vec2{+0.0300f+0.00375f, +0.05f},

      glm::vec2{+0.0150f+0.00375f, +0.01f},
      glm::vec2{+0.0150f+0.00375f, +0.06f},
      glm::vec2{+0.0225f+0.00375f, +0.01f},
      glm::vec2{+0.0225f+0.00375f, +0.06f},

      glm::vec2{+0.0075f+0.00375f, +0.02f},
      glm::vec2{+0.0075f+0.00375f, +0.04f},
      glm::vec2{+0.0150f+0.00375f, +0.02f},
      glm::vec2{+0.0150f+0.00375f, +0.04f},

      glm::vec2{+0.0075f+0.00375f, +0.05f},
      glm::vec2{+0.0075f+0.00375f, +0.07f},
      glm::vec2{+0.0150f+0.00375f, +0.05f},
      glm::vec2{+0.0150f+0.00375f, +0.07f},

      // antennas and claws
      glm::vec2{-0.0150f-0.00375f, +0.00f},
      glm::vec2{-0.0150f-0.00375f, +0.01f},
      glm::vec2{-0.00375f, +0.00f},
      glm::vec2{-0.00375f, +0.01f},

      glm::vec2{-0.0225f-0.00375f, +0.07f},
      glm::vec2{-0.0225f-0.00375f, +0.08f},
      glm::vec2{-0.0150f-0.00375f, +0.07f},
      glm::vec2{-0.0150f-0.00375f, +0.08f},

      glm::vec2{+0.00375f, +0.00f},
      glm::vec2{+0.00375f, +0.01f},
      glm::vec2{+0.0150f+0.00375f, +0.00f},
      glm::vec2{+0.0150f+0.00375f, +0.01f},

      glm::vec2{+0.0150f+0.00375f, +0.07f},
      glm::vec2{+0.0150f+0.00375f, +0.08f},
      glm::vec2{+0.0225f+0.00375f, +0.07f},
      glm::vec2{+0.0225f+0.00375f, +0.08f},
      };

  const std::array indices{// left vertical squares
                           0, 1, 2,
                           1, 2, 3,

                           4, 5, 6,
                           5, 6, 7,

                           8, 9, 10,
                           9, 10, 11,

                           12, 13, 14,
                           13, 14, 15,

                           16, 17, 18,
                           17, 18, 19,

                           // middle square
                           20, 21, 22,
                           21, 22, 23,

                           // right vertical squares
                           24, 25, 26,
                           25, 26, 27,

                           28, 29, 30,
                           29, 30, 31,

                           32, 33, 34,
                           33, 34, 35,

                           36, 37, 38,
                           37, 38, 39,

                           40, 41, 42,
                           41, 42, 43,

                           // antennas and claws
                           44, 45, 46,
                           45, 46, 47,

                           48, 49, 50,
                           49, 50, 51,

                           52, 53, 54,
                           53, 54, 55,

                           56, 57, 58,
                           57, 58, 59
                           };
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
