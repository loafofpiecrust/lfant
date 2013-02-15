## ShadowFox Engine
===================
A so-far small OpenGL game engine. It uses an Entity-Component model, somewhat similar Unity3D, where Entities have a list of Components that they can access at any given time. These components "add" functionality to the owner Entity (albeit not directly). Examples of Components are Rigidbody, Inventory, MeshRenderer, ParticleSystem, etc. This allows the easy manipulation of what Entities can do at runtime.

External Library Usage:
 - Bullet Physics
 - Boost (portions)
 - GLEW
 - GLFW
 - GLM
 - Squirrel (Scripting Language)
 - TinyXML2
 - CEGUI

## Installation
===============
ShadowFox Engine uses primarily CMake for building. 


