project(abcinvaders)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp cannon.cpp
                               bullets.cpp aliens.cpp rays.cpp)

enable_abcg(${PROJECT_NAME})
