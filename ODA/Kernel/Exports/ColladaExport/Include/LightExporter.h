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
#ifndef __LIGHTXPORTER_H__
#define __LIGHTXPORTER_H__

#include "OdaCommon.h"
#include "COLLADASWStreamWriter.h"
#include "COLLADASWLibraryLights.h"
#include "Ge/GePosition3d.h"
#include "Gi/GiDrawable.h"
#include "Gi/GiLightTraitsData.h"
#include "COLLADABUIDList.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_COLLADA_EXPORT
{
  /** \details
    This class implements the light exporter for Collada.
  */
  class LightExporter : public COLLADASW::LibraryLights
  {
    OdStringArray& m_lightNamesArr;
    OdGePoint3dArray& m_lightPositionsArr;
    OdGeVector3dArray& m_lightDirsArr;
  public:
    /** \details
      Constructor for the light exporter.
    */
    LightExporter(COLLADASW::StreamWriter* streamWriter, OdStringArray& lightNamesArr, OdGePoint3dArray& lightPositionsArr, OdGeVector3dArray& lightDirsArr);

    /** \details
      Destructor for the light exporter.
    */
    ~LightExporter();

    /** \details
      Exports the lights.
    */
    void exportLight(unsigned int iLightCounter, OdGiLightTraitsData* pLightTData, const ODCOLORREF* pPallete);
  };
}

#endif //__LIGHTXPORTER_H__
