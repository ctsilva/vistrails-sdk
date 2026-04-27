# VisTrails SDK Developer Guide

This guide shows you how to integrate provenance tracking into your C++ applications using the VisTrails SDK.

## Table of Contents

1. [Core Concepts](#core-concepts)
2. [Quick Example](#quick-example)
3. [Defining Custom Actions](#defining-custom-actions)
4. [Working with Sessions](#working-with-sessions)
5. [Persistence](#persistence)
6. [Advanced Topics](#advanced-topics)
7. [Integrating with Existing Applications](#integrating-with-existing-applications)
8. [Research Background](#research-background)

## Core Concepts

### The Provenance Model

The VisTrails SDK captures your application's history as a **version tree**:

```
Root
 ├─ Version 1: AddShape(circle)
 │   ├─ Version 2: MoveShape(circle, x:10, y:20)
 │   └─ Version 3: AddShape(square)  [branch!]
 └─ Version 4: DeleteShape(circle)    [another branch!]
```

Each **Version** represents a state of your application. **Actions** describe
the transitions between versions.

### Key Classes

| Class | Purpose | Your Responsibility |
|-------|---------|---------------------|
| `vt::Vistrail` | Container for all versions | Create once, reuse |
| `vt::Version` | A single state in history | Created by Session |
| `vt::Action` | Describes a state change | **Define custom subclasses** |
| `vt::Session` | Records and manages versions | Interact with this |
| `vt::Storage` | Saves/loads to disk | Use `XmlStorage` or customize |

## Quick Example

Here's a minimal example tracking shape operations:

```cpp
#include <VisTrails.h>
#include <core/XmlStorage.h>

// 1. Define your action class
class AddShapeAction : public vt::Action {
public:
    AddShapeAction(const std::string& shapeType, int x, int y)
        : type(shapeType), posX(x), posY(y) {}

    // Execute this action (modify your application state)
    void execute(MyApp* app) {
        app->addShape(type, posX, posY);
    }

    // Serialize action data
    void save(vt::HPropertyIO* io) override {
        vt::PDict* dict = io->beginWrite();
        dict->setString("type", type.c_str());
        dict->setInt("x", posX);
        dict->setInt("y", posY);
        io->endWrite(dict);
    }

    // Deserialize action data
    void load(vt::HPropertyIO* io) override {
        vt::PDict* dict = io->beginRead();
        type = dict->getString("type")->asString();
        posX = dict->getInt("x")->asInt();
        posY = dict->getInt("y")->asInt();
        io->endRead(dict);
    }

    // Factory method for deserialization
    static vt::Action* creator() { return new AddShapeAction("", 0, 0); }

    // Unique type name for this action
    const char* name() const override { return "AddShapeAction"; }

private:
    std::string type;
    int posX, posY;
};

// Register the action type
VT_DECLARE_ACTION(AddShapeAction)

int main() {
    // 2. Register your custom actions
    vt::ActionFactory* factory = vt::ActionFactory::globalFactory();
    factory->registerAction(AddShapeAction::creator());

    // 3. Create a vistrail and session
    vt::Vistrail* vistrail = new vt::Vistrail();
    vt::Session* session = new vt::Session(vistrail);

    // 4. Record an action
    AddShapeAction* action = new AddShapeAction("circle", 100, 200);
    AddShapeAction* inverse = new AddShapeAction("circle", 0, 0); // for undo

    vt::Version* version = new vt::Version();
    version->performed()->add(vt::ActionList(action));
    version->inverse()->add(vt::ActionList(inverse));

    session->record(version);

    // 5. Save to file
    vt::XmlStorage storage("myapp.xml");
    session->save(&storage);

    delete session;
    delete vistrail;
    return 0;
}
```

## Defining Custom Actions

Every state change in your application should be represented as an **Action** subclass.

### Action Interface

```cpp
class MyAction : public vt::Action {
public:
    // Required: Serialize your action's data
    void save(vt::HPropertyIO* io) override;

    // Required: Deserialize your action's data
    void load(vt::HPropertyIO* io) override;

    // Required: Return unique action type name
    const char* name() const override;

    // Required: Factory method for deserialization
    static vt::Action* creator();

    // Optional: Execute the action
    void execute(YourAppState* state);
};
```

### Serialization Best Practices

Use the property system to save/load data:

```cpp
void MyAction::save(vt::HPropertyIO* io) {
    vt::PDict* dict = io->beginWrite();

    // Primitive types
    dict->setInt("count", myCount);
    dict->setFloat("value", myValue);
    dict->setString("name", myName.c_str());
    dict->setBoolean("flag", myFlag);

    // Lists
    vt::PList* list = new vt::PList();
    for (int val : myValues) {
        list->add(new vt::PInt(val));
    }
    dict->setList("values", list);

    io->endWrite(dict);
}

void MyAction::load(vt::HPropertyIO* io) {
    vt::PDict* dict = io->beginRead();

    myCount = dict->getInt("count")->asInt();
    myValue = dict->getFloat("value")->asFloat();
    myName = dict->getString("name")->asString();
    myFlag = dict->getBoolean("flag")->asBoolean();

    vt::PList* list = dict->getList("values");
    for (size_t i = 0; i < list->count(); i++) {
        myValues.push_back(list->get(i)->asInt());
    }

    io->endRead(dict);
}
```

### Action Registration

Always register your actions before loading vistrails:

```cpp
// In your application startup code
vt::ActionFactory* factory = vt::ActionFactory::globalFactory();
factory->registerAction(AddShapeAction::creator());
factory->registerAction(MoveShapeAction::creator());
factory->registerAction(DeleteShapeAction::creator());
```

## Working with Sessions

### Creating a Session

```cpp
vt::Vistrail* vistrail = new vt::Vistrail();
vt::Session* session = new vt::Session(vistrail);

// Optional: Set user info
session->info()->setString("user", "alice");
session->info()->setString("machine", "workstation-1");
```

### Recording Versions

When your application state changes, record a version:

```cpp
void MyApp::addShape(const std::string& type, int x, int y) {
    // 1. Modify your application state
    shapes.push_back(Shape{type, x, y});

    // 2. Create action and its inverse (for undo)
    AddShapeAction* action = new AddShapeAction(type, x, y);
    DeleteShapeAction* inverse = new DeleteShapeAction(shapes.size() - 1);

    // 3. Create version with action
    vt::Version* version = new vt::Version();
    version->performed()->add(vt::ActionList(action));
    version->inverse()->add(vt::ActionList(inverse));

    // 4. Record it
    session->record(version);
}
```

### Navigating History

Move between versions:

```cpp
// Get current version
vt::VersionId current = session->currentVersion();

// Move to a different version
vt::VersionId target = /* ... some version ID ... */;
session->change(target);  // Executes actions to reach target state

// Undo
session->undo();

// Redo
session->redo();
```

### Querying the Version Tree

```cpp
// Get all versions
vt::VersionTree* tree = vistrail->versionTree();
const std::vector<vt::VersionId>& versions = tree->vertices();

// Find parent of a version
vt::Version* v = vistrail->getVersion(someId);
vt::VersionId parent = tree->parent(someId);

// Get children (branches)
std::vector<vt::VersionId> children = tree->children(someId);

// Get version path between two versions
vt::VersionPath* path = new vt::VersionPath();
vistrail->getVersionPath(fromId, toId, path);
```

## Persistence

### Saving and Loading

```cpp
// Save
vt::XmlStorage storage("mydata.xml");
session->save(&storage);

// Load
vt::XmlStorage storage("mydata.xml");
vt::Vistrail* vistrail = new vt::Vistrail();
vt::Session* session = new vt::Session(vistrail);
session->load(&storage);
```

### Auto-Save Pattern

Bind a session to storage for automatic persistence:

```cpp
vt::XmlStorage* storage = new vt::XmlStorage("mydata.xml");
session->bind(storage);  // Auto-commits on changes
session->setAutoUpdate(true);

// Now changes are automatically saved
session->record(version);  // Automatically persisted!
```

### Custom Storage Backend

Implement `vt::Storage` interface for custom backends:

```cpp
class DatabaseStorage : public vt::Storage {
public:
    void save(vt::HSaveVistrail* visitor) override {
        // Serialize to database
    }

    void load(vt::HLoadVistrail* visitor) override {
        // Deserialize from database
    }
};
```

## Advanced Topics

### Annotations

Add metadata to versions:

```cpp
vt::Version* version = new vt::Version();
// ... add actions ...

// Tag a version
version->annotations()->setString("tag", "milestone-1");

// Add notes
version->annotations()->setString("notes", "Fixed critical bug");

session->record(version);
```

### Checkpoints

Store application snapshots for faster replay:

```cpp
vt::Version* version = new vt::Version();
version->performed()->add(vt::ActionList(action));

// Add checkpoint action that captures full state
CheckpointAction* checkpoint = new CheckpointAction(app->captureState());
version->checkpoint()->add(vt::ActionList(checkpoint));

session->record(version);
```

### Notification Handlers

React to provenance changes:

```cpp
class MyNotifyHandler : public vt::HNotify {
public:
    void versionAdded(vt::Version* version) override {
        std::cout << "New version: " << version->id().toString().asChar() << std::endl;
    }

    void versionSelected(const vt::VersionId& previous,
                        const vt::VersionId& current) override {
        std::cout << "Changed from " << previous.toString().asChar()
                  << " to " << current.toString().asChar() << std::endl;
    }
};

// Register handler
MyNotifyHandler* handler = new MyNotifyHandler();
session->addNotifyHandler(handler);
```

### Version Tree Filtering

Filter the version tree by criteria:

```cpp
// Show only tagged versions
vt::VersionTreeFilter* filter = new vt::VersionTreeFilter();
filter->setAnnotationFilter("tag", "");  // Has any tag

vistrail->applyFilter(filter);

// Show versions in date range
filter->setDateRange(startTime, endTime);
```

### Merging Branches

Merge two branches of the version tree:

```cpp
// Implement custom merge handler
class MyMergeHandler : public vt::HMerge {
public:
    vt::Version* merge(const vt::Version* v1,
                       const vt::Version* v2) override {
        // Custom merge logic
        vt::Version* merged = new vt::Version();
        // ... combine actions from v1 and v2 ...
        return merged;
    }
};

session->setMergeHandler(new MyMergeHandler());
vt::VersionId merged = session->merge(branch1Id, branch2Id);
```

## Complete Example: Shape Drawing App

See [examples/shapes/](examples/shapes/) for a complete Qt-based application
demonstrating:

- Custom action classes for add/move/delete operations
- Session management and recording
- Undo/redo functionality
- XML persistence
- Version tree visualization

## API Reference

For detailed API documentation, see:

- [src/core/](src/core/) - Core provenance classes
- [CLAUDE.md](CLAUDE.md) - Architecture overview
- [examples/](examples/) - Working code examples

## Integrating with Existing Applications

The VisTrails SDK was specifically designed to **provenance-enable existing applications** without requiring them to be rewritten or integrated into a workflow system. This approach allows users to leverage provenance using the same applications they're already familiar with.

### The Undo/Redo Stack Integration Pattern

Most GUI applications following the **Model-View-Controller (MVC)** pattern already have an undo/redo mechanism. The SDK can tap into this existing infrastructure:

**Key Insight**: The application designers have already determined the appropriate granularity of actions by implementing the undo stack!

```cpp
// Intercept actions being added to the undo stack
class ProvenanceUndoManager {
public:
    void onActionAdded(ApplicationAction* action) {
        // 1. Extract action details
        std::string command = action->serialize();

        // 2. Send to VisTrails Session
        vt::Action* vtAction = new GenericAction(command);
        vt::Version* version = new vt::Version();
        version->performed()->add(vt::ActionList(vtAction));

        session->record(version);
    }
};
```

### Requirements for Integration

Your application needs:

1. **Undo/Redo Stack** - Captures reversible operations
2. **Action Serialization** - Actions can be converted to/from strings or data structures
3. **Replay Capability** - Actions can be re-executed to reconstruct states

### Case Study: ParaView Integration

The paper ["Towards Provenance-Enabling ParaView"](docs/prov-paraview.pdf) (IPAW 2008) describes how the SDK was integrated with ParaView, a popular scientific visualization tool:

- **Integration Point**: ParaView's undo/redo mechanism
- **Captured Actions**: Pipeline changes (add/delete filters, parameter changes)
- **Not Captured**: Camera navigation (intentionally, as it's view state not data)
- **Result**: Complete exploration history without modifying ParaView's core

```cpp
// Pseudocode from ParaView integration
void ParaView::UndoStack::Push(Command* cmd) {
    // Original undo stack behavior
    undoStack.push(cmd);

    // NEW: Also send to Provenance Explorer
    provenanceExplorer->recordAction(cmd->toString());
}
```

### Communication API Pattern

For applications that run as separate processes, use a **Communication API**:

```cpp
// Application side: Send action to Provenance Explorer
void sendActionToProvenance(const std::string& command) {
    socket.send(command);
}

// Provenance Explorer side: Receive and store
void receiveAction() {
    std::string command = socket.receive();
    vt::Action* action = new GenericAction(command);
    // ... create version and record
}

// Replay: Send actions back to application
void replayVersion(vt::VersionId id) {
    vt::VersionPath* path = getPathToVersion(id);
    for (vt::Action* action : path->actions()) {
        socket.send(action->command());
    }
}
```

### When Undo Stack Is Insufficient

Some applications store **state snapshots** instead of actions, or don't capture everything needed for reproducibility. In these cases:

```cpp
// Compute action from state difference
vt::Action* computeAction(State* previous, State* current) {
    // Find differences: sp - sn
    std::vector<Change> changes = diff(previous, current);

    // Create action from changes
    return new DiffAction(changes);
}
```

### Benefits of This Approach

1. **Minimal Disruption**: Users work in familiar applications
2. **No Workflow System Required**: Direct integration, not workflow wrapper
3. **Existing Granularity**: Leverages application's built-in action semantics
4. **Incremental Adoption**: Can start with basic tracking, enhance over time

## Research Background

The VisTrails SDK is based on research published at IPAW (International Provenance and Annotation Workshop) 2006-2008 by researchers at the University of Utah and VisTrails, Inc.

### The Change-Based Provenance Model

**Key Innovation**: Instead of storing multiple versions of application state, store the **operations (actions)** that transform one state into another.

**Mathematical Model**:
```
Let DF = domain of all possible application states
Let x : DF → DF be a state transformation function
Let D = set of all such transformation functions

A version d is constructed by composing actions:
d = (xₙ ∘ (xₙ₋₁ ∘ ... ∘ (x₁ ∘ (∅))...))
```

**Benefits**:
- **Compact**: Uses substantially less space than storing full states
- **Intuitive**: History tree shows decision points and alternatives
- **Queryable**: Can analyze which actions led to specific results
- **Reusable**: Actions can be remapped by analogy to new contexts

### Comparison with Other Provenance Systems

| System | Approach | Application Type |
|--------|----------|------------------|
| **VisTrails SDK** | Action-based, undo stack integration | Interactive GUI applications |
| PASOA | Service assertions | Service-oriented architectures |
| ESSW | Script wrapping | Batch programs |
| Kepler/Taverna | Workflow graphs | Scientific workflows |

### Proven Applications

The VisTrails provenance technology has been successfully used in:

- **Scientific Visualization**: ParaView, VisTrails Builder
- **Climate Science**: Weather and climate model exploration
- **Medical Imaging**: MRI/CT data analysis workflows
- **Computational Biology**: Genomics data pipelines

### Research Publications

For deeper understanding, see:

1. **Callahan et al. (2006)**: "Managing the Evolution of Dataflows with VisTrails"
   - Introduced the version tree model for workflows

2. **Freire et al. (2006)**: "Managing Rapidly-Evolving Scientific Workflows" (IPAW)
   - Detailed provenance infrastructure and query capabilities

3. **Callahan et al. (2008)**: "Towards Provenance-Enabling ParaView" (IPAW)
   - Demonstrated SDK integration with existing applications
   - Available in [docs/prov-paraview.pdf](docs/prov-paraview.pdf)

4. **Scheidegger et al. (2007)**: "Querying and Creating Visualizations by Analogy"
   - Advanced provenance reuse through workflow analogy

### Why This Matters for Your Application

Traditional approaches to tracking computational history are ad-hoc:
- Manual laboratory notebooks
- File naming conventions (`analysis_v1.txt`, `analysis_v2_final_FINAL.txt`)
- Lost exploration paths when using simple undo

**With VisTrails SDK**, you get:
- Automatic capture of complete exploration history
- Branch and merge support for trying alternatives
- Reproducibility even years later
- Collaborative sharing of computational processes
- Compliance with audit trail requirements

## Getting Help

- Check [examples/](examples/) for working code
- Read [CLAUDE.md](CLAUDE.md) for architecture details
- Read the research paper: [docs/prov-paraview.pdf](docs/prov-paraview.pdf)
- Open an issue on GitHub for questions or bugs

---

**Next Steps**: Build the shapes example to see provenance tracking in action!

```bash
cd examples/shapes
mkdir build && cd build
cmake ..
make
./Shapes
```
