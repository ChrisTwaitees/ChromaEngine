# ChromaEngine
ChromaEngine is a real-time 3D rendering engine written in C++ with OpenGL and Vulkan interfaces, featuring a physically-based rendering pipeline, voxel-based global illumination, skeletal animation with state machines, and an ImGui-based scene editor with JSON serialisation.
[Youtube Devlogs](https://www.youtube.com/watch?v=YeyiEYRT1Ac)

## Rendering
![Alt text](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/lookdev/voxel_gi_gif_larger.gif)
* Voxel Based Global Illumination.
* PBR Shading.
* IBL HDR lighting.
* SSR
* SSAO
* Transparent Rendering and Sorting.
* Deffered and Forward Rendering Pipelines.
* Debug drawing.
![Alt text](https://static.wixstatic.com/media/755aac_316019612db440d9a17f566fe23a1654~mv2.gif)

## Features
![Alt text](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/lookdev/character_controller_physics_smaller_still.gif)
  * Bullet Physics.
  * Skeletal Animation and basic state machine. 
  * Bipedal Third Person Character and Camera Controller.
    
## Editor
![](Chroma/Chroma/resources/textures/editor/Editor_00.PNG)
![Alt text](https://github.com/ChrisTwaitees/ChromaEngine/blob/ChromaEditor/Chroma/Chroma/resources/lookdev/voxel_gi_gif_blend.gif)
  * Serialization and deserialization of scene objects using rapidJson. 
  * Entity selection and property serialization. 
  * Multithreaded resource loading. 
