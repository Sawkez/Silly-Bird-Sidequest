# Silly Bird Sidequest
Silly Bird Sidequest is a fast-paced precision 2D platformer for PC and the PSP, with custom level support.

## Controls
- WASD / D-Pad / Left joystick - Move;
- Space / A / Cross - Jump / Dash;
- Left shift / LB / L - Dive / Slide;
- Q / B / Circle - Pan camera (WIP);

## Implemented
- All movement mechanics from *Silly Bird Sequel*;
- Collectible upgrades;
- Wallrunning;
- Loading custom levels;
- Save system;

## WIP
- GUI;
- Level editor for PC;

## Next up
- Visual effects;
- Campaign intro;

## Other planned features
### Gameplay
- All world objects from *Silly Bird Sequel*;
- Brand new world objects and obstacles;
- Brand new movement mechanics;
- Official campaigns;

### Modding
- Custom skin support;
- Customizeable UI themes;
- Backgrounds;

### Tech
- Settings;
- Android port;

## Building from Linux
### For Linux
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake gcc g++ clang ninja-build sdl3-devel```  
    - Ubuntu: ```sudo apt install cmake gcc g++ clang ninja-build libsdl3-dev git```  

2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
3. Compile: ```build/auto.sh linux```  

The executable can be found in build/export/linux.

### Cross-compiling for Windows
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake ninja-build mingw64-gcc-c++```
    - Ubuntu: ```sudo apt install cmake ninja-build gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 git```  

2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
3. Compile: ```build/auto.sh windows```  

The executable can be found in build/export/windows.

### For PSP
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake ninja-build```
    - Ubuntu: ```sudo apt install cmake ninja-build git```  

2. Install and configure the [PSPSDK](https://github.com/pspdev/pspsdk).
3. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
4. Compile: ```build/auto.sh psp```  

The executable can be found in build/export/psp-pbp. Custom firmware is required to run it.

## Building from Windows
Compiling from Windows is not supported. Consider using WSL.

### AI disclosure
Various AI models were used for writing code. All AI-generated code was manually reviewed, tested, and edited. No assets or levels are AI-generated.