# Client_Optimization_Metin2

The main classes of this system are presented here in a simplified way. Keep in mind that this IS JUST THE MAIN CLASSES requires a lot more work to get this to work.
!!c++17 needed!!

Features:
No loading bar, so that means the loading in game is instantly after selecting the character.
Effects, sounds, gr2 models, maps, skills, mobs, motions, are now read only one time, when the client start (runtime). The default method is loading them at each warp.
Processor now is much optimized at loading, the new method just warp you in the specific coordinates x, y, without loading anything in background.
playerSettingsModule.py (cca. 2000 lines) removed, everything is written in C++ and changed totally the struct for a better customization, that means you can add easily new effects, sounds, emotions, characters, motions, skills and others.
