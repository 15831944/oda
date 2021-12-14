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
// OdqClipData.h
//

#ifndef OD_QT_CLIP_DATAH_
#define OD_QT_CLIP_DATAH_

#include <QObject>

#include <QDir>
#include <QTemporaryFile>
#include <QDrag>
#include <QPointer>
#ifdef Q_WS_WIN // Q_CC_MSVC
#include <QWindowsMime>
#include <ActiveQt/QAxObject>
#endif

#if QT_VERSION < 0x050000
#include <QMainWindow>
#else
#include <QtWidgets>
#endif
#include "OdaCommon.h"
#include "DbDatabase.h"
#include "Ge/GePoint3d.h"
#include "SharedPtr.h"
#include "DbSSet.h"

template<class TChar>
struct OdqClipData
{
  void init()
  {
    memset(this, 0, sizeof(OdqClipData<TChar>));
  }
  bool read(const QByteArray& data)
  {
    int size = data.size();
    ODA_ASSERT_ONCE(size == sizeof(OdqClipData<TChar>)); // test
    if (size != sizeof(OdqClipData<TChar>))
      return false;

    memcpy(this, data.data(), sizeof(OdqClipData<TChar>));
    return true;
  }

  TChar _tempFileName[0x104];   // name of the temp dwg file, where dragged entities are
  TChar _origFileName[0x104];   // original file name
  TChar _version[4];            // version of the original file, e.g. 'R15'
  int _one1;                    // seem to be always 1
  double _x, _y, _z;            // pick point
  int _zero1;                   // seem to be always zero
  int _one2;                    // seem to be always 1
  int _unk[4];                  //
  int _zero2[4];                // seem to be always zero
};

template<class TChar>
struct OdqClipDataConstr : public OdqClipData<TChar>
{
  void set(const OdString& tempFileName,const OdString& origFileName, const OdGePoint3d& pickPoint)
  {
    OdqClipData<TChar>::init();
    OdqClipData<TChar>::_one1 = 1;
    OdqClipData<TChar>::_one2 = 1;
    OdqClipData<TChar>::_version[0] = 'R';
    OdqClipData<TChar>::_version[1] = '1';
    OdqClipData<TChar>::_version[2] = '5';
    OdqClipData<TChar>::_version[3] = 0;
    OdqClipData<TChar>::_x = pickPoint.x;
    OdqClipData<TChar>::_y = pickPoint.y;
    OdqClipData<TChar>::_z = pickPoint.z;
    memcpy(OdqClipData<TChar>::_tempFileName,
           (const TChar*) tempFileName,
           odmin((0x100 * sizeof(TChar)), ((tempFileName.getLength() + 1) * sizeof(TChar))));
    memcpy(OdqClipData<TChar>::_origFileName,
           (const TChar*) origFileName,
           odmin((0x100 * sizeof(TChar)), ((origFileName.getLength() + 1) * sizeof(TChar))));
  }

  OdqClipDataConstr(const OdString& tempFileName,const OdString& origFileName, const OdGePoint3d& pickPoint)
  {
    set(tempFileName, origFileName, pickPoint);
  }
  OdqClipDataConstr()
  {
    OdqClipData<TChar>::init();
  }
};
typedef OdqClipDataConstr<char> OdqClipDataR15;
typedef OdqClipDataConstr<OdChar> OdqClipDataR21;

//#define ODQ_CLIPDATA_META_TYPE // test for QVariant serialize support (TODO ? awaited to be useless)
//#ifdef ODQ_CLIPDATA_META_TYPE 
//Q_DECLARE_METATYPE(OdqClipDataR15);
//Q_DECLARE_METATYPE(OdqClipDataR21);
//inline QDataStream& operator<<(QDataStream& out, const OdqClipDataR15&) 
//{ 
//  ODA_FAIL_ONCE(); // TODO
//  return out; 
//}
//inline QDataStream& operator>>(QDataStream& in, OdqClipDataR15&) 
//{ 
//  ODA_FAIL_ONCE(); // TODO
//  return in; 
//}
//inline QDataStream& operator<<(QDataStream& out, const OdqClipDataR21&) 
//{ 
//  ODA_FAIL_ONCE(); // TODO
//  return out; 
//}
//inline QDataStream& operator>>(QDataStream& in, OdqClipDataR21&) 
//{ 
//  ODA_FAIL_ONCE(); // TODO
//  return in; 
//}
//#endif

class OdqClipboardData
{
private:
  union Data
  {
    OdqClipData<char>   _r15;
    OdqClipData<OdChar> _r21;
    Data() 
    { 
      _r21.init(); 
    }
  } _data;
  bool _isR15format;

public:

  static QString mimeTypeByIndex(int index)
  {
    QString sValue;
    switch (index)
    {
    case 0: 
      sValue = "AutoCAD.r15"; break;
    case 1:
      sValue = "AutoCAD.r16"; break;
    case 2:
      sValue = "AutoCAD.r17"; break;
    case 3:
      sValue = "AutoCAD.r18"; break;
    }
    ODA_ASSERT_ONCE(!sValue.isEmpty());
    if (sValue.isEmpty())
      return sValue;
    //mimes << "application/acad"; // = "application/dxf"; // = "application/x-dwg-state"
    sValue = QString("application/x-qt-windows-mime;value=\"%1\"").arg(sValue);
    return sValue;
  }

  static bool isAcadDataAvailable(const QMimeData* pMimeData, OdRxObject* pRxDatabase, 
                                  QByteArray* pData = NULL, QMimeData* pMimeToCopyData = NULL) 
  {
    if (   !pMimeData || !pRxDatabase 
        || !OdDbDatabase::cast(pRxDatabase).get()) // TODO
      return false;

    bool bRes = false;
    if (pData)
      pData->clear(); 
    QStringList lstFormat = pMimeData->formats();
    for (int index = 0; index < 4; index++)
    {
      QString qsMimeType = mimeTypeByIndex(index);
      bool bFound = pMimeData->hasFormat(qsMimeType);
      if (!bFound && lstFormat.indexOf(qsMimeType) >= 0)
      {
        ODA_FAIL_ONCE(); // TODO
       //#ifdef Q_WS_WIN // Q_CC_MSVC
       // QWindowsMime::registerMimeType(qsMimeType);
       // bFound = pMimeData->hasFormat(qsMimeType);
       // //ODA_ASSERT_ONCE(bFound);
       //#endif
       // bFound = true;
      }
      if (bFound) 
      {
        bRes = true;
        if (pData)
          *pData = pMimeData->data(qsMimeType);
        if (pMimeToCopyData)
          pMimeToCopyData->setData(qsMimeType, pMimeData->data(qsMimeType));
        if (!pMimeToCopyData && !pData)
          break;
      }
    }

   #if 0 // test 
    if (!bRes)
    {
      //application/x-qt-windows-mime;value="Embed Source"
      //application/x-qt-windows-mime;value="Object Descriptor"
      //application/x-qt-windows-mime;value="Link Source"
      //application/x-qt-windows-mime;value="Link Source Descriptor"
      //application/x-qt-windows-mime;value="AutoCAD.r17"
      foreach (QString qsFmt, pMimeData->formats())
        getIConsole()->putString(qsFmt);
    }
   #endif

    return bRes;
  }

  static QClipboard::Mode mimeDataMode()
 //#if !defined(Q_OS_LINUX)
  {
    return QClipboard::Clipboard;
  }
 //#else
 // {
 //   return QClipboard::Selection;
 // }
 //#endif

  static bool copyToClipboard(const QMimeData* pMimeData, OdRxObject* pRxDatabase)
  {
    QClipboard* pClipboard = QApplication::clipboard();
    ODA_ASSERT_ONCE(pClipboard);
    if (pMimeData == pClipboard->mimeData(mimeDataMode()))
      return true;

    //pClipboard->setMimeData(const_cast<QMimeData*>(pMimeData));
    OdSharedPtr<QMimeData> pMimeCopy = new QMimeData();
    if (!isAcadDataAvailable(pMimeData, pRxDatabase, NULL, (QMimeData*) pMimeCopy))
      return false;
    pClipboard->setMimeData(pMimeCopy.detach(), mimeDataMode());
    return true;
  }
                                          //COleDataObject* pDataObject, bool bAttach = false)
  static OdSharedPtr<OdqClipboardData> get(const QMimeData* pMimeData, OdRxObject* pRxDatabase) 
  {
    //  if (bAttach && !pDataObject->AttachClipboard())
    //    return 0;

    QByteArray data;
    if (!OdqClipboardData::isAcadDataAvailable(pMimeData, pRxDatabase, &data))
      return OdSharedPtr<OdqClipboardData>();

    OdSharedPtr<OdqClipboardData> pData = new OdqClipboardData();
    if (!pData->read(data))
      return OdSharedPtr<OdqClipboardData>();
    return pData;
  }

  OdqClipboardData()
    : _isR15format(false)
  {
  }

  bool read(const QByteArray& data)
  {
    int size = data.size();
    if (size == sizeof(OdqClipData<char>))
    {
      _isR15format = true;
      return _data._r15.read(data);
    }
    if (size == sizeof(OdqClipData<OdChar>))
    {
      _isR15format = false;
      return _data._r21.read(data);
    }

    ODA_FAIL_ONCE(); // test
    return false;
  }

  OdString tempFileName() 
  { 
    return _isR15format ? OdString(_data._r15._tempFileName) 
                        : OdString(_data._r21._tempFileName); 
  }
  OdGePoint3d pickPoint() 
  { 
    return _isR15format ? OdGePoint3d(_data._r15._x, _data._r15._y, _data._r15._z) 
                        : OdGePoint3d(_data._r21._x, _data._r21._y, _data._r21._z); 
  }
};

class OdqDataSource
{
  QString m_qsTempFilePathName;
  QPointer<QDrag> m_pDrag;
 //#ifdef ODQ_CLIPDATA_META_TYPE 
 // OdqClipDataR15* m_pR15;
 // OdqClipDataR21* m_pR21;
 //#endif
public:
  OdqDataSource()
   //#ifdef ODQ_CLIPDATA_META_TYPE 
   // : m_pR15(NULL)
   // , m_pR21(NULL)
   //#endif
  {
  }

  virtual ~OdqDataSource()
  {
    empty();
  }

  void empty()
  {
    //COleDataSource::Empty();
    //if (m_pDrag)
    //{
    //  ODA_FAIL_ONCE(); // marked through deleteLater and already deleted or will be deleted automatically
    //  delete m_pDrag;
      m_pDrag = NULL;
    //}

   //#ifdef ODQ_CLIPDATA_META_TYPE 
   // if (m_pR15)
   // {
   //   int id = QMetaType::type("OdqClipDataR15");
   //   ODA_ASSERT_ONCE(id);
   //   if (id)
   //     QMetaType::destroy(id, m_pR15); 
   //   m_pR15 = NULL;
   // }
   // if (m_pR21)
   // {
   //   int id = QMetaType::type("OdqClipDataR21");
   //   ODA_ASSERT_ONCE(id);
   //   if (id)
   //     QMetaType::destroy(id, m_pR21); 
   //   m_pR21 = NULL;
   // }
   //#endif

    // TODO move the into destructor of inheritanced class from QDrag
    if (m_qsTempFilePathName.isEmpty())
      return;
    QFile file(m_qsTempFilePathName);
    if (!file.exists())
      return;
    if (!file.remove())
    {
      ODA_FAIL_ONCE();
      return;
    }

    m_qsTempFilePathName.clear();
  }

//#ifdef Q_WS_WIN // Q_CC_MSVC
//  static FORMATETC setCf(CLIPFORMAT cf) // static in Qt surce now
//  {
//    // see also _AfxFillFormatEtc
//    FORMATETC formatetc;
//    formatetc.cfFormat = cf;
//    formatetc.dwAspect = DVASPECT_CONTENT;
//    formatetc.lindex = -1;
//    formatetc.ptd = NULL;
//    formatetc.tymed = TYMED_HGLOBAL;
//    return formatetc;
//  }
//#endif

  bool create(OdSelectionSet* pSSet, const OdGePoint3d& ptPick)
  {
    empty();
    
    OdDbDatabase* pDwgDb = OdDbDatabase::cast(pSSet->baseDatabase()).get();
    if (!pDwgDb)
      return false;

    OdDbObjectIdArray ids = OdDbSelectionSet::cast(pSSet)->objectIdArray();
    if (!ids.size())
      return false;
    //TCHAR tempdir[MAX_PATH];
    //::GetTempPath(MAX_PATH,tempdir);
    //TCHAR tempname[MAX_PATH];
    //::GetTempFileName(tempdir,_T(""),0,tempname);
    //m_tmpPath = tempname;
    //m_tmpPath.makeLower();
    //m_tmpPath.replace(L".tmp",L".dwg");
    {
      m_qsTempFilePathName = QDir::temp().absoluteFilePath("XXXXXX.dwg");
      QTemporaryFile tmp(m_qsTempFilePathName);
      tmp.open();
      m_qsTempFilePathName = QDir::toNativeSeparators(tmp.fileName());
    }
    if (   QFile::exists(m_qsTempFilePathName)
        && !QFile::remove(m_qsTempFilePathName))
    {
      ODA_FAIL_ONCE();
      return false;
    }
    try
    {
      OdDbDatabasePtr pDb = pDwgDb->wblock(ids, OdGePoint3d::kOrigin);
      OdStreamBufPtr pFileBuf = odSystemServices()->createFile(getIApp()->toOdString(m_qsTempFilePathName),
                                                               Oda::kFileWrite,
                                                               Oda::kShareDenyWrite,
                                                               Oda::kCreateNew);
      pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC18);
    }
    catch (OdError&)
    {
      ODA_FAIL_ONCE();
      return false; // throw err;
    }
    OdString sFilePathName = getIConsole()->getFilePathName(pDwgDb);

  //#ifndef ODQ_CLIPDATA_META_TYPE 

   //#if defined(Q_WS_WIN) && defined(__AFXOLE_H__) // Q_CC_MSVC && afxole.h
   // // via MFC (CacheGlobalData COleDataSource::DoDragDrop) // way of OdaMfcApp
   // CLIPFORMAT fmtR15 = (CLIPFORMAT) QWindowsMime::registerMimeType("AutoCAD.r15"),
   //            fmtR17 = (CLIPFORMAT) QWindowsMime::registerMimeType("AutoCAD.r17");
   // // ODA_ASSERT_ONCE(   fmtR15 == (CLIPFORMAT) ::RegisterClipboardFormat(L"AutoCAD.r15")
   // //                 && fmtR17 == (CLIPFORMAT) ::RegisterClipboardFormat(L"AutoCAD.r17"));
   // // //QString qsMimeTypeR15 = QWindowsMime().mimeForFormat(setCf(fmtR15)),
   // // //        qsMimeTypeR17 = QWindowsMime().mimeForFormat(setCf(fmtR17));
   //     ////qApp->clipboard()->setMimeData(pMimeData);
   // HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, sizeof(OdqClipDataR15));
   // new (hGlobal) OdqClipDataR15(getIApp()->toOdString(m_qsTempFilePathName), sFilePathName, ptPick);
   // CacheGlobalData(fmtR15, hGlobal);
   // HGLOBAL hGlobalR21 = GlobalAlloc(GMEM_FIXED, sizeof(OdqClipDataR21));
   // new (hGlobalR21) OdqClipDataR21(getIApp()->toOdString(m_qsTempFilePathName), sFilePathName, ptPick);
   // CacheGlobalData(fmtR17, hGlobalR21);
   // ...
   // COleDataSource::DoDragDrop
   // return true;
   //#endif

   //#if defined(Q_WS_WIN)
   // // TODO try a way via QAxObject to support dragging to AutoCAD
   // //      (this way requares to build QAxContainer.lib QAxServer.lib) 
   // //       #include <ActiveQt/QAxObject>
   // //
   // // http://www.eggheadcafe.com/searchform.aspx?search=Control.DoDragDrop
   // // via QAxObject Control.DoDragDrop of VB.Net
   // //
   // //QAxObject object("{GUID-....-....-....-............}");
   // //QCoreApplication doesn't call CoInitialize - it doesn't know how you want 
   // //to initialize COM. QApplication calls OleInitialize, which is required for 
   // //the clipboard and drag'n'drop support, but if you use QCoreApplication, 
   // //then you need to call CoInitialize and CoUninitialize yourself if you want 
   // //to do COM stuff.
   // //return a.exec();
   // //...
   // //return true
   // QAxObject ctrl;
   // if (ctrl.setControl("Control.DoDragDrop"))
   //   return true;
   //#endif

    ODA_ASSERT_ONCE(!m_pDrag);
    QMimeData* pMimeData = new QMimeData;
    OdqClipDataR15 r15(getIApp()->toOdString(m_qsTempFilePathName), sFilePathName, ptPick);
    pMimeData->setData(OdqClipboardData::mimeTypeByIndex(0), // R15
                       QByteArray((const char *) &r15, sizeof(OdqClipDataR15)));
    OdqClipDataR21 r21(getIApp()->toOdString(m_qsTempFilePathName), sFilePathName, ptPick);
    pMimeData->setData(OdqClipboardData::mimeTypeByIndex(2), // R17
                       QByteArray((const char *) &r21, sizeof(OdqClipDataR21)));

    QWidget* pDragSource = getIApp()->getMainWindow();
    IqViewSystem* iVS = getIViewSystem();
    if (iVS)
    {
      IqView* iView = iVS->getView(pDwgDb);
      if (iView && iView->getWidget())
        pDragSource = iView->getWidget();
    }
    m_pDrag = new QDrag(pDragSource);
    // m_pDrag->deleteLater(); will be requested later via m_pDrag->start
    m_pDrag->setMimeData(pMimeData);

  //#else // ODQ_CLIPDATA_META_TYPE 
  //  ODA_ASSERT_ONCE(!m_pDrag);
  //  QMimeData* pMimeData = new QMimeData;
  //  int id = QMetaType::type("OdqClipDataR15");
  //  if (!id)
  //  {
  //    qRegisterMetaType<OdqClipDataR15>("OdqClipDataR15");
  //    qRegisterMetaTypeStreamOperators<OdqClipDataR15>("OdqClipDataR15");
  //    id = QMetaType::type("OdqClipDataR15");
  //    ODA_ASSERT_ONCE(id);
  //  }
  //  if (id)
  //  {
  //    m_pR15 = (OdqClipDataR15*) QMetaType::construct(id);
  //    m_pR15->set(getIApp()->toOdString(m_qsTempFilePathName), sFilePathName, ptPick);
  //    
  //    pMimeData->setData(OdqClipboardData::mimeTypeByIndex(0), // R15
  //                       QByteArray((const char *) m_pR15, sizeof(OdqClipDataR15)));
  //  }
  //  id = QMetaType::type("OdqClipDataR21");
  //  if (!id)
  //  {
  //    qRegisterMetaType<OdqClipDataR21>("OdqClipDataR21");
  //    qRegisterMetaTypeStreamOperators<OdqClipDataR21>("OdqClipDataR21");
  //    id = QMetaType::type("OdqClipDataR21");
  //    ODA_ASSERT_ONCE(id);
  //  }
  //  if (id)
  //  {
  //    m_pR21 = (OdqClipDataR21*) QMetaType::construct(id);
  //    m_pR21->set(getIApp()->toOdString(m_qsTempFilePathName), sFilePathName, ptPick);
  //    pMimeData->setData(OdqClipboardData::mimeTypeByIndex(2), // R17
  //                       QByteArray((const char *) m_pR21, sizeof(OdqClipDataR21)));
  //  }
  //  m_pDrag = new QDrag(getIApp()->getMainWindow());
  //  m_pDrag->setMimeData(pMimeData);
  // #endif

   #if 0 // test 
    //application/x-qt-windows-mime;value="AutoCAD.r15"
    //application/x-qt-windows-mime;value="AutoCAD.r17"
    // ...
    foreach (QString qsFmt, pMimeData->formats())
    {
      getIConsole()->putString(qsFmt);
    }
   #endif

    return true;
  }

  static bool beginDrag(OdSelectionSet* pSSet, const OdGePoint3d& ptPick)
  {
    OdqDataSource ds;
    if (!ds.create(pSSet, ptPick))
      return false;
    if (!ds.doDragDrop())
      return false;
    //UpdateAllViews(0);
    return true;
  }

  bool doDragDrop()
  {
    if (!m_pDrag)
      return false;
    Qt::DropAction dropAction = m_pDrag->start(Qt::CopyAction | Qt::MoveAction | Qt::TargetMoveAction);
    if (dropAction == Qt::IgnoreAction)
      return false;
    return true;
  }

private:
  Q_DISABLE_COPY(OdqDataSource)
};

#endif // OD_QT_CLIP_DATAH_
