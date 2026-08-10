# Task 1 — Windowed Object Tree with Draw & Click Handling

## Goal
Set up a project with a main window that owns a **tree of sub-objects**. Get the
tree structure working end-to-end for two things:

1. **Sorting draw calls** — walking the tree renders objects in the correct order.
2. **Routing clicks** — a mouse click resolves to the correct object in the tree.

This is a structural pass, not a polish pass — it doesn't need to be perfect,
just functional.

## Requirements

- **Objects are class instances.** Different object types are different C++
  classes (e.g. a panel, a button, a text block), each implementing a common
  `draw()` function.
- **Position and size are set at construction.** Each object receives its
  position and size when it's created — no separate layout pass for this task.
- **Some objects have an `onClick` function.** Not all objects need to be
  clickable — only the ones that should respond to input.
- **`main` wires everything together:**
  - Creates the window.
  - Creates each object instance and builds the tree.
  - Assigns the click callbacks to the objects that need them.

## Explicit constraints

- **No XML / external layout file for this task.** The tree is built directly
  in C++ code. File-driven layout is a later task — not now.

## Out of scope (for this task)

- Layout engine / auto-sizing (positions and sizes are just passed in directly).
- Event bubbling, capture, or routed events.
- Hover states or visual feedback.
- Markup parsing of any kind.

JUST USE NORMAL DIRECT2D DRAW COMMANDS, NO SHADERS YET