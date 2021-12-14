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
using System.Globalization;
using Teigha.PointCloud;

namespace OdExPointCloudConverterSampleSwig
{
  public class OdExSourcePointPtsIterator : OdSourcePointIterator
  {
    private StreamReader m_pPtsFile;
    private long m_pointsCount;

    private long m_pointsRead;

    public OdExSourcePointPtsIterator(StreamReader pPtsFile, long pointsCount) : base()
    {
      m_pPtsFile = pPtsFile;
      m_pointsCount = pointsCount;
      start();
    }

    public void SetCMemOwn(bool bOwn)
    {
      swigCMemOwn = bOwn;
    }

    public override void start()
    {
      m_pointsRead = 0;
    }

    public override bool done()
    {
      if (m_pointsRead == m_pointsCount)
        return true;
      return false;
    }

    public override bool getPoint(OdSourcePoint point)
    {
      String odLine = m_pPtsFile.ReadLine();
      String[] paramsArr = odLine.Split(' ');
      point.m_coord.x = Double.Parse(paramsArr[0], CultureInfo.InvariantCulture);
      point.m_coord.y = Double.Parse(paramsArr[1], CultureInfo.InvariantCulture);
      point.m_coord.z = Double.Parse(paramsArr[2], CultureInfo.InvariantCulture);

      if (paramsArr.Length < 7)
      {
        if (paramsArr.Length > 3)
        {
          point.m_intensity = (float)Double.Parse(paramsArr[3], CultureInfo.InvariantCulture);
        }

        point.m_color = ODRGBA(255, 0, 0, 255);
      }
      else
      {
        point.m_intensity = (float)Double.Parse(paramsArr[3], CultureInfo.InvariantCulture);

        short r, g, b;
        r = Convert.ToInt16(paramsArr[4]);
        g = Convert.ToInt16(paramsArr[5]);
        b = Convert.ToInt16(paramsArr[6]);

        point.m_color = ODRGBA(r, g, b, 255);
      }

      ++m_pointsRead;

      return true;
    }

    private static uint ODRGBA(short r, short g, short b, short a)
    {
      return (uint)((a << 24) | (b << 16) |
                    (g << 8) | (r << 0));
    }
  }

}