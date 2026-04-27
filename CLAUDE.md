# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**STATUS: Active Development - Qt6 Migration Complete ✅**

This is the VisTrails Provenance Software Development Kit (ProvSDK), a C++ library with Python and Java bindings for capturing and utilizing provenance information in software applications. Originally developed commercially by VisTrails, Inc. (2009-2011), now modernized with Qt6 support and released as open-source under 3-clause BSD license.

The SDK enables developers to capture state transitions as provenance, providing infrastructure for creating, searching, and replaying provenance information. Developers define serializations for state changes; the SDK handles the rest.

## Current Status

| Component | Status | Details |
|-----------|--------|---------|
| Core Library | ✅ Complete | Builds with Qt6 and C++17 |
| GUI Library | ✅ Complete | Qt6 migration finished, all components working |
| Version Tree Visualization | ✅ Complete | Interactive history tree with zoom/navigation |
| Shapes Example | ✅ Complete | Fully functional with GUI integration |
| Python Bindings | ⏸️ Not Started | SWIG-based, needs Qt6 update |
| Java Bindings | ⏸️ Not Started | SWIG-based, needs Qt6 update |
| Documentation | ✅ Complete | See DEVELOPER_GUIDE.md, QT6_MIGRATION.md |

**Recent Achievements:**
- ✅ Complete Qt5 → Qt6 migration (all API changes resolved)
- ✅ GUI library compiles successfully (libVisTrailsGUI.dylib)
- ✅ History tree visualization integrated into shapes example
- ✅ Version navigation working (click nodes to jump through history)
- ✅ Crash fixes for version tree interaction

## Build System

The project uses CMake (3.5+) as its build system.

### Requirements

- **CMake** 3.5+
- **C++ Compiler** with C++17 support
- **Qt6** (Widgets, PrintSupport, Concurrent components)
- **Boost** 1.42+ (requires `date_time` component on non-macOS platforms)

### Building the Core Library

```bash
mkdir build && cd build
cmake ../src
make
```

Output: `libVisTrails.dylib` (1.3M)

### Building with GUI Components (Recommended)

The GUI is built by default when Qt6 is found:

```bash
mkdir build && cd build
cmake ../src
make
```

Output:
- `libVisTrails.dylib` - Core library
- `gui/libVisTrailsGUI.dylib` - GUI library with version tree visualization

### Building the Shapes Example

```bash
cd examples/shapes
mkdir build && cd build
cmake ..
make
./Shapes
```

This opens two windows:
1. **Shapes Editor** - Draw boxes and triangles
2. **VisTrails History Tree** - Real-time provenance visualization

**Controls:**
- Add/move/delete shapes → automatically tracked in version tree
- Click nodes in tree → navigate to that historical state
- Mouse wheel on tree → zoom in/out
- Cmd+/Cmd- → zoom keyboard shortcuts

### Building with Python Wrapping (Not Yet Updated for Qt6)

```bash
mkdir build && cd build
cmake -DVisTrails_PYTHON_WRAPPING=ON ../src
make
```

**Note:** Python bindings need Qt6 updates before they will work.

### Building with Java Wrapping (Not Yet Updated for Qt6)

```bash
mkdir build && cd build
cmake -DVisTrails_JAVA_WRAPPING=ON ../src
make
```

**Note:** Java bindings need Qt6 updates before they will work.

## Architecture

### Core Components ([src/core/](src/core/))

The provenance system is built around these key abstractions:

**Vistrail** - The top-level provenance container that manages a version tree of state changes
- Maintains a `VersionTree` of all recorded states
- Provides version path computation between states
- Handles version annotations and pruning

**Version** - Represents a single state in the provenance history
- Contains a unique `VersionId`
- References parent version(s) to form the tree structure
- Stores timestamp, annotations, and actions that created this version

**Action** - User-defined state changes that transition between versions
- Abstract base class for all provenance-tracked operations
- Uses `VT_DECLARE_ACTION` macro to register custom action types
- Must implement `HPropertyIO` interface for serialization
- Registered with `ActionFactory` for deserialization

**Session** - Runtime context for working with a vistrail
- Records new versions into the vistrail
- Manages current state and undo/redo
- Handles persistence via `Storage` interface
- Supports binding to storage for auto-update/commit workflows

**Storage** - Abstract persistence layer (default implementation: `XmlStorage`)
- Saves/loads vistrails to/from persistent storage
- Uses handler pattern (`HSaveVistrail`/`HLoadVistrail`) for extensibility

### Handler Interfaces ([src/core/Handler.h](src/core/Handler.h))

The SDK uses abstract handler classes for extensibility:

- `HPropertyIO` - Serialize/deserialize actions and properties
- `HMerge` - Custom merge strategies when recording conflicting versions
- `HNotify` - Observe vistrail changes (version added, selected, etc.)
- `HStamp` - Add metadata (timestamp, user info) to new versions
- `HFetchData` - Lazy-load associated data on demand

### Language Bindings ([src/swig/](src/swig/))

SWIG interface files (`.i`) wrap C++ classes for Python and Java:
- Python: Generates `PyVisTrails` module
- Java: Generates `com.vistrails.sdk` package + JAR file

**Status:** Not yet updated for Qt6. The bindings exist but need updates to work with the modernized codebase.

Custom action classes in Python/Java inherit from language-specific base classes (`PyAction`, Java action classes) and implement serialization callbacks.

### GUI Components ([src/gui/](src/gui/)) ✅ Qt6 Ready

Qt6-based provenance visualization components:

**Main Components:**
- [vtwindow.cpp](src/gui/vtwindow.cpp) - Main VisTrails window with version tree
- [tree/VersionView.cpp](src/gui/tree/VersionView.cpp) - Interactive version tree visualization
- [tree/VistrailController.cpp](src/gui/tree/VistrailController.cpp) - Controller for tree operations
- [tree/GraphicsView.cpp](src/gui/tree/GraphicsView.cpp) - Zoomable graphics view for tree
- [tree/Theme.cpp](src/gui/tree/Theme.cpp) - Visual theme configuration

**Additional Components:**
- Gallery view for browsing versions
- Image container UI for displaying version states
- Version properties panel
- Search and filter functionality

**Qt6 Migration Notes:**
All GUI components have been successfully migrated to Qt6. See [QT6_MIGRATION.md](QT6_MIGRATION.md) for details on API changes.

## Key Concepts

**Version Tree**: Directed acyclic graph where nodes are versions and edges are state transitions (actions). Supports branching and merging.

**Action Serialization**: Custom actions must serialize their state changes to property lists (`PList`/`PDict`). The SDK handles tree structure and metadata.

**Session Workflow**:
1. Load or create a `Vistrail`
2. Create a `Session` with the vistrail
3. Execute actions to change application state
4. Record new `Version` with actions to the session
5. Save session to storage

**Version Navigation**:
1. Display version tree using `VisTrailsWindow`
2. User clicks on a node in the tree
3. System computes path from current version to target version
4. Actions are replayed (forward) or inverted (backward) to reconstruct state
5. Application state updates to match the selected version

## Examples

- [examples/shapes/](examples/shapes/) - **Qt6 GUI example with full history tree** ✅
  - Interactive shapes editor (boxes and triangles)
  - Real-time provenance tracking
  - Version tree visualization window
  - Click nodes to navigate history
  - Save/load provenance to XML

- [examples/simple/](examples/simple/) - Minimal C++ usage example
  - Basic provenance recording without GUI
  - Good starting point for learning the API

- [examples/python/](examples/python/) - Python binding examples ⏸️
  - Custom actions in Python
  - Needs Qt6 updates

- [examples/java/](examples/java/) - Java binding examples ⏸️
  - Custom actions in Java
  - Needs Qt6 updates

## Documentation

- **[README.md](README.md)** - Project overview and quick start
- **[DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md)** - Comprehensive developer documentation
- **[QT6_MIGRATION.md](QT6_MIGRATION.md)** - Complete Qt5→Qt6 migration guide with all API changes
- **[STATUS.md](STATUS.md)** - Detailed project status and progress tracking
- **[CLAUDE.md](CLAUDE.md)** - This file, guidance for Claude Code

## Common Development Tasks

### Adding a New Custom Action

1. Create action class inheriting from `vt::Action`
2. Implement serialization methods (`save`/`load`)
3. Use `VT_DECLARE_ACTION` macro to register
4. Register with `ActionFactory::globalFactory()`
5. See [examples/shapes/actions.cpp](examples/shapes/actions.cpp) for reference

### Integrating Version Tree GUI

1. Include `gui/vtwindow.hpp`
2. Create `VisTrailsWindow` instance
3. Call `setSessionFocus()` with your `SessionFocus`
4. Show the window
5. See [examples/shapes/mainwindow.cpp](examples/shapes/mainwindow.cpp) for reference

### Qt6 API Changes to Remember

When working with GUI code, remember these Qt6 changes:
- `QDateTime::fromTime_t()` → `fromSecsSinceEpoch()`
- `QFontMetrics::width()` → `horizontalAdvance()`
- `QLayout::setMargin()` → `setContentsMargins(0,0,0,0)`
- `QRegExp` → `QRegularExpression`
- Always add null checks when accessing scene items

See [QT6_MIGRATION.md](QT6_MIGRATION.md) for complete list.

## Testing

Run the shapes example to verify everything works:

```bash
cd examples/shapes/build
./Shapes
```

Expected behavior:
- Two windows open (shapes editor + version tree)
- Adding/moving shapes creates new nodes in tree
- Clicking tree nodes navigates history
- Zoom works with mouse wheel
- No crashes during navigation

## Known Issues

- Python/Java bindings not yet updated for Qt6
- Some deprecation warnings for Qt6 API (setTimeSpec)
- Abstract destructor warnings in shapes example (cosmetic)

## License

3-clause BSD License. See [LICENSE](LICENSE) for details.

Originally developed by VisTrails, Inc. (2009-2011).
Modernized and maintained as open-source (2025+).
