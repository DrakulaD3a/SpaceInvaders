# SpaceInvaders

---
## Windows setup
### 1. Clone this repository
~~~
git clone https://github.com/DrakulaD3a/SpaceInvaders.git Source
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
## Linux setup
### 1. Clone this repository
~~~
git clone https://github.com/DrakulaD3a/SpaceInvaders.git Source
~~~
### 2. Create your build using CMake
- Its recomended to use "out-of-source" build (https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#out-of-source-build-trees) that means you should create a build folder next to the repository you cloned in the step one.
In this particular example its **Build**.

- From command line while you are in the **Source** directory.
~~~
$ cmake -S . -B ../Build
~~~

### 3. Build the project
You can find prepared project in the **Build** folder, where you can "make" it.
~~~
$ make
~~~

### 4. Create symlink to assets
In order to not copy all assets, create symlink in your built game to asset folder in the source.
While being in the **Build** folder, run:
~~~
$ ln -s "../Source/assets/" "./assets"
~~~

### 5. Run the game
You can run the **SDLGame** executable in the **Build** folder
~~~
SDLGameBuild$ ./SDLGame
~~~
