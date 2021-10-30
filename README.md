# ABC Invaders


|            Name                   |    RA    |
|:---------------------------------:|:--------:|
| Pietro Di Consolo Gregorio        | 11038416 |
| Beatriz Libanio de Araujo Yordaky | 11013714 |

## Table of Contents

* [About](#about)
* [Implementation](#implementation)
* [Usage](#usage)
* [License and credits](#license-and-credits)

## About

ABC Invaders is a 2D game developed with the [ABCg](https://github.com/hbatagelo/abcg) framework, developed for the Computer Graphics course at UFABC.

The game is inspired by [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders), originally developed in 1978, and was implemented with C++ programming language and OpenGL. The player controls a laser cannon able to move horizontally on the screen, with the objective to shoot all the aliens, that can also shoot the player (and then the game is over). The aliens move both horizontally and vertically, constantly approching the laser cannon.

## Implementation

The main libraries used by ABCg are: [SDL](https://www.libsdl.org/) to handle inputs and outputs, [GLEW](http://glew.sourceforge.net/) to access the OpenGL graphical API and [emscripten](https://emscripten.org/) to generate the application's WebAssembly.

The game begins on `Playing` stage, with the cannon free to move (`A`/`D` or `←`/`→` keys) and fire (`space` or `left mouse button`) and the aliens releasing their ray shots while moving. When the player destroys all the aliens the game ends with a message of victory (`Win` state), but when the player is hit by an alien ray, or the alien itself, the game is over (`GameOver` state). In both cases the game restarts within five seconds.

The game components are: cannon (player), cannon bullets, aliens and aliens bullets (rays). Each component is composed by triangles forming small rectangles that shapes the final image (see the example bellow). All components have their logic, such as velocities, colors and behaviour, implemented in the respective header and class files.


<p align="center">
  <img width="350" src="https://raw.githubusercontent.com/pi-etro/abc-invaders/main/img/component_example.svg">
</p>

The openglwindow component is responsible to centralize the game control, calling all components subroutines, the game state checkup routine and the colision routine.

## Usage

Build the application with `build.sh` (Linux/macOS) or `build.bat` (Windows) and run the executable file created at `abcg/build/bin/abcinvaders`.

## License and credits

ABCg is a framework developed by Harlen Batagelo for the Computer Graphics course at UFABC, and is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
