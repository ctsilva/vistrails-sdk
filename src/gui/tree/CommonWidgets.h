/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef __COMMONWIDGETS_H__
#define __COMMONWIDGETS_H__

#include <QtGui>
#include <QtCore>

class QSearchBox;

//////////////////////////////////////////////////////////////////////////////
// QToolWindow is a floating-dockable widget. It also keeps track of
// its widget window title to update the tool window accordingly
class QToolWindow: public QDockWidget
{
public:
  // Construct a floating, dockable widget
  QToolWindow(QWidget *widget = NULL, QWidget *parent = NULL);

  // Watching window title changed on widget and use it as a window
  void monitorWindowTitle(QWidget *widget);

  // Filter window title change event to change the tool window title
  bool eventFilter(QObject *object, QEvent *event);

  QWidget* parent;
};

// QToolWindowInterface can be co-inherited in any class to allow the
// inherited class to switch to be contained in a window
class QToolWindowInterface
{
public:
  QToolWindowInterface(QWidget *self = NULL);

  //Return the tool window && set its parent to this->parent()
  //while having this as its contained widget
  QToolWindow *toolWindow();

  //Make sure to update the tool parent when to match the widget"s
  //real parent
  void changeEvent(QEvent *e);

protected:
  QToolWindow *_toolWindow;
  QWidget *_self;
};


//////////////////////////////////////////////////////////////////////////////
// QDockContainer is a window that can contain dock widgets while
// still be contained in a tool window. It is just a straight
// inheritance from QMainWindow
class QDockContainer: public QMainWindow
{
public:
  // Setup window to have its widget dockable everywhere
  QDockContainer(QWidget *parent = NULL);
};

///////////////////////////////////////////////////////////////////////////////
// QSearchTreeWidget is just a QTreeWidget with a support function to
// refine itself when searching for some text
class QSearchTreeWidget: public QTreeWidget
{
public:
  // Set up size policy and header
  QSearchTreeWidget(QWidget *parent = NULL);
    
  // Search and refine the module tree widget to contain only items
  // whose name is 'name'
  void searchItemName(QString name);
    
  // Setup the mime data to contain itemList because Qt 4.2.2
  // implementation doesn't instantiate QTreeWidgetMimeData
  // anywhere as it's supposed to. It must have been a bug...
  // FIXME
  //QMimeData *mimeData(QList itemList);
  
  // Set the flags for matched item in the search tree. Parents of matched 
  // node will be visible with these flags off.
  void setMatchedFlags(Qt::ItemFlags _flags);

private:
  Qt::ItemFlags flags;
};
    
//////////////////////////////////////////////////////////////////////////////
// QSearchTreeWindow contains a search box on top of a tree widget
// for easy search and refine. The subclass has to implement
// createTreeWidget() method to return a tree widget that is also 
// needs to expose searchItemName method
class QSearchTreeWindow: public QWidget
{
public:
  // Intialize all GUI components
  QSearchTreeWindow(QWidget *parent = NULL);
  ~QSearchTreeWindow();

  // Return the default search tree
  void clearTreeWidget();

  // Return a default searchable tree widget
  void createTreeWidget();

private:
  QVBoxLayout *vLayout;
  QSearchTreeWidget *treeWidget;
  QSearchBox *searchBox;
};

//////////////////////////////////////////////////////////////////////////////
// QPromptWidget is a widget that will display a prompt text when it
// doesn't have any child visible, or else, it will disappear. This
// is good for drag and drop prompt. The inheritance should call
// setPromptText and showPrompt in appropriate time to show/hide the
// prompt text
class QPromptWidget: public QLabel
{
public:
  // Set up the font and alignment for the prompt
  QPromptWidget(QWidget *parent = NULL);

  // Set the prompt text string
  void setPromptText(QString text);

  // Show/Hide the prompt
  void showPrompt(bool show = true);
        
  // Show/Hide the prompt based on the current state of children
  void showPromptByChildren();

  // Paint the prompt in the center if neccesary
  void paintEvent(QPaintEvent *event);

private:
  QFont regularFont;
  QFont promptFont;
  QString promptText;
  bool promptVisible;
};

//////////////////////////////////////////////////////////////////////////////
// QStringEdit is a line edit that has an extra button to allow user
// to use a file as the value
class QStringEdit: public QFrame
{
  Q_OBJECT

public:
  // Create a hbox layout to contain a line edit and a button
  QStringEdit(QWidget *parent = NULL);
  ~QStringEdit();

  // Overloaded function for setting the line edit text
  void setText(QString text);

  QString text();

  void selectAll();

  // Show/Hide the frame of this widget
  void setFrame(bool frame);

public slots:
  // Allow user to insert a file name as a value to the string
  void insertFileNameDialog();

private:
  QHBoxLayout *hLayout;
  QLineEdit *lineEdit;
  QToolButton *fileButton;
};
        
//////////////////////////////////////////////////////////////////////////////
// FIXME: StandardConstantWidget not converted yet
//class MultiLineWidget: public StandardConstantWidget
//{
//public:
//  // Initialize the line edit with its contents. Content type is limited
//  // to 'int', 'float', and 'string'
//  MultiLineWidget(QString contents, QString contentType, QWidget *parent = NULL);
//
//   void update_parent();
//     
//   // If this is a string line edit, we can use Ctrl+Enter to enter
//   // the file name
//   void keyPressEvent(QKeyEvent *event);
//
//   bool multiLines;
//   bool contentIsString;
// };

//////////////////////////////////////////////////////////////////////////////
class QSearchEditBox: public QComboBox
{
  Q_OBJECT

public:
  QSearchEditBox(QWidget *parent = NULL);
  ~QSearchEditBox();

  void keyPressEvent(QKeyEvent *e);

signals:
  void resetText();

private:
  QRegExpValidator *validator;
};
        
//////////////////////////////////////////////////////////////////////////////
// QSearchBox contains a search combo box with a clear button and
// a search icon.
class QSearchBox: public QWidget
{
  Q_OBJECT

public:
  // Intialize all GUI components
  QSearchBox(bool refine = true, QWidget *parent = NULL);
  ~QSearchBox();

  // Clear the edit text without emitting resetSearch() signal
  // This is for when the search is rest from the version view and
  // the signal are already taken care of
  void clearSearch();

  void searchMode();

public slots:
  // Emit a signal to clear the search.
  void emitResetSearch();

  void emitRefineMode();    

  // The text is changing, so update the search.
  void emitExecuteIncrementalSearch(QString text);

  // The text is finished changing or a different item was selected.
  void emitExecuteSearch(int index);

signals:
  void resetSearch();
  void refineMode(bool);
  void executeIncrementalSearch(QString);
  void executeSearch(QString);

private:
  QHBoxLayout *hLayout;
  QActionGroup *actionGroup;
  QAction *searchAction;
  QAction *refineAction;
  QMenu *searchMenu;
  QToolButton *searchButton;
  QToolButton *resetButton;
  QLabel *searchLabel;
  QSearchEditBox *searchEdit;
};

#endif
