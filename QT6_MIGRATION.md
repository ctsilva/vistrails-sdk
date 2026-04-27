# Qt6 Migration Guide for VisTrails SDK

This document tracks the changes needed to build the VisTrails SDK with Qt6.

## Status

**Core Library**: ✅ Complete (builds successfully)
**GUI Library**: 🚧 In Progress
**Examples/Shapes**: ⏸️ Pending GUI completion

## Core Library Changes

### CMake Configuration

**File**: [src/CMakeLists.txt](src/CMakeLists.txt)

Changed CMake minimum version:
```cmake
# Before:
cmake_minimum_required(VERSION 2.6)

# After:
cmake_minimum_required(VERSION 3.5)
```

## GUI Library Changes (In Progress)

### CMake Configuration

**File**: [src/gui/CMakeLists.txt](src/gui/CMakeLists.txt)

1. Updated CMake version and added Qt6 auto-moc/uic/rcc:
```cmake
cmake_minimum_required(VERSION 3.5)
project(VisTrailsGUI)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
```

2. Replaced Qt4 with Qt6:
```cmake
# Before:
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
QT4_ADD_RESOURCES(...)
QT4_WRAP_UI(...)
QT4_WRAP_CPP(...)

# After:
find_package(Qt6 COMPONENTS Widgets REQUIRED)
# Auto-moc/uic/rcc handles the rest
```

3. Updated target linking:
```cmake
# Before:
target_link_libraries(VisTrailsGUI ${QT_LIBRARIES} VisTrails)

# After:
target_link_libraries(VisTrailsGUI Qt6::Widgets VisTrails)
```

### Include Statement Changes

Qt6 requires explicit includes instead of monolithic `<QtGui>` and `<QtCore>`:

**Pattern**: Replace `#include <QtGui>` with specific headers

Examples:
```cpp
// Before:
#include <QtGui>

// After (include only what you need):
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
// etc.
```

**Files Updated**:
- [src/gui/delegate.hpp](src/gui/delegate.hpp)
- [src/gui/vtwindow.hpp](src/gui/vtwindow.hpp)
- [src/gui/versionedit.hpp](src/gui/versionedit.hpp)
- [src/gui/galleryview.hpp](src/gui/galleryview.hpp)
- [src/gui/imageviewer.hpp](src/gui/imageviewer.hpp)
- [src/gui/imagevisualizer.hpp](src/gui/imagevisualizer.hpp)
- [src/gui/vtsortfilterproxy.hpp](src/gui/vtsortfilterproxy.hpp)
- [src/gui/aspectpreservinglabel.hpp](src/gui/aspectpreservinglabel.hpp)
- [src/gui/tree/GraphicsView.h](src/gui/tree/GraphicsView.h)
- [src/gui/tree/Theme.h](src/gui/tree/Theme.h)
- [src/gui/tree/CommonWidgets.h](src/gui/tree/CommonWidgets.h)
- [src/gui/tree/VersionProp.h](src/gui/tree/VersionProp.h)

### API Changes

#### 1. Layout Margins

**Pattern**: `setMargin(int)` → `setContentsMargins(int, int, int, int)`

```cpp
// Before:
horizontalLayout->setMargin(0);

// After:
horizontalLayout->setContentsMargins(0, 0, 0, 0);
```

**Files**: [src/gui/gallerylabel.cpp:23](src/gui/gallerylabel.cpp#L23)

#### 2. QLabel::pixmap() Return Type

**Pattern**: Returns `QPixmap` by value, not pointer

```cpp
// Before:
return QPixmap(*lblPixmapHolder->pixmap());

// After:
return lblPixmapHolder->pixmap();
```

**Files**: [src/gui/galleryimageholder.cpp:48](src/gui/galleryimageholder.cpp#L48)

#### 3. Qt::WindowFlags Initialization

**Pattern**: Cannot initialize with `0`, use `{}` instead

```cpp
// Before:
ImageProxyWidget(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);

// After:
ImageProxyWidget(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
```

**Files**: [src/gui/galleryproxy.hpp:20](src/gui/galleryproxy.hpp#L20)

#### 4. Qt::ItemFlags Initialization

**Pattern**: Cannot initialize with `0`, use `{}` instead

```cpp
// Before:
Qt::ItemFlags VtModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return 0;
  // ...
}

// After:
Qt::ItemFlags VtModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return {};
  // ...
}
```

**Files**: [src/gui/vtmodel.cpp:52](src/gui/vtmodel.cpp#L52)

#### 5. QRegExpValidator → QRegularExpressionValidator

**Pattern**: Qt6 uses regular expressions instead of old regex

```cpp
// Before:
#include <QRegExpValidator>
QRegExpValidator *validator;

// After:
#include <QRegularExpressionValidator>
QRegularExpressionValidator *validator;
```

**Files**: [src/gui/tree/CommonWidgets.h:225](src/gui/tree/CommonWidgets.h#L225)

#### 6. Mouse Event Position

**Pattern**: `x()` and `y()` deprecated, use `position()`

```cpp
// Before:
int mouseX = e->x();

// After:
int mouseX = e->position().x();
```

**Files**: [src/gui/galleryview.cpp:198,231,233](src/gui/galleryview.cpp#L198)

#### 7. Wheel Event Delta

**Pattern**: `delta()` removed, use `angleDelta()`

```cpp
// Before:
int numDegrees = e->delta() / 8;

// After:
int numDegrees = e->angleDelta().y() / 8;
```

**Files**: [src/gui/galleryview.cpp:174](src/gui/galleryview.cpp#L174)

#### 8. QPainter RenderHints

**Pattern**: `HighQualityAntialiasing` removed in Qt6

```cpp
// Before:
setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
               QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

// After:
setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
               QPainter::TextAntialiasing);
```

**Files**: [src/gui/galleryview.cpp:49](src/gui/galleryview.cpp#L49)

#### 9. QDateTime Conversion

**Pattern**: `toTime_t()` → `toSecsSinceEpoch()`, `fromTime_t()` → `fromSecsSinceEpoch()`

```cpp
// Before:
time_t timestamp = qtime.toUTC().toTime_t();
QDateTime dt = QDateTime::fromTime_t(timestamp);

// After:
qint64 timestamp = qtime.toUTC().toSecsSinceEpoch();
QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
```

**Files**: [src/gui/vtnode.cpp:195](src/gui/vtnode.cpp#L195)

#### 10. QDateTime TimeSpec

**Pattern**: `setTimeSpec()` deprecated, use `setTimeZone()`

```cpp
// Before:
qtime.setTimeSpec(Qt::LocalTime);

// After:
qtime.setTimeZone(QTimeZone::LocalTime);
```

**Requires**: `#include <QTimeZone>`

**Files**: [src/gui/vtnode.cpp:194](src/gui/vtnode.cpp#L194)

#### 11. QtConcurrent Include

**Pattern**: Must use full path

```cpp
// Before:
#include <QtConcurrent>

// After:
#include <QtConcurrent/QtConcurrent>
```

**Files**: [src/gui/galleryview.cpp:18](src/gui/galleryview.cpp#L18)

## Known Issues / TODO

The following Qt6 changes still need to be implemented:

### Missing Includes

Many .cpp files need additional includes:
- `QMessageBox`
- `QTimer`
- `QActionGroup`
- `QScrollBar`
- `QApplication`
- `QTimeZone`

### API Changes Still Needed

1. **QMatrix → QTransform**: Qt6 removed QMatrix entirely
2. **Qt::MidButton → Qt::MiddleButton**: Button enum renamed
3. **QGraphicsScene::itemAt()**: Now requires QTransform parameter
4. **QWidget::enterEvent()**: Now takes `QEnterEvent*` instead of `QEvent*`
5. **QDateTime::fromTime_t()**: Needs conversion to `fromSecsSinceEpoch()`

## Building

### Core Library Only

```bash
mkdir build && cd build
cmake ../src
make
```

### With GUI (Currently Incomplete)

```bash
mkdir build && cd build
cmake -DVisTrails_BUILD_GUI=ON ../src
make
# Note: GUI library does not fully build yet due to remaining Qt6 porting work
```

## Contributing

When porting additional code to Qt6:

1. Update this migration guide with new patterns discovered
2. Note file:line references for changes
3. Include both "before" and "after" code snippets
4. Update the status section at the top

## References

- [Qt 6 Porting Guide](https://doc.qt.io/qt-6/portingguide.html)
- [Porting to Qt 6 - QWidget](https://doc.qt.io/qt-6/widgets-changes-qt6.html)
- [Qt 6.0 Breaking Changes](https://www.qt.io/blog/qt-6.0-released)
