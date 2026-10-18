# Direct2D

A small experiment in building a 2D UI framework from scratch on top of **Direct2D** and **DirectWrite**, using plain Win32 for windowing and input. The layout model deliberately mimics Unity's `RectTransform`: nodes are positioned with anchors, a pivot, an anchored offset and a size delta, so the UI responds naturally when the window is resized.

> Work in progress. See [Notes.md](Notes.md) for goals and the roadmap.

## Features

- **Unity-style `RectTransform`**: anchors (min/max as a rect), pivot, anchored offset and size delta, resolved against the parent's rect.
- **Scene graph**: a tree of `Node`s, each with an optional `Graphic` (drawing) and `Collider` (hit testing).
- **Responsive layout**: the whole tree is recalculated against the client area each frame, so resizing the window reflows the UI.
- **Graphics**: filled rectangles, circles and wrapped DirectWrite text.
- **Hit testing**: clicks are tested against colliders, topmost (last drawn) node first, with pixel to DIP conversion for DPI awareness.
- **Debug panel**: clicking a node opens a side panel on the right showing that node's hierarchy path from the root.
- **DPI aware**: ships with a manifest enabling DPI awareness, and the render target uses the window's DPI.

## Requirements

- Windows (Win32, Direct2D and DirectWrite are Windows-only)
- CMake 3.10 or newer
- A C++20 compiler: MSVC (Visual Studio 2022) or clang-cl

## Building

### Visual Studio

Open the folder in Visual Studio 2022 (CMake support). `CMakeSettings.json` provides these configurations:

- `x64-Debug` / `x64-Release`
- `x86-Debug` / `x86-Release`
- `x64-Clang-Debug` / `x64-Clang-Release` (Ninja with clang-cl)

### Command line

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The executable is named `2DWINDOWS`. `d2d1` and `dwrite` are linked via `#pragma comment(lib, ...)`, and with MSVC the `partial.manifest` is embedded into the binary.

## Usage

Run the executable. You will see a demo layout made of nested panels, circular buttons and a block of wrapping text. Then:

- **Resize the window** to watch the anchors and offsets reflow the layout and re-wrap the text.
- **Click an element** to select it. A green debug panel appears on the right listing the selected node's chain of parents.
- **Click empty space** (or anything without a collider) to dismiss the panel.

## Project structure

```
.
├── CMakeLists.txt        Build definition (C++20, WIN32 executable)
├── CMakeSettings.json    Visual Studio CMake configurations
├── partial.manifest      Enables DPI awareness
├── Notes.md              Goals, design decisions and roadmap
├── includes/
│   ├── App.h
│   ├── UI/
│   │   ├── RectTransform.h   Anchor / pivot / offset / size layout maths
│   │   ├── Node.h            Scene graph node and Collider
│   │   ├── Graphic.h         Square, circle and text graphics
│   │   ├── Scene.h           Tree layout and hit-test search
│   │   ├── Renderer.h        Direct2D / DirectWrite renderer
│   │   ├── RenderContext.h   Shared factory, render target, brush, text format
│   │   └── Utils.h           Helpers for building panels, buttons and text
│   └── Window/
│       ├── BaseWindow.h      Generic Win32 window wrapper
│       ├── MainWindow.h      Main window and input events
│       └── WindowsUtils.h
└── src/
    ├── main.cpp          wWinMain and the main loop
    ├── App.cpp           Demo scene, input handling, render loop, debug panel
    ├── UI/               Scene and Renderer implementations
    └── Window/           MainWindow implementation
```

## How it works

1. **Window**: `MainWindow` wraps a Win32 window and surfaces resize, click, drag and close events as callbacks.
2. **Layout**: `Scene::RecalculateTree` walks the tree breadth first, calling `RectTransform::CalculateRect` with each node's parent rect.
3. **Render**: `Renderer` creates an `HwndRenderTarget`, clears the frame, then draws each node's `Graphic` in breadth-first order.
4. **Input**: `Scene::SearchTreeForCollision` searches children in reverse draw order and returns the topmost node whose collider contains the point.

## Roadmap

Planned work, tracked in [Notes.md](Notes.md):

- Selection gizmos showing anchors and pivot
- Dirty flagging and an idle-when-clean loop
- Time-based update loop with transforms and rotation
- Hover, click and drag interaction, plus sliders, text inputs and buttons
- Text options: wrapping, overflow, autosize and alignment
- Device-loss handling and DPI change support
- Texture loading and shaders for richer visuals

## License

Released under the [MIT License](LICENSE).
