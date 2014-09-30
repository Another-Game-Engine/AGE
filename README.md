OpenGL Game Engine
================

An OpenGL Game Engine made by computer science students in partnership with AMD

Work is still in progress and many things have to be implemented, improved or fixed :D

We are very far away to have a complete engine :D

VIdeo :
[![Video](http://img.youtube.com/vi/ebwDNA2wz-k/0.jpg)](http://www.youtube.com/watch?v=ebwDNA2wz-k)

Demo .exe : https://www.dropbox.com/s/dot6ecqo59i1prz/AGE-V0.0-demo.7z

- C++11, OpenGL 4.1
- Entity component system based architecture
- Multi-threaded execution pipeline (Gameplay, culling, pre-render, rendering)
- Bullet Physics Library integration
- Flexible scene organization and possibility to run multiple scenes at the same time
- Tile based forward rendering (no implemented yet)
- Assets conversion pipeline and assets edition at runtime (only .obj, .mtl, .tga supported at the moment, animations are not implemented yet)
- Entities and component serialization (using [Cereal](https://github.com/USCiLab/cereal) )
- Raycast (basic implementation for FP raycast)
- 3D sound (basic implemetation using FMOD)
- Post FX