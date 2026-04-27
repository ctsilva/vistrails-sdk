# VisTrails Provenance Software Development Kit

**Status**: 🔄 Active Development - Modernizing for Qt6 and C++17

This package contains the source code for the VisTrails Provenance
Software Development Kit (ProvSDK) as well as examples demonstrating its use.

Originally developed commercially by VisTrails, Inc. (2009-2011), the project
is now being actively modernized with Qt6 support, updated build systems,
and comprehensive documentation.

## What is ProvSDK?

The ProvSDK enables software developers to capture and utilize **provenance information**
in their applications. Provenance tracking records the history of data transformations,
allowing you to:

- **Track Changes**: Automatically capture every state transition in your application
- **Navigate History**: Move forward and backward through your application's history
- **Branch & Merge**: Create alternative execution paths and merge them
- **Replay Actions**: Reconstruct any previous state by replaying recorded actions
- **Query History**: Search through your application's execution history

The SDK provides infrastructure for creating, searching, and replaying provenance;
developers need only define serializations for state changes. Everything else is handled
automatically.

## Quick Start

### Building the Core Library

```bash
mkdir build && cd build
cmake ../src
make
```

**Requirements:**
- CMake 3.5+
- C++17 compiler
- Boost 1.42+ (requires `date_time` component on non-macOS platforms)

### Running the Examples

```bash
# Simple C++ example
cd examples/simple
mkdir build && cd build
cmake ..
make
./simple_example

# Python example (requires Python wrapping)
cd build && cmake -DVisTrails_PYTHON_WRAPPING=ON ../src && make
PYTHONPATH=/path/to/build/swig python examples/python/testing.py
```

## Project Status

| Component | Status | Notes |
|-----------|--------|-------|
| Core Library | ✅ Complete | Builds with Qt6 and C++17 |
| Python Bindings | ⏸️ Not Started | SWIG-based |
| Java Bindings | ⏸️ Not Started | SWIG-based |
| GUI Components | 🚧 In Progress | Qt6 migration underway |
| Examples | ⏸️ Pending | Awaiting GUI completion |
| Documentation | 📝 In Progress | See [CLAUDE.md](CLAUDE.md) |

### Recent Updates

- ✅ **Qt6 Migration**: Core library successfully ported to Qt6
- ✅ **Modern CMake**: Updated build system to CMake 3.5+
- ✅ **C++17**: Modernized codebase for current standards
- 🚧 **GUI Library**: Qt6 porting in progress (see [QT6_MIGRATION.md](QT6_MIGRATION.md))

## Documentation

- **[CLAUDE.md](CLAUDE.md)** - Project architecture and developer guide
- **[QT6_MIGRATION.md](QT6_MIGRATION.md)** - Qt6 porting guide and API changes
- **[examples/](examples/)** - Working code examples demonstrating SDK usage

## Architecture Overview

The provenance system is built around these core abstractions:

**Vistrail** - The top-level provenance container managing a version tree
of all recorded states and transitions.

**Version** - Represents a single state in the provenance history. Contains
a unique ID, parent references, timestamp, and the actions that created it.

**Action** - User-defined state changes that transition between versions.
Developers create custom action classes that serialize their state changes.

**Session** - Runtime context for working with a vistrail. Records new
versions, manages undo/redo, and handles persistence.

**Storage** - Abstract persistence layer with XML implementation included.
Saves/loads vistrails to/from disk.

See [CLAUDE.md](CLAUDE.md) for detailed architecture documentation.

## Contributing

Contributions are welcome! Current priorities:

1. Complete Qt6 GUI library migration
2. Modernize Python/Java bindings
3. Update examples for Qt6
4. Expand documentation
5. Add unit tests

Please check existing issues or create a new one to discuss major changes.

## License

3-clause BSD License. See [LICENSE](LICENSE) for details.

Originally developed by VisTrails, Inc. (2009-2011).
Now maintained as an open-source project.

