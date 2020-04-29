# Ariyana

Ariyana Game Engine

# How To Build

We are using [fips](https://github.com/floooh/fips) as a highlevel build system for Ariyana.

### Requirements

- **Python** (2.7.x or 3.x)
- **CMake** (3.x)
- **A working C/C++ development environment**:
    - on **OSX**: Xcode + command line tools
    - on **Linux**: make/gcc (or clang)
    - on **Windows**: Visual Studio 2017 or better

### Cloning the repository

As [fips](https://github.com/floooh/fips) clone all the dependencies during building, 
it's better to create a Workspace directory (e.g. "ari-workspace") and clone the repository in it:

```bash
> cd ~/your-workspace
> git clone https://github.com/kochol/ariyana.git
```

### Build the engine

Here is the easiest part, run ```./fips build``` in ariyana directory (```fips build``` in windows):

```bash
> cd your-workspace/ariyana   
> ./fips build
```

now ```./fips open``` will open it up in your C/C++ development environment:

```bash   
> ./fips open
```

done! it's ready to rock.

# Changes

## Ariyana Game Engine 0.3

- Adding gltf mesh loader
  - Load meshes and nodes
  - Supports sparse buffers
  - Load textures
  - Support vertex colors
- Add input handling in ImGUI
- Add some basic default shaders

### Ariyana Game Engine 0.2.5

- Fix a bug in android file path

### Ariyana Game Engine 0.2.4

- Compile fix for linux

### Ariyana Game Engine 0.2.3

- Compile fix for android

### Ariyana Game Engine 0.2.2

- Updated to work with the latest sokol gfx.

### Ariyana Game Engine 0.2.1

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
