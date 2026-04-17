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
    - Fedora: ```sudo dnf install cmake gcc g++ clang sdl2-compat-devel SDL2_image-devel```  
    - Ubuntu: ```sudo apt install cmake gcc g++ clang libsdl2-compat-dev libsdl2-image-dev git```  

2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
3. Compile: ```build/scripts/linux.sh```  

The executable can be found in build/export/linux.

### Cross-compiling for Windows
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake mingw64-gcc-c++ mingw64-sdl2-compat mingw64-SDL2_image```
    - Ubuntu: Not currently supported  

2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
3. Compile: ```build/scripts/windows.sh```  

The executable can be found in build/export/windows.

### For PSP
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake```
    - Ubuntu: ```sudo apt install git cmake```  

2. Install and configure the [PSPSDK](https://github.com/pspdev/pspsdk).
3. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
4. Compile: ```build/scripts/psp-pbp.sh```  

The executable can be found in build/export/psp-pbp. Custom firmware is required to run it.

## Building from Windows
Compiling from Windows is not supported. Consider using Fedora under WSL.

### AI disclosure
Various AI models were used for writing code. All AI-generated code was manually reviewed, tested, and edited. No assets or levels are AI-generated.