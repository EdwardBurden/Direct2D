# Goal
### Basic(Transformm Only)
- Create a working example of a simple UI framework that mimics unitys built in rect transform.
- Users will load the application and see a layout already defined. 
- When resizeing the window the application should respond.
- Users can click on one UI element at a time and see a debug side panel with the transform information.
- Gizmos are draw on the selected object to show Anchors and Pivot

### Advanced (Interactions)
- A selection of different interactable objects are shown to the user.
- These inculde text input fields, sliders, buttons and moving elements.
- Objects can also respond to the mouse hovering over.
- Application layer can have custom interactions hooked up to the elements in addtionon to their standard behaviour.

### Big Goal
- Load And draw textures with shaders to add better visuals.



# 2D UI Renderer (Direct2D) Todo

## Decisions to make first
- [ ] Render target: `HwndRenderTarget` (simple, limited) or DXGI swap chain + `DeviceContext` (layers, effects, dirty rects)
- [ ] Layout: automatic (containers arrange children) or manual positions
- [ ] Transforms: do children inherit the parent's transform, or is it independent
- [ ] Rotation scope: leaf elements only, or containers too (rotated clipping needs layers)
- [ ] DPI: support scaling and multi-monitor now, or fixed 96 DPI for now
- [ ] Loop style: idle-when-clean (blocking message loop) or always running
- [ ] Pointer: single mouse only, or touch/multi-pointer later
- [ ] Event bubbling: yes or no (fine to say "no" for now)

---

## 0. Resource lifetime and invalidation
- [ ] Separate device-independent resources (factories, text formats, text layouts, geometries) from device-dependent ones (render target, brushes, bitmaps)
- [ ] Detect device loss (`D2DERR_RECREATE_TARGET` from `EndDraw`) and recreate device resources
- [ ] Treat "recreate device resources" as its own dirty state
- [ ] Use a shared brush (set colour before each draw) instead of one brush per element
- [ ] Handle `WM_DPICHANGED`: mark whole tree layout-dirty and invalidate cached text layouts

## 1. Dirty flagging
- [ ] Add a `dirty` flag per node
- [ ] Propagate layout-dirty upward to ancestors, stopping early if an ancestor is already dirty
- [ ] Setters mark dirty automatically (`set_text`, `set_size`, `add_child`, etc.); no public fields mutated directly
- [ ] Window resize: resize target/swap chain buffers before next draw, and mark root layout-dirty
- [ ] Fixed frame order: input, update, layout (if dirty), render
- [ ] Layout never marks anything dirty; cap iterations if multi-pass is allowed
- [ ] Clear flags at the correct level (parent and children consistently)
- [ ] Global "anything dirty" and "anything animating" flags
- [ ] When clean and not animating, block on messages instead of spinning
- [ ] Later, if profiling says so: split into tree / layout / render dirty
- [ ] Later, optional: partial redraw with dirty rects (`Present1`), not needed yet

## 2. Time-based update loop
- [ ] Compute `dt` with `QueryPerformanceCounter`
- [ ] Clamp `dt` (about 0.1s), especially because Win32 move/size loops stall the app
- [ ] Add `update(dt)` step and per-element update hook
- [ ] Present with vsync so the loop is throttled
- [ ] Add time scale / pause value for debugging
- [ ] Transform model: position, rotation, scale, pivot (choose centre or top-left default)
- [ ] Compose parent and child 3x2 matrices down the tree
- [ ] Rotation about pivot = translate, rotate, translate back
- [ ] Keep transform render-only where possible so animation doesn't trigger layout
- [ ] Use a separate visual offset/transform on top of laid-out position so layout doesn't overwrite animation
- [ ] Later: tween/animation list for easing and sequences (skip Windows Animation Manager)

## 3. Interaction: hover, click, drag
- [ ] Hit testing: topmost element under pointer (iterate children in reverse draw order)
- [ ] Hit testing with rotation: inverse world matrix applied to the mouse point
- [ ] Allow elements to opt out of hit testing (pass-through)
- [ ] Convert mouse pixels to DIPs using current DPI before hit testing
- [ ] Feed `WM_MOUSEMOVE`, `WM_LBUTTONDOWN`, `WM_LBUTTONUP` into one pointer state machine
- [ ] Hover: track hovered element at app level, fire enter/exit when it changes
- [ ] Hover exit on leaving window: `TrackMouseEvent` and `WM_MOUSELEAVE`
- [ ] Click: press and release on the same element
- [ ] Drag: remember pressed element and start position, start drag after a few-pixel threshold
- [ ] Drag events: drag-start, drag-move (with delta), drag-end
- [ ] Capture: `SetCapture` on press, `ReleaseCapture` on release, handle `WM_CAPTURECHANGED` to cancel cleanly
- [ ] Decide whether a draggable element still fires click if it wasn't moved
- [ ] Element states: normal, hovered, pressed, dragging (state change marks render-dirty)
- [ ] Re-run hit testing after update and layout, not only on mouse move (element moving under a stationary pointer)
- [ ] Cursor change on hover via `WM_SETCURSOR`
- [ ] Simple behaviours to validate: hover changes colour, click fires callback, drag moves element by pointer delta

## 4. Text options (DirectWrite)
- [ ] Single `TextStyle` struct owned by the text element (defaults, copying, sharing)
- [ ] Any style change marks dirty (layout-dirty for font/size/wrap/overflow/autosize/alignment, render-dirty for colour)
- [ ] **Font**: family and size live on the text format; changing means recreating the format and invalidating cached measurement
- [ ] **Colour**: brush colour at draw time, render-only
- [ ] **Wrapping**: none, word, character, emergency break (breaks a word only if it can't fit a line)
- [ ] **Overflow**: visible (no clip), clip (axis-aligned clip rect; rotated needs a layer with geometry mask), ellipsis (trimming with ellipsis sign, choose character vs word granularity)
- [ ] **Autosize**: build text layout with available width, read metrics, size the box from them
- [ ] Autosize flavours: shrink-to-fit width (single line), fixed width and grow height (wrapped)
- [ ] Autosize + wrapping needs a max width or it never wraps
- [ ] Overflow only matters when the box is not autosizing
- [ ] Layout order: available width, wrap, measure, resolve size, apply overflow at draw time
- [ ] Cache one text layout per element; rebuild only when text, font, or available width changes
- [ ] Add horizontal and paragraph (vertical) alignment (built into the format, nearly free)
- [ ] Later, separate mode: shrink-font-to-fit (binary search on font size against metrics, cache the result)

---

## Suggested order
1. [ ] Decide render target type and resource lifetime handling (section 0)
2. [ ] Dirty system
3. [ ] Time loop and transforms
4. [ ] Interaction (hover, then click, then drag)
5. [ ] Text options, built on cached text layouts