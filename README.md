# Silly Bird Sidequest
Silly Bird Sidequest is a fast-paced precision 2D-platformer for PC and the PSP, with custom level support.

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
idk. try wsl

### AI disclosure
Various AI models were used for writing code. All AI-generated code was manually reviewed, tested, and edited. No assets or levels are AI-generated.