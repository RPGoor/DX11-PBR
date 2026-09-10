# DirectX 11 Grass and Procedural Terrain Renderer

A **DirectX 11** real-time rendering project written in **C++**, focused on procedural terrain generation and dense instanced grass rendering inspired by **Ghost of Tsushima**.

The project explores GPU-driven rendering techniques, procedural content generation, physically-based lighting, and real-time optimisation.

## Features

### Grass Rendering
* Instanced grass rendering
* GPU indirect drawing
* Procedural grass placement
* Animated wind deformation
* View-dependent blade thickness
* Rounded Normals
* Runtime grass controls through ImGui
* Terrain aware grass placement

### Procedural Terrain
* Procedural terrain generation
* PCG value-noise 
* Compute shader generated hieght and normal maps

### Rendering
* Physically-based rendering
* Cook-Torrance microfacet specular reflectance model
* Lambertian diffuse lighting
* Directional lighting
* Chunking, frust and distance culling
* HLSL vertex, pixel, and compute shaders

### Engine
* DirectX 11 rendering backend
* GPU resource and bindable system
* Instanced rendering support
* Indirect draw rendering supprot
* Free-look camera
* ImGui debugging and runtime controls

## Technologies
* C++20
* HLSL
* DirectX 11
* Win32 API
* Dear ImGui
* Assimp

## Requirements
* Visual Studio
* Windows SDK
* Direct3D 11
* Windows 10 or later

## Controls
ESC - Toggle mouse/camera
WASD - Move Camera 
R — Move camera up
F — Move camera down
Mouse — Look around

## License

This project is licensed under the MIT License.

