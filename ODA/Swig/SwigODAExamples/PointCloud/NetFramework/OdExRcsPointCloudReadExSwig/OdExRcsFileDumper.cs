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
  public class OdExRcsFileDumper : OdExConsoleDumper
  {
    private OdPointCloudScanDatabase m_rcsDatabase;
    public OdExRcsFileDumper(OdPointCloudScanDatabase rcsDatabase)
    {
      m_rcsDatabase = rcsDatabase;
    }

    public override void Dump()
    {
      String filePath = m_rcsDatabase.getScanDatabaseFilePath();
      Console.WriteLine("Dump .rcs file: {0} \n\n", filePath);

      OdGeVector3d translation = m_rcsDatabase.getTranslation();
      Console.WriteLine("translation = ({0}, {1}, {2})\n", translation.x, translation.y, translation.z);
      OdGeVector3d rotation = m_rcsDatabase.getRotation();
      Console.WriteLine("rotation = ({0}, {1}, {2})\n", rotation.x, rotation.y, rotation.z);
      OdGeVector3d scale = m_rcsDatabase.getScale();
      Console.WriteLine("scale = ({0}, {1}, {2})\n\n", scale.x, scale.y, scale.z);

      OdGeExtents3d extents = m_rcsDatabase.getExtents();
      Console.WriteLine("extents.minPoint = ({0}, {1}, {2})\n", extents.minPoint().x, extents.minPoint().y, extents.minPoint().z);
      Console.WriteLine("extents.maxPoint = ({0}, {1}, {2})\n\n", extents.maxPoint().x, extents.maxPoint().y, extents.maxPoint().z);

      OdGeExtents3d transformedExtents = m_rcsDatabase.getTransformedExtents();
      Console.WriteLine("transformedExtents.minPoint = ({0}, {1}, {2})\n", transformedExtents.minPoint().x,
                           transformedExtents.minPoint().y, transformedExtents.minPoint().z);
      Console.WriteLine("transformedExtents.maxPoint = ({0}, {1}, {2})\n\n", transformedExtents.maxPoint().x,
                           transformedExtents.maxPoint().y, transformedExtents.maxPoint().z);

      ulong totalAmountOfPoints = m_rcsDatabase.getTotalAmountOfPoints();
      Console.WriteLine("totalAmountOfPoints = {0}\n\n", totalAmountOfPoints);

      bool hasRGB = m_rcsDatabase.hasRGB();
      Console.WriteLine("hasRGB = {0}\n", hasRGB);
      bool hasNormals = m_rcsDatabase.hasNormals();
      Console.WriteLine("hasNormals = {0}\n", hasNormals);
      bool hasIntensity = m_rcsDatabase.hasIntensity();
      Console.WriteLine("hasIntensity = {0}\n\n", hasIntensity);

      String scanId = m_rcsDatabase.getScanId();
      Console.WriteLine("scanId = {0}\n\n", scanId);

      bool normalizeIntensity = m_rcsDatabase.getNormalizeIntensity();
      Console.WriteLine("normalizeIntensity = {0}\n", normalizeIntensity);

      float maxIntensity = m_rcsDatabase.getMaxIntensity();
      Console.WriteLine("maxIntensity = {0}\n", maxIntensity);

      float minIntensity = m_rcsDatabase.getMinIntensity();
      Console.WriteLine("minIntensity = {0}\n", minIntensity);

      uint rangeImageWidth = m_rcsDatabase.getRangeImageWidth();
      Console.WriteLine("rangeImageWidth = {0}\n", rangeImageWidth);

      uint rangeImageHeight = m_rcsDatabase.getRangeImageHeight();
      Console.WriteLine("rangeImageHeight = {0}\n", rangeImageHeight);

      bool isLidarData = m_rcsDatabase.isLidarData();
      Console.WriteLine("isLidarData = {0}\n\n", isLidarData);

      ulong amountOfVoxels = m_rcsDatabase.getAmountOfVoxels();
      Console.WriteLine("amountOfVoxels = {0}\n\n", amountOfVoxels);

      OdRcsVoxelIterator pVoxelIt = m_rcsDatabase.getVoxelIterator();
      ulong voxelIndex = 0;
      for (; !pVoxelIt.done(); pVoxelIt.step())
      {
        OdRcsVoxel pVoxel = pVoxelIt.getVoxel();

        Console.WriteLine("Data from Voxel[{0}] :\n\n", voxelIndex);

        OdGeExtents3d voxelExtents = pVoxel.getExtents();
        Console.WriteLine("\tvoxelExtents.minPoint = ({0}, {1}, {2})\n", voxelExtents.minPoint().x,
                             voxelExtents.minPoint().y, voxelExtents.minPoint().z);
        Console.WriteLine("\tvoxelExtents.maxPoint = ({0}, {1}, {2})\n\n", voxelExtents.maxPoint().x,
          voxelExtents.maxPoint().y, voxelExtents.maxPoint().z);

        OdGeMatrix3d transformMatrix = m_rcsDatabase.getTransformMatrix();

        OdGeExtents3d transformedVoxelExtents = voxelExtents;
        transformedVoxelExtents.transformBy(transformMatrix);
        Console.WriteLine("\ttransformedVoxelExtents.minPoint = ({0}, {1}, {2})\n", transformedVoxelExtents.minPoint().x,
          transformedVoxelExtents.minPoint().y, transformedVoxelExtents.minPoint().z);
        Console.WriteLine("\ttransformedVoxelExtents.maxPoint = ({0}, {1}, {2})\n\n", transformedVoxelExtents.maxPoint().x,
          transformedVoxelExtents.maxPoint().y, transformedVoxelExtents.maxPoint().z);

        uint numberOfPoints = pVoxel.getTotalNumberOfPoints();
        Console.WriteLine("\tnumberOfPoints = {0}\n\n", numberOfPoints);

        Console.WriteLine("\tPoints data (coordinates are printed as non-transformed) :\n\n");

        OdRcsPointDataIterator pPointDataIt = pVoxel.getPointDataIterator();
        OdGePoint3dArray coords = new OdGePoint3dArray();
        OdCmEntityColorArray colors = new OdCmEntityColorArray();
        OdUInt16Array normalIndexes = new OdUInt16Array();
        OdUInt8Array intensities = new OdUInt8Array();
        pPointDataIt.getPoints(coords, colors, normalIndexes, intensities, numberOfPoints);

        OdGePoint3d voxelExtentsMinPoint = voxelExtents.minPoint();
        for (int pointIndex = 0; pointIndex < numberOfPoints; pointIndex++)
        {
          OdGeVector3d vec = coords[pointIndex].asVector();
          vec += voxelExtentsMinPoint.asVector();

          Console.WriteLine("\tcoordinates = ({0}, {1}, {2});\n", coords[pointIndex].x,
                                                                           coords[pointIndex].y,
                                                                           coords[pointIndex].z);

          byte red = colors[pointIndex].red();
          byte green = colors[pointIndex].green();
          byte blue = colors[pointIndex].blue();
          Console.WriteLine("\t\tRGB = 0x{0:X} 0x{1:X} 0x{2:X}; ", red, green, blue);

          Console.WriteLine("normalIndex = {0}; ", normalIndexes[pointIndex]);

          Console.WriteLine("intensity = {0}\n", intensities[pointIndex]);
        }

        Console.WriteLine("\n");
        voxelIndex++;
      }
    }
  }
}