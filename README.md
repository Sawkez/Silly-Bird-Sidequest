# Silly Bird Sidequest
Silly Bird Sidequest is an official follow-up to [Silly Bird Sequel](https://store.steampowered.com/app/4269360/Silly_Bird_Sequel), built on a custom engine and expanded with mod and platform support.

## *Platform support
✅ - Working;  
❔ - Unattempted;  
⏹️ - Currently unsupported;  
❌ - Unsupported.  

|Platform            |Status |Note                                                 |
|:-------------------|:-----:|:----------------------------------------------------|
|Desktop             |       |                                                     |
|Windows x64         |✅     |                                                     |
|Windows x32         |✅     |Works on XP and up!                                  |
|Linux               |✅     |                                                     |
|Haiku OS            |✅     |                                                     |
|DOS                 |❔     |Seems scary                                          |
|FreeBSD             |❔     |Seems easy                                           |
|NetBSD              |❔     |Seems easy                                           |
|OpenBSD             |❔     |Seems easy                                           |
|QNX                 |❔     |                                                     |
|RISC OS             |❔     |                                                     |
|MacOS               |⏹️     |Don't have the hardware                              |
|Portable            |       |                                                     |
|Android             |✅     |                                                     |
|PlayStation Portable|✅     |Homebrew. Primary performance target. Easy to emulate|
|Nokia N-Gage        |❔     |Seems scary                                          |
|PlayStation Vita    |⏹️     |Don't have the hardware                              |
|iOS                 |⏹️     |Don't have the hardware                              |
|Nintendo 3DS        |⏹️     |Don't have the hardware                              |
|Nintendo Switch     |⏹️     |Don't have the hardware                              |
|Nintendo Switch 2   |⏹️     |Don't have the hardware                              |
|Home consoles       |       |                                                     |
|PlayStation 2       |⏹️     |Don't have the hardware                              |
|PlayStation 4       |⏹️     |Don't have the hardware                              |
|PlayStation 5       |⏹️     |Don't have the hardware                              |
|Xbox One            |⏹️     |Don't have the hardware                              |
|Xbox Series X\|S    |⏹️     |Don't have the hardware                              |
|Other               |       |                                                     |
|Web                 |✅     |Includes touch controls                              |
|tvOS                |⏹️     |Don't have the hardware                              |
|visionOS            |⏹️     |Don't have the hardware                              |

Platforms not listed are unsupported by SDL and as such cannot be easily ported to.

## Implemented
- All movement mechanics from *Silly Bird Sequel*;
- Collectible upgrades;
- Wallrunning;
- Save system;
- Touch controls;
- Custom skin support;

## WIP
- GUI;
- Dev console;
- Level editor for PC;

## Other planned features
### Gameplay
- All world objects from *Silly Bird Sequel*;
- Brand new world objects and obstacles;
- Brand new movement mechanics;
- Official campaigns;

### Modding
- Customizeable UI themes;
- Backgrounds;

### Tech
- Settings;

## Building from Linux
### For Linux
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake gcc g++ clang ninja-build SDL3-devel```  
    - Ubuntu: ```sudo apt install cmake gcc g++ clang ninja-build libsdl3-dev git```  

2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
3. Compile: ```build/auto.sh linux```  

The executable can be found in build/export/linux.

### For Windows
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake ninja-build mingw64-gcc-c++```
    - Ubuntu: ```sudo apt install cmake ninja-build gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 git```  

2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
3. Compile: ```build/auto.sh windows```  

The executable can be found in build/export/windows.

### For Web
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake ninja-build```
    - Ubuntu: ```sudo apt install cmake ninja-build git```

2. Install [Emscripten](https://emscripten.org)
3. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
4. Compile: ```build/auto.sh web```

The output can be found in build/export/web.

### For PSP
1. Install dependencies:
    - Fedora: ```sudo dnf install cmake ninja-build```
    - Ubuntu: ```sudo apt install cmake ninja-build git```  

2. Install and configure the [PSPSDK](https://github.com/pspdev/pspsdk).
3. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules && cd Silly-Bird-Sidequest```
4. Compile: ```build/auto.sh psp```  

The executable can be found in build/export/psp-pbp. Custom firmware is required to run it.

### For Android
1. Install [Android Studio](https://developer.android.com/studio)
2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules```
3. Open Silly-Bird-Sidequest/android-project in Android Studio
4. Click <img width="40" height="36" alt="image" src="https://github.com/user-attachments/assets/a08836ce-5f99-48d3-a0c3-946a865b327e" /> in the top-right corner and wait for the project to sync
5. Click <img width="40" height="36" alt="image" src="https://github.com/user-attachments/assets/04811722-c1d7-423d-875c-fab21ed3e341" /> in the top-right corner and wait for the project to build

The APK can be found in android-project/app/build/outputs/apk/debug.

## Building for Haiku OS
Cross-compiling for / from Haiku OS is not supported.
To build for Haiku OS you will need a Haiku OS installation on a PC or virtual machine.

1. Install dependencies: ```pkgman install cmake gcc ninja```
2. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules```
3. Compile: ```build/auto.sh haiku```

The executable can be found in build/export/haiku.

## Building from Windows
Compiling from Windows is not supported. Consider using WSL.

### AI disclosure
Various AI models were used for writing code. All AI-generated code was manually reviewed, tested, and edited. No assets or levels are AI-generated.
