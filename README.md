# ChromaEngine
**ChromaEngine** is a real-time 3D rendering engine written in C++ with OpenGL, featuring a physically-based rendering pipeline, voxel-based global illumination, skeletal animation with state machines, and an ImGui-based scene editor with JSON serialisation.

Built as a personal deep-dive into real-time graphics and engine architecture — as a platform for understanding how renderers work under the hood.

[![Voxel GI](https://github.com/ChrisTwaitees/ChromaEngine/raw/ChromaEditor/Chroma/Chroma/resources/lookdev/voxel_gi_gif_larger.gif)](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/lookdev/voxel_gi_gif_larger.gif)

---

## Rendering Pipeline
The renderer supports both deferred and forward rendering paths, selecting per-object based on material properties (transparency, special effects). The deferred path handles the majority of opaque PBR geometry; the forward path handles transparency with depth-sorted draw calls.

[![PBR Rendering](https://static.wixstatic.com/media/755aac_316019612db440d9a17f566fe23a1654~mv2.gif)](https://static.wixstatic.com/media/755aac_316019612db440d9a17f566fe23a1654~mv2.gif)

### Global Illumination
 
- **Voxel-based Global Illumination (Experimental)** — scene geometry is voxelised into a 3D texture and used for cone-traced indirect diffuse and specular. Provides single-bounce GI with dynamic light bleeding and occlusion.

[![Voxel GI Blend](https://github.com/ChrisTwaitees/ChromaEngine/raw/ChromaEditor/Chroma/Chroma/resources/lookdev/voxel_gi_gif_blend.gif)](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/lookdev/voxel_gi_gif_blend.gif)

### Shading & Lighting
 
- **PBR shading** — Cook-Torrance BRDF with metallic/roughness workflow
- **Image-Based Lighting (IBL)** — HDR environment maps with split-sum approximation for specular
- **Screen-Space Reflections (SSR)**
- **Screen-Space Ambient Occlusion (SSAO)**
- Transparent geometry rendering with depth sorting
- Debug draw overlay (AABBs, normals, voxel visualisation)

## Animation System
 
[![Character Controller](https://github.com/ChrisTwaitees/ChromaEngine/raw/ChromaEditor/Chroma/Chroma/resources/lookdev/character_controller_physics_smaller_still.gif)](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/lookdev/character_controller_physics_smaller_still.gif)
 
- **GPU skeletal animation** — bone transforms uploaded as shader uniforms, skinning computed in vertex shader
- **Animation state machine** — state graph with transition conditions, blending between clips
- Bipedal third-person character and camera controller as a reference implementation
 
---
 
## Physics
 
- **Bullet Physics integration** — rigid body dynamics, collision detection, character controller
- Physics components attached to scene entities via the component system
 
---
 
## Editor & Architecture
 
[![Editor](https://github.com/ChrisTwaitees/ChromaEngine/raw/ChromaEditor/Chroma/Chroma/resources/textures/editor/Editor_00.PNG)](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/textures/editor/Editor_00.PNG)
 
- **ImGui-based editor** — entity selection, component inspection, renderer debug panels
- **Scene serialisation** — entities and components serialised/deserialised via RapidJSON
- **Entity-component architecture** — scene objects composed from discrete, typed components
- **Multithreaded resource loading** — assets loaded off the main thread to avoid stalls
 
---
 
## Tech Stack
 
| Area | Technology |
|---|---|
| Language | C++14 |
| Graphics API | OpenGL |
| Windowing | GLFW |
| UI | Dear ImGui |
| Physics | Bullet Physics |
| Serialisation | RapidJSON |
| Build | Visual Studio / Windows |
 
---
 
## Devlogs
 
Development process documented on [YouTube](https://www.youtube.com/watch?v=YeyiEYRT1Ac).
 
---
 
