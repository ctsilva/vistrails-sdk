# VisTrails SDK Project Status

**Last Updated**: 2026-04-27

## Overview

The VisTrails SDK is being actively modernized from Qt4/Qt5 to Qt6 and updated for modern C++ standards. This document tracks the current status of the migration effort.

## Build Status

| Component | Status | Details |
|-----------|--------|---------|
| **Core Library** | ✅ **COMPLETE** | Successfully builds with Qt6 and C++17 |
| **GUI Library** | 🚧 **95% Complete** | ~9 errors remaining (down from 100+) |
| **Examples** | ⏸️ Pending | Awaiting GUI completion |
| **Python Bindings** | ⏸️ Not Started | SWIG-based, needs updating |
| **Java Bindings** | ⏸️ Not Started | SWIG-based, needs updating |

## GUI Library Progress

### Completed Fixes (✅)

1. **Include Statement Migration** - All QtGui/QtCore → specific Qt6 headers
   - Fixed 30+ header and source files
   - Updated include paths for Qt6 module structure

2. **Layout API Changes**
   - `setMargin()` → `setContentsMargins()` (11 locations)
   - Most instances fixed, 2 remain in GraphicsView.cpp

3. **Mouse/Wheel Event API**
   - `QMouseEvent::x()` → `position().x()` ✅
   - `QWheelEvent::delta()` → `angleDelta().y()` (partially complete)
   - `QMouseEvent::globalPos()` → `globalPosition()` ✅

4. **DateTime API Changes**
   - `QDateTime::fromTime_t()` → `fromSecsSinceEpoch()` ✅
   - `QDateTime::toTime_t()` → `toSecsSinceEpoch()` ✅
   - `setTimeSpec()` → `setTimeZone()` ✅

5. **Graphics API Changes**
   - `QMatrix` → `QTransform` ✅
   - `setMatrix()` → `setTransform()` ✅
   - `matrix()` → `transform()` ✅

6. **Button Enums**
   - `Qt::MidButton` → `Qt::MiddleButton` ✅

7. **Method Signature Changes**
   - `QWidget::enterEvent(QEvent*)` → `enterEvent(QEnterEvent*)` (mostly done)
   - `QGraphicsScene::itemAt(QPointF)` → `itemAt(QPointF, QTransform)` ✅
   - `QHeaderView::setResizeMode()` → `setSectionResizeMode()` ✅

8. **Type Initialization**
   - `Qt::WindowFlags = 0` → `= {}` ✅
   - `Qt::ItemFlags = 0` → `= {}` ✅

9. **Validators**
   - `QRegExpValidator` → `QRegularExpressionValidator` ✅

10. **Rendering Hints**
    - Removed deprecated `QPainter::HighQualityAntialiasing` ✅

### Remaining Issues (🚧)

**File**: [src/gui/tree/GraphicsView.cpp](src/gui/tree/GraphicsView.cpp)

1. **QPrinter Support** (2 errors)
   ```cpp
   // Need to add Qt6::PrintSupport to CMakeLists.txt
   #include <QPrinter>
   #include <QPrinterInfo>
   ```

2. **QWheelEvent Constructor** (3 errors)
   - Qt6 changed QWheelEvent constructor signature
   - Lines 510, 516, 523
   - Need to use new constructor with angleDelta

3. **Layout Margin** (1 error)
   - Line 567: `layout()->setMargin()` → `setContentsMargins()`

4. **Enter Event Signature** (2 errors)
   - Lines 723: `QPIPGraphicsView::enterEvent()` signature mismatch
   - Header file needs updating to match implementation

## Documentation Status

| Document | Status | Description |
|----------|--------|-------------|
| **README.md** | ✅ Complete | Project overview, quick start, status tracking |
| **DEVELOPER_GUIDE.md** | ✅ Complete | Comprehensive API guide with research background |
| **QT6_MIGRATION.md** | ✅ Complete | Detailed porting guide with all API changes |
| **CLAUDE.md** | ✅ Complete | Architecture documentation (pre-existing) |
| **STATUS.md** | ✅ Complete | This file - current project status |

## Next Steps

### Immediate (Complete GUI Library)

1. Add `Qt6::PrintSupport` to [src/gui/CMakeLists.txt](src/gui/CMakeLists.txt)
2. Fix QWheelEvent constructors in GraphicsView.cpp
3. Fix remaining setMargin calls
4. Fix enterEvent signature mismatch
5. Verify build completes successfully

### Short Term

1. Update shapes example for Qt6
2. Test provenance tree visualization
3. Add unit tests for core library
4. Set up CI/CD pipeline

### Medium Term

1. Update Python bindings (SWIG)
2. Update Java bindings (SWIG)
3. Modernize remaining examples
4. Performance testing

### Long Term

1. Add comprehensive test suite
2. API modernization (smart pointers, move semantics)
3. Documentation expansion
4. Community building

## Known Issues

### Build Warnings

The build generates deprecation warnings for:
- `setTimeSpec()` (fixed but warnings remain)
- Various deprecated Qt5 APIs

These are non-critical but should be addressed for clean builds.

### Platform Support

Currently tested on:
- ✅ macOS (Darwin 25.5.0, arm64)
- ❓ Linux (untested)
- ❓ Windows (untested)

## How to Build

### Core Library Only (Working)

```bash
cd /Users/csilva/src/vistrails-sdk
mkdir -p build && cd build
cmake ../src
make
```

### With GUI (In Progress)

```bash
cd /Users/csilva/src/vistrails-sdk
mkdir -p build && cd build
cmake -DVisTrails_BUILD_GUI=ON ../src
make
# Currently fails with ~9 errors
```

## Contributing

See remaining issues above. The GUI library is very close to completion!

Priority areas:
1. QPrinter/PrintSupport integration
2. QWheelEvent constructor fixes
3. Testing on Linux/Windows

## Research Foundation

This SDK is based on peer-reviewed research:
- **Callahan et al. (2008)**: "Towards Provenance-Enabling ParaView" (IPAW)
- **Freire et al. (2006)**: "Managing Rapidly-Evolving Scientific Workflows" (IPAW)
- See [docs/prov-paraview.pdf](docs/prov-paraview.pdf) for details

## License

3-clause BSD License

Original: VisTrails, Inc. (2009-2011)
Revival: Active development (2026-)
