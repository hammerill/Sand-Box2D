# Box2D Test with Objects
An organized (divided by classes separated per files) cross-platform (Windows, Linux, PS Vita) sample project of using Box2D with SDL2.

## Controls
### For Desktops
- <kbd>R</kbd> - reset box position.
- <kbd>WASD</kbd> or <kbd>Arrows</kbd> - move camera.
- <kbd>Q</kbd>/<kbd>E</kbd> - adjust zoom.
- <kbd>F</kbd> - go/exit fullscreen (may not work properly on Linux).
- <kbd>X</kbd> - delete marked objects (marking happens in the code).
Also, you can consider using mouse for camera controls and to go/exit fullscreen mode.

### For PS Vita
- <kbd>Cross</kbd> - reset box position.
- <kbd>Left Stick</kbd> or <kbd>DPAD</kbd> - move camera.
- <kbd>L</kbd>/<kbd>R</kbd> or <kbd>Right Stick</kbd> - adjust zoom.
- <kbd>Circle</kbd> - delete marked objects (marking happens in the code).

## Build 
### On Windows
1. Ensure you have all necessaire tools installed and wrote to your PATH variable (cmake, make, mingw (g++), git, etc.).
2. Install [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0) (put contents of SDL2-devel...mingw.zip/x86-64... somewhere). As well as [SDL_Image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.2), [SDL_Mixer](https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.6.2) (put contents to the same directory as for SDL2), [SDL2_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/) and [Box2D](https://github.com/erincatto/box2d). Or simply just unpack contents of this [archive](https://github.com/Hammerill/box2d-test-obj/releases/download/v1.0.0/packages-win64-sdl2-sdl2_image-sdl2_mixer-sdl2_gfx-box2d.zip) somewhere. Also, you have to add "bin" directory (located where the packages are installed) to your PATH variable, or copy its contents (*.dll) to the same directory where .exe file should be (build).
3. Clone repo:
  ```
  git clone https://github.com/Hammerill/box2d-test-obj && cd box2d-test-obj
  ```
4. Build (at the 4th line at the end change path to where you have installed packages or extracted an archive):
  ```
  mkdir build
  cd build
  cmake .. -G "MinGW Makefiles"
  cmake .. -DBUILD_WINDOWS_PKGS=C:/path/to/pkgs
  make
  ```
5. .exe file should appear, you can now launch this program:
  ```
  Box2D-test.exe
  ```

### On Linux
1. Install libraries SDL2, SDL2_image, SDL2_mixer, SDL2_gfx and Box2D somehow with your package manager.
2. Clone repo:
  ```
  git clone https://github.com/Hammerill/box2d-test-obj && cd box2d-test-obj
  ```
3. Build:
  ```
  mkdir build
  cd build
  cmake .. -DBUILD_WINDOWS=OFF -DBUILD_LINUX=ON
  make -j4
  ```
4. Executable should appear, you can now launch this program:
  ```
  ./Box2D-test
  ```

### For PS Vita
Build for PS Vita should be performed in Linux environment (or in MSYS2 in Windows, see step 1).
1. Install [VITASDK](https://vitasdk.org/).
2. Clone repo:
  ```
  git clone https://github.com/Hammerill/box2d-test-obj && cd box2d-test-obj
  ```
3. Build:
  ```
  mkdir build
  cd build
  cmake .. -DBUILD_WINDOWS=OFF -DBUILD_VITA=ON
  cmake ..
  make -j4
  ```
4. .vpk file should appear. Copy this file to your hacked PS Vita and install via VitaShell.
5. Bubble "Box2D OBJ" will appear, you can now launch this program.
