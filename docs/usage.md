# Usage Guide

This guide covers everything you need to know to use EthoWild for behavior labeling.

---

## Opening Videos

### Single Video

1. Go to **File → Open Video...**
2. Select a video file (supported formats: `.mp4`, `.avi`, `.mkv`, `.mov`, `.wmv`)
3. The video will load and begin playing automatically

### Video Directory

For batch processing multiple videos:

1. Go to **File → Open Video Directory...**
2. Select a folder containing video files
3. The first video in the directory loads automatically
4. Use the **⏮** and **⏭** buttons to navigate between videos

!!! info "Directory Order"
    Videos are sorted alphabetically by filename. When you reach the last video and press Next, it wraps around to the first video.

---

## Video Controls

### Playback

| Control | Action |
|---------|--------|
| **▶ / ⏸** | Toggle play/pause |
| **⏮** | Previous video (directory mode) |
| **⏭** | Next video (directory mode) |
| **Seek slider** | Drag to jump to any position |
| **Speed dropdown** | Adjust playback speed (0.5x, 0.75x, 1.0x, 1.25x, 1.5x, 2.0x) |

### Time Display

The time label shows the current position and total duration in `MM:SS / MM:SS` format.

---

## Navigation and Zoom

### Panning

Click and drag anywhere on the video to pan around. This is especially useful when zoomed in.

### Zooming

Hold **Ctrl** and scroll the mouse wheel to zoom in and out.

- **Ctrl + Scroll Up** → Zoom in
- **Ctrl + Scroll Down** → Zoom out

The zoom is anchored to the mouse cursor position, so you can zoom directly into the area you're observing.

!!! tip "Reset View"
    If you get lost while zoomed in, resize the window or reload the video to reset the view to fit the frame.

---

## Session Parameters

Before labeling behaviors, configure the session parameters in the **Controls** dock (right side of the window).

| Parameter | Description |
|-----------|-------------|
| **Tag** | Free-text identifier for the current observation session |
| **Role** | The role of the observed subject (e.g., mother, calf, escort) |
| **Group Type** | Whether observing an individual or group |
| **Sex** | Sex of the observed subject |
| **Stage** | Life stage (calf, juvenile, adult) |
| **Group Size** | Number of individuals in the group (0 = N/A) |
| **Mother & Calves** | Number of mother-calf pairs |
| **Calves** | Number of calves observed |
| **Observations** | Free-text field for additional notes |

These parameters are attached to every behavior record you create.

---

## Labeling Behaviors

The **Behaviors** dock (right side) shows a tree of all available behaviors organized by category.

### Recording an Event

1. Navigate to the moment in the video where the behavior occurs
2. Set the appropriate session parameters
3. **Double-click** the behavior in the tree
4. The event is instantly recorded at the current timestamp

### Recording a State

States have a start and end time:

1. Navigate to where the behavior **begins**
2. **Double-click** the state behavior → Recording starts
3. A green indicator appears: `🔄 Active: [Behavior Name]`
4. All other behaviors are disabled while a state is active
5. Navigate to where the behavior **ends**
6. **Double-click** the same behavior again → Recording ends
7. The state is saved with start time, end time, and calculated duration

!!! warning "Active State"
    You must end the current state before recording any other behavior. Double-click the active state behavior to end it.

### Visual Indicators

In the behavior tree:

- **Blue text** in the Type column indicates an EVENT
- **Green text** in the Type column indicates a STATE
- **Grayed out** behaviors are disabled (during an active state)

---

## Viewing Records

The **Records** dock (bottom of the window) shows all labeled behaviors for the current video.

| Column | Content |
|--------|---------|
| **Time** | Start time, or start - end for states |
| **Behavior** | Category / Behavior name |
| **Type** | EVENT or STATE |
| **🗑** | Delete button |

Hover over a row to see a tooltip with full record details including all session parameters.

### Deleting Records

Click the **🗑** button in any row to delete that record. This action cannot be undone.

---

## Exporting Records

### Save to CSV

1. Go to **File → Save Records...** or click the **💾 Save Records** button
2. Choose a location and filename
3. Records are exported in CSV format

The CSV includes all fields:

- Session number
- Role, behavior, parent category
- Start time, end time, duration
- Record type (EVENT/STATE)
- Tag, group type, sex, stage
- Group size, mother & calves, calves
- Observations

### Auto-suggested Filename

When working with a video directory, EthoWild suggests a filename based on the current video name with a unique suffix to prevent overwriting.

!!! note "Records Cleared After Save"
    After successfully saving, all records are cleared from the current session. This is intentional to prevent duplicate exports.

---

## Window Layout

EthoWild uses a dockable window layout. You can:

- **Drag docks** to rearrange them
- **Stack docks** as tabs by dropping one onto another
- **Float docks** by dragging them outside the main window
- **Hide/show docks** via **View** menu

### Default Layout

| Area | Dock |
|------|------|
| Center | Video player |
| Right (top) | Behaviors tree |
| Right (bottom) | Controls |
| Bottom | Records table |

---

## Keyboard Shortcuts

Currently, EthoWild is primarily mouse-driven. The main keyboard interaction is:

| Shortcut | Action |
|----------|--------|
| **Ctrl + Scroll** | Zoom in/out |

---

## Workflow Example

Here's a typical labeling workflow:

1. **Open** a video directory containing your observation recordings
2. **Configure** session parameters (tag, role, sex, etc.)
3. **Play** the video and observe
4. When a behavior occurs, **pause** and **double-click** the behavior
5. For states, double-click to start, then double-click again to end
6. Continue until the video is complete
7. **Save** records to CSV
8. Press **⏭** to load the next video
9. Repeat

