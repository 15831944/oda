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
// OdqPropertyDialog.h
//

#ifndef ODA_QT_PROPERTY_DIALOG_H_
#define ODA_QT_PROPERTY_DIALOG_H_

#include <QAbstractItemModel>
#include <QItemDelegate>

#include <QDialog> 
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QStack>
#include <QPointer>
#include <QVariant>

#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QFileDialog>
#include <QGroupBox>

QT_BEGIN_NAMESPACE
  class QVBoxLayout;
  class QLabel;
  class QTableWidget;
  class QBoxLayout;
  class QTreeWidgetItem;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "SharedPtr.h"
#include "PropServices.h"

#include "OdqXmlUtils.h"

///////////////////////////////////////////////////////////////////////////////

class IqPropControl
{
public:
  virtual QModelIndex modelIndex() = 0;
  virtual void setModelIndex(QModelIndex index) = 0;

  virtual QVariant value() = 0;
  virtual void setValue(const QVariant& value) = 0;

  virtual void createConnections() = 0;
};
Q_DECLARE_INTERFACE(IqPropControl, "oda.IqPropControl")

class OdqPropControlImpl : public IqPropControl
{
public:
  OdqPropControlImpl()
  {
  }

  virtual QModelIndex modelIndex()
  {
    return m_index;
  }

  virtual void setModelIndex(QModelIndex index)
  {
    m_index = index;
  }

protected:    
  QPersistentModelIndex m_index; // index in model

private:
  Q_DISABLE_COPY(OdqPropControlImpl)
};

//-----------------------------------------------------------------------------

class OdqPropControlLineEdit : public QLineEdit
                             , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlLineEdit(QWidget *widget = NULL);
  virtual ~OdqPropControlLineEdit();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }
signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlComboBox : public QComboBox
                             , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

  IqAppProps::TPropType m_type;
public:
  OdqPropControlComboBox(QWidget* widget = NULL, IqAppProps::TPropType type = IqAppProps::prpNone);
  virtual ~OdqPropControlComboBox();

  virtual IqAppProps::TPropType type() const;
  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  virtual void fillCombo(const QStringList& qsItems, const QStringList& qsImages);

protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }
signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlOdColor : public QComboBox
                            , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlOdColor(const QString& color, QWidget* pParent);
  virtual ~OdqPropControlOdColor();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  virtual void setVisible(bool visible);

  void addColor(const QString& qsColor, OdRxObject* pRxDb = NULL);
  QColor getQColor(OdRxObject* pRxDb = NULL);
  void updateSelection();

  static QColor toQColor(const class OdCmEntityColor& color, OdRxObject* pRxDb = NULL);
  static QIcon createIcon(const QString& sPacked, OdRxObject* pRxDb = NULL);
  static QIcon createIcon(const QColor& color);

protected slots:
  void onColorChanged();

signals:
  void colorChanged();

protected:
  QString m_qsColor;
};

//-----------------------------------------------------------------------------

class OdqPropControlButton : public QPushButton
                           , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlButton(QWidget* pParent = NULL);
  OdqPropControlButton(const QString& qsText, QWidget* pParent = NULL);
  OdqPropControlButton(const QIcon& icon, const QString& qsText = "", QWidget* pParent = NULL);
  virtual ~OdqPropControlButton();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }

signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlRadioButton : public QRadioButton
                                , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlRadioButton(QWidget* pParent = NULL);
  OdqPropControlRadioButton(const QString& qsText, QWidget* pParent = NULL);
  virtual ~OdqPropControlRadioButton();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  protected slots:
    void notifyParent()
    {
      emit propEdited(this);
    }

signals:
    void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlCheckBox : public QCheckBox
                             , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlCheckBox(const QString& qsText, QWidget* pParent = NULL);
  virtual ~OdqPropControlCheckBox();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }
signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlSpinBox : public QSpinBox
                            , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlSpinBox(QWidget* pWidget = NULL);
  virtual ~OdqPropControlSpinBox();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }
signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlDoubleSpinBox : public QDoubleSpinBox
                                  , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlDoubleSpinBox(QWidget* pWidget = NULL);
  virtual ~OdqPropControlDoubleSpinBox();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }
signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlFolder : public QWidget
                           , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlFolder(const QString& qsFolder, QWidget* pParent);
  virtual ~OdqPropControlFolder();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  QString getPath();
  virtual void resizeEvent(QResizeEvent* pEvent);

protected slots:
  void selectDirectory();
  void notifyParent()
  {
    emit propEdited(this);
  }
protected:
  QLineEdit* m_pText;
  QPushButton* m_pButton;

signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------

class OdqPropControlColorref : public QPushButton
                             , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlColorref(const QColor& color, QWidget* pParent);
  virtual ~OdqPropControlColorref();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  QColor getColor();

protected slots:
  void notifyParent()
  {
    emit propEdited(this);
  }
  void pickColor();    

protected:
  void paintEvent(QPaintEvent* pEvent);

signals:
  void propEdited(QWidget* pWidget);
private:    
  QColor m_color;
};

//-----------------------------------------------------------------------------

class OdqPropControlFileDialog : public QFileDialog
                               , public OdqPropControlImpl
{
  Q_OBJECT
  Q_INTERFACES(IqPropControl)

public:
  OdqPropControlFileDialog(QWidget* pParent,
                           const QString& qsFilter = "", // all
                           bool bForSave = false, // Open
                           const QString& qsAcceptLabel = "", // empty - default via modeAccept
                           const QString& qsRejectLabel = "", // empty - without cancel
                           const QString& qsDefFileName = ""); 
  virtual ~OdqPropControlFileDialog();

  virtual QVariant value();
  virtual void setValue(const QVariant& value);
  virtual void createConnections();

  //virtual bool event(QEvent* pEvent);
  virtual void hideEvent(QHideEvent* pEvent);

protected slots:
  void notifyParent();

signals:
  void propEdited(QWidget* pWidget);
};

//-----------------------------------------------------------------------------
// TODO

//typedef enum 
//{
//  enNone
//, enGroup
//, enDirect
//, enReverse
//} TEnablerType;
//
//class OdqPropControlEnabler : public QGroupBox
//                            , public OdqPropControlImpl
//{
//  Q_OBJECT
//  Q_INTERFACES(IqPropControl)
//
//public:
//  OdqPropControlEnabler(const QString& qsTitle, 
//                        Qt::CheckState checkState, 
//                        TEnablerType enType, 
//                        QWidget* pParent);
//  virtual ~OdqPropControlEnabler();
//  
//  virtual QVariant value();
//  virtual void setValue(const QVariant& value);
//  virtual void createConnections();
//
//protected:
//  virtual void childEvent(QChildEvent *event); // reimplemented to allow reverse enablers
//  virtual void changeEvent(QEvent *event); // reimplemented to allow reverse enablers
//
//protected slots:
//  void notifyParent()
//  {
//    emit propEdited(this);
//  }
//signals:
//  void propEdited(QWidget* pWidget);
//};

//-----------------------------------------------------------------------------
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)

class OdqColorDialog : public QColorDialog
{
  Q_OBJECT
public:

  OdqColorDialog(QWidget* pParent = NULL);

  virtual void setVisible(bool visible);
};

#endif

//-----------------------------------------------------------------------------

class OdqOdColorDialog : public QDialog
{
  Q_OBJECT

  OdqOdColorDialog(QWidget* pParent = NULL);
public:

  static class OdCmEntityColor getColor(const class OdCmEntityColor& colorInitial, 
                                        QWidget* pParent = NULL,
                                        OdRxObject* pRxDb = NULL, // means current
                                        bool isLayerEnabled = true,
                                        bool isBlockEnabled = true);
};

//-----------------------------------------------------------------------------

class OdqPropDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  OdqPropDelegate(QObject* pParent);
  virtual ~OdqPropDelegate();

  virtual QWidget* createEditor(QWidget* pParent, 
                                const QStyleOptionViewItem& option, 
                                const QModelIndex& index) const;
  virtual bool editorEvent(QEvent* pEvent, 
                           QAbstractItemModel* model, 
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index);
  virtual void paint(QPainter* pPainter, 
                     const QStyleOptionViewItem& option, 
                     const QModelIndex& index) const;
  virtual void setEditorData(QWidget* pWidget, 
                             const QModelIndex& index) const;
  virtual void setModelData(QWidget* pWidget, 
                            QAbstractItemModel* pModel, 
                            const QModelIndex& index) const;
  virtual QSize sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const;
  virtual void updateEditorGeometry(QWidget* pWidget, 
                                    const QStyleOptionViewItem& item, 
                                    const QModelIndex& index) const;
public slots:
  void emitCommitData();

private:
  Q_DISABLE_COPY(OdqPropDelegate)
};

///////////////////////////////////////////////////////////////////////////////
class OdqGridProxyItem;

class OdqGridProxyModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  OdqGridProxyModel(QObject *pParent, class OdqPropertyModel* pModel);
  ~OdqGridProxyModel();

  void setRootIndex(const QPersistentModelIndex& idxRoot);

  QModelIndex mapToSource(const QModelIndex& proxyIndex) const;
  QModelIndex mapFromSource(const QModelIndex& sourceIndex) const;

  virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex& child) const;
  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
  virtual Qt::ItemFlags flags(const QModelIndex& index) const;
  virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());

  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public slots:
  //virtual void sourceRowsRemoved(const QModelIndex& parent, int start, int end);
  //virtual void sourceRowsInserted(const QModelIndex& parent, int start, int end);
  //virtual void sourceDataChanged(const QModelIndex& index);
  void sourceLayoutChanged();
  void setCurrentIndex(const QModelIndex& indexProxy);
  void currentRowChanged(const QModelIndex& indexProxy);

signals:
  void notifyGridSourceChanged(const QModelIndex& indexSrc);
  void notifyGridRowChanged(const QModelIndex& indexSrc);

private:
  void collectColumnNames(const QModelIndex& index, QStringList& namesColumn);
  OdqGridProxyItem* createProxyIndex(OdqGridProxyItem* pItem, const QModelIndex& index);

  QStringList m_namesColumn; // m_columns
  QPersistentModelIndex m_idxRoot;
  OdqPropertyModel* m_pModel;
  mutable OdSharedPtr<OdqGridProxyItem> m_pRoot;

  Q_DISABLE_COPY(OdqGridProxyModel)
};

///////////////////////////////////////////////////////////////////////////////

class OdqGridProxyDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  OdqGridProxyDelegate(OdqGridProxyModel* pProxyModel);
  ~OdqGridProxyDelegate();

  virtual QWidget* createEditor(QWidget* pParent, 
                                const QStyleOptionViewItem& option, 
                                const QModelIndex& index) const;
  virtual bool editorEvent(QEvent* pEvent, 
                           QAbstractItemModel* pModel, 
                           const QStyleOptionViewItem& option, 
                           const QModelIndex& index);
  virtual void setEditorData(QWidget* pEditor, 
                             const QModelIndex& index) const;
  virtual void setModelData(QWidget* pEditor, 
                            QAbstractItemModel* pModel, 
                            const QModelIndex& index) const;
  virtual void updateEditorGeometry(QWidget* pEditor,
                                    const QStyleOptionViewItem& item,
                                    const QModelIndex& index) const;

private:
  OdqGridProxyModel* m_pProxyModel;
  OdqPropDelegate* m_pDelegate;

  Q_DISABLE_COPY(OdqGridProxyDelegate)
};

///////////////////////////////////////////////////////////////////////////////
class OdqPropertyItem;

typedef bool (*TFun_updateXml)(void* pSrc);
typedef bool (*TFun_setPropertyValue)(void* pSrc, const QString& qsPath, const QString& qsValue);
typedef bool (*TFun_getPropertyPossibleValues)(void* pSrc, const QString& qsPath, QStringList& values, QStringList& icons);

class OdqPropertyModel : public QAbstractItemModel
{
  Q_OBJECT

  OdqPropertyModel(const QString& qsRootPath = "");
public:
  virtual ~OdqPropertyModel();

  bool isLoaded();
  //const QModelIndex& activeIndex();    
  QModelIndex modelIndex(const QString& qsPathName);
  QModelIndex rootIndex() const; // is base path

  void setWidget(QWidget* pWidget);
  QWidget* widget() const;

  void setPropertySource(void* pSrc);
  void setFun_updateXml(TFun_updateXml pFun);
  void setFun_setPropertyValue(TFun_setPropertyValue pFun);
  void setFun_getPropertyPossibleValues(TFun_getPropertyPossibleValues pFun);

  bool updateFromXml(const QString& qsXml = "",
                     const QString& qsBasePath = "",
                     const QString& qsRootNodeName = "Properties",
                     const QString& qsUnmergedBasePath = "", // without merging
                     const QString& qsMergeFilter = "", // all
                     const QString& qsMergeFilterAttribName = "Filter");
  bool setMergeFilter(const QString& qsMergeFilter = "", // all
                      const QString& qsMergeFilterAttribName = "Filter",
                      const QString& qsUnmergedBasePath = ""); // without merging
  void updateWidget(const QString& qsPagePath,
                    bool bRecreateWidgets = false);

  QString path(const QModelIndex& index) const;

  virtual QModelIndex index(int row, int column, 
                            const QModelIndex& parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex& index) const;
  virtual int rowCount(const QModelIndex& indexParent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex& indexParent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  virtual Qt::ItemFlags flags(const QModelIndex& index) const;

  //virtual bool canFetchMore(const QModelIndex& indexParent) const;
  //virtual void fetchMore(const QModelIndex& indexParent);
  virtual bool hasChildren(const QModelIndex& indexParent = QModelIndex()) const;

  void insertRow(int row, QModelIndex indexParent, OdqPropertyItem* pItem);
  bool removeRow(int row, QModelIndex indexParent);
  bool removeRow(OdqPropertyItem* pItem);

  void restoreExpanded();
  static bool isExpanded(const QModelIndex& index);

  static OdSharedPtr<OdqPropertyModel> createModel(const QString& qsRootPath = "",
                                                   const QString& qsRootNodeName = "Properties");
  static IqAppProps::TPropType type(const QModelIndex& index);
  static bool isEnabled(const QModelIndex& index);
  static bool isReadOnly(const QModelIndex& index);
  static int indentation(const QModelIndex &index);
  static bool isWithImage(const QModelIndex& index, QString* pqsImagePath = NULL);
  static int countChildren(const QModelIndex& index);
  static bool isProperty(const QModelIndex& index);
  static bool isTreeItem(const QModelIndex& index);
  static bool isPaletteItem(const QModelIndex& index);
  static bool isVarious(const QModelIndex& index);
  static bool isSelected(const QModelIndex& index);
  static QString attr(const QModelIndex& index, const QString& qsName, 
                      const QString& qsDefValue = "", bool* pbFound = NULL);
  static int attrToInt(const QModelIndex& index, const QString& qsName, 
                       int nDefValue = 0);
  static double attrToDouble(const QModelIndex& index, const QString& qsName, 
                             double dDefValue = 0.0);
  static bool getPropertyPossibleValues(const QModelIndex& index, 
                                        QStringList& values, QStringList& icons);

  // -------------------

  static QString extractFirst(QString& qsPath);

  //static QString stringByOdColor(const class OdCmColorBase& color);
  static QString stringByOdColor(const class OdCmColor& color);
  static QString stringByOdColor(const OdCmEntityColor& color, OdRxObject* pRxDb = NULL);
  static OdCmEntityColor OdColorByString(const QString& qsColor, bool* pIsOk = NULL, OdRxObject* pRxDb = NULL);
  static bool getPossibleOdColors(QStringList& values, bool bSkipByLayer = false);

  static QString stringByLineType(class OdDbObjectId idLinetype);
  static OdDbObjectId lineTypeIdByString(OdRxObject* pRxDatabase, const QString& qsValue);
  static bool getPossibleLineTypes(OdRxObject* pRxDatabase, 
                                   QStringList& values, QStringList& icons, bool bSkipByLayer = false);

  static QString stringByLineWeight(int nLineWeight, bool bLineWeightAsIndex = false);
  static OdDb::LineWeight lineWeightByString(const QString& qsValue);
  static bool getPossibleLineWeights(QStringList& values, QStringList& icons, bool bSkipByLayer = false);

  static QString stringByTransparency(const class OdCmTransparency& transparency);
  static OdCmTransparency transparencyByString(const QString& qsValue);
  static bool getPossibleTransparencies(QStringList& values, bool bSkipByLayer = false);

  // -------------------

public slots:
  void currentChanged(const QModelIndex& index);
  void propEdited(QWidget* pPropControl);
  void currentTreeItemChanged(QTreeWidgetItem* pTreeItem);
  void currentGridRowChanged(const QModelIndex& index);
  void expanded(const QModelIndex& index);
  void collapsed(const QModelIndex& index);

protected:
  bool loadXmlFromString(const QString& qsXml,
                         const QString& qsBasePath, // = "" // all
                         const QString& qsRootNodeName); // = "Properties"

  OdqPropertyItem* createItem(const QString& qsPathName, 
                              bool bMarkAsLastForParent = true);

  // add widgets
  void addPropControls(const QModelIndex& index, 
                       QBoxLayout* pLayout); 
  QWidget* createPropControl(const QModelIndex& index);

  QWidget* createLabelControl(const QModelIndex& index);
  QAbstractItemView* createGridControl(const QModelIndex& index);
  QWidget* createTreeControl(const QModelIndex& index);
  void populateTreeChildren(const QModelIndex& index,
                            QTreeWidgetItem* pTreeItem = NULL);
  QWidget* createPaletteControl(const QModelIndex& index);
  QWidget* createFileDialogControl(const QModelIndex& index);

  OdSharedPtr<OdqPropertyItem> m_pRoot;
  QPointer<QWidget> m_pWidget; // m_pPage
  QPersistentModelIndex m_indexCurrent;

  OdqXmlUtils m_utils; // friend class OdqPropertyItem; 

  void* m_pPropertySource;
  TFun_updateXml m_pFun_updateXml;
  TFun_setPropertyValue m_pFun_setPropertyValue;
  TFun_getPropertyPossibleValues m_pFun_getPropertyPossibleValues;

  QHash<QString, bool> m_mapExpanded; // by path;
  QHash<QString, OdqPropertyItem*> m_mapItems; // m_cacheByName
  QHash<QString, QString> m_mapLastChildName; // m_mLastProcessed;
  QHash<QString, bool> m_mapInlineWithParent; // m_mSiblingToParent;

  OdRxObject* m_pRxDb;

private:
  Q_DISABLE_COPY(OdqPropertyModel)
};

///////////////////////////////////////////////////////////////////////////////

class OdqPropertyDialog : public QDialog
{                 
  Q_OBJECT 

public:                 
  OdqPropertyDialog(QWidget* pParent = NULL,
                    Qt::WindowFlags flags = 0);   
  virtual ~OdqPropertyDialog();   

  // setWindowTitle ...
  void setButtons(QDialogButtonBox::StandardButtons buttons = (QDialogButtonBox::Ok | QDialogButtonBox::Cancel));

  QBoxLayout* topLayout(); // current
  QBoxLayout* pushLayout(QBoxLayout* pLayout = NULL, // NULL - with in perpendicular direction to current layout
                        int stretch = 0);
  bool popLayout();

  QWidget* addWidget(QWidget* pWidget, 
                     int stretch = 0, Qt::Alignment alignment = 0);
  bool removeWidget(QWidget* pWidget);

      // use as : setTableHeaders(2, QStringList() << tr("Name") << tr("Value"));
  //bool setTableHeaders(int numColumns = 2,
  //                     const QStringList& lstLabels = QStringList());
  //bool addTableProperty(const QString& qsName, const QVariant& vValue);
  //bool addTableProperty(const QString& qsName, QVariant::Type type);
  //QList<QVariant> tableValues() const;

  int exec();

private:                                 
  QStack<QPointer<QBoxLayout> > m_stackLayouts;
  //QPointer<QTableWidget> m_pPropertyTable;

  Q_DISABLE_COPY(OdqPropertyDialog)
};  

///////////////////////////////////////////////////////////////////////////////

class OdqPropDialogData : public QObject
{
  Q_OBJECT
public:
  OdqPropDialogData(QWidget* pParent = NULL,
                    Qt::WindowFlags flags = 0,
                    OdPropServices::TSeparateCategoryTreePlace catPlace = OdPropServices::enSepCatPlace_None);
  virtual ~OdqPropDialogData();

  OdqPropertyDialog* getDialog();
  bool refreshSources(class OdPropSource* pSourceOnly = NULL, // null for all
                      bool bRecreateWidgets = false);

public slots:
  void refresh(const QString& qsKey, const QString& qsArg); // refresh states

protected:
  friend class OdPropServicesImpl;

  OdSharedPtr<OdqPropertyDialog> m_pDialog;
  QList<OdSharedPtr<class OdqPropSourceData> > m_lstSrcData;

private:
  Q_DISABLE_COPY(OdqPropDialogData)
};

///////////////////////////////////////////////////////////////////////////////

class PreviewFileDialog : public QFileDialog
{
  Q_OBJECT
public:
  explicit PreviewFileDialog(QWidget* parent = 0,
                             const QString & caption = QString(),
                             const QString & directory = QString(),
                             const QString & filter = QString()
  );

  protected slots:
  void OnCurrentChanged(const QString & path);

protected:
  QLabel* mpPreview;
};

#endif // ODA_QT_PROPERTY_DIALOG_H_
