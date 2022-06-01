# SpaceInvaders

---
## Windows setup
### 1. Clone this repository (do not initialize submodules)
~~~
git clone https://github.com/DrakulaD3a/SpaceInvaders/ Source
~~~
This will create **Source** folder with the source code and assets wherever you run the clone command.

### 2. Create your build using CMake
- Its recomended to use "out-of-source" build (https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#out-of-source-build-trees) that means you should create a build folder next to the repository you cloned in the step one.
In this particular example its **SDLGameBuild**.
- Using CMake GUI
  - Specify **Source** folder as Source
  - Specify **Build** folder as Build folder.
  - Configure (Don't forget to use the right generator accordingly to what you have available)
  - Generate
  - Open project

### 3. Open and build project
You can find prepared project in the **Build** folder.
If you use Visual Studio, you can find **.sln** file there. Don't forget to set your project as the one being run by default.
