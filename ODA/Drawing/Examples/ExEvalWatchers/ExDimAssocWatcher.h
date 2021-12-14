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

/************************************************************************/
/* Defines notification functions for the evaluation of                 */
/* OdDHatch  entities                                                   */
/************************************************************************/
#ifndef _ODEXDIMASSOCWATCHER_INCLUDED_
#define _ODEXDIMASSOCWATCHER_INCLUDED_

#include "DbEvalWatchers.h"

class OdExDimAssocWatcherPE : public OdDbDimAssocWatcherPE
{
public:
  OdExDimAssocWatcherPE();

  /**********************************************************************/
  /* OdDbEvalWatcherPE overrides                                        */
  /**********************************************************************/
  void modified(OdDbObject* pObj, const OdDbObject* pAssocObj);

  /**********************************************************************/
  /* OdDbDimAssocWatcherPE overrides                                    */
  /**********************************************************************/
  void copied(OdDbObject *pObj, const OdDbObject *pAssocObj, const OdDbObject *pNewObj);
  void erased(OdDbObject *pObj, const OdDbObject *pAssocObj, bool erasing = true);

  /**********************************************************************/
  /* Start/end watching for modifying of associative hatches            */
  /**********************************************************************/
  void open();
  void close(bool bUpdate);

private:
  void clear();
  void update();

  OdDbObjectIdArray _removeAssocIds;
  int               _openCnt;
};

#endif // _ODEXDIMASSOCWATCHER_INCLUDED_
