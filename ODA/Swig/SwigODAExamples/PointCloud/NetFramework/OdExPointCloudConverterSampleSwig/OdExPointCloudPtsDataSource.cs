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

using System;
using System.Collections.Generic;
using System.Text;
using Teigha.Core;
using System.IO;
using Teigha.PointCloud;

namespace OdExPointCloudConverterSampleSwig
{
  public class OdExPointCloudPtsDataSource : OdPointCloudDataSource
  {
    private String m_filePath;
    private StreamReader m_pPtsFile;

    private long m_pointsCount;

    private Units m_unit;

    public OdExPointCloudPtsDataSource(String filePath) : this(filePath, Units.kMeter)
    {
    }

    public OdExPointCloudPtsDataSource(String filePath, Units unit)
    {
      m_filePath = filePath;
      m_unit = unit;
      OdResult res = Open(filePath);
      if (res != OdResult.eOk)
        throw new OdError_CantOpenFile(filePath);
    }

    public override ulong pointsCount()
    {
      return (ulong)m_pointsCount;
    }

    public override Units getUnits()
    {
      return m_unit;
    }

    public override OdSourcePointIterator newSourcePointIterator()
    {
      OdExSourcePointPtsIterator ret = new OdExSourcePointPtsIterator(m_pPtsFile, m_pointsCount);

      //.NET object not need dispose C++ object, because C++ procedure after execute dispose object (object is SharedPtr)
      ret.SetCMemOwn(false);
      return ret;
    }

    private OdResult Open(String filePath)
    {
      unchecked
      {
        if (!Globals.odrxSystemServices().accessFile(filePath, (int)FileAccessMode.kFileRead))
          return OdResult.eCantOpenFile;
      }

      m_filePath = filePath;
      m_pPtsFile = new StreamReader(m_filePath);

      m_pointsCount = Convert.ToInt64(m_pPtsFile.ReadLine());

      return OdResult.eOk;
    }

  }

}