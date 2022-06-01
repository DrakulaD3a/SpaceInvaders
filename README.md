# SpaceInvaders
My first SDL game.

- It is recomended to use "out-of-source" build (https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#out-of-source-build-trees) that means you should create a build folder next to the repository you cloned in the step one.
In this particular example its **SDLGameBuild**.
- Using CMake GUI
  - Specify **SDLGameTemplate** folder as Source
  - Specify **SDLGameBuild** folder as Build folder.
  - Configure (Don't forget to use the right generator accordingly to what you have available)
  - Generate
