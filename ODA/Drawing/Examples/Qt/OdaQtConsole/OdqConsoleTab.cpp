/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
//
// OdqConsoleTab.cpp
//
#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

#include "OdqConsole.h"
#include "OdqConsoleTab.h"
#include "ConsoleModule.h"
#include "SysVarPE.h"
#include "ExDynamicModule.h" // for ODRX_SELECTION_SET
#include "ExtDbModule.h"

//#define OD_ANDROID_TRACEDEFS

#ifdef OD_ANDROID_TRACEDEFS
#ifdef ANDROID
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  "OdqConsoleTab", __VA_ARGS__)
#else
#undef OD_ANDROID_TRACEDEFS
#endif
#endif

//////////////////////////////////////////////////////////////////////////

OdqConsoleTab::OdqConsoleTab(QWidget* pParent,
                             OdRxObject* pRxDatabase,
                             const TCmdMap& mapCmds)
  : QWidget(pParent)
  , m_levelUndoBlock(0)
  , m_stateKey(0)
  , m_touchEvent(TouchUnknownEvent)
  , m_doubleTapTimer(false)
  , m_bBlinkTextState(false)
//, m_bDatabaseModified(false)
{
  m_pCmdContext = CommandContextImpl::createObject(getCmdIO(), pRxDatabase);

  ODA_ASSERT_ONCE(!m_pCmdContext.isNull());
  // unfortunately ExDbCommandContext::m_pDb is not smart pointer
  // therefore the next is used as holder
  m_pRxDatabase = pRxDatabase;
  ODA_ASSERT_ONCE(getCmdContext().baseDatabase() == m_pRxDatabase);

  //getSelectionSet(); // create selection set
  //getGripPointsSet(); // create grip points set

  // -----

  // http://www.opennet.ru/docs/RUS/qt3_prog/qt3_prog-prog.html.gz
  m_pOut = new QTextEdit;
  m_pOut->setReadOnly(true);
  m_pOut->setLineWrapMode(QTextEdit::NoWrap);
  m_pOut->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  m_pOut->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_pOut->setFrameStyle(QFrame::NoFrame);
  // dismiss readonly //m_pOut->setTextInteractionFlags(Qt::TextEditorInteraction);
  //m_pOut->setFocusPolicy(Qt::ClickFocus);
  // unfortunately QTextEdit and QTextBrowser in read-only mode are incompatible with cursor moving // TOincompatibleDO
  // TODO try via other way
  //m_pOut->ensureCursorVisible();
  //// ... m_pOut->setTextCursor(QTextCursor());
  //m_pOut->setWordWrapMode(QTextOption::WrapAnywhere);
  QFont fnt = m_pOut->font();
  fnt.setFamily("Courier");
  fnt.setFixedPitch(true);
  ODA_ASSERT_ONCE(fnt.fixedPitch());
  //fnt.setPointSize(10);
  m_pOut->setFont(fnt);

  m_pOutHorScrollBar = new QScrollBar(Qt::Horizontal);
  m_pOut->setHorizontalScrollBar(m_pOutHorScrollBar);
  m_pOutHorScrollBar->setFocusPolicy(Qt::StrongFocus);

  //QScrollBar* pHorScroolBar = m_pOut->horizontalScrollBar();
  //m_pOutHorScrollBar->setRange(pHorScroolBar->minimum(), pHorScroolBar->maximum());
  ////m_pOut->addScrollBarWidget(m_pOutHorScrollBar, Qt::Alignment alignment);

  //QObject::connect(hbar, SIGNAL(valueChanged(int)), q, SLOT(_q_hslide(int)));
  //QObject::connect(hbar, SIGNAL(rangeChanged(int,int)), q, SLOT(_q_showOrHideScrollBars()), Qt::QueuedConnection);
  //if (!QObject::connect(m_pOutHorScrollBar, SIGNAL(valueChanged(int)), pHorScroolBar, SLOT(setValue(int))))
  //  ODA_FAIL_ONCE();
  //QObject::connect(pHorScroolBar, SIGNAL(rangeChanged(int,int)), m_pOutHorScrollBar, SIGNAL(rangeChanged(int,int)));
  //if (!QObject::connect(pHorScroolBar, SIGNAL(rangeChanged()), m_pOutHorScrollBar, SIGNAL(rangeChanged()))0
  //  ODA_FAIL_ONCE();
  // rangeChanged

  m_pIn = new OdqCommandLineEdit(mapCmds);
  m_pLabel = new QLabel(getCommandQPrompt()); // "Command: "
  m_pLabel->setAutoFillBackground(true);
  QPalette plt = m_pIn->palette(); // = m_pLabel->palette();
  plt.setColor(QPalette::Active, QPalette::Window, Qt::white);
  plt.setColor(QPalette::Inactive, QPalette::Window, Qt::white);
  //plt.setColor(QPalette::Active, QPalette::WindowText, Qt::black);
  //plt.setColor(QPalette::Inactive, QPalette::WindowText, Qt::black);
  //
  //plt.setColor(QPalette::Active, QPalette::AlternateBase, Qt::white);
  //plt.setColor(QPalette::Inactive, QPalette::AlternateBase, Qt::white);
  m_pLabel->setPalette(plt);
  m_pLabel->setFont(fnt);
  m_pLabel->setAlignment(Qt::AlignLeft);
  // TODO by default half the width of the "x" character of the widget's current font().
  int indent = fnt.weight();
  indent /= 10;
  m_pLabel->setIndent(indent);
  //m_pIn->setPalette(plt);
  m_pIn->setFont(fnt);

  ////QSizePolicy pol = m_pIn->sizePolicy();
  ////pol.setControlType(QSizePolicy::Label);
  ////m_pIn->setSizePolicy(pol);

  //m_pIn->setMinimumHeight(m_pLabel->minimumHeight());
  //m_pIn->setMaximumHeight(m_pLabel->maximumHeight());
  ////m_pLabel->setMinimumHeight(m_pIn->minimumHeight());
  ////m_pLabel->setMaximumHeight(m_pIn->maximumHeight());

  m_pLabel->setBuddy(m_pIn);
  //m_pLabel = new QTextEdit(getCommandQPrompt()); // "Command: "
  //m_pLabel->setReadOnly(true);

  // test
  //m_pLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
  //m_pLabel->setText("Command<a href=\"open\">open</a>: "); // test
  m_pLabel->setText(CommandState::getDefaultQPrompt() + ": ");

  //m_pLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken );
  //m_pLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken );
  m_pLabel->setFrameStyle(QFrame::NoFrame); // | QFrame::Raised
  m_pIn->setFrame(false);
  //m_pLabel->setFrameStyle(QFrame::Box | QFrame::Plain);

  // set invisible background color
  // (unfortunately m_pBannerBrowser->setTextBackgroundColor is not usable for it)
  //QPalette palette = qApp->palette();
  //QColor colorSysGrey = palette.color(QPalette::Normal, QPalette::Window); //Base);
  //int red = colorSysGrey.red(), // 236
  //  green = colorSysGrey.green(), // 233
  //  blue = colorSysGrey.blue(); // 216
  //QString qsSysGrey = QString("bgcolor=\"#%1%2%3\"").arg(red, 2, 16, QLatin1Char('0'))
  //  .arg(green, 2, 16, QLatin1Char('0'))
  //  .arg(blue, 2, 16, QLatin1Char('0'));
  //qsSysGrey = qsSysGrey.toUpper();
  //qsText.replace("bgcolor=\"#ffffff\"", qsSysGrey, Qt::CaseInsensitive);

  if (!QObject::connect(m_pLabel, SIGNAL(linkActivated(const QString&)),
                        this, SLOT(keywordByParam(const QString &)),
                        Qt::QueuedConnection)) // for test only
  {
    ODA_FAIL_ONCE();
  }

  //
  setFocusProxy(m_pIn);


  if (!QObject::connect(m_pIn, SIGNAL(returnPressed()),
                        this, SLOT(executeCommandByInput()), 
                        Qt::QueuedConnection)) // needs for such command as close
  {
    ODA_FAIL_ONCE();
  }
  //(void) 
  new QShortcut(Qt::CTRL + Qt::Key_N, this, SLOT(newCommand()));
  new QShortcut(Qt::CTRL + Qt::Key_O, this, SLOT(openCommand()));
  new QShortcut(Qt::Key_Escape, this, SLOT(cancelCommand()));
  new QShortcut(Qt::Key_Delete, this, SLOT(eraseCommand()));
  new QShortcut(Qt::CTRL + Qt::Key_Z, this, SLOT(undoCommand()));
  new QShortcut(Qt::CTRL + Qt::Key_Y, this, SLOT(redoCommand()));
  new QShortcut(Qt::CTRL + Qt::Key_A, this, SLOT(selectAllCommand()));

  //m_pOut->setSizePolicy(m_pOut->sizePolicy().horizontalPolicy(),
  //                             QSizePolicy::Minimum); // QSizePolicy::Expanding); // QSizePolicy::Preferred);
  //m_pOut->size().setHeight(qApp->activeWindow()->height() / 5);

  //QTextCursor cursor(textEdit->textCursor());
  //cursor.movePosition(QTextCursor::Start);
  //QTextFrame *topFrame = cursor.currentFrame();

  //QVBoxLayout *pLayout = new QVBoxLayout;
  QGridLayout *pLayout = new QGridLayout;
  pLayout->setMargin(2); //(0); 90
  pLayout->setSpacing(0);

  //QHBoxLayout *pHorLayout = new QHBoxLayout;
  //pHorLayout->addWidget(m_pLabel);
  //pHorLayout->addWidget(m_pIn);
  //pLayout->addLayout(pHorLayout, 
  //                   1, 0,
  //                   1, 1);
  pLayout->addWidget(m_pLabel,
                     1, 0);
  pLayout->addWidget(m_pIn, // in focus as first 
                     1, 1,
                     1, 1);
  pLayout->addWidget(m_pOutHorScrollBar,
                     1, 2,
                     1, 1);
  pLayout->addWidget(m_pOut, 
                     0, 0, // row column
                     1, 4); // span

  //setLayout(pLayout);
  QLayout* pMainLayout = new QVBoxLayout();
  QGroupBox* pFrameBox = new QGroupBox();
  //pFrameBox->setFlat(false);
  pFrameBox->setLayout(pLayout);
  pMainLayout->addWidget(pFrameBox);
  setLayout(pMainLayout);

  if (qApp->activeWindow())
    setMinimumHeight(qApp->activeWindow()->height() / 15);

  //QScrollView  QLineEdit QSpinBox 
  //size().setHeight(qApp->activeWindow()->height() / 5);

  //setWidget(m_pList);
}

OdqConsoleTab::~OdqConsoleTab()
{
  if (!m_pRxDatabase.isNull())
  {
    QString qsMessage(QObject::tr("File \"%1\" was closed").arg(getQTitle()));
    getIApp()->toStatusBar(qsMessage, 4000);
  }

  m_pCmdIO = NULL;
  m_pCmdContext = NULL;
  m_pRxDatabase = NULL;
}

//static
OdqConsole& OdqConsoleTab::getConsole()
{
  return OdqConsole::getConsole();
}

//static 
CommandContextImpl& OdqConsoleTab::getCmdContext(OdEdCommandContext *pCmdCtx)
{
  ODA_ASSERT_ONCE(pCmdCtx);

  CommandContextImpl* pExCmdCtx = static_cast<CommandContextImpl*>(pCmdCtx);
  ODA_ASSERT_ONCE(pExCmdCtx);

  return *pExCmdCtx;
}

CommandContextImpl& OdqConsoleTab::getCmdContext()
{
  ODA_ASSERT_ONCE(!m_pCmdContext.isNull());
  //CommandContextImpl& context = getCmdContext(m_pCmdContext.get());
  OdEdCommandContext* pExCmdCtx = static_cast<OdEdCommandContext*>(m_pCmdContext.get());
  CommandContextImpl& context = getCmdContext(pExCmdCtx);;
  return context;
}

CommandState &OdqConsoleTab::getTopCmdState()
{
  return getCmdContext().getTopCmdState();
}

OdEdBaseIOPtr OdqConsoleTab::getCmdIO()
{
  if (m_pCmdIO.isNull())
    m_pCmdIO = ConsoleIoImpl::createObject(*this);
  ODA_ASSERT_ONCE(!m_pCmdIO.isNull());

  return m_pCmdIO;
}

OdRxObjectPtr OdqConsoleTab::getRxDatabase()
{
  // unfortunately ExDbCommandContext::m_pDb is not smart pointer
  // therefore m_pDatabase next is used as holder for database
  ODA_ASSERT_ONCE(getCmdContext().baseDatabase() == m_pRxDatabase);        
  return m_pRxDatabase;
}

bool OdqConsoleTab::isDatabaseModified() const
{
  //return m_bDatabaseModified;
  // TODO 
  //if (m_pBaseDbReactor.isNull())
  //  return m_pBaseDbReactor->isModified();

  return false; 
}

void OdqConsoleTab::setDatabaseModified(bool) // bValue)
{
  //m_bDatabaseModified = bValue;
  // TODO 
  //if (m_pBaseDbReactor.isNull())
  //  m_pBaseDbReactor->setModified(bValue);
}

// Returns a SmartPointer to the arbitrary data dictionary 
// (A dictionary is created if a dictionary does not exist)
OdRxDictionaryPtr OdqConsoleTab::arbDataDic() const
{
  if (m_pArbDataDic.isNull())
    m_pArbDataDic = odrxCreateRxDictionary();
  return m_pArbDataDic;
}

void OdqConsoleTab::setArbitraryData(const OdString &sPathName, OdRxObject *pDataObj)
{
  arbDataDic()->putAt(sPathName, pDataObj);
}

OdRxObjectPtr OdqConsoleTab::arbitraryData(const OdString &sPathName) const
{
  return arbDataDic()->getAt(sPathName);
}

OdSelectionSet* OdqConsoleTab::workingSelectionSet()
{
  OdSelectionSetPtr pSSet = arbitraryData(ODRX_SELECTION_SET);
  if (pSSet.isNull() && getRxDatabase().get())
  {
    pSSet = getCmdContext().createSelectionSet(); 
    setArbitraryData(ODRX_SELECTION_SET, pSSet);
  }

  return pSSet.get();
}

void OdqConsoleTab::setWorkingSelectionSet(OdSelectionSet* pSSet)
{
  OdSelectionSetPtr pSSetPrev = arbitraryData(ODRX_SELECTION_SET);
  if (pSSet != pSSetPrev)
    setArbitraryData(ODRX_SELECTION_SET, pSSet);
}

void OdqConsoleTab::clearSelectionSet()
{
  //executeCommand("ai_deselect", false);
#if !defined(_TOOLKIT_IN_DLL_)
    OdString moduleName(OD_T("OdaQtConsole"));
#else
    OdString moduleName(OD_T("OdaQtConsole.dll"));
#endif
  OdRxModule* pModule = ::odrxDynamicLinker()->loadModule(moduleName).get();
  static_cast<OdqConsoleModule*>(pModule)->m_cmd_DeselectAll.execute(&getCmdContext());
}

void OdqConsoleTab::workingSelectionSetChanged()
{
  OdSelectionSet* pSSet = workingSelectionSet();
  ODA_ASSERT_ONCE(pSSet);
  getIApp()->refreshQObjetcts("<event-refresh:selection_changed>");

  OdqGripManagerPtr pGripManager = getCmdContext().gripManager(); // pPointTracker 

  if (pGripManager.isNull())
  {
    if (!pSSet || !pSSet->numEntities())
      return;

    pGripManager = OdqGripManager::createObject(*this);
    getCmdContext().setGripManager(pGripManager);
  }

  pGripManager->selectionSetChanged(NULL);

  // temp solution for m_pOwner of OdExGripData
  //if (!pSSet || !pSSet->numEntities())
  //  getCmdContext().setGripManager(NULL);
}

OdString OdqConsoleTab::getFilePathName()
{
  if (!m_sFilePathName.isEmpty())
    return m_sFilePathName;

  OdRxObject* pRxDb = getRxDatabase().get();
  if (!pRxDb)
    return OD_T("");

  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);
  OdDbBaseDatabasePEPtr pDbPe = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPe.isNull() && pExtDb)
    pDbPe = OdDbBaseDatabasePE::cast(pRxDb = pExtDb->getWrappedDatabase(pRxDb));
  if (pDbPe.get())
    return pDbPe->getFilename(pRxDb);

  if (pExtDb)
    return pExtDb->getFilename(getRxDatabase().get());

  ODA_FAIL_ONCE();
  return L"";
}

// is used for command new and qnew
void OdqConsoleTab::setFilePathName(const OdString& sFilePathName)
{
  m_sFilePathName = sFilePathName;
}

QString OdqConsoleTab::getQTitle()
{
  OdRxObject* pRxDatabase = getRxDatabase().get();
  if (!pRxDatabase)
    return QObject::tr("None");

  // get via pDatabase
  // (name of the file associated with this *database* object)
  QString qsTabName = getIApp()->toQString(getFilePathName()); 

  // remove path
  QFileInfo infoFile(qsTabName);
  qsTabName = infoFile.baseName();
  QString qsSfx = infoFile.completeSuffix().toLower();
  if (!qsSfx.indexOf("teg") || !qsSfx.indexOf("tig"))
  {
    if (!(qsSfx = qsSfx.mid(3)).indexOf('.'))
      qsSfx = qsSfx.mid(1);
  }
  if (!qsSfx.isEmpty())
    qsTabName += "." + qsSfx;
  ODA_ASSERT_ONCE(!qsTabName.isEmpty());

  return qsTabName;
}

QString OdqConsoleTab::updateQTitle() // used after saveAs
{
  QString qsTabName(getQTitle());

  if (!parentWidget() || !parentWidget()->parentWidget())
  {
    ODA_FAIL_ONCE(); // test
    return qsTabName;
  }

  QTabWidget *pTabWidget = qobject_cast<QTabWidget *>(parentWidget()->parentWidget());
  ODA_ASSERT_ONCE(pTabWidget); // && pTabWidget == getConsole().m_pTabWidget);

  int index = pTabWidget->indexOf(this);
  ODA_ASSERT_ONCE(index >= 0);

  if (pTabWidget->tabText(index) == qsTabName)
    return qsTabName; 

  pTabWidget->setTabText(index, qsTabName); 
  return qsTabName; 
}

void OdqConsoleTab::putMsgText(const QString &qsText, 
                               TMsgType type) // = enMsgTextNormal
{
  getCmdContext().accumulateScriptData(qsText, type); // check for script accumulation

  if (!getCmdContext().isEchoOn(type))
    return;

  QString qsMessage(qsText);

  if ((type & enMsgTextPrompted) != 0)
  {
    QString sFormat("%1%2");
    if (!(type & enMsgTextByInput)) // is not full duplicate of input string
      sFormat = "; %1%2";

    qsMessage = sFormat.arg(getCommandQPrompt()).arg(qsMessage);
  }

  if (!m_sTraceBuf.isEmpty())
  {
    QString qsTrace = getIApp()->toQString(m_sTraceBuf);
    m_sTraceBuf.empty();
    if (qsTrace.lastIndexOf('\n') < (qsTrace.length() - 1))
      qsTrace += '\n';
    m_pOut->append(qsTrace);
  }
  m_pOut->append(qsMessage);

  // move cursor to end of text

  //http://www.qtforum.org/post/54097/Auto-scroll-with-QTextEdit.html
  // It should but does not work here
  // TODO
  QTextCursor cursor(m_pOut->textCursor());
  cursor.movePosition(QTextCursor::End); //QTextCursor::MoveAnchor))
  m_pOut->setTextCursor(cursor);
  m_pOut->ensureCursorVisible();

  qApp->processEvents(); // show it
}

void OdqConsoleTab::putMsgText(const OdString &sText, 
                               TMsgType type) // = enMsgTextNormal
{
  QString qsText = getIApp()->toQString(sText);
  putMsgText(qsText, type);
}

OdString& OdqConsoleTab::traceBuf()
{
  return m_sTraceBuf;
}

OdString OdqConsoleTab::getCommandPrompt()
{
  OdString sPrompt(getIApp()->toOdString(getCommandQPrompt()));
  return sPrompt;
}

QString OdqConsoleTab::getCommandQPrompt()
{
  QString qsPrompt(getTopCmdState().m_qsCommandPrompt);
  ODA_ASSERT_ONCE(!qsPrompt.isEmpty());

  int pos = qsPrompt.lastIndexOf("\n");
  if (pos >= 0)
  {
    putMsgText(qsPrompt.left(pos));
    qsPrompt = qsPrompt.mid(++pos);
    getTopCmdState().m_qsCommandPrompt = qsPrompt;

    if (pos == (getTopCmdState().m_qsPromptAndKeywords.lastIndexOf("\n") + 1))
      getTopCmdState().m_qsPromptAndKeywords = getTopCmdState().m_qsPromptAndKeywords.mid(pos);
    else
      ODA_ASSERT_ONCE(getTopCmdState().m_qsPromptAndKeywords.isEmpty());
  }

  qsPrompt += ": ";
  return qsPrompt;
}

static QString getSuitableKeyword(const QStringList& lstKeywords, const QString& cqsRef)
{
  if (cqsRef.isEmpty())
    return QString();

  //if (lst.contains(qsRef, Qt::CaseInsensitive))
  QString qsRef = cqsRef.toLower();
  QString qsKeyword;
  foreach (QString qsKey, lstKeywords)
  {
    ODA_ASSERT_ONCE(!qsKey.isEmpty());

    // strange but the next does not work without toLower (qsRef="dwg", qsKey="DWg")
    //int pos = qsRef.indexOf(qsKey, Qt::CaseInsensitive);
    int pos = qsRef.indexOf(qsKey.toLower());
    if (pos < 0)
      continue;
    ODA_ASSERT_ONCE(qsKeyword.isEmpty() || qsKeyword.length() < qsKey.length());

    qsKeyword = qsKey;
    if (qsKeyword.length() != qsRef.length())
      continue;
    break;
  }
  return qsKeyword;
}

QString OdqConsoleTab::getQPromptWithLinks()
{
  QString qsPrompt = getCommandQPrompt(),
          qsKeywords = getTopCmdState().m_qsPromptAndKeywords; // as "prompt: keywords"
  int pos = qsKeywords.indexOf(qsPrompt);
  if (!pos)
    qsKeywords.replace(qsPrompt, "");
  else
  {
    ODA_ASSERT_ONCE(qsKeywords.isEmpty())
    if (!qsKeywords.isEmpty())
      return qsPrompt;
  }

  QString qsDefault;
  pos = qsPrompt.indexOf("<"); 
  if (pos >= 0)
  {
    qsDefault = qsPrompt.mid(pos + 1);
    qsPrompt.replace(pos, 1, "&lt;"); // ( "<", "&lt;")
    pos = qsDefault.lastIndexOf(">"); 
    if (pos >= 0)
    {
      qsDefault = qsDefault.left(pos);

      pos = qsPrompt.lastIndexOf(">"); 
      ODA_ASSERT_ONCE(pos > 0);
      qsPrompt.replace(pos, 1, "&gt;"); // (">", "&gt;")
    }
  }

  QStringList lstKeywords = qsKeywords.split(" ", QString::SkipEmptyParts);

  int from = 0;
  while ((pos = qsPrompt.indexOf("[", from)) >= from)
  {
    from = pos + 1;
    QString qsHead = qsPrompt.left(pos);
    QString qsRefs = qsPrompt.mid(++pos);
    pos = qsRefs.indexOf("]"); 
    ODA_ASSERT_ONCE(pos >= 0); // check for []
    if (pos < 0)
      break;
    QString qsTail = qsRefs.mid(pos + 1);
    qsRefs = qsRefs.left(pos);

    QStringList lstRefs = qsRefs.split("/");
    qsRefs.clear();
    int index = -1;
    foreach (QString qsRef, lstRefs)
    {
      index++;
      if (qsRef.isEmpty())
      {
        ODA_ASSERT_ONCE(index == (lstRefs.count() - 1));
        continue;
      }

      QString qsLink = qsRef;

      QString qsKeyword = getSuitableKeyword(lstKeywords, qsRef);
      if (!qsKeyword.isEmpty())
        qsLink = QString("<a href=\"%1\">%2</a>").arg(qsKeyword).arg(qsRef);
      if (!qsRefs.isEmpty())
        qsRefs += "/";
      qsRefs += qsLink;
    }
    // unite back
    qsPrompt = qsHead + "[" + qsRefs + "]";
    from = qsPrompt.length();
    qsPrompt += qsTail;
  } // while

  QString qsKeyword = " "; // empty value as default
  if (!qsDefault.isEmpty())
  {
    qsKeyword = getSuitableKeyword(lstKeywords, qsDefault);
    if (qsKeyword.isEmpty())
    {
      // for such things as folder path, 
      //   current visual style name (maybe with spaces) of vscurrent command
      //   "Enter name of Block to clone entities ...
      //   ...
      //ODA_ASSERT_ONCE(getTopCmdState().m_optionsRequestToGet & OdEd::kGstAllowSpaces); // test
      qsKeyword = qsDefault;
    }
  }
  
  if (!qsKeyword.isEmpty())
  {
    qsKeyword.replace("\"", "&quot;");
    qsPrompt.replace("&lt;" + qsDefault + "&gt;", 
                     "&lt;<a href=\"" + qsKeyword + "\">" + qsDefault + "</a>&gt;");
  }
  return qsPrompt;
}

// slot
void OdqConsoleTab::keywordByParam(const QString &cqsParam)
{
  ODA_ASSERT_ONCE(!cqsParam.isEmpty());
  QString qsParam = cqsParam.trimmed(); // may be empty via empty default value (zoom command for example)

  CommandState& stateCmd = getTopCmdState();
  if (   !stateCmd.m_bRequestToGetString
      && !stateCmd.m_bRequestToGetPoint)
  {
    // test // ODA_FAIL_ONCE();
    m_pIn->setText(qsParam);
    executeCommandByInput();
    return;
  }

  ODA_ASSERT_ONCE(   !stateCmd.m_bGetStringReady
                  && !stateCmd.m_bGetPointReady);

  stateCmd.m_qsGetString = qsParam;
  stateCmd.m_bGetStringReady = true;
  //stateCmd.m_bGetPointReady = false;

  stateCmd.m_bRequestToGetString = false;
  stateCmd.m_pStringTracker = NULL;
  stateCmd.m_bRequestToGetPoint = false;
  stateCmd.m_pErrCtx = NULL;
}

// slot
void OdqConsoleTab::executeCommandByInput()
{
  CommandState& stateCmd = getTopCmdState();
  if (   stateCmd.m_bRequestToGetString
      || stateCmd.m_bRequestToGetPoint)
  {
    ODA_ASSERT_ONCE(   !stateCmd.m_bGetStringReady
                    && !stateCmd.m_bGetPointReady);

    stateCmd.m_qsGetString = m_pIn->text();
    stateCmd.m_bGetStringReady = true;
    //stateCmd.m_bGetPointReady = false;

    stateCmd.m_bRequestToGetString = false;
    stateCmd.m_pStringTracker = NULL;
    stateCmd.m_bRequestToGetPoint = false;
    stateCmd.m_pErrCtx = NULL;

    return;
  }
#ifdef ANDROID
  QInputMethod *inputMethod = QApplication::inputMethod();
  inputMethod->commit();
#endif
  QString qsCmd(m_pIn->text());

#ifdef ANDROID
  if (!qsCmd.isEmpty())
      qsCmd = qsCmd.toLower();
#endif

  m_pIn->clear();

  QPointer<OdqConsoleTab> pTestThis = this;

  OdVariant vRes = executeCommand(qsCmd);

  if (pTestThis.isNull())
    // this was deleted in executeCommand via <mark to delete tab with end of command>
    return;

  putResultAsText(vRes);
}

void OdqConsoleTab::putResultAsText(const OdVariant &vRes)
{
  QString qsResType,
          qsResValue;
  try
  {
    switch (vRes.varType())
    {
    case OdVariant::kVoid:
      break;
    case OdVariant::kBool:
      qsResType = "kBool";
      qsResValue = vRes.getBool() ? "TRUE" : "FALSE";
      break;
    case OdVariant::kString:
      qsResType = "kString";
      qsResValue = getIApp()->toQString(vRes.getString());
      qsResValue = QString("\"%1\"").arg(qsResValue);
      break;
    case OdVariant::kInt8: // case OdVariant::kUInt8:
      qsResType = "kInt8";
      qsResValue = QString::number(vRes.getInt8());
      break;
    case OdVariant::kInt16: // case OdVariant::kUInt16:
      qsResType = "kInt16";
      qsResValue = QString::number(vRes.getInt16());
      break;
    case OdVariant::kInt32: // case OdVariant::kUInt32:
      qsResType = "kInt32";
      qsResValue = QString::number(vRes.getInt32());
      break;
    case OdVariant::kInt64: // case OdVariant::kUInt64:
      qsResType = "kInt64";
      qsResValue = QString::number(vRes.getInt64());
      break;
    case OdVariant::kDouble:
      qsResType = "kDouble";
      qsResValue = QString::number(vRes.getDouble());
      break;
    case OdVariant::kWString: // case OdVariant::kAnsiString:
      qsResType = "kWString";
      ODA_FAIL_ONCE(); // TODO
      qsResValue = getIApp()->toQString(OdString(vRes.getAnsiString()));
      qsResValue = QString("\"%1\"").arg(qsResValue);
      break;
    case OdVariant::kRxObjectPtr:
      qsResType = "kRxObjectPtr";
      ODA_FAIL_ONCE(); // TODO
      if (vRes.getRxObjectPtr().isNull())
        qsResValue = "NULL";
      else
      {
        QString qsClassName = getIApp()->toQString(vRes.getRxObjectPtr()->isA()->name());

        OdDbObjectPtr pObject =  OdDbObject::cast(vRes.getRxObjectPtr());
        if (!pObject.isNull())
        {
          QString qsHandle(getIApp()->toQString(pObject->handle().ascii()));
          qsResValue = QString("handle %1 of class %2").arg(qsHandle).arg(qsClassName);
        }
        else
          qsResValue = QString("class %1").arg(qsClassName);
      }
      break;
    case OdVariant::kNextType:
      qsResType = "kNextType";
      ODA_FAIL_ONCE(); // TODO //qsResValue = 
      break;
    case OdVariant::kByRef:
      qsResType = "kByRef";
      ODA_FAIL_ONCE(); // TODO //qsResValue = 
      break;
    case OdVariant::kArray:
      qsResType = "kArray";
      ODA_FAIL_ONCE(); // TODO //qsResValue = 
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      break;
    }
  }
  catch (const OdError&) // err
  {
    ODA_FAIL_ONCE();
  }

  if (!qsResType.isEmpty())
  {
    QString qsMsg;
    if (!qsResValue.isEmpty())
      qsMsg = QObject::tr("    return(type is %1) = %2").arg(qsResType).arg(qsResValue);
    else
      qsMsg = QObject::tr("    return(type is %1)").arg(qsResType);

    putMsgText(qsMsg);
  }
  else
    ODA_ASSERT_ONCE(qsResValue.isEmpty());
}


// slot
void OdqConsoleTab::newCommand()
{
  getConsole().postponeCommand("new", true, getRxDatabase());
}

// slot
void OdqConsoleTab::openCommand()
{
  getConsole().postponeCommand("open", true, getRxDatabase());
}

// slot
void OdqConsoleTab::cancelCommand()
{
  // escape was pressed

  QString qsText(m_pIn->text());
  if (!qsText.isEmpty())
  {
    m_pIn->setType(enHintCommand);
    m_pIn->clear();
    return;
  }

  CommandState& stateCmd = getTopCmdState();
  if (   (   !stateCmd.m_bRequestToGetString
          || stateCmd.m_bGetStringReady)
      && (   !stateCmd.m_bRequestToGetPoint
          || stateCmd.m_bGetPointReady))
  {
    OdSelectionSetPtr pSSet = workingSelectionSet();
    if (!pSSet.isNull() && pSSet->numEntities())
    {
      QString qsCmd = "ai_deselect"; // deselect all
      getConsole().postponeCommand(qsCmd, false, getRxDatabase());
    }
    return;
  }

  stateCmd.m_qsGetString = MARKER_TO_CANCEL;
  stateCmd.m_bGetStringReady = true;
  //stateCmd.m_bGetPointReady = false;

  stateCmd.m_bRequestToGetString = false;
  stateCmd.m_pStringTracker = NULL;
  stateCmd.m_bRequestToGetPoint = false;
  stateCmd.m_pErrCtx = NULL;
}

// slot
void OdqConsoleTab::eraseCommand()
{
  if (m_levelUndoBlock)
    return;

  OdSelectionSet* pSSet = workingSelectionSet();
  if (!pSSet || !pSSet->numEntities())
    return; // nothing to erase

  //OdDbDatabase* pDwgDb = OdDbDatabase::cast(getRxDatabase().get()).get();
  //if (!pDwgDb)
  //{
  //  bool bTestDgnGrips = getIAppProps()->toBool(OD_T("Vars/dgngrips"), false);
  //  if (!bTestDgnGrips)
  //    return;
  //}

  QString qsCmd = "_erase";
  getConsole().postponeCommand(qsCmd, true, getRxDatabase());
}

// slot
void OdqConsoleTab::undoCommand()
{
  if (m_levelUndoBlock)
    return;

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(getRxDatabase().get()).get();
  OdExtDbModule* pExtDb = NULL;
  if (!pDwgDb)
    pExtDb = getIApp()->getExtDbModule(getRxDatabase());
  if (!pDwgDb && !pExtDb)
    return;

  if ((pDwgDb && !pDwgDb->hasUndo()) || (pExtDb && !pExtDb->hasUndo(getRxDatabase())))
    return;

  QString qsCmd = "_u"; // "undo"
  getConsole().postponeCommand(qsCmd, true, getRxDatabase());
}

// slot
void OdqConsoleTab::redoCommand()
{
  if (m_levelUndoBlock)
    return;

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(getRxDatabase().get()).get();
  OdExtDbModule* pExtDb = NULL;
  if (!pDwgDb)
    pExtDb = getIApp()->getExtDbModule(getRxDatabase());
  if (!pDwgDb && !pExtDb)
    return;

  if ((pDwgDb && !pDwgDb->hasRedo()) || (pExtDb && !pExtDb->hasRedo(getRxDatabase())))
    return;

  QString qsCmd = "_mredo 1"; // "redo"
  getConsole().postponeCommand(qsCmd, true, getRxDatabase());
}

// slot
void OdqConsoleTab::selectAllCommand()
{
  if (m_levelUndoBlock)
    return;

  OdRxObject* pRxDatabase = getRxDatabase().get();
  if (!pRxDatabase)
    return;
  //if (!OdDbDatabase::cast(pRxDatabase).get()) // skip for DGN
  //  return;

  QString qsCmd = "ai_selall";
  getConsole().postponeCommand(qsCmd, true, getRxDatabase());
}

//slot
void OdqConsoleTab::timeTextBlink()
{
  m_bBlinkTextState = !m_bBlinkTextState;
}

class SaveViewStringTracker
{
  OdqConsoleTab& m_tab;
  OdRxObject* m_pRxDatabase;
  OdEdStringTracker* m_pTracker;
  OdSharedPtr<QCursor> m_pCursorSave;
  IqView* m_iView;

  QTimer* m_pTimer;

public:
  SaveViewStringTracker(OdqConsoleTab& tab,
                        OdEdStringTracker* pTracker, 
                        const QCursor* pQCursor = NULL)
    : m_tab(tab)
    , m_pTracker(pTracker)
    , m_iView(NULL)
    , m_pTimer(NULL)
  {
    IqViewSystem* iVS = getIViewSystem();
    if (!iVS)
      return;
    m_iView = iVS->getView(m_tab.getRxDatabase());
    ODA_ASSERT_ONCE(m_iView);
    if (!m_iView)
      return;

    if (m_pTracker)
    {
      m_pTracker->addDrawables(m_iView->getActiveTopGsView());
      m_tab.m_bBlinkTextState = true;
      m_pTracker->setCursor(true); // blinker
      int	msBlink = qApp->cursorFlashTime(); // or via WINAPI GetCaretBlinkTime()
      if (msBlink <= 0)
        msBlink = 1000;

      m_pTimer = new QTimer();
      if (!QObject::connect(m_pTimer, SIGNAL(timeout()), &m_tab, SLOT(timeTextBlink())))
      {
        ODA_FAIL_ONCE();
      }
      m_pTimer->start(msBlink);
    }

    if (pQCursor)
    {
      m_pCursorSave = new QCursor(m_iView->cursor());
      m_iView->setCursor(*pQCursor);
    }
  }

  ~SaveViewStringTracker()
  {
    if (!m_iView)
      return;

    if (m_pTracker)
    {
      m_pTracker->removeDrawables(m_iView->getActiveTopGsView());
      if (m_pTimer)
      {
        m_pTimer->stop();
        delete m_pTimer;
        m_tab.m_bBlinkTextState = false;
      }
    }

    if (m_pCursorSave.get())
      m_iView->setCursor(*m_pCursorSave);
  }
};

OdString OdqConsoleTab::getPromptedString(int options, 
                                          OdEdStringTracker* pTracker) // = NULL
{
  CommandState& stateCmd = getTopCmdState();

  ODA_ASSERT_ONCE(   !stateCmd.m_bRequestToGetString
                  && !stateCmd.m_bRequestToGetPoint);
  stateCmd.m_bRequestToGetString = true;
  stateCmd.m_pStringTracker = pTracker;
  stateCmd.m_bRequestToGetPoint = false;
  stateCmd.m_optionsRequestToGet = options;

  OdString sValue;
  try
  {
    OdSharedPtr<SaveViewStringTracker> pSetTracket;
    if (pTracker)
      pSetTracket = new SaveViewStringTracker(*this, pTracker);

    getPromptedData(); // throw OdEdOtherInput by string

    ODA_FAIL_ONCE(); // should be inaccessible in runtime
  }
  catch (const OdEdOtherInput& other)
  {
    sValue = other.string();
  }

  return sValue;
}

OdGePoint3d OdqConsoleTab::getPromptedPoint(int options) // throw OdEdOtherInput by string
{
  CommandState& stateCmd = getTopCmdState();

  ODA_ASSERT_ONCE(   !stateCmd.m_bRequestToGetString
                  && !stateCmd.m_bRequestToGetPoint);
  stateCmd.m_bRequestToGetString = false;
  stateCmd.m_pStringTracker = NULL;
  stateCmd.m_bRequestToGetPoint = true;
  stateCmd.m_optionsRequestToGet = options;

  return getPromptedData();
}

// used as common for getPromptedString and getPromptedPoint
OdGePoint3d OdqConsoleTab::getPromptedData() // throw OdEdOtherInput by string
{
  QString qsPromptWithLinks = getQPromptWithLinks();

  CommandState& stateCmd = getTopCmdState();

  ODA_ASSERT_ONCE(   !stateCmd.m_bGetStringReady
                  && !stateCmd.m_bGetPointReady
                  && (stateCmd.m_qsGetString.isEmpty() || stateCmd.m_qsGetString == MARKER_TO_CANCEL)
                  && (stateCmd.m_bRequestToGetPoint ^ stateCmd.m_bRequestToGetString));

  stateCmd.m_bGetStringReady = false;
  stateCmd.m_bGetPointReady = false;
  stateCmd.m_qsGetString.clear();
  if (!isErrorExecuteAfterCancel(stateCmd.m_pErrCtx))
    stateCmd.m_pErrCtx = NULL;
  else
    ODA_ASSERT_ONCE(true); // brk
  bool bBlinkTextState = m_bBlinkTextState;

  QString qsValue;
  m_pIn->clear();

  QPointer<OdqConsoleTab> pTestThis = this;
  CommandContextImpl* pCmdContextAtTestThis = &getCmdContext(); // #8027

  if (stateCmd.m_pErrCtx.isNull())
  {
    QString qsSvLabel(m_pLabel->text()),
            qsStringTrackerText;
    THintType typeSv = m_pIn->type(); 
    // unfortunately m_pIn->type() value is invalid to restore as already changed here
    if (!qsSvLabel.indexOf(CommandState::getDefaultQPrompt()))
      typeSv = enHintCommand;

    OdGePoint3d ptPrev = stateCmd.m_ptGetPoint;

    do
    {
      if (m_pLabel->text() != qsPromptWithLinks)
        m_pLabel->setText(qsPromptWithLinks); // m_pLabel->setText(qsPrompt);
      else if (stateCmd.m_pStringTracker)
      {
        if (qsStringTrackerText != m_pIn->text())
        {
          qsStringTrackerText = m_pIn->text();
          stateCmd.m_pStringTracker->setValue(getIApp()->toOdString(qsStringTrackerText));
          getIViewSystem()->updateViews(getRxDatabase());
        }
        if (bBlinkTextState != m_bBlinkTextState)
        {
          stateCmd.m_pStringTracker->setCursor(bBlinkTextState = m_bBlinkTextState);
          getIViewSystem()->updateViews(getRxDatabase());
        }
      }

      qApp->processEvents();

      // moved below after problem by close 
      // the application during meaguring command
      //if (pTestThis.isNull()) // look also <mark to delete tab with end of command> // #12785
      //  throw OdError(eNoDatabase); // "No Database"

      if (!stateCmd.m_pErrCtx.isNull())
      {
        stateCmd.m_bGetStringReady = false;
        stateCmd.m_bGetPointReady = false;
        break;
      }

      if (pTestThis.isNull()) // look also <mark to delete tab with end of command> // #12785
        throw OdError(eNoDatabase); // "No Database"

      //if (   stateCmd.m_bRequestToGetPoint
      //    && ptPrev != stateCmd.m_ptGetPoint)
      //{
      //  ptPrev = stateCmd.m_ptGetPoint;
      //
      //  OdqGripManagerPtr pPointTracker = stateCmd.m_pPointTracker;
      //  if (!pPointTracker.isNull())
      //    pPointTracker->setValue(stateCmd.m_ptGetPoint);
      //  else
      //      ODA_ASSERT_ONCE(true); // brk
      //}

    } while (!stateCmd.m_bGetStringReady && !stateCmd.m_bGetPointReady);

    if (stateCmd.m_bGetPointReady)
    {
      // Clear states

      ODA_ASSERT_ONCE(   stateCmd.m_bRequestToGetPoint
                      && !stateCmd.m_bGetStringReady
                      && stateCmd.m_qsGetString.isEmpty());

      stateCmd.m_bRequestToGetPoint = false;
      stateCmd.m_bGetPointReady = false;
      stateCmd.m_bGetStringReady = false;
      stateCmd.m_optionsRequestToGet = 0;
      stateCmd.m_pErrCtx = NULL;

      if (getCmdContext().isEchoOn(enMsgTextNormal))
      {
        QString qsPoint = QString("%1,%2").arg(stateCmd.m_ptGetPoint.x)
                                          .arg(stateCmd.m_ptGetPoint.y);
        if (stateCmd.m_ptGetPoint.z != 0.0)
          qsPoint += QString(",%1").arg(stateCmd.m_ptGetPoint.z);
        m_pIn->addToHistory(qsPoint);
      }

      m_pLabel->setText(qsSvLabel); // restore
      m_pIn->setType(typeSv);
      m_pIn->clear();

      return stateCmd.m_ptGetPoint;
    }

    if (pTestThis.isNull())
    {
      pCmdContextAtTestThis->m_pRxDb = NULL; // #8027
      throw OdEdCancel();
    }

    qsValue = stateCmd.m_qsGetString;
    int pos = -1;
    if (!(stateCmd.m_optionsRequestToGet & OdEd::kGstAllowSpaces) && (pos = qsValue.indexOf(" ")) >= 0)
    {
      if (!pos)
        pos = (qsValue = qsValue.trimmed()).indexOf(" ");
      if (pos > 0 && qsValue.indexOf("\"") != 0)
      {
        ODA_ASSERT_ONCE(stateCmd.m_pStringIO.isNull() || stateCmd.m_pStringIO->isEof());
        OdString sNext = getIApp()->toOdString(qsValue.mid(pos)).trimLeft();
        stateCmd.m_pStringIO = ExStringIO::create(sNext); // attach for next input
        qsValue = qsValue.left(pos);
      }
    }

    stateCmd.m_bRequestToGetString = false;
    stateCmd.m_pStringTracker = NULL;
    stateCmd.m_bGetStringReady = false;
    stateCmd.m_qsGetString.clear();
    stateCmd.m_bRequestToGetPoint = false;

    if (!pTestThis.isNull())
    {
      if (   !qsValue.isEmpty() && qsValue != MARKER_TO_CANCEL
          && stateCmd.m_pErrCtx.isNull()
          && getCmdContext().isEchoOn(enMsgTextNormal))
        m_pIn->addToHistory(qsValue);

      m_pLabel->setText(qsSvLabel); // restore
      m_pIn->setType(typeSv);
      m_pIn->clear();
    }

    if (!isErrorExecuteAfterCancel(stateCmd.m_pErrCtx))
      stateCmd.m_pErrCtx = NULL;

    if (qsValue == MARKER_TO_CANCEL)
      throw OdEdCancel();

  } // if (stateCmd.m_pErrCtx.isNull())
  else
    ODA_ASSERT_ONCE(true); // brk

  if (!stateCmd.m_pErrCtx.isNull())
  {
    //stateCmd.m_pErrCtx.detach()->proceedThrow();
    throw OdException(stateCmd.m_pErrCtx.detach());

    //stateCmd.m_pErrCtx = NULL;
  }
  OdString sValue = getIApp()->toOdString(qsValue);

  bool bPrecanceled = false;
  while (!sValue.trimLeft().find(L"^C"))
  {
    sValue = sValue.mid(2);
    if (bPrecanceled)
      continue;
    bPrecanceled = true;
  }
  if (bPrecanceled)
  {
    sValue.trimRight();
    if (stateCmd.m_pStringIO.get() && !stateCmd.m_pStringIO->isEof())
    {
      if (!sValue.isEmpty())
        sValue += L" ";
      sValue += stateCmd.m_pStringIO->getInitString();
      stateCmd.m_pStringIO->reset(L"");
    }

    bool bEcho = getCmdContext().isEchoOn();
    if (getCmdContext().markToCancelAndExecute(sValue, bEcho))
      throw OdEdCancel();
  }
  throw OdEdOtherInput(sValue);
}

// via flags of first input command
bool OdqConsoleTab::checkToCancelCommandWithUndo(const OdString &sCmd)
{
  if (!getCmdContext().hasCmdWithUndo())
    return false;

  // via flags of first command in sequence

  OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(sCmd);

  while (pStringIO.get() && !pStringIO->isEof())
  {
    // after ConsoleIoImpl::getString
    OdString sCmdFirst(pStringIO->getString(OD_T(""), // any
                       OdEd::kInpDefault, // options
                       NULL));
    ODA_ASSERT_ONCE(!sCmdFirst.isEmpty());

    if (sCmdFirst == OD_T("{"))
      continue;

    //sCmdFirst.makeUpper();

    QString qsCmdName(getIApp()->toQString(sCmdFirst)); // qsCmdName = qsCmdName.toLower();
    OdEdCommandStackPtr pCommands = ::odedRegCmds();
    OdEdCommandPtr pCmd = pCommands->lookupCmd(sCmdFirst);
    // possible via variables //ODA_ASSERT_ONCE(pCmd.get());
    if (pCmd.isNull())
      break; // return false;
    OdInt32 flgs = pCmd->flags();
    if (GETBIT(flgs, OdEdCommand::kNoUndoMarker))
      break; // return false;

    return true;
  } // while

  return false;
}

OdVariant OdqConsoleTab::executeCommand(const OdString &sCmd,
                                        bool bEcho, // = true // less priority than echo... commands
                                        bool bSubCommand) // = false
{
  QString qsCmd(getIApp()->toQString(sCmd));

  return executeCommand(qsCmd, bEcho, bSubCommand);
}

static void PutMsgError(OdqConsoleTab* pTestThis,
                        const OdError& err,
                        const QString& qsFormat = "",
                        int timeout = 1000)
{
  if (!pTestThis || !err.context()) // requested without message
    return;

  QString qsMessage(getIApp()->toQString(err.description()));

  if (qsMessage.isEmpty())
    return;

  ODA_ASSERT_ONCE(qsMessage.indexOf("%1") < 0 && qsMessage.indexOf("%ls") < 0);
  int pos = qsMessage.indexOf(MARKER_TO_SKIP_TRANSLATION); // "##"
  if (!pos)
    // remove marker only
    qsMessage = qsMessage.mid(QString(MARKER_TO_SKIP_TRANSLATION).length());
  else
  {
    ODA_ASSERT_ONCE(pos < 0);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qsMessage = QObject::tr(qsMessage.toAscii());
#else
    qsMessage = QObject::tr(qsMessage.toLatin1());
#endif
  }

  if (!qsFormat.isEmpty())
  {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qsMessage = QObject::tr(qsFormat.toAscii()).arg(qsMessage);
#else
    qsMessage = QObject::tr(qsFormat.toLatin1()).arg(qsMessage);
#endif
  }

  pTestThis->putMsgText(qsMessage);

  if (!timeout)
    return;

  getIApp()->toStatusBar(qsMessage, timeout);
}

OdVariant OdqConsoleTab::executeCommand(const QString& cqsCmd,
                                        bool bEcho, // = true // less priority than echo... commands
                                        bool bSubCommand) // = false   
{
  QPointer<OdqConsoleTab> pTestThis = this;

  OdString sCmdAfterCancel;
  bool bEchoAfterCancel = false;

  QString qsCmd = cqsCmd.trimmed();
  while (!qsCmd.indexOf('\''))
  {
    qsCmd = qsCmd.mid(1);
    if (bEcho)
      bEcho = false;
  }
  OdString sCmd(getIApp()->toOdString(qsCmd));
  
  bool bPrecanceled = false;
  while (!sCmd.trimLeft().find(L"^C"))
  {
    sCmd = sCmd.mid(2);
    if (bPrecanceled)
      continue;
    bPrecanceled = true;
  }
  if (!bPrecanceled && checkToCancelCommandWithUndo(sCmd))
    bPrecanceled = true;

  if (bPrecanceled)
  {
    if (getCmdContext().markToCancelAndExecute(sCmd, bEcho)) // sCmdAfterCancel
      return OdVariant();
  }

  if (!bEcho)
  {
    if (qsCmd.isEmpty())
    {
      return OdVariant();
    }

    getCmdContext().m_levelCmdToOffEcho--; // off
  }

  bool bOwnerOfState = getTopCmdState().isAllocated();
  if (bOwnerOfState)
  {
    int levelCmd = 0;
    if (bSubCommand)
    {
      levelCmd = getTopCmdState().m_levelCmd + 1;
    }
    getCmdContext().m_stackCommandStates.push_back(CommandState());
    ODA_ASSERT_ONCE(!getTopCmdState().m_levelCmd);
    getTopCmdState().m_levelCmd = levelCmd;
  }

  CommandState& stateCmd = getTopCmdState();
  ODA_ASSERT_ONCE(   !stateCmd.m_bInUse
                  && stateCmd.isDefaultPrompt() && stateCmd.m_pStringIO.isNull());
  stateCmd.m_bInUse = true; // lock for command

  stateCmd.m_qsCommandEcho = qsCmd;

  //putCmdEchoText(qsCmd);
  //if (sCmd.isEmpty())
  //  return true;
  //OdSaveState<int> save_levelCmd(getCmdContext().m_levelCmd);
  //getCmdContext().m_levelCmd++;

  if (!stateCmd.m_qsCommandEcho.isEmpty())
  {
    try
    {
      OdEdCommandStackPtr pCommands = ::odedRegCmds();

      //OdDbBaseDatabasePEPtr pDB = OdDbBaseDatabasePE::cast(pRxDatabase);
      //if (!pDB.isNull() && pDB->appServices(getRxDatabase())->getPICKFIRST())
      //{
      //  //ODA_FAIL_ONCE(); // TODO
      //  //getCmdContext().setPickfirst(selectionSet());
      //}

      //OdString sCmdName = sCmd.spanExcluding(L" \t\r\n");
      //if (sCmdName.getLength() == sCmd.getLength())
      //{
      //  sCmdName.makeUpper();
      //  getTopCmdState().m_qsLastInput = sCmdName;
      //  pCommands->executeCommand(sCmdName, &getCmdContext());
      //}
      //else
      //{

      //------------------------------------------------------------------------

      stateCmd.m_pStringIO = ExStringIO::create(sCmd); // ExStringIO::create(sCmd);
      // the next code may contain cmd stack readjusting
      // therefor stateCmd is not valid to use (should be getTopCmdState() )

      IqViewSystem* iVS = getIViewSystem();

      while (getTopCmdState().m_pStringIO.get() && !getTopCmdState().m_pStringIO->isEof())
      {
        OdString sCmdName;
        try
        {
          sCmdName = getCmdContext().userIO()->getString(getTopCmdState().getPrompt());
          if (sCmdName.isEmpty()) // possible vis ^J ^M
            continue;
          //sCmdName.makeUpper();
          getTopCmdState().m_qsLastInput = sCmdName;

          // PICKFIRST - means use current selection in command
          if (   getRxDatabase() 
              && workingSelectionSet()) // && workingSelectionSet()->numEntities())
          {
            OdEdCommandContext* pCmdCtx = &getCmdContext();
            OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
            ODA_ASSERT_ONCE(!pSysVarPE.isNull());
            if (!pSysVarPE.isNull())
            {
              OdString sValue = pSysVarPE->valueToString(pCmdCtx, L"PICKFIRST").makeLower();
              ODA_ASSERT_ONCE(   sValue == L"0" || sValue == L"1" // DWG
                              || sValue == L"false" || sValue == L"true"); // DGN
              bool bValue = !(sValue.isEmpty() || sValue == L"0" || sValue == L"false");
              if (bValue)
                getCmdContext().setPickfirst(workingSelectionSet());
            }
          }

          // is needed for end reactors after cmd like saveas ...tig 
          // via changed commandEnded in ModuleReactors.cpp
          OdEdCommandContextPtr pCmdCtxHolder = &getCmdContext();
          OdRxObjectPtr pRxDbHolder = pCmdCtxHolder->baseDatabase();

          pCommands->executeCommand(sCmdName, pCmdCtxHolder);
          //OdEdCommandPtr pCmd = pCommands->lookupCmd(sCmdName);
          //if (pCmd.isNull())
          //  pCmd = s_pCmdReactor.unknownCommand(sCmdName, &getCmdContext());
          //else
          //{
          //  //ExCommandPtr pExCmd = ExCommand::cast(pCmd);
          //  //if (!pExCmd.isNull())
          //  pCommands->executeCommand(pCmd, &getCmdContext());
          //  //else
          //  //{
          //  //  getTopCmdState().putCmdStringEcho(&getCmdContext()); // it's here to resolve echo problem
          //  //  QString qsMessage(QObject::tr("Error: Command \"%1\" cannot work without DWG file.")
          //  //                         .arg(getIApp()->toQString(sCmdName)));
          //  //  //getCmdContext().userIO()->putString(getIApp()->toQString(qsMessage));
          //  //  // or
          //  //  putMsgText(qsMessage);
          //  //}
          //}
        }
        catch (const OdEdEmptyInput&)
        {
          ODA_FAIL_ONCE(); // test
        }

        if (pTestThis.isNull())
          return OdVariant();

        // update Gl views and grips after commands of TX SDK
        OdEdCommandPtr pCmd = pCommands->lookupCmd(sCmdName);
        if (pCmd.get())
        {
          OdInt32 flgs = pCmd->flags();
          //if (iVS && !GETBIT(flgs, OdEdCommand::kNoUndoMarker))
          if (iVS)
            iVS->updateGL(getRxDatabase());
          OdqGripManagerPtr pGripManager = getCmdContext().gripManager();
          if (pGripManager.get() && !GETBIT(flgs, OdEdCommand::kNoUndoMarker))
            pGripManager->updateAllGrips();
        }

      } // end while
    }
    catch (const OdEdEmptyInput&)
    {
      ODA_FAIL_ONCE(); // test
    }
    catch (const ExCancelAndExecute& err)
    {
      if (pTestThis.isNull())
        return OdVariant();

      sCmdAfterCancel = err.getCommand();
      bEchoAfterCancel = err.getEcho();
    }
    catch (const OdEdCancel& err)
    {
      if (pTestThis.isNull())
        return OdVariant();

      PutMsgError(pTestThis, err);
    }
    catch (const OdError& err)
    {
      if (pTestThis.isNull())
        return OdVariant();

      PutMsgError(pTestThis, err, "Error: %1", 2000);
    }
  }
  else
  {
    // put prompt only with checking of echo state
    putMsgText(qsCmd, TMsgType(enMsgTextPrompted | enMsgTextByInput));
  }

  CommandState &stateCmdRes = getTopCmdState();
  //ODA_ASSERT_ONCE(&stateCmdRes == &stateCmd);

  ODA_ASSERT_ONCE(stateCmdRes.isAllocated());
  stateCmdRes.setPrompt(CommandState::getDefaultQPrompt()); // restore default // "Command"
  stateCmdRes.m_pStringIO = NULL;

  OdVariant vRes = stateCmdRes.m_vValue;
  stateCmdRes.m_vValue = OdVariant();

  OdSharedPtr<OdException> wrpSvError;

  if (!stateCmdRes.m_pErrCtx.isNull())
  {
    if (isErrorExecuteAfterCancel(stateCmdRes.m_pErrCtx))
    {
      const OdErrorExecuteCommandAfterCancelContext* pErrCtx
        = (const OdErrorExecuteCommandAfterCancelContext *) stateCmdRes.m_pErrCtx.get();
      sCmdAfterCancel = pErrCtx->getCommand();
      bEchoAfterCancel = pErrCtx->getEcho();
    }
    else
      ODA_FAIL_ONCE(); // test

    stateCmdRes.m_pErrCtx = NULL;
  }

  stateCmdRes.m_bInUse = false; // unlock

  if (!pTestThis.isNull() && !sCmdAfterCancel.isEmpty())
  {
    QString qsMessage 
      = QObject::tr("Command \"%1\" was canceled by next one")
      .arg(stateCmdRes.m_qsLastCommandName);
    if (bEcho)
      pTestThis->putMsgText(qsMessage);

    getIApp()->toStatusBar(qsMessage, 1000);
  }

  if (bOwnerOfState)
    getCmdContext().m_stackCommandStates.pop_back();

  if (!bEcho)
    getCmdContext().m_levelCmdToOffEcho++; // restore

  if (pTestThis.isNull())
    return vRes;
  if (!parentWidget()) // look also <mark to delete tab with end of command>
  {
    if (!bSubCommand)
    {
      ODA_ASSERT_ONCE(   !getCmdContext().hasCmdWithUndo(true)
                      && !getTopCmdState().m_levelCmd 
                      && getCmdContext().m_stackCommandStates.size() == 1);

      getConsole().removeConsoleTab(*this); // confirmToDeleteMarkedTab
    }
    else
      ODA_FAIL_ONCE(); // test
  }

  if (!sCmdAfterCancel.isEmpty())
    executeCommand(sCmdAfterCancel, bEchoAfterCancel);

  return vRes;
}

OdVariant OdqConsoleTab::executeCommandScript(const OdString& sScriptName, 
                                              bool* pbWasFound) // = NULL
{
  return executeCommandScript(getIApp()->toQString(sScriptName), pbWasFound);
}

OdVariant OdqConsoleTab::executeCommandScript(const QString& qsScriptName, 
                                              bool* pbWasFound) // = NULL
{
  if (pbWasFound)
    *pbWasFound = false;

  ODA_ASSERT_ONCE(!qsScriptName.isEmpty());

  QString qsScriptFileName(qsScriptName);
  int posExt = qsScriptName.lastIndexOf(".scr");
  if (   posExt <= 0 
      || posExt != (qsScriptName.length() - 4))
    qsScriptFileName += ".scr";

  if (!QFileInfo(qsScriptFileName).exists())
  {
    OdString sScriptFileName = getIApp()->toOdString(qsScriptFileName);
    sScriptFileName = getIApp()->getBaseHostAppServices()->findFile(sScriptFileName, m_pRxDatabase);
    if (sScriptFileName.isEmpty())
      return OdVariant();
    qsScriptFileName = getIApp()->toQString(sScriptFileName);
  }

  QFile file(qsScriptFileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) 
  {
    ODA_FAIL_ONCE();
    return OdVariant();
  }

  if (pbWasFound)
    *pbWasFound = true;

  QString qsScript;
  QVector<QString> aqsParams;
  while (!file.atEnd()) 
  {
    QByteArray line = file.readLine();
    QString qsLine(line);
    
    int posSemicolon = qsLine.indexOf(';');
    if (posSemicolon >= 0)
      qsLine = qsLine.left(posSemicolon);
    qsLine = qsLine.trimmed();

    if (qsLine.isEmpty())
      continue;
    if (!qsScript.isEmpty())
      qsScript += " ";

    for (int posParam = qsLine.indexOf('%');
         posParam >= 0; posParam = qsLine.indexOf('%'))
    {
      QRegExp regNum("(\\d+)");
      int posNum = regNum.indexIn(qsLine, posParam + 1);
      if (posNum <= 0)
      {
        ODA_FAIL_ONCE();
        QString qsMes(QObject::tr("Syntax error in file \"%1\" at \"%2\"")
                          .arg(qsScriptFileName)
                          .arg(qsLine.mid(posParam)));
        putMsgText(qsMes);
        throw OdEdCancel(); // return true;
      }
         
      QString qsNum = regNum.cap(0);

      int indexParam = qsNum.toInt() - 1;
      if (indexParam < 0 || indexParam > 20)
      {
        ODA_FAIL_ONCE();
        QString qsMes(QObject::tr("Invalid parameter value \"%1\" at \"%2\"")
                          .arg(qsScriptFileName)
                          .arg(qsLine.mid(posParam)));
        putMsgText(qsMes);
        throw OdEdCancel(); // return true;
      }

      qsScript += qsLine.left(qsLine[posParam - 1] != '\"' ? posParam : (posParam - 1));
      qsLine = qsLine.mid(posNum + qsNum.length());

      while (indexParam >= aqsParams.size())
      {
        CommandState& stateCmd = getTopCmdState();

        if (stateCmd.m_pStringIO->isEof())
            break;

        OdString sParam(stateCmd.m_pStringIO->getString(OdString(), 0, NULL));
        if (sParam.isEmpty())
        {
            ODA_FAIL_ONCE();
            break;
        }

        aqsParams.append(getIApp()->toQString(sParam));
      } // end while

      if (indexParam >= aqsParams.size())
      {
        qsScript.insert(0, "{");
        qsLine.insert(0, "}");
        continue;
      }

      QString &qsParam = aqsParams[indexParam];
      bool inQuotes = (qsLine == "\"");
      qsLine += qsParam;
      if (inQuotes)
        qsLine += '\"';

    } // end while 

    qsScript += qsLine;
  }

  OdVariant vResult = executeCommand(qsScript, true, true); // subcommand with echo
  return vResult;
}

//static double SnapToInc(double dValue,
//                        double dInc)
//{
//  double dRest = fmod(dValue, dInc);
//
//  dValue = dValue - dRest;
//
//  if (dRest > 0.0)
//  {
//    if (dRest > 0.5 * dInc)
//      dValue += dInc;
//  }
//  else
//  {
//    if (-dRest > 0.5 * dInc)
//      dValue -= dInc;
//  }
//
//  return dValue;
//}

//static 
void OdqConsoleTab::snapPoint(CommandState& stateCmd, IqView* iView,
                              OdGePoint3d& pt, // in / out
                              enum QEvent::Type typeEvent) // = QEvent::None)
{
  if (!stateCmd.m_bRequestToGetPoint || !iView
    //|| typeEvent == QEvent::TouchBegin || typeEvent == QEvent::TouchUpdate || typeEvent == QEvent::TouchEnd // TODO
      || typeEvent == QEvent::Wheel)
    return;

  OdqSnapManagerPtr pSnapManager = stateCmd.snapManager();
  if (pSnapManager.isNull() || GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptNoOSnap))
    return;
  OdGsView* pGsView = iView->getActiveTopGsView();
  if (!pGsView)
    return;

  const OdGePoint3d* pLastPt = pSnapManager->basePtRef();
  if (pSnapManager->snap(pGsView, pt, pLastPt))
  {
    OdGsModel* p2dModel = iView->getGsModel(true);
    if (p2dModel)
      p2dModel->onModified(pSnapManager.get(), (OdGiDrawable*)0);
    iView->updateGL();
    // snapping works better without next update
    //if (pLastPt)
    //  *const_cast<OdGePoint3d*>(pLastPt) = pt; // is a referent to instance inside class SaveViewParams
  }
}

bool OdqConsoleTab::getPoint(IqView* iView, const QPoint& ptEyes, // in
                             OdGePoint3d& pt, // out
                             enum QEvent::Type typeEvent) // = QEvent::None) // in
{
  CommandState& stateCmd = getTopCmdState();
  if (!iView->eyesTo(ptEyes, stateCmd.m_optionsRequestToGet, 
                     pt)) // out
    return false;
  if (!GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptNoOSnap))
    OdqConsoleTab::snapPoint(stateCmd, iView, pt, typeEvent);
  return true;
}

template <class T>
  static bool GetPoint(OdqConsoleTab& tab, IqView* iView, T* pEvent, // in
                       OdGePoint3d& pt) // out
  {
    // T:: is needed to fix compiling error on mac xcode
    QPoint ptEyes(pEvent->T::x(), pEvent->T::y());
    return tab.getPoint(iView, ptEyes, pt, pEvent->T::type());
  }


void OdqConsoleTab::updateKeyboardModifiers(QInputEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent);

  // update stateKey (used for ConsoleIoImpl::getKeyState())

  Qt::KeyboardModifiers mdfs = pEvent->modifiers();
  if (mdfs & Qt::ShiftModifier)
    m_stateKey |= OdEdBaseIO::kShiftIsDown;
  else
    m_stateKey &= ~OdEdBaseIO::kShiftIsDown;
  if (mdfs & Qt::ControlModifier)
    m_stateKey |= OdEdBaseIO::kControlIsDown;
  else
    m_stateKey &= ~OdEdBaseIO::kControlIsDown;
}

// slot
void OdqConsoleTab::slotMouseMove(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::MouseMove && !bAccepted);
  QMouseEvent* pMouseEvent = (QMouseEvent*) pEvent;
  updateKeyboardModifiers(pMouseEvent);

  CommandState& stateCmd = getTopCmdState();

  bool isPtValid = false;
  OdGePoint3d pt;

  if (GetPoint(*this, iView, pMouseEvent, pt))
  {
    isPtValid = true;
    bool bTracePt = true;
    OdGePoint3d ptWcs = pt,
                ptUcs = pt;

    OdUnitsFormatter* pBaseFormatter = NULL;
    OdRxObject* pRxDb = m_pRxDatabase;
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase);
    OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(m_pRxDatabase);
    if (pDbPE.isNull() && pExtDb)
      pDbPE = OdDbBaseDatabasePE::cast(pRxDb = pExtDb->getWrappedDatabase(pRxDb));
    if (!pDbPE.isNull())
      pBaseFormatter = pDbPE->baseFormatter(pRxDb);
    if (!pBaseFormatter && pExtDb)
      pBaseFormatter = pExtDb->getFormatter(pRxDb);
    ODA_ASSERT_ONCE(   pBaseFormatter 
                    || (pExtDb && pExtDb->baseModuleName(true) == L"bim")
                  //|| (pExtDb && pExtDb->baseModuleName(true) == L"ifc")
                    || (pExtDb && pExtDb->baseModuleName(true) == L"dwf"));
    if (pBaseFormatter)
    {
      QString qsPtTrace;
      if (OdDbDatabase::cast(m_pRxDatabase).get())
      {
        if (!GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptNoUCS))
        {
          OdDbUnitsFormatter* pFormatter = (OdDbUnitsFormatter*) pBaseFormatter;
          ptUcs = pFormatter->toUCS(ptWcs);
          // normalize Z
          if (ptUcs.x != 0.0 && OdZero(ptUcs.x))
            ptUcs.z = 0.0;
        }
        // more convenient to keep it always on //else bTracePt = false;
        ODA_ASSERT_VAR(else)
          ODA_ASSERT(true); // brk
      }

      if (bTracePt)
      {
        qsPtTrace += getIApp()->toQString(  pBaseFormatter->formatLinear(ptUcs.x)
                                          + OD_T(" ") + pBaseFormatter->formatLinear(ptUcs.y)
                                          + OD_T(" ") + pBaseFormatter->formatLinear(ptUcs.z));
        if (ptWcs != ptUcs)
          qsPtTrace += QObject::tr("    (WCS: %1)")
                          .arg(getIApp()->toQString(  pBaseFormatter->formatLinear(ptWcs.x)
                                                    + OD_T(" ") + pBaseFormatter->formatLinear(ptWcs.y)
                                                    + OD_T(" ") + pBaseFormatter->formatLinear(ptWcs.z)));
      }
      getIApp()->toStatusBar(qsPtTrace);
    }
  }

  if (!stateCmd.m_pPointTracker.isNull())
  {
    if (!stateCmd.m_bRequestToGetPoint)
    {
      OdqGripManagerPtr pGripManager = getCmdContext().gripManager();
      if (pGripManager.isNull())
        return;
      int x = pMouseEvent->x(),
          y = pMouseEvent->y();
      pGripManager->startHover(x, y, GETBIT(m_stateKey, OdEdBaseIO::kShiftIsDown)); // renamed checkHoverAt(x, y);
      return;
    }

    if (isPtValid)
    {
      stateCmd.m_pPointTracker->setValue(pt);
      getIViewSystem()->updateViews(getRxDatabase());
    }
  }
}

bool OdqConsoleTab::awaitForInput()
{
  CommandState& stateCmd = getTopCmdState();
  return (stateCmd.m_bRequestToGetPoint ^ stateCmd.m_bRequestToGetString);
}

// slot
void OdqConsoleTab::slotMouseButtonPress(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::MouseButtonPress && !bAccepted);
  QMouseEvent* pMouseEvent = (QMouseEvent*) pEvent;
  updateKeyboardModifiers(pMouseEvent);

  CommandState& stateCmd = getTopCmdState();
  switch (pMouseEvent->button())
  {
  case Qt::LeftButton:
    if (   stateCmd.m_bRequestToGetPoint
        && !GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptEndDrag))
    {
      OdGePoint3d pt;
      if (!GetPoint(*this, iView, pMouseEvent, pt))
        return;

      stateCmd.m_ptGetPoint = pt;
      stateCmd.m_ptGetEyes = QPoint(pMouseEvent->x(), pMouseEvent->y());
      stateCmd.m_bGetPointReady = true;
      bAccepted = true;
    }
    else
    {
      // update stateKey (used for ConsoleIoImpl::getKeyState())
      m_stateKey |= OdEdBaseIO::kLeftButtonIsDown;

      if (awaitForInput())
        return;

      QString qsCmd;

      OdqGripManagerPtr pGripManager = getCmdContext().gripManager();
      if (   !pGripManager.isNull()
          && workingSelectionSet() && workingSelectionSet()->numEntities())
      {
        int x = pMouseEvent->x(),
            y = pMouseEvent->y();
        bool bMultiplyMode = false;
        if (pGripManager->checkGripSetAt(x, y, bMultiplyMode))
        {
          if (!bMultiplyMode)
          {
            qsCmd = "_stretch";

            if (getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
            {
              OdGePoint3d pt;
              //if (!iView->eyesTo(QPoint(x, y), stateCmd.m_optionsRequestToGet, pt))
              // or
              if (!GetPoint(*this, iView, pMouseEvent, pt))
                return;

              if (   OdDbDatabase::cast(m_pRxDatabase).get()
                  && pGripManager->numGripPoints() > 1
                  && pGripManager->numGripPoints(OdDbGripOperations::kHotGrip) < 2)
                getIApp()->toStatusBar(QObject::tr("Press and hold the SHIFT key"
                                                 //" and use the cursor"
                                                   " to select a set of grip points to manipulate."),
                                                   2000);
              qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
              if (!OdZero(pt.z))
                qsCmd += QString(",%1").arg(pt.z);
              getConsole().postponeCommand(qsCmd, false, getRxDatabase());
            }
          }
          bAccepted = true;
          return;
        }
      }
      // Object selection is not allowed now.
#if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && !defined(ANDROID)
      qsCmd = "select";
      if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
        return;

      OdGePoint3d pt;
      SETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptNoLimCheck | OdEd::kGptNoUCS | OdEd::kGptNoOSnap, true);
      int svOpt = stateCmd.m_optionsRequestToGet;
      bool res = GetPoint(*this, iView, pMouseEvent, pt);
      stateCmd.m_optionsRequestToGet = svOpt;
      if (!res)
        return;

      qsCmd += " single";
      //if (Qt::ControlModifier & pMouseEvent->modifiers())
      //  qsCmd += "subobject";
      qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
      if (!OdZero(pt.z))
        qsCmd += QString(",%1").arg(pt.z); 
      
      if (!stateCmd.m_bRequestToGetPoint)
      {
        // Simplest way to save eyes point - will be used to determinate active viewport
        // This point cannot be correctly transformed from pt.
        stateCmd.m_ptGetPoint = OdGePoint3d(QString("%1").arg(pt.x).toDouble(),
                                            QString("%1").arg(pt.y).toDouble(), 
                                            QString("%1").arg(pt.z).toDouble());
        stateCmd.m_ptGetEyes = QPoint(pMouseEvent->x(), pMouseEvent->y());
      }

      getConsole().postponeCommand(qsCmd, false, getRxDatabase());
      bAccepted = true;
#else //Use "pan" cmd in case of touch
      //if ((m_touchEvent & Touch3dOrbitEvent) || (m_touchEvent & TouchZoomEvent))

      m_touchEvent = TouchPanEvent;
      qsCmd = "pan ";

      if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
        return;

      OdGePoint3d pt;
      if (!GetPoint(*this, iView, pMouseEvent, pt))
        return;

      qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
      if (!OdZero(pt.z))
        qsCmd += QString(",%1").arg(pt.z);
      getConsole().postponeCommand(qsCmd, false, getRxDatabase());
      bAccepted = true;
    
      break;
#endif //__IPHONE_OS_VERSION_MIN_REQUIRED
    }
    break;

  case Qt::RightButton:
    // update stateKey (used for ConsoleIoImpl::getKeyState())
    m_stateKey |= OdEdBaseIO::kRightButtonIsDown;
    break;

  case Qt::MidButton:
    {
      // TODO generate pan command if current command is absent + the next
      // update stateKey (used for ConsoleIoImpl::getKeyState())
      //m_stateKey |= kMiddleButtonIsDown;

      QString qsCmd = "pan ";
      if (Qt::ShiftModifier & pMouseEvent->modifiers())
        qsCmd = ((Qt::ControlModifier & pMouseEvent->modifiers()) ? "3dforbit " : "3dorbit ");

      if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
        return;

      OdGePoint3d pt;
      if (!GetPoint(*this, iView, pMouseEvent, pt))
        return;

      qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
      if (!OdZero(pt.z))
        qsCmd += QString(",%1").arg(pt.z);
      getConsole().postponeCommand(qsCmd, false, getRxDatabase());
      bAccepted = true;
      break;
    }
  }
}

// slot
void OdqConsoleTab::slotMouseButtonRelease(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::MouseButtonRelease && !bAccepted);
  QMouseEvent* pMouseEvent = (QMouseEvent*) pEvent;
  updateKeyboardModifiers(pMouseEvent);

  CommandState& stateCmd = getTopCmdState();

  // update stateKey (used for ConsoleIoImpl::getKeyState())
  switch (pMouseEvent->button())
  {
  case Qt::LeftButton:
    m_stateKey &= ~OdEdBaseIO::kLeftButtonIsDown;
    break;
  case Qt::RightButton:
    m_stateKey &= ~OdEdBaseIO::kRightButtonIsDown;
    break;
  case Qt::MidButton:
    m_stateKey &= ~OdEdBaseIO::kMiddleButtonIsDown;
    break;
  }

  switch (pMouseEvent->button())
  {
  case Qt::LeftButton:
  case Qt::MidButton:
    if (   stateCmd.m_bRequestToGetPoint
        && GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptEndDrag))
    {
      ODA_ASSERT_ONCE(!GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptBeginDrag));

      OdGePoint3d pt;
      if (!GetPoint(*this, iView, pMouseEvent, pt))
        return;

      stateCmd.m_ptGetPoint = pt;
      stateCmd.m_ptGetEyes = QPoint(pMouseEvent->x(), pMouseEvent->y());
      stateCmd.m_bGetPointReady = true;
      bAccepted = true;
    }
    break;
  }
#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED) || defined(ANDROID)
    if (m_doubleTapTimer == false) {
        m_doubleTapTimer = true;
        QTimer::singleShot(2000, this, SLOT(resetTimeout()));
    } else {
        //Disable previous cmd on double tap
        m_doubleTapTimer = false;
        cancelCommand();
    }
#endif
}
void OdqConsoleTab::resetTimeout()
{
  m_doubleTapTimer = false;
}
// slot
void OdqConsoleTab::slotMouseDoubleClick(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::MouseButtonDblClick && !bAccepted);
  QMouseEvent* pMouseEvent = (QMouseEvent*) pEvent;
  updateKeyboardModifiers(pMouseEvent);

  switch (pMouseEvent->button())
  {
  case Qt::MidButton:
    getConsole().postponeCommand("zoom extents", false, getRxDatabase());
    bAccepted = true;
    break;
  }
}

// slot
void OdqConsoleTab::slotMouseWheel(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::Wheel);
  QWheelEvent* pWheelEvent = (QWheelEvent*) pEvent;
  updateKeyboardModifiers(pWheelEvent);

  if (pWheelEvent->orientation() != Qt::Vertical)
    return;

  QString qsCmd = tr("zoom "); // TODO "'_zoom "

  if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
    return;

  double dScale = 0.75; // TODO via ZOOMWHEEL and ZOOMFACTOR ACAD variables
  if (pWheelEvent->delta() > 0)
    dScale = 1.0 / dScale; //wheel UP
  //else
    //wheel DOWN

  OdGePoint3d pt;
  if (!GetPoint(*this, iView, pWheelEvent, pt))
    return;

  qsCmd += QString("%1at %2,%3").arg(dScale).arg(pt.x).arg(pt.y);

  getConsole().postponeCommand(qsCmd, false, getRxDatabase());
  bAccepted = true;
}

// Touch slots
void OdqConsoleTab::slotTouchPressed(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
    ODA_ASSERT_ONCE(iView && pEvent);
    bAccepted = true;
}

bool OdqConsoleTab::prepareCmdToExecute(const QPoint& ptEyes, IqView* iView, CommandState& stateCmd, OdGePoint3d& pt, bool &bAccepted)
{
    if (   stateCmd.m_bRequestToGetPoint
        && !GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptEndDrag))
    {
      if (!getPoint(iView, ptEyes, pt))
        return false;

      stateCmd.m_ptGetPoint = pt;
      stateCmd.m_ptGetEyes = QPoint(ptEyes.x(), ptEyes.y());
      stateCmd.m_bGetPointReady = true;
      bAccepted = true;
    } else
    {
        if (awaitForInput())
            return false;
    }
    return true;
}

void OdqConsoleTab::setCmdOption(CommandState& stateCmd, const QPoint& ptEyes, OdGePoint3d& pt)
{
    if (!stateCmd.m_bRequestToGetPoint)
    {
    // Simplest way to save eyes point - will be used to determinate active viewport
    // This point cannot be correctly transformed from pt.
    stateCmd.m_ptGetPoint = OdGePoint3d(QString("%1").arg(pt.x).toDouble(),
                                      QString("%1").arg(pt.y).toDouble(), 0.0);
    stateCmd.m_ptGetEyes = QPoint(ptEyes.x(), ptEyes.y());
    }
}

void OdqConsoleTab::slotTouchMove(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
    ODA_ASSERT_ONCE(iView && pEvent);
    CommandState& stateCmd = getTopCmdState();
    QTouchEvent *touchEvent = static_cast<QTouchEvent *>(pEvent);
    QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

    QPoint touchPointCenter = touchPoints.first().scenePos().toPoint();
    switch (touchPoints.count())
    {
        case 1:
        {
            if ((m_touchEvent & Touch3dOrbitEvent) || (m_touchEvent & TouchZoomEvent))
                return;

            if (m_touchEvent != TouchPanEvent)
            {
                const QPoint ptEyes = touchPoints.first().scenePos().toPoint();
                OdGePoint3d pt;
                if (!prepareCmdToExecute(ptEyes, iView, stateCmd, pt, bAccepted))
                    return;

                if (!bAccepted)
                {
                    QString qsCmd = "pan ";

                    if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
                        return;

                    OdGePoint3d pt;

                    if (!getPoint(iView, ptEyes, pt))
                        return;

                    m_touchEvent = TouchPanEvent;
                    setCmdOption(stateCmd, ptEyes, pt);
                    qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
                    if (!OdZero(pt.z))
                      qsCmd += QString(",%1").arg(pt.z);
                    getConsole().postponeCommand(qsCmd, false, getRxDatabase());
                }
            }
            break;
        }
        case 2:
        {
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();

            QLineF line1(touchPoint0.lastScenePos(), touchPoint1.lastScenePos());
            QLineF line2(touchPoint0.scenePos(), touchPoint1.scenePos());

            if ((m_touchEvent & TouchPanEvent) || (m_touchEvent & Touch3dOrbitEvent))
            {
                cancelCommand();
                m_touchEvent = TouchUnknownEvent;
            }

            const QPoint ptEyes = touchPoints.first().scenePos().toPoint();
            OdGePoint3d pt;
            if (!(m_touchEvent & TouchZoomEvent))
            {
                if (!prepareCmdToExecute(ptEyes, iView, stateCmd, pt, bAccepted))
                    return;
                if (!bAccepted)
                    m_touchEvent = TouchZoomEvent;
            } else
            {
                QString qsCmd = "zoom Scale ";
                if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
                    return;

                if (!getPoint(iView, ptEyes, pt))
                    return;

                setCmdOption(stateCmd, ptEyes, pt);
                // determine scale factor
                qreal currentScaleFactor = line2.length() / line1.length();

                if (currentScaleFactor != 1)
                {
                    qsCmd += QString("%1").arg(currentScaleFactor);
                    getConsole().postponeCommand(qsCmd, false, getRxDatabase());
                    bAccepted = true;
                }
            }
            break;
        }
#if QT_VERSION < QT_VERSION_CHECK(5,3,0)
        case 3:
        {
            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(pEvent);
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
            if ((m_touchEvent & TouchZoomEvent) || (m_touchEvent & TouchPanEvent))
            {
                cancelCommand();
                m_touchEvent = TouchUnknownEvent;
            }
            // Start rotation drawing
            if (!(m_touchEvent & Touch3dOrbitEvent))
            {
                const QPoint ptEyes = touchPoints.first().scenePos().toPoint();
                OdGePoint3d pt;
                if (!prepareCmdToExecute(ptEyes, iView, stateCmd, pt, bAccepted))
                    return;

                if (!bAccepted)
                {
                    QString qsCmd = "3dorbit ";
                    if (!getConsole().findPostponedCommand(qsCmd, getRxDatabase()).isEmpty())
                        return;
                    OdGePoint3d pt;

                    if (!getPoint(iView, ptEyes, pt))
                      return;

                    m_touchEvent = Touch3dOrbitEvent;
                    setCmdOption(stateCmd, ptEyes, pt);
                    qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
                    if (!OdZero(pt.z))
                      qsCmd += QString(",%1").arg(pt.z);
                    getConsole().postponeCommand(qsCmd, false, getRxDatabase());
                }
            }
            break;
        }
#endif
        default:
        {
            return;
        }
    }

    bool isPtValid = false;
    OdGePoint3d pt;
    if (getPoint(iView, touchPointCenter, pt))
    {
      isPtValid = true;
      bool bTracePt = true;
      OdGePoint3d ptWcs = pt,
                  ptUcs = pt;

      OdUnitsFormatter* pBaseFormatter = NULL;
      OdDbBaseDatabasePEPtr pDbPE(m_pRxDatabase);
      if (!pDbPE.isNull())
        pBaseFormatter = pDbPE->baseFormatter(m_pRxDatabase);
      ODA_ASSERT_VAR(OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase);)
      ODA_ASSERT_ONCE(   pBaseFormatter 
                      || (pExtDb && pExtDb->baseModuleName(true) == L"bim")
                      || (pExtDb && pExtDb->baseModuleName(true) == L"ifc")
                      || (pExtDb && pExtDb->baseModuleName(true) == L"dwf"));
      if (pBaseFormatter)
      {
        //QString qsPtTrace;
        if (OdDbDatabase::cast(m_pRxDatabase).get())
        {
          if (!GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptNoUCS))
          {
            OdDbUnitsFormatter* pFormatter = (OdDbUnitsFormatter*) pBaseFormatter;
            ptUcs = pFormatter->toUCS(ptWcs);
            // normalize Z
            if (ptUcs.x != 0.0 && OdZero(ptUcs.x))
              ptUcs.z = 0.0;
          }
          else
            bTracePt = false;
        }
      }
    }
    if (!stateCmd.m_pPointTracker.isNull())
    {
      if (!stateCmd.m_bRequestToGetPoint)
      {
        OdqGripManagerPtr pGripManager = getCmdContext().gripManager();
        if (pGripManager.isNull())
          return;
        int x = touchPointCenter.x(),
            y = touchPointCenter.y();
        pGripManager->startHover(x, y); // renamed checkHoverAt(x, y);
        return;
      }

      if (isPtValid)
      {
        stateCmd.m_pPointTracker->setValue(pt);
        getIViewSystem()->updateViews(getRxDatabase());
      }
    }
#endif
}

void OdqConsoleTab::slotTouchReleased(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
    ODA_ASSERT_ONCE(iView && pEvent);
    QTouchEvent *touchEvent = static_cast<QTouchEvent *>(pEvent);
    QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
    CommandState& stateCmd = getTopCmdState();
    if (stateCmd.m_bRequestToGetPoint
        && GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptEndDrag))
    {
      ODA_ASSERT_ONCE(!GETBIT(stateCmd.m_optionsRequestToGet, OdEd::kGptBeginDrag));

      OdGePoint3d pt;
      QPoint touchPointCenter = touchPoints.first().pos().toPoint();

      if (getPoint(iView, touchPointCenter, pt))
      {
        stateCmd.m_ptGetPoint = pt;
        stateCmd.m_ptGetEyes = QPoint(touchPointCenter.x(), touchPointCenter.y());
        stateCmd.m_bGetPointReady = true;
        bAccepted = true;
      }
    }

    if ((touchEvent->touchPointStates() & Qt::TouchPointReleased))
    {
        // Simple touch|End of touch should stop all command
        cancelCommand();
        m_touchEvent = TouchUnknownEvent;
    }
#endif
}

// slot
void OdqConsoleTab::slotKeyPress(IqView* iView, QEvent* pEvent, bool &bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::KeyPress);
  QKeyEvent* pKeyEvent = (QKeyEvent *) pEvent;
  updateKeyboardModifiers(pKeyEvent);

  getCommandLineEdit().event(pKeyEvent); // redirect from IqView
}

// slot
void OdqConsoleTab::slotKeyRelease(IqView* iView, QEvent* pEvent, bool& bAccepted)
{
  ODA_ASSERT_ONCE(iView && pEvent && pEvent->type() == QEvent::KeyRelease);
  QKeyEvent* pKeyEvent =  (QKeyEvent *) pEvent;
  updateKeyboardModifiers(pKeyEvent);

  //CommandState& stateCmd = getTopCmdState();
  //if (stateCmd.m_bRequestToGetString || stateCmd.m_bRequestToGetPoint)
  //{
  //  Qt::Key key = (Qt::Key) pKeyEvent->key();
  //  switch (key)
  //  {
  //  case Qt::Key_Return:
  //  case Qt::Key_Enter:
  //    bAccepted = false;
  //    executeCommandByInput();
  //    return;
  //  }
  //}
  getCommandLineEdit().event(pKeyEvent); // redirect from IqView
}


OdGePoint3d OdqConsoleTab::lastGetPoint()
{
  CommandState& stateCmd = getTopCmdState();
  ODA_ASSERT_ONCE(stateCmd.m_bRequestToGetPoint);

  return stateCmd.m_ptGetPoint;
}

OdqCommandLineEdit& OdqConsoleTab::getCommandLineEdit()
{
  ODA_ASSERT_ONCE(!m_pIn.isNull());
  return *m_pIn;
}

OdqConsoleTab::OdEdBaseIoStateKey OdqConsoleTab::getKeyState()
{
  return m_stateKey;
}
