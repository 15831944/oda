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
using Teigha.Core;
using Teigha.PointCloud;

namespace OdExRcsPointCloudReadExSwig
{
  public class OdExRcpFileDumper : OdExConsoleDumper
  {
    private OdPointCloudProjectDatabase m_rcpDatabase;
    public OdExRcpFileDumper(OdPointCloudProjectDatabase rcpDatabase)
    {
      m_rcpDatabase = rcpDatabase;
    }

    public override void Dump()
    {
      String filePath = m_rcpDatabase.getProjectDatabaseFilePath();
      Console.WriteLine("Dump .rcp file: {0}\n\n", filePath);

      ulong totalAmountOfPoints = m_rcpDatabase.getTotalAmountOfPoints();
      Console.WriteLine("totalAmountOfPoints = {0}\n\n", totalAmountOfPoints);

      uint totalScansCount = m_rcpDatabase.getTotalScansCount();
      Console.WriteLine("totalScansCount = {0}\n\n", totalScansCount);

      uint totalRegionsCount = m_rcpDatabase.getTotalRegionsCount();
      Console.WriteLine("totalRegionsCount = {0}\n\n", totalRegionsCount);

      String coordSystemName = m_rcpDatabase.getCoordinateSystemName();
      Console.WriteLine("coordSystemName = {0}\n\n", coordSystemName);

      sbyte hasRGB = m_rcpDatabase.hasRGB();
      Console.WriteLine("hasRGB = {0}\n", hasRGB);
      sbyte hasNormals = m_rcpDatabase.hasNormals();
      Console.WriteLine("hasNormals = {0}\n", hasNormals);
      sbyte hasIntensity = m_rcpDatabase.hasIntensity();
      Console.WriteLine("hasIntensity = {0}\n\n", hasIntensity);

      OdGeMatrix3d globalTransform = m_rcpDatabase.getGlobalTransformation();
      OdGeVector3d globalTransform_CsXAxis = globalTransform.getCsXAxis();
      OdGeVector3d globalTransform_CsYAxis = globalTransform.getCsYAxis();
      OdGeVector3d globalTransform_CsZAxis = globalTransform.getCsZAxis();
      OdGePoint3d globalTransform_CsOrigin = globalTransform.getCsOrigin();
      Console.WriteLine("globalTransform :\n");
      Console.WriteLine("\t{0}  {1}  {2}  0.0\n", globalTransform_CsXAxis.x, globalTransform_CsXAxis.y, globalTransform_CsXAxis.z);
      Console.WriteLine("\t{0}  {1}  {2}  0.0\n", globalTransform_CsYAxis.x, globalTransform_CsYAxis.y, globalTransform_CsYAxis.z);
      Console.WriteLine("\t{0}  {1}  {2}  0.0\n", globalTransform_CsZAxis.x, globalTransform_CsZAxis.y, globalTransform_CsZAxis.z);
      Console.WriteLine("\t{0}  {1}  {2}  1.0\n\n", globalTransform_CsOrigin.x, globalTransform_CsOrigin.y, globalTransform_CsOrigin.z);

      Console.WriteLine("---------------------------------------------------------\n\n");

      OdPointCloudScanIterator pScanIt = m_rcpDatabase.getScanIterator();
      uint scanIndex = 0;
      for (; !pScanIt.done(); pScanIt.step())
      {
        Console.WriteLine("Data from SCAN[{0}] : \n\n", scanIndex);

        Console.WriteLine("Title = {0}\n", pScanIt.getScanTitle());
        Console.WriteLine("Visible = {0}\n\n", pScanIt.getScanIsVisible());

        OdGeMatrix3d scanTransform = pScanIt.getScanTransform();
        OdGeVector3d scanTransform_CsXAxis = scanTransform.getCsXAxis();
        OdGeVector3d scanTransform_CsYAxis = scanTransform.getCsYAxis();
        OdGeVector3d scanTransform_CsZAxis = scanTransform.getCsZAxis();
        OdGePoint3d scanTransform_CsOrigin = scanTransform.getCsOrigin();
        Console.WriteLine("scanTransform in project:\n");
        Console.WriteLine("\t{0}  {1}  {2}  0.0\n", scanTransform_CsXAxis.x, scanTransform_CsXAxis.y, scanTransform_CsXAxis.z);
        Console.WriteLine("\t{0}  {1}  {2}  0.0\n", scanTransform_CsYAxis.x, scanTransform_CsYAxis.y, scanTransform_CsYAxis.z);
        Console.WriteLine("\t{0}  {1}  {2}  0.0\n", scanTransform_CsZAxis.x, scanTransform_CsZAxis.y, scanTransform_CsZAxis.z);
        Console.WriteLine("\t{0}  {1}  {2}  1.0\n\n", scanTransform_CsOrigin.x, scanTransform_CsOrigin.y, scanTransform_CsOrigin.z);

        OdPointCloudScanDatabase pScanDb = pScanIt.getScanDb();
        OdExRcsFileDumper rcsDumper = new OdExRcsFileDumper(pScanDb);
        rcsDumper.Dump();

        Console.WriteLine("---------------------------------------------------------\n\n");

        scanIndex++;
      }
    }
  }
}