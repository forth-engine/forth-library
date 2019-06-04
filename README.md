# Forth-Library

> ⚠ WARNING: This library's current state is in heavy development ⚠

Forth Engine's Core C++ Library

Forth-Library is a Library to manipulate four dimensional objects via Cross Sectional Projection. My biggest motivation to create this library is to have my asset [Engine4](https://assetstore.unity.com/packages/tools/modeling/engine-4-34475) moving away from Unity. Creating one more degree of freedom to create higher-dimensional gaming to interested parties.

Forth-Library is part of higher project *Forth Engine*. Forth Engine is not just about displaying higher dimensional objects: There will be Physics, Scene Management, Import/Export, Graphics Patches, The Editor, etc.. It's future aim is to became a full blown Independent Game Engine.

This Library does not depends on any other library except STL (Standard Libraries) implemented in C++11. Currently it's tested only in MSVC 15 (Windows). More platform and compiler supports is on the way.

Playable demos is at [Forth-Engine/demo](https://github.com/forth-engine/demo)

# Prelease Feature Checklists

*Legend: ✅ Finished ⌛ Not implemented / WIP*

+ ⌛ Core Rendering Functions
  - ✅ Procedural Shape Generation Functions
  - ✅ Primitive Shapes (Procedural)
  - ✅ CrossSection (Flatland) Projection
  - ✅ Frustum (Perspective) Projection
  - ✅ (Basic) Shape Import/Export
  - ✅ OpenGL Integration
  - ⌛ Vertex Buffer (UV + Color)
+ ⌛ Graphic Features
  - ✅ Simplexes (Point+Line+Surface Rendering)
  - ✅ Culling Optimizations
  - ⌛ Particle System
  - ⌛ Line Trails
+ ⌛ Physics Module
  - ✅ Collisions and Rigidbody
  - ✅ Bounce and Frictions
  - ✅ Primitive Collider (Box + Sphere + Capsule)
  - ⌛ Convex Collider
  - ⌛ Static Concave Collider
  - ⌛ Kinematic Character Controller
  - ⌛ Joints/Effectors
+ ⌛ Scene Managements
+ ⌛ Documentation
+ ⌛ ...etc...

# LICENSE

[GPLv3](LICENSE). This could change in future but currently we want to motivate parties who like to do researches rather than those who do commercial.