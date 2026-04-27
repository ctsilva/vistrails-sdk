# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**DEPRECATED PROJECT - Not actively maintained**

This is the VisTrails Provenance Software Development Kit (ProvSDK), a C++ library with Python and Java bindings for capturing and utilizing provenance information in software applications. Originally developed commercially by VisTrails, Inc. (2009-2011), now open-source under 3-clause BSD license.

The SDK enables developers to capture state transitions as provenance, providing infrastructure for creating, searching, and replaying provenance information. Developers define serializations for state changes; the SDK handles the rest.

## Build System

The project uses CMake as its build system.

### Building the Core Library

```bash
mkdir build && cd build
cmake ../src
make
```

Requirements:
- CMake 2.6+
- Boost 1.42+ (requires `date_time` component on non-macOS platforms)

### Building with Python Wrapping

```bash
mkdir build && cd build
cmake -DVisTrails_PYTHON_WRAPPING=ON ../src
make
```

To run Python examples:
```bash
PYTHONPATH=/path/to/build/swig python examples/python/testing.py
```

### Building with Java Wrapping

```bash
mkdir build && cd build
cmake -DVisTrails_JAVA_WRAPPING=ON ../src
make
```

To compile and run Java examples:
```bash
javac -classpath .:build/swig examples/java/*.java
DYLD_LIBRARY_PATH=build/swig java -classpath .:build/swig Testing
```

### Building with GUI Components

```bash
cmake -DVisTrails_BUILD_GUI=ON ../src
```

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

Custom action classes in Python/Java inherit from language-specific base classes (`PyAction`, Java action classes) and implement serialization callbacks.

### GUI Components ([src/gui/](src/gui/))

Qt-based provenance visualization components (optional):
- Gallery view for browsing versions
- Version tree visualization
- Image container UI for displaying version states

## Key Concepts

**Version Tree**: Directed acyclic graph where nodes are versions and edges are state transitions (actions). Supports branching and merging.

**Action Serialization**: Custom actions must serialize their state changes to property lists (`PList`/`PDict`). The SDK handles tree structure and metadata.

**Session Workflow**:
1. Load or create a `Vistrail`
2. Create a `Session` with the vistrail
3. Execute actions to change application state
4. Record new `Version` with actions to the session
5. Save session to storage

## Examples

- [examples/python/](examples/python/) - Python binding examples with custom actions
- [examples/java/](examples/java/) - Java binding examples (shapes demo)
- [examples/shapes/](examples/shapes/) - Qt GUI example with shape drawing provenance
- [examples/simple/](examples/simple/) - Minimal C++ usage example
