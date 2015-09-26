OpenGL Game Engine
================

An OpenGL Game Engine made by computer science students in partnership with AMD

Work is still in progress and many things have to be implemented, improved or fixed :D

- C++11, OpenGL 4.1
- Entity component system based architecture
- Multi-threaded execution pipeline (Gameplay, culling, pre-render, rendering)
- Bullet Physics Library integration
- Tile based deferred rendering
- Assets conversion pipeline (.obj, .fbx)
- Game object serialization
- Mesh skinning (not ready)

Video :
[![Video](http://img.youtube.com/vi/ebwDNA2wz-k/0.jpg)](http://www.youtube.com/watch?v=ebwDNA2wz-k)

Demo .exe : https://www.dropbox.com/s/dot6ecqo59i1prz/AGE-V0.0-demo.7z

This program is released under the terms of the license contained in the file LICENSE.txt


==========================================

## Installation :

1 ) In git bash run : GenerateProjects.bat

2 ) In git bash run : GenerateSolution.bat

3 ) Install Google drive Desktop

4 ) Get Vendors and Assets on google drive :

- https://drive.google.com/folderview?id=0ByluCL7s-tHGT19YMWRadUtjWWc&usp=sharing
- https://drive.google.com/folderview?id=0ByluCL7s-tHGeU8wUTROMUhrYUE&usp=sharing

5 ) In cmd.exe go to AGE/ and run :

- `mklink /J Vendors "{Path_to_your_Google Drive\Vendors"`
- `mklink /J Datas/Assets "{Path_to_your_Google Drive\Assets"`

Generated solutions will be found in Projects folder.
