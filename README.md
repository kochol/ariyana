# Ariyana
Ariana Game Engine

# Changes

## Ariyana Game Engine 0.3

Adding gltf mesh loader

## Ariyana Game Engine 0.2.2

- Updated to work with the latest sokol gfx.

## Ariyana Game Engine 0.2.1

- Fix an important memory corruption bug.
- Add GetLastRpcClientIndex to find out which client called the function.

## Ariyana Game Engine 0.2

The main changes for this version are adding networking.

- Auto serialize and deserialize of entities and their components.
- Sync entities with clients automatically
- Automatic property replication
- RPC support with auto parameter serializing and deserializing.
- Add multi-window rendering support.

## Ariyana Game Engine 0.1

The first release of the engine

- Simple Orthodox C++ coding style and APIs
- Initial code for entity and core systems
- Cross-platform not only the code but also the textures and shaders will be converted to the - - - target platform on the build.
- Async file I/O. Can pack assets inside the APK and load them from APK at runtime.
- Texture conversion and loading.
- Dear-ImGUI support.
