#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "aliens.hpp"
#include "bullets.hpp"
#include "cannon.hpp"
#include "rays.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Cannon m_cannon;
  Bullets m_bullets;
  Aliens m_aliens;
  Rays m_rays;

  abcg::ElapsedTimer m_restartWaitTimer;  // TODO remove

  ImFont* m_font{};

  void checkCollisions();
  void checkWinCondition();

  void restart();
  void update();
};

#endif
