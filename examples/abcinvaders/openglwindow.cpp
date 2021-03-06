#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

  // mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
  }
}

void OpenGLWindow::initializeGL() {
  // load new font
  ImGuiIO &io{ImGui::GetIO()};
  const auto filename{getAssetsPath() + "Retro-Gaming.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // create program to render the objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_cannon.initializeGL(m_objectsProgram);
  m_bullets.initializeGL(m_objectsProgram);
  m_aliens.initializeGL(m_objectsProgram);
  m_rays.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_cannon.update(m_gameData, deltaTime);
  m_bullets.update(m_cannon, m_gameData, deltaTime);
  m_aliens.update(deltaTime);
  m_rays.update(m_aliens, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_cannon.paintGL(m_gameData);
  m_bullets.paintGL();
  m_aliens.paintGL();
  m_rays.paintGL();
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(350, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("GAME OVER");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text(" You Win!");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_cannon.terminateGL();
  m_bullets.terminateGL();
  m_aliens.terminateGL();
  m_rays.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // check collision between cannon and aliens
  for (const auto &alien : m_aliens.m_aliens) {
    const auto alienTranslation{alien.m_translation};
    const auto distance{
        glm::distance(m_cannon.m_translation, alienTranslation)};

    if (distance < 0.1125f / 2 + 0.0825f / 2 || alien.m_translation.y < -1.0f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }

  // check collision between cannon and rays
  for (auto &ray : m_rays.m_rays) {
    auto rayTranslation{ray.m_translation};
    auto distance{glm::distance(m_cannon.m_translation, rayTranslation)};

    if (distance < 0.0225f / 2 + 0.1125f / 2 || ray.m_translation.y < -1.1f) {
      ray.m_dead = true;
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }

  // check collision between bullets and aliens
  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_dead) continue;

    for (auto &alien : m_aliens.m_aliens) {
      for (const auto i : {-2, 0, 2}) {
        for (const auto j : {-2, 0, 2}) {
          const auto alienTranslation{alien.m_translation + glm::vec2(i, j)};
          const auto distance{
              glm::distance(bullet.m_translation, alienTranslation)};

          if (distance < 0.0075f / 2 + 0.0825f / 2) {
            alien.m_hit = true;
            bullet.m_dead = true;
          }
        }
      }
    }

    // kill aliens marked as hit
    m_aliens.m_aliens.remove_if([](const Aliens::Alien &a) { return a.m_hit; });
  }

  // check collision between bullets and rays
  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_dead) continue;

    for (auto &ray : m_rays.m_rays) {
      for (const auto i : {-2, 0, 2}) {
        for (const auto j : {-2, 0, 2}) {
          const auto rayTranslation{ray.m_translation + glm::vec2(i, j)};
          const auto distance{
              glm::distance(bullet.m_translation, rayTranslation)};

          if (distance < 0.0075f / 2 + 0.0225f / 2) {
            ray.m_dead = true;
            bullet.m_dead = true;
          }
        }
      }
    }
  }
}

void OpenGLWindow::checkWinCondition() {
  if (m_aliens.m_aliens.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}
