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
// OdqRevDiffHelper.h
// 

#ifndef ODA_QT_REV_DIFF_HELPER_H_
#define ODA_QT_REV_DIFF_HELPER_H_

#include "UInt8Array.h"
#include "DbObjectId.h"
#include "DbDatabase.h"
#include "RevisionController.h"
#include <map>
#include "Gs/Gs.h"

class OdGsDevice;
class OdGiDefaultContext;
class OdGsModel;

typedef std::map< OdDbStub*, OdDbObjectId > visualStylesMap;

class OdqRevDiffHelper
{
public:
  static void modelLWeights(OdUInt8Array& lweights, double scale);

  OdqRevDiffHelper();
  ~OdqRevDiffHelper();

  void setVS( OdDbObjectId, OdDbObjectId );
  OdDbObjectId getVS( bool bFirst );

  void setDB( OdDbDatabasePtr, OdDbDatabasePtr );
  OdDbDatabasePtr getDatabase( bool first = true );

  void setDevice( OdGsDevice* );
  OdGsDevice* getDevice();

  void setModel( OdGsModel* );
  OdGsModel* getModel();

  void setViews( OdGsView*, OdGsView* );
  OdGsView* getView( bool first = true );

  void setHandles( const OdDbHandleList& handles );

  void clear();

  void setupDefaultDifferenceView( bool bPlotGeneration );
  void initViews( const OdDbHandleList& handles );

  void reloadMainView( bool bShowAll );
  void loadSecondView();

  OdDbObjectId createVisualStyleBasedOn( const OdString& base, const OdString& name );
  OdDbObjectId createVisualStyleBasedOn( OdDbStub*, const OdString& name );
  void applyVisualStyles();

  void syncViews();
  void setViewsVisible( bool bMainVisible, bool bSecondVisible );
  void setVSOpacity( double op, bool bFirst );
  void setVSbyVS( const OdString& );
  OdDbStub* getOrigVisualStyle( const OdString& sName );

  bool isValid() const;

  void setReColorOption( bool bFirstRevision, bool doReColor, const OdCmColor&, bool b2DOnly );
protected:
  OdDbObjectId m_firstVS;
  OdDbObjectId m_secondVS;
  OdDbDatabasePtr m_firstDB;
  OdDbDatabasePtr m_secondDB;
  OdGsDevice* m_device;
  OdGsModel* m_model;

  OdGsView* m_mainView;
  OdGsView* m_secondView;

  bool m_bShowAll;
  double m_dOpac1;
  double m_dOpac2;

  OdDbHandleList m_handles;

  visualStylesMap m_firstVSMap;
  visualStylesMap m_secondVSMap;

  bool m_bReColorFirst;
  OdCmColor m_clReColorFirst;
  bool m_b2DOnlyFirst;
  bool m_bReColorSecond;
  OdCmColor m_clReColorSecond;
  bool m_b2DOnlySecond;
};


class RevisionDiffSingleViewHelper : public OdRxObject
{
public:
  RevisionDiffSingleViewHelper();
  virtual ~RevisionDiffSingleViewHelper();

  void setDB( OdDbDatabasePtr, OdDbDatabasePtr );
  OdDbDatabasePtr getDatabase( bool first = true );

  void setDevice( OdGsDevicePtr );
  OdGsDevice* getDevice();

  void setModel( OdGsModel* );
  OdGsModel* getModel();

  OdGsView* getView();

  void setHandles( const OdDbHandleList& handles );

  bool isValid() const;

  void clear();

  void setupView( bool bShowAll, const OdDbHandleList& handles );
  void reloadView( bool bShowAll );

  OdDbObjectId createVisualStyleBasedOn( const OdString& base, const OdString& name, bool bSecondDB = false );
  OdDbObjectId createVisualStyleBasedOn( OdDbStub*, const OdString& name, bool bSecondDB = false );

  void setViewsVisible( bool bMainVisible, bool bSecondVisible );
  void setVSOpacity( double op, bool bFirst );
  void setVSbyVS( const OdString& );
  OdDbStub* getOrigVisualStyle( const OdString& sName );

  void setReColorOption( bool bFirstRevision, bool doReColor, const OdCmColor&, bool b2DOnly = true );
protected:

  OdDbDatabasePtr m_firstDB;
  OdDbDatabasePtr m_secondDB;
  OdGsDevicePtr m_device;
  OdGsModel* m_model;

  OdGsView* m_pView;
  OdDbHandleList m_handles;
  bool m_bShowAll;

  void loadFirstEntities();
  void loadSecondEntities();

  double m_dOpac1;
  double m_dOpac2;
  visualStylesMap m_firstVSMap;
  visualStylesMap m_secondVSMap;
  OdDbObjectId m_firstVS;
  OdDbObjectId m_secondVS;
  OdDbObjectId m_origVS;

  void applyVisualStyles();
  OdArray< OdDbHandle > m_secondEntitiesList;
  void clearVS();

  bool m_bReColorFirst;
  OdCmColor m_clReColorFirst;
  bool m_b2DOnlyFirst;
  bool m_bReColorSecond;
  OdCmColor m_clReColorSecond;
  bool m_b2DOnlySecond;
};

typedef OdSmartPtr< RevisionDiffSingleViewHelper > RevisionDiffSingleViewHelperPtr;


#endif
