/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "CommonWidgets.h"
#include "Theme.h"
#include <QPainter>
#include <QLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QActionGroup>
#include <QMenu>

//////////////////////////////////////////////////////////////////////////////
QToolWindow::QToolWindow(QWidget *widget, QWidget *parent)
: QDockWidget(parent)
{
  this->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  this->setWidget(widget);
  if(widget)
    this->setWindowTitle(widget->windowTitle());
  this->monitorWindowTitle(widget);
}

void QToolWindow::monitorWindowTitle(QWidget *widget)
{
  if(widget)
    widget->installEventFilter(this);
}

bool QToolWindow::eventFilter(QObject *object, QEvent *e)
{
  if(e->type()==QEvent::WindowTitleChange)
  {
    QWidget* widget = dynamic_cast<QWidget*>(object);
    if(widget)
      this->setWindowTitle(widget->windowTitle());
  }
  else if(e->type()==QEvent::Close)
    object->removeEventFilter(this);
  return QDockWidget::eventFilter(object, e);
}

//////////////////////////////////////////////////////////////////////////////
QToolWindowInterface::QToolWindowInterface(QWidget *self)
{
  this->_self = self;
}

QToolWindow *QToolWindowInterface::toolWindow()
{
  if(!this->_toolWindow)
    this->_toolWindow = new QToolWindow(_self, _self->parentWidget());
  else if (this->_toolWindow->widget() != _self)
    this->_toolWindow->setWidget(_self);
  return this->_toolWindow;
}

void QToolWindowInterface::changeEvent(QEvent *e)
{  
  if(e->type() == QEvent::ParentChange && this->_toolWindow)
    if(_self->parent() != this->_toolWindow)
      this->_toolWindow->setParent(_self->parentWidget());
}

//////////////////////////////////////////////////////////////////////////////
QDockContainer::QDockContainer(QWidget *parent): QMainWindow(parent)
{
  this->setDockNestingEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
QSearchTreeWidget::QSearchTreeWidget(QWidget *parent): QTreeWidget(parent)
{
  this->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
  this->setRootIsDecorated(true);
  this->setDragEnabled(true);
  this->flags =Qt::ItemIsDragEnabled;
}

//FIXME
// Pass through all items of a item
//bool recursiveSetVisible(item, testFunction))
//{
//enabled = testFunction(item);

//visible = enabled;
//child = item.child;
//for (childIndex in xrange(item.childCount()))
//{
//    visible |= recursiveSetVisible(child(childIndex),
//                                   testFunction);
//}

//// if item is hidden or has changed visibility
//if(! visible || (item.isHidden() != (!visible)))
//    item.setHidden(not visible);

//if(visible)
//{
//    f = item.flags();
//    b = f & this->flags;
//    if(enabled)
//        if(not b)
//            item.setFlags(f | this->flags);
//    else if(b)
//        item.setFlags(f & ~this->flags);
//}

//return visible;
//}

void QSearchTreeWidget::searchItemName(QString name)
{
  // FIXME
  //matchedItems = [];

  //if(str(name)=="")
  //    testFunction = lambda x) true;
  //else
  //    matchedItems = set(this->findItems(name,
  //                                     Qt::MatchContains |
  //                                     Qt::MatchWrap |
  //                                     Qt::MatchRecursive));
  //    testFunction = matchedItems.__contains__
  //for itemIndex in xrange(this->topLevelItemCount()))
  //    recursiveSetVisible(this->topLevelItem(itemIndex),
  //                        testFunction)
}

//  FIXME
//QMimeData *QSearchTreeWidget::mimeData(QList itemList)
//{
//  QMimeData *data = QTreeWidget::mimeData(itemList);
//  data->items = itemList;
//  return data;
//  return new QMimeData()
//}

void QSearchTreeWidget::setMatchedFlags(Qt::ItemFlags _flags)
{
  this->flags = _flags;
}

//////////////////////////////////////////////////////////////////////////////
QSearchTreeWindow::QSearchTreeWindow(QWidget *parent)
: QWidget(parent) 
{
  this->setWindowTitle("Search Tree");

  this->vLayout = new QVBoxLayout(this);
  this->vLayout->setContentsMargins(0, 0, 0, 0);
  this->vLayout->setSpacing(0);
  this->setLayout(vLayout);

  this->searchBox = new QSearchBox(false, this);
  this->vLayout->addWidget(this->searchBox);

  this->treeWidget = new QSearchTreeWidget(this);
  this->vLayout->addWidget(this->treeWidget);

  this->connect(this->searchBox,
    SIGNAL(executeIncrementalSearch(QString)),
    SLOT(treeWidget.searchItemName()));
  this->connect(this->searchBox,
    SIGNAL(executeSearch(QString)),
    SLOT(treeWidget.searchItemName()));
  this->connect(this->searchBox,
    SIGNAL(resetSearch()),
    SLOT(clearTreeWidget()));
}

QSearchTreeWindow::~QSearchTreeWindow()
{
  delete this->searchBox;
  delete this->treeWidget;
  delete this->vLayout;

  this->searchBox = NULL;
  this->treeWidget = NULL;
  this->vLayout = NULL;
}

// Return the default search tree
void QSearchTreeWindow::clearTreeWidget()
{
  this->treeWidget->searchItemName(QString(""));
}

//////////////////////////////////////////////////////////////////////////////
QPromptWidget::QPromptWidget(QWidget *parent): QLabel(parent)
{
  this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  this->setWordWrap(true);
  this->regularFont = this->font();
  this->promptFont = QFont(this->font());
  this->promptFont.setItalic(true);
  this->promptText = "";
  this->promptVisible = false;
}

void QPromptWidget::setPromptText(QString text)
{
  this->promptText = text;
}

void QPromptWidget::showPrompt(bool show)
{
  if(show!=this->promptVisible)
  {
    this->promptVisible = show;
    this->repaint(this->rect());
  }
}

void QPromptWidget::showPromptByChildren()
{
  if(this->promptText=="")
    this->showPrompt(false);
  else
  {
    this->showPrompt(this->layout() == NULL ||
      this->layout()->count()==0);
  }

}

void QPromptWidget::paintEvent(QPaintEvent *e)
{
  if(this->promptVisible)
  {
    QPainter *painter = new QPainter(this);
    painter->setFont(this->promptFont);
    painter->drawText(this->rect(),
      Qt::AlignCenter |Qt::TextWordWrap,
      this->promptText);
    painter->end();
    delete painter;
  }
  QLabel::paintEvent(e);
}

//////////////////////////////////////////////////////////////////////////////
QStringEdit::QStringEdit(QWidget *parent): QFrame(parent)
{
  this->hLayout = new QHBoxLayout(this);
  this->hLayout->setContentsMargins(0, 0, 0, 0);
  this->hLayout->setSpacing(0);
  this->setLayout(hLayout);

  this->lineEdit = new QLineEdit();
  this->lineEdit->setFrame(false);
  this->lineEdit->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
  hLayout->addWidget(this->lineEdit);
  this->setFocusProxy(this->lineEdit);

  this->fileButton = new QToolButton();
  this->fileButton->setText("...");
  this->fileButton->setSizePolicy(QSizePolicy::Maximum,
    QSizePolicy::Expanding);
  this->fileButton->setFocusPolicy(Qt::NoFocus);
  this->fileButton->setAutoFillBackground(true);

  this->connect(this->fileButton, SIGNAL(clicked()),
    SLOT(insertFileNameDialog()));
  hLayout->addWidget(this->fileButton);
}

QStringEdit::~QStringEdit()
{
  delete hLayout;
  delete lineEdit;
  delete fileButton;

  this->hLayout = NULL;
  this->lineEdit = NULL;
  this->fileButton = NULL;
}

void QStringEdit::setText(QString text)
{
  this->lineEdit->setText(text);
}

QString QStringEdit::text()
{
  return this->lineEdit->text();
}

void QStringEdit::selectAll()
{
  this->lineEdit->selectAll();
}

void QStringEdit::setFrame(bool frame)
{
  if(frame)
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
  else
    this->setFrameStyle(QFrame::NoFrame);
}

void QStringEdit::insertFileNameDialog()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    "Use Filename as Value...",
    this->text(),
    "All files (*.*)");
  if(!fileName.isEmpty())
    this->setText(fileName);
}

//////////////////////////////////////////////////////////////////////////////
// FIXME: StandardConstantWidget not converted yet
//MultiLineWidget::MultiLineWidget(QString contents, 
//                                 QString contentType, 
//                                 QWidget *parent)
//: StandardConstantWidget(parent)
//{ }
//
//void MultiLineWidget::update_parent()
//{ }
//
//void MultiLineWidget::keyPressEvent(QKeyEvent *e)
//{
//  int k = e->key();
//  Qt::KeyboardModifiers s = e->modifiers();
//  if((k == Qt::Key_Enter || k == Qt::Key_Return) && (s & Qt::ShiftModifier))
//  {
//     e->accept();
//     if(this->contentIsString && this->multiLines)
//     {
//         QStringList fileNames = QFileDialog::getOpenFileNames(this,
//                                                        "Use Filename "
//                                                        "as Value...",
//                                                        this->text(),
//                                                        "All files "
//                                                        "(*.*)");
//         fileName = fileNames.join(",");
//         if(not fileName.isEmpty())
//         {
//             this->setText(fileName);
//             return;
//         }
//     }
//  }
//  QLineEdit::keyPressEvent(e);
//}

//////////////////////////////////////////////////////////////////////////////
QSearchEditBox::QSearchEditBox(QWidget *parent): QComboBox(parent)
{
  this->setEditable(true);
  this->setInsertPolicy(QComboBox::InsertAtTop);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  QRegularExpression regexp = QRegularExpression("\\S.*");
  this->setDuplicatesEnabled(false);
  this->validator = new QRegularExpressionValidator(regexp, this);
  this->setValidator(validator);
  this->addItem("Clear Recent Searches");

  // FIXME
  //item = this->model()->item(0, 0);
  //font = QFont(item.font());
  //font.setItalic(true);
  //item.setFont(font);
}

QSearchEditBox::~QSearchEditBox()
{
  delete this->validator;
  this->validator = NULL;
}

void QSearchEditBox::keyPressEvent(QKeyEvent *e)
{
  int k = e->key();
  if(k == Qt::Key_Return || k == Qt::Key_Enter)
    if(!this->currentText().isEmpty())
      emit resetText();
  QComboBox::keyPressEvent(e);
}

//////////////////////////////////////////////////////////////////////////////
QSearchBox::QSearchBox(bool refine, QWidget *parent): QWidget(parent)
{
  this->setWindowTitle("Search");

  hLayout = new QHBoxLayout(this);
  this->hLayout->setContentsMargins(0, 0, 0, 0);
  this->hLayout->setSpacing(2);
  this->setLayout(hLayout);

  if(refine)
  {
    this->actionGroup = new QActionGroup(this);
    this->searchAction = new QAction("Search", this);
    this->searchAction->setCheckable(true);
    this->actionGroup->addAction(this->searchAction);
    this->refineAction = new QAction("Refine", this);
    this->refineAction->setCheckable(true);
    this->actionGroup->addAction(this->refineAction);
    this->searchAction->setChecked(true);

    this->searchMenu = new QMenu();
    this->searchMenu->addAction(this->searchAction);
    this->searchMenu->addAction(this->refineAction);

    this->searchButton = new QToolButton(this);
    this->searchButton->setIcon(
      ThemeHolder::getCurrentTheme()->QUERY_ARROW_ICON);
    this->searchButton->setAutoRaise(true);
    this->searchButton->setPopupMode(QToolButton::InstantPopup);
    this->searchButton->setMenu(this->searchMenu);
    this->hLayout->addWidget(this->searchButton);

    this->connect(this->searchAction, SIGNAL(triggered()),
      SLOT(searchMode()));
    this->connect(this->refineAction, SIGNAL(triggered()),
      SLOT(refineMode()));
  }
  else
  {
    this->searchLabel = new QLabel(this);
    QPixmap pix = ThemeHolder::getCurrentTheme()
      ->QUERY_VIEW_ICON.pixmap(QSize(16,16));
    this->searchLabel->setPixmap(pix);
    this->searchLabel->setAlignment(Qt::AlignCenter);
    this->searchLabel->setMargin(4);
    this->hLayout->addWidget(this->searchLabel);
  }

  this->searchEdit = new QSearchEditBox(this);
  this->setFocusProxy(this->searchEdit);
  // TODO: Add separator!
  this->searchEdit->clearEditText();
  this->hLayout->addWidget(this->searchEdit);

  this->resetButton = new QToolButton(this);
  this->resetButton->setIcon(QIcon(
    this->style()->standardPixmap(QStyle::SP_DialogCloseButton)));
  this->resetButton->setIconSize(QSize(12,12));
  this->resetButton->setAutoRaise(true);
  this->resetButton->setEnabled(false);
  this->hLayout->addWidget(this->resetButton);

  this->connect(this->resetButton, SIGNAL(clicked()),
    SLOT(emitResetSearch()));
  this->connect(this->searchEdit, SIGNAL(editTextChanged(QString)),
    SLOT(emitExecuteIncrementalSearch()));
  this->connect(this->searchEdit, SIGNAL(activated(int)),
    SLOT(emitExecuteSearch()));
  this->connect(this->searchEdit, SIGNAL(resetText()),
    SLOT(emitResetSearch()));
}

QSearchBox::~QSearchBox()
{
  delete this->hLayout;
  delete this->actionGroup;
  delete this->searchAction;
  delete this->refineAction;
  delete this->searchMenu;
  delete this->searchButton;
  delete this->resetButton;
  delete this->searchLabel;
  delete this->searchEdit;

  this->hLayout = NULL;
  this->actionGroup = NULL;
  this->searchAction = NULL;
  this->refineAction = NULL;
  this->searchMenu = NULL;
  this->searchButton = NULL;
  this->resetButton = NULL;
  this->searchLabel = NULL;
  this->searchEdit = NULL;
}

void QSearchBox::emitResetSearch()
{
  this->searchEdit->clearEditText();
  this->resetButton->setEnabled(false);
  emit resetSearch();
}

void QSearchBox::clearSearch()
{
  this->searchEdit->clearEditText();
  this->resetButton->setEnabled(false);
}

void QSearchBox::searchMode()
{
  this->emitRefineMode();
}

void QSearchBox::emitRefineMode()
{
  emit refineMode(false);
}

void QSearchBox::emitExecuteIncrementalSearch(QString text)
{
  this->resetButton->setEnabled(!text.isEmpty());
  emit executeIncrementalSearch(text);
}

void QSearchBox::emitExecuteSearch(int index)
{
  int count = this->searchEdit->count();
  if(index == count-1)
  {
    for( int i = 0; i < count-1; i++)
      this->searchEdit->removeItem(0);
    this->emitResetSearch();
  }
  else
  {
    this->resetButton->setEnabled(true);
    emit executeSearch(this->searchEdit->currentText());
  }
}
