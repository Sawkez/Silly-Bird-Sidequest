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
Start by cloning the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest.git --recurse-submodules```  

### For Linux
Install CMake, SDL2, SDL2_image and GCC.   
Compile: ```build/scripts/linux.sh```  
Run: ```build/export/linux/sidequest.sillybird```  

### For Windows
Install CMake, MinGW-w64, and the MinGW versions of SDL2, SDL2_image and GCC.  
Compile: ```build/scripts/windows.sh```  
Run: ```build\export\windows\sbsidequest.exe```

### For PSP
Install and configure the [PSPSDK](https://github.com/pspdev/pspsdk).  
Compile: ```build/scripts/psp-pbp.sh```  

To run:
- Install a custom firmware;  
- Connect PSP to PC via USB cable;  
- Copy ```build/export/psp-pbp``` to PSP/GAME in the PSP's storage.

## Building from Windows
Compiling from Windows is not supported. Consider using WSL.

### AI disclosure
Various AI models were used for writing code. All AI-generated code was manually reviewed, tested, and edited. No assets or levels are AI-generated.