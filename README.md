![Logo](./pics/Logo-wide.jpg)
<h1 align="center">
  An open source cross-platform game based on Box2D with SDL2
</h1>

Sand-Box2D is a game where you can try out possibilities of the Box2D physics engine,
can [make your own levels](https://github.com/Hammerill/Sand-Box2D-levels) and share them with others.

This project was developed mainly for educational purposes.
I've started developing it for myself just to understand basis of Box2D, how to link it with SDL2 and etc. 
But then, I've realized it could be more than just some rewritten code: a game.
So, I took [this project](https://github.com/eder13/Box2D-SDL2-Demo) as the base,
have rewritten it from zero using OOP, and now you're free to play this game and use its code at way as you want.

<h2 align="center">
  Get it for your platform
</h2>
<center>
  <a href="#on-windows">
    <span style=" width: 75px;
                  height: 75px;
                  background-color: #CCC;
                  border-radius: 50%;
                  display: inline-block;
                  padding: 25px;
                  margin: 10px;"
          onmouseover="this.style.backgroundColor='#FFF'"
          onmouseout="this.style.backgroundColor='#CCC'">
      <img src="./pics/icons/windows.svg" width="75" height="75">
    </span>
  </a>
  <a href="#on-linux">
    <span style=" width: 75px;
                  height: 75px;
                  background-color: #CCC;
                  border-radius: 50%;
                  display: inline-block;
                  padding: 25px;
                  margin: 10px;"
          onmouseover="this.style.backgroundColor='#FFF'"
          onmouseout="this.style.backgroundColor='#CCC'">
      <img src="./pics/icons/linux.svg" width="75" height="75">
    </span>
  </a>
  <a href="#for-ps-vita-1">
    <span style=" width: 75px;
                  height: 75px;
                  background-color: #CCC;
                  border-radius: 50%;
                  display: inline-block;
                  padding: 25px;
                  margin: 10px;"
          onmouseover="this.style.backgroundColor='#FFF'"
          onmouseout="this.style.backgroundColor='#CCC'">
      <img src="./pics/icons/playstation.svg" width="75" height="75">
    </span>
  </a>
</center>

<hr>

## Contents
- [Controls](#controls)
  * [For Desktops](#for-desktops)
  * [For PS Vita](#for-ps-vita)
- [Future plans](#future-plans)
- [Future architecture](#future-architecture)
- [Build](#build)
  * [On Windows](#on-windows)
  * [On Linux](#on-linux)
  * [For PS Vita](#for-ps-vita-1)
- [Credits](#credits)

<hr>

## Controls
### For Desktops
- <kbd>WASD</kbd> and <kbd>Space</kbd> or <kbd>R</kbd> - actions (not implemented yet).
- <kbd>Arrows</kbd> - move camera.
- <kbd>Q</kbd>/<kbd>E</kbd> - adjust zoom.
- <kbd>Tab</kbd> - on/off debug screen.
- <kbd>F</kbd> - go/exit fullscreen (gonna be moved to options menu soon).
- <kbd>X</kbd> - reload current level (gonna be moved to options menu soon).
- Also, you can consider using mouse for camera controls and to go/exit fullscreen mode.

### For PS Vita
- <kbd>DPAD</kbd> and <kbd>Cross</kbd> - actions (not implemented yet).
- <kbd>Left Stick</kbd> - move camera.
- <kbd>Right Stick</kbd> or <kbd>L</kbd>/<kbd>R</kbd> - adjust zoom.
- <kbd>Triangle</kbd> - on/off debug screen.
- <kbd>Circle</kbd> - reload current level (gonna be moved to options menu soon).
- Also, you can consider using touchscreen for camera controls.

## Future plans
- ~~Add support for loading custom levels~~ ✅ Done!
- Make a main menu, where player can:
  * Download, manage and start levels;
  * Adjust settings;
  * Access menu "About".
- Implement level editor (right now to make levels you need to manually edit JSON).
- Add support of a lot of objects (joints included).

Feel free to propose ideas [here](https://github.com/Hammerill/Sand-Box2D/discussions)!

## Future architecture
![Arch](./pics/project-arch.drawio.svg)

## Build 
### On Windows
1. Ensure you have all necessaire tools installed and wrote to your PATH variable (cmake, make, mingw (g++), git, etc.).
2. Install packages:
- You need to install all those packages and unpack them in the same directory:
  * [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0)
  (put contents of SDL2-devel...mingw.zip/x86-64... somewhere, for all next packages use the same dir);
  * [SDL_Image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.2);
  * [SDL_Mixer](https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.6.2);
  * [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases);
  * [SDL2_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/);
  * [Box2D](https://github.com/erincatto/box2d);
  * [jsoncpp](https://github.com/open-source-parsers/jsoncpp);
  * [libcurl](https://github.com/curl/curl).
- Or simply just unpack contents of [this](https://github.com/Hammerill/Sand-Box2D/releases/download/v1.0.0/win64-packages.zip) archive somewhere. 
- Also, you have to add "bin" directory (located where the packages are installed) to your PATH variable,
or copy its contents (*.dll) to the same directory where .exe file should be (build).
3. Clone repo:
  ```cmd
  git clone https://github.com/Hammerill/Sand-Box2D && cd Sand-Box2D
  ```
4. Build (at the 4th line at the end change path to where you have installed packages or extracted an archive):
  ```cmd
  mkdir build
  cd build
  cmake .. -G "MinGW Makefiles"
  cmake .. -DBUILD_WINDOWS_PKGS=C:/path/to/pkgs
  make
  ```
5. .exe file should appear, you can now launch this game:
  ```cmd
  Sand-Box2D.exe
  ```

### On Linux
1. Install following libraries somehow with your package manager:
- SDL2;
- SDL2_image;
- SDL2_mixer;
- SDL2_ttf;
- SDL2_gfx;
- Box2D;
- jsoncpp;
- libcurl.
2. Clone repo:
  ```bash
  git clone https://github.com/Hammerill/Sand-Box2D && cd Sand-Box2D
  ```
3. Build:
  ```bash
  mkdir build
  cd build
  cmake .. -DBUILD_WINDOWS=OFF -DBUILD_LINUX=ON
  make -j4
  ```
4. Executable should appear, you can now launch this game:
  ```bash
  ./Sand-Box2D
  ```

### For PS Vita
Build for PS Vita should be performed in Linux environment (or in MSYS2 in Windows, see step 1).

Also, you can consider auto build system provided by GitHub [actions](https://github.com/Hammerill/Sand-Box2D/actions).

1. Install [VITASDK](https://vitasdk.org/).
2. Clone repo:
  ```bash
  git clone https://github.com/Hammerill/Sand-Box2D && cd Sand-Box2D
  ```
3. Build:
  ```bash
  mkdir build
  cd build
  cmake .. -DBUILD_WINDOWS=OFF -DBUILD_VITA=ON
  make -j4
  ```
4. .vpk file should appear. Copy this file to your hacked PS Vita and install via VitaShell.
5. Bubble "Sand-Box2D" will appear, you can now launch this game.

## Credits
- [eder13](https://github.com/eder13) - base project (Box2D-SDL2-Demo) author.
- [codeman38](https://www.fontspace.com/codeman38) - main font (PressStart2P) author.
- [Hammerill](https://github.com/Hammerill) - author of this game.
- To everybody who contributes to open source, to those who made these (used in the project) libraries exist.

Translations:
- English - Hammerill
- Russian and Ukrainian are pending - Hammerill
