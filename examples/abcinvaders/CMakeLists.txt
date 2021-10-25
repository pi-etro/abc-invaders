project(abcinvaders)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp cannon.cpp
                               aliens.cpp)

enable_abcg(${PROJECT_NAME})
