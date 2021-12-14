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
/* Implementation of the ExProtocolExtension class                      */
/************************************************************************/
#include "OdaCommon.h"
#include "ExProtocolExtension.h"
#include "RxObjectImpl.h"
#include "Db2LineAngularDimension.h"
#include "Db2dPolyline.h"
#include "Db3PointAngularDimension.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "Db3dSolid.h"
#include "DbAlignedDimension.h"
#include "DbArc.h"
#include "DbArcAlignedText.h"
#include "DbArcDimension.h"
#include "DbAttribute.h" 
#include "DbBlockReference.h"
#include "DbBlockTableRecord.h"
#include "DbBody.h"
#include "DbCircle.h"
#include "DbDiametricDimension.h"
#include "DbEllipse.h"
#include "DbFace.h"
#include "DbFaceRecord.h"
#include "DbFcf.h"
#include "DbHatch.h"
#include "DbIndex.h"
#include "DbLine.h"
#include "DbMInsertBlock.h"
#include "DbMText.h"
#include "DbMline.h"
#include "DbOle2Frame.h"
#include "DbOrdinateDimension.h"
#include "DbPoint.h"
#include "DbPolyFaceMesh.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbPolygonMesh.h"
#include "DbPolygonMeshVertex.h"
#include "DbPolyline.h"
#include "DbProxyEntity.h"
#include "DbRadialDimension.h"
#include "DbRasterImage.h"
#include "DbRay.h"
#include "DbRegion.h"
#include "DbRotatedDimension.h"
#include "DbShape.h"
#include "DbSolid.h"
#include "DbSpatialFilter.h"
#include "DbSpline.h"
#include "DbTable.h"
#include "DbTrace.h"
#include "DbViewport.h"
#include "DbWipeout.h"
#include "DbXline.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve2d.h"
#include "GeometryFromProxy.h"
#include "GiWorldDrawDumper.h"
#include "Gs/Gs.h"

#include "OdFileBuf.h"
#include "StaticRxObject.h"

#include "toString.h"
#include "DbProxyExt.h"

/************************************************************************/
/* Construction/Destruction                                             */
/************************************************************************/

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbEntity_Dumper, OdRxObject)
ODRX_NO_CONS_DEFINE_MEMBERS(OdDbObject_Dumper, OdRxObject);

ExProtocolExtension::ExProtocolExtension()
  : m_pDumpers(0)
{

}

ExProtocolExtension::~ExProtocolExtension()
{
  uninitialize();
}

/************************************************************************/
/* Dump the common data and WorldDraw information for all               */
/* entities without explicit dumpers                                    */
/************************************************************************/
void OdDbEntity_Dumper::dump(OdDbEntity* pEnt, int indent) const
{
  writeLine(indent++, toString(pEnt->isA()),toString(pEnt->getDbHandle()));
  dumpEntityData(pEnt, indent);
  writeLine(indent, OD_T("WorldDraw()"));
  
  /**********************************************************************/
  /* Create an OdGiContext instance for the vectorization               */
  /**********************************************************************/
  OdGiContextDumper ctx(pEnt->database());
  /**********************************************************************/
  /* Create an OdGiWorldDraw instance for the vectorization             */
  /**********************************************************************/
  OdGiWorldDrawDumper wd(indent + 1);
  /**********************************************************************/
  /* Set the context                                                    */
  /**********************************************************************/
  wd.setContext(&ctx);
  /**********************************************************************/
  /* Call worldDraw()                                                   */
  /**********************************************************************/
  pEnt->worldDraw(&wd);
}

/************************************************************************/
/* Dump the common data and WorldDraw information for all               */
/* entities without explicit dumpers                                    */
/************************************************************************/
void OdDbObject_Dumper::dump(OdDbObject* pObj, int indent) const
{
  // TODO
}

/************************************************************************/
/* Dump data common to all entities                                     */
/************************************************************************/
void dumpEntityData(OdDbEntity* pEnt, int indent)
{
  OdGeExtents3d extents;
  if (eOk == pEnt->getGeomExtents(extents)) {
    writeLine(indent, OD_T("Min Extents"),        toString(extents.minPoint()));
    writeLine(indent, OD_T("Max Extents"),        toString(extents.maxPoint()));
  }
  writeLine(indent, OD_T("Layer"),                toString(pEnt->layer()));
  writeLine(indent, OD_T("Color Index"),          toString(pEnt->colorIndex()));
  writeLine(indent, OD_T("Color"),                toString(pEnt->color()));
  writeLine(indent, OD_T("Linetype"),             toString(pEnt->linetype()));
  writeLine(indent, OD_T("LTscale"),              toString(pEnt->linetypeScale()));
  writeLine(indent, OD_T("Lineweight"),           toString(pEnt->lineWeight()));
  writeLine(indent, OD_T("Plot Style"),           toString(pEnt->plotStyleName()));
  writeLine(indent, OD_T("Transparency Method"),  toString(pEnt->transparency().method()));
  writeLine(indent, OD_T("Visibility"),           toString(pEnt->visibility()));
  writeLine(indent, OD_T("Planar"),               toString(pEnt->isPlanar()));

  OdGePlane plane;
  OdDb::Planarity planarity = OdDb::kNonPlanar;
  pEnt->getPlane(plane, planarity);
  writeLine(indent, OD_T("Planarity"),            toString(planarity));
  if (pEnt->isPlanar()) 
  {
    OdGePoint3d origin;
    OdGeVector3d uAxis;
    OdGeVector3d vAxis;
    plane.get(origin, uAxis, vAxis);
    writeLine(indent+1, OD_T("Origin"),           toString(origin));
    writeLine(indent+1, OD_T("u-Axis"),           toString(uAxis));
    writeLine(indent+1, OD_T("v-Axis"),           toString(vAxis));
  }

}
/************************************************************************/
/* Dump Text data                                                       */
/************************************************************************/
void dumpTextData(OdDbText* pText, int indent)
{
  writeLine(indent, OD_T("Text String"),          toString(pText->textString()));
  writeLine(indent, OD_T("Text Position"),        toString(pText->position()));
  writeLine(indent, OD_T("Default Alignment"),    toString(pText->isDefaultAlignment()));
  writeLine(indent, OD_T("Alignment Point"),      toString(pText->alignmentPoint()));
  writeLine(indent, OD_T("Height"),               toString(pText->height()));
  writeLine(indent, OD_T("Rotation"),             toDegreeString(pText->rotation()));
  writeLine(indent, OD_T("Horizontal Mode"),      toString(pText->horizontalMode()));
  writeLine(indent, OD_T("Vertical Mode"),        toString(pText->verticalMode()));
  writeLine(indent, OD_T("Mirrored in X"),        toString(pText->isMirroredInX()));
  writeLine(indent, OD_T("Mirrored in Y"),        toString(pText->isMirroredInY()));
  writeLine(indent, OD_T("Oblique"),              toDegreeString(pText->oblique()));
  writeLine(indent, OD_T("Text Style"),           toString(pText->textStyle()));
  writeLine(indent, OD_T("Width Factor"),         toString(pText->widthFactor()));

   /*********************************************************************/
  /* Dump Bounding Points                                               */
  /**********************************************************************/
  OdGePoint3dArray points;
  pText->getBoundingPoints(points);
  writeLine(indent, OD_T("TL Bounding Point"),    toString(points[0]));
  writeLine(indent, OD_T("TR Bounding Point"),    toString(points[1]));
  writeLine(indent, OD_T("BL Bounding Point"),    toString(points[2]));
  writeLine(indent, OD_T("BR Bounding Point"),    toString(points[3]));
  writeLine(indent, OD_T("Normal"),               toString(pText->normal()));
  writeLine(indent, OD_T("Thickness"),            toString(pText->thickness()));
  dumpEntityData(pText, indent);
};

/************************************************************************/
/* Dump Attribute data                                                  */
/************************************************************************/
void dumpAttributeData(int indent, OdDbAttribute* pAttr, int i)
{
  writeLine(indent++, toString(pAttr->isA()), toString(i));
  writeLine(indent, OD_T("Handle"),                 toString(pAttr->getDbHandle()));
  writeLine(indent, OD_T("Tag"),                    toString(pAttr->tag()));
  writeLine(indent, OD_T("Field Length"),           toString(pAttr->fieldLength()));
  writeLine(indent, OD_T("Invisible"),              toString(pAttr->isInvisible()));
  writeLine(indent, OD_T("Preset"),                 toString(pAttr->isPreset()));
  writeLine(indent, OD_T("Verifiable"),             toString(pAttr->isVerifiable()));
  writeLine(indent, OD_T("Locked in Position"),     toString(pAttr->lockPositionInBlock()));
  writeLine(indent, OD_T("Constant"),               toString(pAttr->isConstant()));
  dumpTextData(pAttr, indent);
};

/************************************************************************/
/* Dump Block Reference Data                                             */
/************************************************************************/
void dumpBlockRefData (OdDbBlockReference* pBlkRef, int indent)
{
  writeLine(indent, OD_T("Position"),                toString(pBlkRef->position()));
  writeLine(indent, OD_T("Rotation"),                toDegreeString(pBlkRef->rotation()));
  writeLine(indent, OD_T("Scale Factors"),           toString(pBlkRef->scaleFactors()));
  writeLine(indent, OD_T("Normal"),                  toString(pBlkRef->normal()));
  dumpEntityData(pBlkRef, indent);
  /**********************************************************************/
  /* Dump the attributes                                                */
  /**********************************************************************/
  OdDbObjectIteratorPtr pIter = pBlkRef->attributeIterator();
  for (int i=0; !pIter->done(); i++, pIter->step())
  {
    OdDbAttributePtr pAttr = pIter->entity();
    if (!pAttr.isNull())
    {
      dumpAttributeData(indent, pAttr, i);
    }
  }
}

/************************************************************************/
/* Dump data common to all OdDbCurves                                   */
/************************************************************************/
void dumpCurveData(OdDbEntity* pEnt, int indent)
{
  OdDbCurvePtr pEntity = pEnt;
  OdGePoint3d startPoint;
  if (eOk == pEntity->getStartPoint(startPoint)) 
  {
    writeLine(indent, OD_T("Start Point"),              toString(startPoint));
  }

  OdGePoint3d endPoint;
  if (eOk == pEntity->getEndPoint(endPoint)) 
  {
    writeLine(indent, OD_T("End Point"),                toString(endPoint));
  }
  writeLine(indent, OD_T("Closed"),                     toString(pEntity->isClosed()));
  writeLine(indent, OD_T("Periodic"),                   toString(pEntity->isPeriodic()));

  double area;
  if (eOk == pEntity->getArea(area)) 
  {
    writeLine(indent, OD_T("Area"),                     toString(area));
  }
  dumpEntityData(pEntity, indent);
}
/************************************************************************/
/* Dump Dimension data                                                  */
/************************************************************************/
void dumpDimData(OdDbDimension* pDim, int indent)
{
  writeLine(indent, OD_T("Measurement"),                toString(pDim->getMeasurement()));
  writeLine(indent, OD_T("Dimension Text"),             toString(pDim->dimensionText()));

  if (pDim->getMeasurement() >= 0.0)
  {
    OdString formattedMeasurement;
    pDim->formatMeasurement(formattedMeasurement, 
        pDim->getMeasurement(), pDim->dimensionText());
    writeLine(indent, OD_T("Formatted Measurement"), formattedMeasurement);
  }
  OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::cast(pDim->dimBlockId().openObject());
  if (pBlock.isNull())
  {
    writeLine(indent, L"Dimension Block NULL");
  }
  else
  {
    writeLine(indent, OD_T("Dimension Block Name"),       toString(pBlock->getName()));
  }
  writeLine(indent+1, OD_T("Position"),                 toString(pDim->dimBlockPosition()));
  writeLine(indent+1, OD_T("Rotation"),                 toString(pDim->dimBlockRotation()));
  writeLine(indent+1, OD_T("Scale"),                    toString(pDim->dimBlockScale()));
  writeLine(indent, OD_T("Text Position"),              toString(pDim->textPosition()));
  writeLine(indent, OD_T("Text Rotation"),              toDegreeString(pDim->textRotation()));
  writeLine(indent, OD_T("Dimension Style"),            toString(pDim->dimensionStyle()));
  OdCmColor bgrndTxtColor;
  OdUInt16 bgrndTxtFlags = pDim->getBgrndTxtColor(bgrndTxtColor);
  writeLine(indent, OD_T("Background Text Color"),      toString(bgrndTxtColor));
  writeLine(indent, OD_T("BackgroundText Flags"),       toString(bgrndTxtFlags));
  writeLine(indent, OD_T("Extension Line 1 Linetype"),  toString(pDim->getDimExt1Linetype()));
  writeLine(indent, OD_T("Extension Line 2 Linetype"),  toString(pDim->getDimExt2Linetype()));
  writeLine(indent, OD_T("Dim Line Linetype"),          toString(pDim->getDimExt2Linetype()));
  writeLine(indent, OD_T("Horizontal Rotation"),        toDegreeString(pDim->horizontalRotation()));
  writeLine(indent, OD_T("Elevation"),                  toString(pDim->elevation()));
  writeLine(indent, OD_T("Normal"),                     toString(pDim->normal()));
  dumpEntityData(pDim, indent);
};

/************************************************************************/
/* 2 Line Angular Dimension Dumper                                      */
/************************************************************************/
class OdDb2LineAngularDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDb2LineAngularDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),   toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Arc Point"),               toString(pDim->arcPoint()));
    writeLine(indent, OD_T("Extension Line 1 Start"),  toString(pDim->xLine1Start()));
    writeLine(indent, OD_T("Extension Line 1 End"),    toString(pDim->xLine1End()));
    writeLine(indent, OD_T("Extension Line 2 Start"),  toString(pDim->xLine2Start()));
    writeLine(indent, OD_T("Extension Line 2 End"),    toString(pDim->xLine2End()));
    dumpDimData(pDim, indent);
  }
};

/************************************************************************/
/* Dump 2D Vertex data                                                  */
/************************************************************************/
void dump2dVertex(int indent, OdDb2dVertex* pVertex, int i)
{
    writeLine(indent++, toString(pVertex->isA()), toString(i));
    writeLine(indent, OD_T("Handle"),                   toString(pVertex->getDbHandle()));
    writeLine(indent, OD_T("Vertex Type"),              toString(pVertex->vertexType()));
    writeLine(indent, OD_T("Position"),                 toString(pVertex->position()));
    writeLine(indent, OD_T("Start Width"),              toString(pVertex->startWidth()));
    writeLine(indent, OD_T("End Width"),                toString(pVertex->endWidth()));
    writeLine(indent, OD_T("Bulge"),                    toString(pVertex->bulge()));

    if (pVertex->bulge() != 0) {
      writeLine(indent, OD_T("Bulge Angle"),            toDegreeString(4 * atan(pVertex->bulge())));
    }

    writeLine(indent, OD_T("Tangent Used"),             toString(pVertex->isTangentUsed()));
    if (pVertex->isTangentUsed()) {
      writeLine(indent, OD_T("Tangent"),                toString(pVertex->tangent()));
    }
}

/************************************************************************/
/* 2D Polyline Dumper                                                   */
/************************************************************************/
class OdDb2dPolyline_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDb2dPolylinePtr pPolyline = pEnt;
    writeLine(indent++, toString(pPolyline->isA()),   toString(pPolyline->getDbHandle()));
    writeLine(indent, OD_T("Elevation"),                    toString(pPolyline->elevation()));
    writeLine(indent, OD_T("Normal"),                       toString(pPolyline->normal()));
    writeLine(indent, OD_T("Thickness"),                    toString(pPolyline->thickness()));
    /********************************************************************/
    /* Dump the vertices                                                */
    /********************************************************************/
    OdDbObjectIteratorPtr pIter = pPolyline->vertexIterator();
    for (int i = 0; !pIter->done(); i++, pIter->step())
    {
      OdDb2dVertexPtr pVertex = pIter->entity();
      if (pVertex.get())
      {
        dump2dVertex(indent, pVertex, i);
      }
    }
    dumpCurveData(pPolyline, indent);
  }
};


/************************************************************************/
/* Dump 3D Polyline Vertex data                                         */
/************************************************************************/
void dump3dPolylineVertex(int indent, OdDb3dPolylineVertex* pVertex, int i)
{
    writeLine(indent++, toString(pVertex->isA()), toString(i));
    writeLine(indent, OD_T("Handle"),                   toString(pVertex->getDbHandle()));
    writeLine(indent, OD_T("Type"),                     toString(pVertex->vertexType()));
    writeLine(indent, OD_T("Position"),                 toString(pVertex->position()));
};

/************************************************************************/
/* 3D Polyline Dumper                                                   */
/************************************************************************/
class OdDb3dPolyline_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDb3dPolylinePtr pPolyline = pEnt;
    writeLine(indent++, toString(pPolyline->isA()),toString(pPolyline->getDbHandle()));
    /********************************************************************/
    /* Dump the vertices                                                */
    /********************************************************************/
    OdDbObjectIteratorPtr pIter = pPolyline->vertexIterator();
    for (int i = 0; !pIter->done(); i++, pIter->step())
    {
      OdDb3dPolylineVertexPtr pVertex = pIter->entity();
      if (pVertex.get())
      {
        dump3dPolylineVertex(indent, pVertex, i);
      }
    }
    dumpCurveData(pPolyline, indent);
  }
};

/************************************************************************/
/* 3DSolid Dumper                                                       */
/************************************************************************/
class OdDb3dSolid_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDb3dSolidPtr  pSolid = pEnt;
    writeLine(indent++, toString(pSolid->isA()),  toString(pSolid->getDbHandle()));
    dumpEntityData(pSolid, indent);
  }
};

/************************************************************************/
/* 3 Point Angular Dimension Dumper                                     */
/************************************************************************/
class OdDb3PointAngularDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDb3PointAngularDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),    toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Arc Point"),                toString(pDim->arcPoint()));
    writeLine(indent, OD_T("Center Point"),             toString(pDim->centerPoint()));
    writeLine(indent, OD_T("Extension Line 1 Point"),   toString(pDim->xLine1Point()));
    writeLine(indent, OD_T("Extension Line 2 Point"),   toString(pDim->xLine2Point()));
    dumpDimData(pDim, indent);
  }
};

/************************************************************************/
/* Aligned Dimension Dumper                                             */
/************************************************************************/
class OdDbAlignedDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDbAlignedDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),   toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Dimension line Point"),    toString(pDim->dimLinePoint()));
    writeLine(indent, OD_T("Oblique"),                 toDegreeString(pDim->oblique()));
    writeLine(indent, OD_T("Extension Line 1 Point"),  toString(pDim->xLine1Point()));
    writeLine(indent, OD_T("Extension Line 2 Point"),  toString(pDim->xLine2Point()));
    dumpDimData(pDim, indent);
  }
};
/************************************************************************/
/* Arc Dumper                                                           */
/************************************************************************/
class OdDbArc_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbArcPtr pArc = pEnt;
    writeLine(indent++, toString(pArc->isA()),  toString(pArc->getDbHandle()));
    writeLine(indent, OD_T("Center"),                 toString(pArc->center()));
    writeLine(indent, OD_T("Radius"),                 toString(pArc->radius()));
    writeLine(indent, OD_T("Start Angle"),            toDegreeString(pArc->startAngle()));
    writeLine(indent, OD_T("End Angle"),              toDegreeString(pArc->endAngle()));
    writeLine(indent, OD_T("Normal"),                 toString(pArc->normal()));
    writeLine(indent, OD_T("Thickness"),              toString(pArc->thickness()));
    dumpCurveData(pArc, indent);
  }
};

/************************************************************************/
/* Arc Aligned Text                                                           */
/************************************************************************/
class OdDbArcAlignedText_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbArcAlignedTextPtr pText = pEnt;  
    writeLine(indent++, toString(pText->isA()), toString(pText->getDbHandle()));
    writeLine(indent, OD_T("Center"),                 toString(pText->center()));
    writeLine(indent, OD_T("Radius"),                 toString(pText->radius()));
    writeLine(indent, OD_T("Start Angle"),            toDegreeString(pText->startAngle()));
    writeLine(indent, OD_T("End Angle"),              toDegreeString(pText->endAngle()));
    writeLine(indent, OD_T("Normal"),                 toString(pText->normal()));
    if (!pText->arcId().isNull())
    {
       writeLine(indent, OD_T("Associated Arc"),      toString(pText->arcId().getHandle()));
    }
    else
    {
      writeLine(indent, OD_T("Associated Arc"), OD_T("Null"));
    }
    writeLine(indent, OD_T("Text String"),            toString(pText->textString()));
    writeLine(indent, OD_T("Alignment"),              toString(pText->alignment()));
    writeLine(indent, OD_T("Character Spacing"),      toString(pText->charSpacing()));
    writeLine(indent, OD_T("Reversed Char Order"),    toString(pText->isReversedCharOrder()));
    writeLine(indent, OD_T("Offset from Arc"),        toString(pText->offsetFromArc()));
    writeLine(indent, OD_T("Left Offset"),            toString(pText->leftOffset()));
    writeLine(indent, OD_T("Right Offset"),           toString(pText->rightOffset()));
    writeLine(indent, OD_T("Text Position"),          toString(pText->textPosition()));
    writeLine(indent, OD_T("Text Direction"),         toString(pText->textDirection()));
    writeLine(indent, OD_T("Text Style"),             toString(pText->textStyle()));
    writeLine(indent, OD_T("Text Height"),            toString(pText->textSize()));
    writeLine(indent, OD_T("Width Factor"),           toString(pText->xScale()));
    writeLine(indent, OD_T("SHX font"),               toString(pText->isShxFont()));    
    writeLine(indent, OD_T("Filename"),               shortenPath(pText->fileName()));
    writeLine(indent, OD_T("BigFont Filename"),       shortenPath(pText->bigFontFileName()));

    OdString typeface;
    bool bold;
    bool italic;
    int charset;
    int pitchAndFamily;
    pText->font(typeface, bold, italic, charset, pitchAndFamily); 
    writeLine(indent, OD_T("Typeface"),               toString(typeface));
    writeLine(indent, OD_T("Character Set"),          toString(charset));
    writeLine(indent, OD_T("Bold"),                   toString(bold));
    writeLine(indent, OD_T("Italic"),                 toString(italic));
    writeLine(indent, OD_T("Font Pitch and Family"),  toHexString(pitchAndFamily));
    dumpEntityData(pText, indent);
  }
};
/************************************************************************/
/* Arc Dimension Dumper                                                 */
/************************************************************************/
class OdDbArcDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDbArcDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),   toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Arc Point"),               toString(pDim->arcPoint()));
    writeLine(indent, OD_T("Center Point"),            toString(pDim->centerPoint()));
    writeLine(indent, OD_T("Arc symbol"),              toArcSymbolTypeString(pDim->arcSymbolType()));
    writeLine(indent, OD_T("Partial"),                 toString(pDim->isPartial()));
    writeLine(indent, OD_T("Has leader"),              toString(pDim->hasLeader()));

    if (pDim->hasLeader())
    {
      writeLine(indent, OD_T("Leader 1 Point"),        toString(pDim->leader1Point()));
      writeLine(indent, OD_T("Leader 2 Point"),        toString(pDim->leader2Point()));
    }
    writeLine(indent, OD_T("Extension Line 1 Point"),  toString(pDim->xLine1Point()));
    writeLine(indent, OD_T("Extension Line 2 Point"),  toString(pDim->xLine2Point()));
    dumpDimData(pDim, indent);
  }
};

/************************************************************************/
/* Block Reference Dumper                                                */
/************************************************************************/
class OdDbBlockReference_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbBlockReferencePtr   pBlkRef = pEnt;
    writeLine(indent++, toString(pBlkRef->isA()),       toString(pBlkRef->getDbHandle()));

    OdDbBlockTableRecordPtr pRecord = pBlkRef->blockTableRecord().openObject();
    if (pRecord.get())
    {
      writeLine(indent, OD_T("Name"),                           toString(pRecord->getName()));
    }

    dumpBlockRefData(pBlkRef, indent);

    OdDbSpatialFilterPtr pFilt = OdDbIndexFilterManager::getFilter(pBlkRef,
      OdDbSpatialFilter::desc(), OdDb::kForRead);
    
    /********************************************************************/
    /* Dump the Spatial Filter  (Xref Clip)                             */
    /********************************************************************/
    if(pFilt.get())
    {
      writeLine(indent++, toString(pFilt->isA()),         toString(pFilt->getDbHandle()));
      OdGePoint2dArray points;
      OdGeVector3d normal;
      double elevation, frontClip, backClip;
      bool   enabled;
      pFilt->getDefinition(points, normal, elevation, frontClip, backClip, enabled);
      writeLine(indent, OD_T("Normal"),                         toString(normal));
      writeLine(indent, OD_T("Elevation"),                      toString(elevation));
      writeLine(indent, OD_T("Front Clip Distance"),            toString(frontClip));
      writeLine(indent, OD_T("Back Clip Distance"),             toString(backClip));
      writeLine(indent, OD_T("Enabled"),                        toString(enabled));
      for (int i = 0; i < (int) points.size(); i++)
      {
        writeLine(indent, toString(OD_T("Clip point %d"),i),    toString(points[i]));
      }
    }
  }
};
/************************************************************************/
/* Body Dumper                                                        */
/************************************************************************/
class OdDbBody_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbBodyPtr  pBody = pEnt;
    writeLine(indent++, toString(pBody->isA()), toString(pBody->getDbHandle()));
    dumpEntityData(pBody, indent);
  }
};

/************************************************************************/
/* Circle Dumper                                                        */
/************************************************************************/
class OdDbCircle_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbCirclePtr pCircle = pEnt;
    writeLine(indent++, toString(pCircle->isA()), toString(pCircle->getDbHandle()));
    writeLine(indent, OD_T("Center"),                   toString(pCircle->center()));
    writeLine(indent, OD_T("Radius"),                   toString(pCircle->radius()));
    writeLine(indent, OD_T("Diameter"),                 toString(2*pCircle->radius()));
    writeLine(indent, OD_T("Normal"),                   toString(pCircle->normal()));
    writeLine(indent, OD_T("Thickness"),                toString(pCircle->thickness()));
    dumpCurveData(pCircle, indent);
  }
};

/************************************************************************/
/* Diametric Dimension Dumper                                           */
/************************************************************************/
class OdDbDiametricDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDbDiametricDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),   toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Chord Point"),             toString(pDim->chordPoint()));
    writeLine(indent, OD_T("Far chord Point"),         toString(pDim->farChordPoint()));
    writeLine(indent, OD_T("Leader Length"),           toString(pDim->leaderLength()));
    dumpDimData(pDim, indent);
  }
};

/************************************************************************/
/* Ellipse Dumper                                                       */
/************************************************************************/
class OdDbEllipse_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbEllipsePtr pEllipse = pEnt;
    writeLine(indent++, toString(pEllipse->isA()),  toString(pEllipse->getDbHandle()));
    writeLine(indent, OD_T("Center"),                     toString(pEllipse->center()));
    writeLine(indent, OD_T("Major Axis"),                 toString(pEllipse->majorAxis()));
    writeLine(indent, OD_T("Minor Axis"),                 toString(pEllipse->minorAxis()));
    writeLine(indent, OD_T("Major Radius"),               toString(pEllipse->majorAxis().length()));
    writeLine(indent, OD_T("Minor Radius"),               toString(pEllipse->minorAxis().length()));
    writeLine(indent, OD_T("Radius Ratio"),               toString(pEllipse->radiusRatio()));
    writeLine(indent, OD_T("Start Angle"),                toDegreeString(pEllipse->startAngle()));
    writeLine(indent, OD_T("End Angle"),                  toDegreeString(pEllipse->endAngle()));
    writeLine(indent, OD_T("Normal"),                     toString(pEllipse->normal()));
    dumpCurveData(pEllipse, indent);
   }
};
/************************************************************************/
/* Face Dumper                                                       */
/************************************************************************/
class OdDbFace_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbFacePtr pFace = pEnt;
    writeLine(indent++, toString(pFace->isA()),           toString(pFace->getDbHandle()));
    int i;
    for (i = 0; i < 4; i++)
    {
      OdGePoint3d pt;
      pFace->getVertexAt((OdUInt16) i, pt);
      writeLine(indent, toString(OD_T("Vertex %d "), i),        toString(pt));
    }
    for (i = 0; i < 4; i++)
    {
      writeLine(indent, toString(OD_T("Edge %d visible"), i),   toString(pFace->isEdgeVisibleAt((OdUInt16) i)));
    }
    dumpEntityData(pFace, indent);
   }
};

/************************************************************************/
/* FCF Dumper                                                           */
/************************************************************************/
class OdDbFcf_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbFcfPtr  pFcf = pEnt;
    writeLine(indent++, toString(pFcf->isA()),      toString(pFcf->getDbHandle()));
    writeLine(indent, OD_T("Location"),                   toString(pFcf->location()));
    writeLine(indent, OD_T("Text"),                       toString(pFcf->text()));
    writeLine(indent, OD_T("Dimension Style"),            toString(pFcf->dimensionStyle()));
    writeLine(indent, OD_T("Dimension Gap"),              toString(pFcf->dimgap()));
    writeLine(indent, OD_T("Dimension Scale"),            toString(pFcf->dimscale()));
    writeLine(indent, OD_T("Text Height"),                toString(pFcf->dimtxt()));
    writeLine(indent, OD_T("Frame Color"),                toString(pFcf->dimclrd()));
    writeLine(indent, OD_T("Text Style"),                 toString(pFcf->dimtxsty()));
    writeLine(indent, OD_T("Text Color"),                 toString(pFcf->dimclrd()));
    writeLine(indent, OD_T("X-Direction"),                toString(pFcf->direction()));
    writeLine(indent, OD_T("Normal"),                     toString(pFcf->normal()));
    dumpEntityData(pFcf, indent);
  }
};

/************************************************************************/
/* Hatch Dumper                                                         */
/************************************************************************/
class OdDbHatch_Dumper : public OdDbEntity_Dumper
{
private:
  /***********************************************************************/
  /* Dump Polyline Loop                                                  */
  /***********************************************************************/
  static void dumpPolylineType(int loopIndex , OdDbHatchPtr &pHatch,int indent){

    OdGePoint2dArray vertices;
    OdGeDoubleArray bulges;
    pHatch->getLoopAt (loopIndex, vertices, bulges);
    bool hasBulges = (bulges.size() > 0);
    for (int i = 0; i < (int) vertices.size(); i++)
    {
      writeLine(indent, toString(OD_T("Vertex %d"), i),           toString(vertices[i]));
      if (hasBulges)
      {
        writeLine(indent+1, toString(OD_T("Bulge %d"), i),        toString(bulges[i]));
        writeLine(indent+1, toString(OD_T("Bulge angle %d"), i),  toDegreeString(4 * atan(bulges[i])));
      }
    }
  }

  /**********************************************************************/
  /* Dump Circular Arc Edge                                             */
  /**********************************************************************/
  static void dumpCircularArcEdge(int indent, OdGeCurve2d* pEdge) {
    OdGeCircArc2d* pCircArc = (OdGeCircArc2d*)pEdge;
    writeLine(indent, OD_T("Center"),                 toString(pCircArc->center()));
    writeLine(indent, OD_T("Radius"),                 toString(pCircArc->radius()));
    writeLine(indent, OD_T("Start Angle"),            toDegreeString(pCircArc->startAng()));
    writeLine(indent, OD_T("End Angle"),              toDegreeString(pCircArc->endAng()));
    writeLine(indent, OD_T("Clockwise"),              toString(pCircArc->isClockWise()));
  }
  
  /**********************************************************************/
  /* Dump Elliptical Arc Edge                                           */
  /**********************************************************************/
  static void dumpEllipticalArcEdge(int indent, OdGeCurve2d* pEdge) {
    OdGeEllipArc2d* pEllipArc = (OdGeEllipArc2d*)pEdge;
    writeLine(indent, OD_T("Center"),                 toString(pEllipArc->center()));
    writeLine(indent, OD_T("Major Radius"),           toString(pEllipArc->majorRadius()));
    writeLine(indent, OD_T("Minor Radius"),           toString(pEllipArc->minorRadius()));
    writeLine(indent, OD_T("Major Axis"),             toString(pEllipArc->majorAxis()));
    writeLine(indent, OD_T("Minor Axis"),             toString(pEllipArc->minorAxis()));
    writeLine(indent, OD_T("Start Angle"),            toDegreeString(pEllipArc->startAng()));
    writeLine(indent, OD_T("End Angle"),              toDegreeString(pEllipArc->endAng()));
    writeLine(indent, OD_T("Clockwise"),              toString(pEllipArc->isClockWise()));
  }
  /**********************************************************************/
  /* Dump NurbCurve Edge                                           */
  /**********************************************************************/
  static void dumpNurbCurveEdge(int indent, OdGeCurve2d* pEdge) {

    OdGeNurbCurve2d* pNurbCurve = (OdGeNurbCurve2d*)pEdge;
    int degree;
    bool rational, periodic;
    OdGePoint2dArray ctrlPts;
    OdGeDoubleArray weights;
    OdGeKnotVector  knots;

    pNurbCurve->getDefinitionData (degree, rational, periodic, knots, ctrlPts, weights);
    writeLine(indent, OD_T("Degree"),                             toString(degree));
    writeLine(indent, OD_T("Rational"),                           toString(rational));
    writeLine(indent, OD_T("Periodic"),                           toString(periodic));

    writeLine(indent, OD_T("Number of Control Points"),           toString((int) ctrlPts.size()));
    int i;
    for (i = 0; i < (int) ctrlPts.size(); i++)
    {
      writeLine(indent, toString(OD_T("Control Point %d"), i),    toString(ctrlPts[i]));
    }
    writeLine(indent, OD_T("Number of Knots"),                    toString(knots.length()));
    for (i = 0; i < knots.length(); i++)
    {
      writeLine(indent, toString(OD_T("Knot %d"), i),             toString(knots[i]));
    }

    if (rational) 
    {
      writeLine(indent, OD_T("Number of Weights"),                toString((int) weights.size()));
      for (i = 0; i < (int) weights.size(); i++)
      {
        writeLine(indent, toString(OD_T("Weight %d"), i),         toString(weights[i]));
      }
    }
  }
  
  /***********************************************************************/
  /* Dump Edge Loop                                                      */
  /***********************************************************************/
  static void dumpEdgesType(int loopIndex , OdDbHatchPtr &pHatch , int indent){
    
    EdgeArray edges;
    pHatch->getLoopAt (loopIndex, edges);
//    writeLine(indent++, OD_T("Edges"));
    for (int i = 0; i < (int) edges.size(); i++)
    {
      OdGeCurve2d* pEdge = edges[i];
      writeLine(indent, toString(OD_T("Edge %d"), i), toString(pEdge->type()));
      switch (pEdge->type ()) 
      {
        case OdGe::kLineSeg2d   :                                               break;
        case OdGe::kCircArc2d   : dumpCircularArcEdge(indent + 1, pEdge);   break;
        case OdGe::kEllipArc2d  : dumpEllipticalArcEdge(indent + 1, pEdge); break;
        case OdGe::kNurbCurve2d : dumpNurbCurveEdge(indent + 1, pEdge);     break;
        default:break;
      }
 
      /******************************************************************/
      /* Common Edge Properties                                         */
      /******************************************************************/
      OdGeInterval interval;
      pEdge->getInterval(interval);
      double lower;
      double upper;
      interval.getBounds(lower, upper);
      writeLine(indent+1, OD_T("Start Point"),    toString(pEdge->evalPoint(lower)));
      writeLine(indent+1, OD_T("End Point"),      toString(pEdge->evalPoint(upper)));
      writeLine(indent+1, OD_T("Closed"),         toString(pEdge->isClosed()));
    }
  }

public:
  
  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbHatchPtr pHatch = pEnt;
    writeLine(indent++, toString(pHatch->isA()),      toString(pHatch->getDbHandle()));
    writeLine(indent, OD_T("Hatch Style"),                  toString(pHatch->hatchStyle()));
    writeLine(indent, OD_T("Hatch Object Type"),            toString(pHatch->hatchObjectType()));
    writeLine(indent, OD_T("Is Hatch"),                     toString(pHatch->isHatch()));
    writeLine(indent, OD_T("Is Gradient"),                  toString(!pHatch->isGradient()));
    if (pHatch->isHatch())
    {
      /******************************************************************/
      /* Dump Hatch Parameters                                          */
      /******************************************************************/
      writeLine(indent, OD_T("Pattern Type"),               toString(pHatch->patternType()));
      switch (pHatch->patternType())
      {
        case OdDbHatch::kPreDefined:
        case OdDbHatch::kCustomDefined: 
          writeLine(indent, OD_T("Pattern Name"),           toString(pHatch->patternName()));
          writeLine(indent, OD_T("Solid Fill"),             toString(pHatch->isSolidFill()));
          if (!pHatch->isSolidFill())
          {          
            writeLine(indent, OD_T("Pattern Angle"),        toDegreeString(pHatch->patternAngle()));
            writeLine(indent, OD_T("Pattern Scale"),        toString(pHatch->patternScale()));
          }
          break;
        case OdDbHatch::kUserDefined:
          writeLine(indent, OD_T("Pattern Angle"),          toDegreeString(pHatch->patternAngle()));
          writeLine(indent, OD_T("Pattern Double"),         toString(pHatch->patternDouble()));
          writeLine(indent, OD_T("Pattern Space"),          toString(pHatch->patternSpace()));
          break;
      }      
    }
    if (pHatch->isGradient())
    {
      /******************************************************************/
      /* Dump Gradient Parameters                                       */
      /******************************************************************/
      writeLine(indent, OD_T("Gradient Type"),              toString(pHatch->gradientType()));
      writeLine(indent, OD_T("Gradient Name"),              toString(pHatch->gradientName()));
      writeLine(indent, OD_T("Gradient Angle"),             toDegreeString(pHatch->gradientAngle()));
      writeLine(indent, OD_T("Gradient Shift"),             toString(pHatch->gradientShift()));
      writeLine(indent, OD_T("Gradient One-Color Mode"),    toString(pHatch->getGradientOneColorMode()));
      if (pHatch->getGradientOneColorMode()) {
        writeLine(indent, OD_T("ShadeTintValue"),           toString(pHatch->getShadeTintValue()));
      }
      OdCmColorArray colors;
      OdGeDoubleArray values;
      pHatch->getGradientColors(colors, values);
      for (int i = 0; i < (int) colors.size(); i++)
      {
        writeLine(indent, toString(OD_T("Color         %d"), i),   toString(colors[i]));
        writeLine(indent, toString(OD_T("Interpolation %d"), i),   toString(values[i]));
      }
    }

    /********************************************************************/
    /* Dump Associated Objects                                          */
    /********************************************************************/
    writeLine(indent, OD_T("Associated objects"), toString(pHatch->associative()));
    OdDbObjectIdArray assocIds;
    pHatch->getAssocObjIds(assocIds);
    int i;
    for (i = 0; i < (int) assocIds.size(); i++)
    {
      OdDbEntityPtr pAssoc = assocIds[i].openObject();
      if (!pAssoc.isNull())
        writeLine(indent+1, toString(pAssoc->isA()),      toString(pAssoc->getDbHandle()));
    }

    /********************************************************************/
    /* Dump Seed Points                                                 */
    /********************************************************************/
    writeLine(indent, OD_T("Seed points"), toString(pHatch->numSeedPoints()));
    for (i = 0; i < pHatch->numSeedPoints(); i++)
    {
      writeLine(indent+1, toString(OD_T("Seed point %d"), i),   toString(pHatch->getSeedPointAt(i)));
    }    

    /********************************************************************/
    /* Dump Loops                                                       */
    /********************************************************************/
    writeLine(indent, OD_T("Loops"), toString(pHatch->numLoops()));
    for (i = 0; i < pHatch->numLoops(); i++)
    {
      writeLine(indent + 1, toString(OD_T("Loop %d"), i), toLooptypeString((int) pHatch->loopTypeAt(i)));

      /******************************************************************/
      /* Dump Loop                                                      */
      /******************************************************************/
      if(pHatch->loopTypeAt(i) & OdDbHatch::kPolyline)
      {
        dumpPolylineType(i , pHatch, indent + 2);
      }
      else
      {
        dumpEdgesType(i , pHatch , indent + 2);
      }
      /******************************************************************/
      /* Dump Associated Objects                                        */
      /******************************************************************/
      if (pHatch->associative())
      {
        writeLine(indent + 2, OD_T("Associated objects"));
        assocIds.clear();
        pHatch->getAssocObjIdsAt(i, assocIds);
        for (int j = 0; j < (int) assocIds.size(); j++)
        {
          OdDbEntityPtr pAssoc = assocIds[j].openObject();
          if (!pAssoc.isNull())
            writeLine(indent+3, toString(pAssoc->isA()),      toString(pAssoc->getDbHandle()));
        }
      }
    }    

    writeLine(indent, OD_T("Elevation"), toString(pHatch->elevation()));
    writeLine(indent, OD_T("Normal"), toString(pHatch->normal()));
    dumpEntityData(pHatch, indent);
  }
private:
};

/************************************************************************/
/* Leader Dumper                                                          */
/************************************************************************/
class OdDbLeader_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbLeaderPtr pLeader = pEnt;
    writeLine(indent++, toString(pLeader->isA()),     toString(pLeader->getDbHandle()));
    writeLine(indent, OD_T("Dimension Style"),              toString(pLeader->dimensionStyle()));
    
    OdDbObjectPtr pAnno = pLeader->annotationObjId().openObject();
    writeLine(indent, OD_T("Annotation"));
    if (!pAnno.isNull())
    {
      writeLine(indent+1, toString(pAnno->isA()),       toString(pAnno->getDbHandle()));
    }
    writeLine(indent+1, OD_T("Type"),                       toString(pLeader->annoType()));
    writeLine(indent+1, OD_T("Height"),                     toString(pLeader->annoHeight()));
    writeLine(indent+1, OD_T("Width"),                      toString(pLeader->annoWidth()));
    writeLine(indent+1, OD_T("Offset"),                     toString(pLeader->annotationOffset()));
    writeLine(indent+1, OD_T("X-Direction"),                toString(pLeader->annotationXDir()));
    writeLine(indent, OD_T("Has Arrowhead"),                toString(pLeader->hasArrowHead()));
    writeLine(indent, OD_T("Has Hook Line"),                toString(pLeader->hasHookLine()));

    if (pLeader->hasHookLine())
    {
      writeLine(indent, OD_T("Hook Line on X dir"),         toString(pLeader->isHookLineOnXDir()));
    }
    writeLine(indent, OD_T("Splined"),                      toString(pLeader->isSplined()));
    
    for (int i = 0; i <  pLeader->numVertices(); i++)
    { 
      writeLine(indent, toString(OD_T("Vertex %d"),i),      toString(pLeader->vertexAt(i)));
    }
    writeLine(indent, OD_T("Normal"),                       toString(pLeader->normal()));
    dumpCurveData(pLeader, indent);
  }
};

/************************************************************************/
/* Line Dumper                                                          */
/************************************************************************/
class OdDbLine_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbLinePtr pLine = pEnt;
    writeLine(indent++, toString(pLine->isA()),   toString(pLine->getDbHandle()));
    writeLine(indent, OD_T("Normal"),                   toString(pLine->normal()));
    writeLine(indent, OD_T("Thickness"),                toString(pLine->thickness()));
    dumpEntityData(pLine, indent);
  }
};
/************************************************************************/
/* MInsertBlock Dumper                                                  */
/************************************************************************/
class OdDbMInsertBlock_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbMInsertBlockPtr pMInsert = pEnt;
    writeLine(indent++, toString(pMInsert->isA()),  toString(pMInsert->getDbHandle()));

    OdDbBlockTableRecordPtr pRecord = pMInsert->blockTableRecord().safeOpenObject();
    writeLine(indent, OD_T("Name"),                       toString(pRecord->getName()));
    writeLine(indent, OD_T("Rows"),                       toString(pMInsert->rows()));
    writeLine(indent, OD_T("Columns"),                    toString(pMInsert->columns()));
    writeLine(indent, OD_T("Row Spacing"),                toString(pMInsert->rowSpacing()));
    writeLine(indent, OD_T("Column Spacing"),             toString(pMInsert->columnSpacing()));
    dumpBlockRefData(pMInsert,indent);
  }
};

/************************************************************************/
/* Mline Dumper                                                         */
/************************************************************************/
class OdDbMline_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbMlinePtr pMline = pEnt;
    writeLine(indent++, toString(pMline->isA()),  toString(pMline->getDbHandle()));
    writeLine(indent, OD_T("Style"),                    toString(pMline->style()));
    writeLine(indent, OD_T("Closed"),                   toString(pMline->closedMline()));
    writeLine(indent, OD_T("Scale"),                    toString(pMline->scale()));
    writeLine(indent, OD_T("Suppress Start Caps"),      toString(pMline->supressStartCaps()));
    writeLine(indent, OD_T("Suppress End Caps"),        toString(pMline->supressEndCaps()));
    writeLine(indent, OD_T("Normal"),                   toString(pMline->normal()));

    /********************************************************************/
    /* Dump the segment data                                            */
    /********************************************************************/
    for (int i = 0; i < pMline->numVertices(); i++)
    {
      OdMLSegmentArray segments;
      pMline->getParametersAt(i, segments);
      writeLine(indent, OD_T("Segment"),                toString(i));
      writeLine(indent+1,   OD_T("Vertex"),             toString(pMline->vertexAt(i)));
      for (int j = 0; j < (int) segments.size(); j++)
      {
        int k;
        OdString rightString;
        OdString leftString;
        
        for (k = 0; k < (int) segments[j].m_SegParams.size(); k++)
        {
          if (k)
          {
            rightString += OD_T(", ");
          }
          rightString += toString(segments[j].m_SegParams[k]);
        }
        if (rightString != OD_T("")) {
          leftString.format(OD_T("SegParams %d"), j);
          writeLine(indent+2, leftString,         rightString);
        }
        rightString = OD_T("");
        for (k = 0; k < (int) segments[j].m_AreaFillParams.size(); k++)
        {
          if (k)
          {
            rightString += OD_T(", ");
          }
          rightString += toString(segments[j].m_AreaFillParams[k]);
        }
        if (rightString != OD_T("")) {
          leftString.format(OD_T("FillParams %d"), j);
          writeLine(indent+2, leftString,         rightString);
        }        
      }
    }
    dumpEntityData(pMline, indent);
  }
}; 
/************************************************************************/
/* MText Dumper                                                         */
/************************************************************************/
class OdDbMText_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbMTextPtr pMText = pEnt;
    writeLine(indent++, toString(pMText->isA()), toString(pMText->getDbHandle()));

    writeLine(indent, OD_T("Contents"),                       toString(pMText->contents()));
    writeLine(indent, OD_T("Location"),                       toString(pMText->location()));
    writeLine(indent, OD_T("Height"),                         toString(pMText->textHeight()));
    writeLine(indent, OD_T("Rotation"),                       toDegreeString(pMText->rotation()));
    writeLine(indent, OD_T("Text Style"),                     toString(pMText->textStyle()));
    writeLine(indent, OD_T("Attachment"),                     toString(pMText->attachment()));
    writeLine(indent, OD_T("Background Fill On"),             toString(pMText->backgroundFillOn()));
    writeLine(indent, OD_T("Background Fill Color"),          toString(pMText->getBackgroundFillColor()));
    writeLine(indent, OD_T("Background Scale Factor"),        toString(pMText->getBackgroundScaleFactor()));
    writeLine(indent, OD_T("Background Transparency Method"), toString(pMText->getBackgroundTransparency().method()));
    writeLine(indent, OD_T("X-Direction"),                    toString(pMText->direction()));
    writeLine(indent, OD_T("Flow Direction"),                 toString(pMText->flowDirection()));
    writeLine(indent, OD_T("Horizontal Mode"),                toString(pMText->horizontalMode()));
    writeLine(indent, OD_T("Vertical Mode"),                  toString(pMText->verticalMode()));
    writeLine(indent, OD_T("Width"),                          toString(pMText->width()));
    writeLine(indent, OD_T("Actual Height"),                  toString(pMText->actualHeight()));
    writeLine(indent, OD_T("Actual Width"),                   toString(pMText->actualWidth()));
    
    OdGePoint3dArray points;
    pMText->getBoundingPoints(points);
    writeLine(indent, OD_T("TL Bounding Point"),              toString(points[0]));
    writeLine(indent, OD_T("TR Bounding Point"),              toString(points[1]));
    writeLine(indent, OD_T("BL Bounding Point"),              toString(points[2]));
    writeLine(indent, OD_T("BR Bounding Point"),              toString(points[3]));
    writeLine(indent, OD_T("Normal"),                         toString(pMText->normal()));

    /********************************************************************/
    /* Dump Indents and Tabs                                            */
    /********************************************************************/
    OdDbMTextIndents indents;
    pMText->getParagraphsIndent(indents);
    for (int i = 0; i < (int) indents.size(); i++)
    {
      writeLine(indent, OD_T("Indent"),                       toString(i));
      writeLine(indent+1, OD_T("First Line"),                 toString(indents[i].firstLineInd));
      writeLine(indent+1, OD_T("Paragraph"),                  toString(indents[i].paragraphInd));
      OdString rightString = OD_T("");
      for (int j = 0; j < (int) indents[i].tabs.size(); j++)
      {
        if (j) {
          rightString += OD_T(", ");
        }
        rightString += toString(indents[i].tabs[j]); 
      }
      if (rightString != OD_T(""))
      {
        writeLine(indent+1, OD_T("Tabs"),                     rightString);
      }
    }
    dumpEntityData(pMText, indent);
  }
};

/************************************************************************/
/* Ordinate Dimension Dumper                                            */
/************************************************************************/
class OdDbOrdinateDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDbOrdinateDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),  toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Defining Point"),         toString(pDim->definingPoint()));
    writeLine(indent, OD_T("Using x-Axis"),           toString(pDim->isUsingXAxis()));
    writeLine(indent, OD_T("Using y-Axis"),           toString(pDim->isUsingYAxis()));
    writeLine(indent, OD_T("Leader End Point"),       toString(pDim->leaderEndPoint()));
    writeLine(indent, OD_T("Origin"),                 toString(pDim->origin()));
    dumpDimData(pDim, indent);
  }
};
/************************************************************************/
/* PolyFaceMesh Dumper                                                  */
/************************************************************************/
class OdDbPolyFaceMesh_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbPolyFaceMeshPtr pPoly = pEnt;
    writeLine(indent++, toString(pPoly->isA()),  toString(pPoly->getDbHandle()));
    writeLine(indent, OD_T("Number of Vertices"),      toString(pPoly->numVertices()));
    writeLine(indent, OD_T("Number of Faces"),         toString(pPoly->numFaces()));

    /********************************************************************/
    /* dump vertices and faces                                          */
    /********************************************************************/
    OdDbObjectIteratorPtr pIter = pPoly->vertexIterator();
    int vertexCount = 0;
    int faceCount = 0;
    for (; !pIter->done(); pIter->step())
    {
      OdDbPolyFaceMeshVertexPtr pVertex = OdDbPolyFaceMeshVertex::cast(pIter->entity());
      if (!pVertex.isNull())
      {
          writeLine(indent, toString(pVertex->isA()),   toString(++vertexCount));
          writeLine(indent+1, OD_T("Handle"),                 toString(pVertex->getDbHandle()));

          writeLine(indent+1, OD_T("Position"),               toString(pVertex->position()));
          dumpEntityData(pVertex, indent+1);
      }
      else
      {
        OdDbFaceRecordPtr pFace = OdDbFaceRecord::cast(pIter->entity());
        if (!pFace.isNull())
        {
          OdString face(OD_T("{"));
          for (int i = 0; i < 4; i++)
          {
            if (i) 
            {
              face += OD_T(" ");
            }
            face += toString(pFace->getVertexAt(i));
          }
          face += OD_T("}");
          writeLine(indent, toString(pFace->isA()),       toString(++faceCount));
          writeLine(indent+1, OD_T("Handle"),                   toString(pFace->getDbHandle()));
          writeLine(indent+1, OD_T("Vertices"),                 face);
          dumpEntityData(pFace, indent+1);
        }
        else
        { // Unknown entity type
          writeLine(indent, OD_T("Unexpected Entity"));
        }
      }
    }
    dumpEntityData(pPoly, indent);
  }

};

/************************************************************************/
/* Ole2Frame                                                            */
/************************************************************************/
class OdDbOle2Frame_Dumper : public OdDbEntity_Dumper
{
public:
  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbOle2FramePtr pOle = pEnt;
    writeLine(indent++, toString(pOle->isA()), toString(pOle->getDbHandle()));
    
    OdRectangle3d pos;
    pOle->position(pos); 
    writeLine(indent, OD_T("Lower Left"),       toString(pos.lowLeft));
    writeLine(indent, OD_T("Lower Right"),      toString(pos.lowRight));
    writeLine(indent, OD_T("Upper Left"),       toString(pos.upLeft));
    writeLine(indent, OD_T("Upper Right"),      toString(pos.upRight));

    OdDbOle2Frame::Type oleType = OdDbOle2Frame::kUnknown;
    try
    {
      oleType = pOle->getType();
      writeLine(indent, OD_T("Type"),             toString(oleType));
      writeLine(indent, OD_T("User Type"),        toString(pOle->getUserType()));
    }
    catch (OdError& ) {} // getUserType may call exception in case if no ole application installed.

    if(oleType==OdDbOle2Frame::kLink)
    {
      writeLine(indent, OD_T("Link Name"),        toString(pOle->getLinkName()));
      writeLine(indent, OD_T("Link Path"),        toString(pOle->getLinkPath()));
    }
    writeLine(indent, OD_T("Output Quality"),   toString(pOle->outputQuality()));
    writeLine(indent, OD_T("Auto Output Quality"), toString(pOle->autoOutputQuality()));
    writeLine(indent, OD_T("Rotation"),         toString(pOle->rotation()));
    writeLine(indent, OD_T("WCS Width"),        toString(pOle->wcsWidth()));
    writeLine(indent, OD_T("WCS Height"),       toString(pOle->wcsHeight()));
    writeLine(indent, OD_T("Lock Aspect"),      toString(pOle->lockAspect()));
    dumpEntityData(pOle, indent); 
  }
}; 

/************************************************************************/
/* Point Dumper                                                         */
/************************************************************************/
class OdDbPoint_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbPointPtr pPoint = pEnt;
    writeLine(indent++, toString(pPoint->isA()),  toString(pPoint->getDbHandle()));
    writeLine(indent, OD_T("Position"),                 toString(pPoint->position()));
    writeLine(indent, OD_T("ECS Rotation"),             toDegreeString(pPoint->ecsRotation()));
    writeLine(indent, OD_T("Normal"),                   toString(pPoint->normal()));
    writeLine(indent, OD_T("Thickness"),                toString(pPoint->thickness()));
    dumpEntityData(pPoint, indent);
  }
};

/************************************************************************/
/* Polygon Mesh Dumper                                                  */
/************************************************************************/
class OdDbPolygonMesh_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbPolygonMeshPtr pPoly = pEnt;
    writeLine(indent++, toString(pPoly->isA()),   toString(pPoly->getDbHandle()));
    writeLine(indent, OD_T("m Size"),                   toString(pPoly->mSize()));
    writeLine(indent, OD_T("m-Closed"),                 toString(pPoly->isMClosed()));
    writeLine(indent, OD_T("m Surface Density"),        toString(pPoly->mSurfaceDensity()));
    writeLine(indent, OD_T("n Size"),                   toString(pPoly->nSize()));
    writeLine(indent, OD_T("n-Closed"),                 toString(pPoly->isNClosed()));
    writeLine(indent, OD_T("n Surface Density"),        toString(pPoly->nSurfaceDensity()));
    /********************************************************************/
    /* dump vertices                                                    */
    /********************************************************************/
    OdDbObjectIteratorPtr pIter = pPoly->vertexIterator();

    int vertexCount = 0;
    for (; !pIter->done(); pIter->step())
    {
      OdDbPolygonMeshVertexPtr pVertex = pIter->entity();
      if (pVertex.get())
      {
        writeLine(indent, toString(pVertex->isA()), toString(vertexCount++));
        writeLine(indent+1, OD_T("Handle"),               toString(pVertex->getDbHandle()));
        writeLine(indent+1, OD_T("Position"),             toString(pVertex->position()));
        writeLine(indent+1, OD_T("Type"),                 toString(pVertex->vertexType()));
      }
    }

    dumpEntityData(pPoly, indent);
  }
};

/************************************************************************/
/* Polyline Dumper                                                      */
/************************************************************************/
class OdDbPolyline_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbPolylinePtr pPoly = pEnt;
    writeLine(indent++, toString(pPoly->isA()),   toString(pPoly->getDbHandle()));
    writeLine(indent, OD_T("Has Width"),                toString(pPoly->hasWidth()));
    if (!pPoly->hasWidth())
    {
      writeLine(indent, OD_T("Constant Width"),         toString(pPoly->getConstantWidth()));
    }
    writeLine(indent, OD_T("Has Bulges"),               toString(pPoly->hasBulges()));
    writeLine(indent, OD_T("Elevation"),                toString(pPoly->elevation()));
    writeLine(indent, OD_T("Normal"),                   toString(pPoly->normal()));
    writeLine(indent, OD_T("Thickness"),                toString(pPoly->thickness()));

    /********************************************************************/
    /* dump vertices                                                    */
    /********************************************************************/
    for (int i = 0; i < (int) pPoly->numVerts(); i++)
    { 
      writeLine(indent, toString(OD_T("Vertex %d"),i));
      writeLine(indent+1, OD_T("Segment Type"),         toString(pPoly->segType(i)));
      OdGePoint3d pt;
      pPoly->getPointAt(i, pt);
      writeLine(indent+1, OD_T("Point"), toString(pt));
      if (pPoly->hasWidth())
      {     
        double startWidth;
        double endWidth;
        pPoly->getWidthsAt(i, startWidth, endWidth);
        writeLine(indent, OD_T("Start Width"),          toString(startWidth));
        writeLine(indent, OD_T("End Width"),            toString(endWidth));
      }
      if (pPoly->hasBulges())
      {
        writeLine(indent, OD_T("Bulge"),                toString(pPoly->getBulgeAt(i)));
        if (pPoly->segType(i) == OdDbPolyline::kArc) 
        {
          writeLine(indent, OD_T("Bulge Angle"),        toDegreeString(4 * atan(pPoly->getBulgeAt(i))));
        }
      }
    }
    dumpEntityData(pPoly, indent);
  }
};

/************************************************************************/
/* Proxy Entity Dumper                                                  */
/************************************************************************/
class OdDbProxyEntity_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    // this will dump proxy entity graphics
    OdDbEntity_Dumper::dump(pEnt, indent);

    OdSmartPtr<OdDbProxyExt> proxyEntExt = pEnt;

    writeLine(indent, OD_T("Proxy OriginalClassName"), toString(proxyEntExt->originalClassName(pEnt)));
    writeLine(indent, OD_T("Proxy ApplicationDescription"), toString(proxyEntExt->applicationDescription(pEnt)));
    writeLine(indent, OD_T("Proxy OriginalDxfName"), toString(proxyEntExt->originalDxfName(pEnt)));

    // If the entity is of type AcAdPart, then dump the associated SAT file.
    OdDbProxyEntityPtr pProxy(pEnt);
    OdAnsiString satString;
    if (pProxy->originalClassName() == OD_T("AcAdPart") &&
        odGetSatFromProxy(pProxy, satString))
    {
      writeLine(indent, OD_T("AD_PART SAT file"), shortenPath(satString));
    }      
  }
};

/************************************************************************/
/* Radial Dimension Dumper                                              */
/************************************************************************/
class OdDbRadialDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDbRadialDimensionPtr pDim = pEnt;
    
    writeLine(indent++, toString(pDim->isA()),   toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Center"),                  toString(pDim->center()));
    writeLine(indent, OD_T("Chord Point"),             toString(pDim->chordPoint()));
    writeLine(indent, OD_T("Leader Length"),           toString(pDim->leaderLength()));

    dumpDimData(pDim, indent);
  }
};
/************************************************************************/
/* Dump Raster Image Def                                               */
/************************************************************************/
void dumpRasterImageDef(OdDbRasterImage* pImage, int indent)
{
  OdDbObjectId objectId = pImage->imageDefId();
  if (objectId.isNull()) 
  {
    return;
  }

  OdDbRasterImageDefPtr pDef = objectId.openObject();
  if (pDef.isNull()) 
    return;

  writeLine(indent++, toString(pDef->isA()),  toString(pDef->getDbHandle()));
  writeLine(indent,   OD_T("Source Filename"),      shortenPath(pDef->sourceFileName()));
  writeLine(indent,   OD_T("Loaded"),               toString(pDef->isLoaded()));
  writeLine(indent,   OD_T("mm per Pixel"),         toString(pDef->resolutionMMPerPixel()));
  writeLine(indent,   OD_T("Loaded"),               toString(pDef->isLoaded()));
  writeLine(indent,   OD_T("Resolution Units"),     toString(pDef->resolutionUnits()));
  writeLine(indent,   OD_T("Size"),                 toString(pDef->size()));
}
/************************************************************************/
/* Dump Raster Image Data                                               */
/************************************************************************/
void dumpRasterImageData(OdDbRasterImage* pImage, int indent)
{
  writeLine(indent, OD_T("Brightness"),               toString(pImage->brightness()));
  writeLine(indent, OD_T("Clipped"),                  toString(pImage->isClipped()));
  writeLine(indent, OD_T("Contrast"),                 toString(pImage->contrast()));
  writeLine(indent, OD_T("Fade"),                     toString(pImage->fade()));
  writeLine(indent, OD_T("kClip"),                    toString(pImage->isSetDisplayOpt(OdDbRasterImage::kClip)));
  writeLine(indent, OD_T("kShow"),                    toString(pImage->isSetDisplayOpt(OdDbRasterImage::kShow)));
  writeLine(indent, OD_T("kShowUnAligned"),           toString(pImage->isSetDisplayOpt(OdDbRasterImage::kShowUnAligned)));
  writeLine(indent, OD_T("kTransparent"),             toString(pImage->isSetDisplayOpt(OdDbRasterImage::kTransparent)));
  writeLine(indent, OD_T("Scale"),                    toString(pImage->scale()));

  /********************************************************************/
  /* Dump clip boundary                                               */
  /********************************************************************/
  if (pImage->isClipped())
  {
    writeLine(indent, OD_T("Clip Boundary Type"),  toString(pImage->clipBoundaryType()));
    if (pImage->clipBoundaryType() != OdDbRasterImage::kInvalid)
    {
      OdGePoint2dArray pt = pImage->clipBoundary();
      for (int i = 0; i < (int) pt.size(); i++)
      {
        writeLine(indent, toString(OD_T("Clip Point %d"), i), toString(pt[i]));
      }
    }
  }

  /********************************************************************/
  /* Dump frame                                                       */
  /********************************************************************/
  OdGePoint3dArray vertices;
  pImage->getVertices(vertices);
  for (int i = 0; i < (int) vertices.size(); i++)
  {
    writeLine(indent, toString(OD_T("Frame Vertex %d"), i), toString(vertices[i]));
  }
        
  /********************************************************************/
  /* Dump orientation                                                 */
  /********************************************************************/
  OdGePoint3d origin;
  OdGeVector3d u,v,u2,v2;
  pImage->getOrientation (origin,u,v);
  writeLine(indent, OD_T("Orientation"));
  writeLine(indent+1, OD_T("Origin"),         toString(origin));
  writeLine(indent+1, OD_T("uVector"),        toString(u));
  writeLine(indent+1, OD_T("vVector"),        toString(v));
  dumpRasterImageDef(pImage, indent);
  dumpEntityData(pImage, indent);
}

/************************************************************************/
/* Raster Image Dumper                                                  */
/************************************************************************/
class OdDbRasterImage_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbRasterImagePtr pImage = pEnt;  
    writeLine(indent++, toString(pImage->isA()),  toString(pImage->getDbHandle()));
    writeLine(indent, OD_T("Image size"),               toString(pImage->imageSize()));
    dumpRasterImageData(pImage, indent);
  }
}; 

/************************************************************************/
/* Ray Dumper                                                          */
/************************************************************************/
class OdDbRay_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbRayPtr pRay = pEnt;
    writeLine(indent++, toString(pRay->isA()),  toString(pRay->getDbHandle()));
    writeLine(indent, OD_T("Base Point"),             toString(pRay->basePoint()));
    writeLine(indent, OD_T("Unit Direction"),         toString(pRay->unitDir()));
    dumpCurveData(pRay, indent);
  }
};

/************************************************************************/
/* Region Dumper                                                        */
/************************************************************************/
class OdDbRegion_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbRegionPtr pRegion = pEnt;
    writeLine(indent++, toString(pRegion->isA()), toString(pRegion->getDbHandle()));
    dumpEntityData(pRegion, indent);

    //OdWrFileBuf ow(OD_T("RegionAcisOut.acis"));
    //pRegion->acisOut(&ow , kAfTypeASCII);

  }
}; 
/************************************************************************/
/* Rotated Dimension Dumper                                             */
/************************************************************************/
class OdDbRotatedDimension_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {    
    OdDbRotatedDimensionPtr pDim = pEnt;
    writeLine(indent++, toString(pDim->isA()),   toString(pDim->getDbHandle()));
    writeLine(indent, OD_T("Dimension Line Point"),    toString(pDim->dimLinePoint()));
    writeLine(indent, OD_T("Oblique"),                 toDegreeString(pDim->oblique()));
    writeLine(indent, OD_T("Rotation"),                toDegreeString(pDim->rotation()));
    writeLine(indent, OD_T("Extension Line 1 Point"),  toString(pDim->xLine1Point()));
    writeLine(indent, OD_T("Extension Line 2 Point"),  toString(pDim->xLine2Point()));
    dumpDimData(pDim, indent);
  }
};
/************************************************************************/
/* Shape Dumper                                                          */
/************************************************************************/
class OdDbShape_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbShapePtr pShape = pEnt;
    writeLine(indent++, toString(pShape->isA()),  toString(pShape->getDbHandle()));
    
    if (!pShape->styleId().isNull())
    {
      OdDbTextStyleTableRecordPtr pStyle = pShape->styleId().safeOpenObject();
      writeLine(indent, OD_T("Filename"),                 shortenPath(pStyle->fileName()));
    }

    writeLine(indent, OD_T("Shape Number"),             toString(pShape->shapeNumber()));
    writeLine(indent, OD_T("Shape Name"),               toString(pShape->name()));
    writeLine(indent, OD_T("Position"),                 toString(pShape->position()));
    writeLine(indent, OD_T("Size"),                     toString(pShape->size()));
    writeLine(indent, OD_T("Rotation"),                 toDegreeString(pShape->rotation()));
    writeLine(indent, OD_T("Oblique"),                  toDegreeString(pShape->oblique()));
    writeLine(indent, OD_T("Normal"),                   toString(pShape->normal()));
    writeLine(indent, OD_T("Thickness"),                toString(pShape->thickness()));
    dumpEntityData(pShape, indent);
  }
};

/************************************************************************/
/* Solid Dumper                                                         */
/************************************************************************/
class OdDbSolid_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbSolidPtr pSolid = pEnt;
    writeLine(indent++, toString(pSolid->isA()),  toString(pSolid->getDbHandle()));

    for (int i = 0; i < 4; i++)
    {
      OdGePoint3d pt;
      pSolid->getPointAt(i, pt);
      writeLine(indent, toString(OD_T("Point %d"), i),  toString(pt));
    }
    dumpEntityData(pSolid, indent);
  }
};

/************************************************************************/
/* Spline Dumper                                                        */
/************************************************************************/
class OdDbSpline_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {

    OdDbSplinePtr pSpline = pEnt;
    writeLine(indent++, toString(pSpline->isA()), toString(pSpline->getDbHandle()));

    int degree;
    bool rational, closed, periodic;
    OdGePoint3dArray ctrlPts;
    OdGeDoubleArray weights;
    OdGeDoubleArray  knots;
    double tol, kTol;

    pSpline->getNurbsData(degree, rational, closed, periodic, ctrlPts, knots, weights, tol, kTol);
    writeLine(indent, OD_T("Degree"),                   toString(degree));
    writeLine(indent, OD_T("Rational"),                 toString(rational));
    writeLine(indent, OD_T("Periodic"),                 toString(periodic));
    writeLine(indent, OD_T("Control Point Tolerance"),  toString(OD_T("%.4E"), tol));
    writeLine(indent, OD_T("Knot Tolerance"),           toString(OD_T("%.4E"), kTol));

    writeLine(indent, OD_T("Number of control points"),                toString((int) ctrlPts.size()));
    int i;
    for (i = 0; i < (int) ctrlPts.size(); i++)
    {
      writeLine(indent, toString(OD_T("Control Point %d"), i),  toString(ctrlPts[i]));
    }

    writeLine(indent, OD_T("Number of Knots"),                  toString((int) knots.length()));
    for (i = 0; i < (int) knots.length(); i++)
    {
      writeLine(indent, toString(OD_T("Knot %d"), i),           toString(knots[i]));
    }

    if (rational) 
    {
      writeLine(indent, OD_T("Number of Weights"),              toString((int) weights.size()));
      for (i = 0; i < (int) weights.size(); i++)
      {
        writeLine(indent, toString(OD_T("Weight %d"), i),       toString(weights[i]));
      }
    }
    dumpCurveData(pEnt, indent);
  }
};
/************************************************************************/
/* Table Dumper                                                         */
/************************************************************************/
class OdDbTable_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbTablePtr pTable = pEnt;
    writeLine(indent++, toString(pTable->isA()),  toString(pTable->getDbHandle()));
    writeLine(indent, OD_T("Position"),                 toString(pTable->position()));
    writeLine(indent, OD_T("X-Direction"),              toString(pTable->direction()));
    writeLine(indent, OD_T("Normal"),                   toString(pTable->normal()));
    writeLine(indent, OD_T("Height"),                   toString((int) pTable->height()));
    writeLine(indent, OD_T("Width"),                    toString((int) pTable->width()));
    writeLine(indent, OD_T("Rows"),                     toString((int) pTable->numRows()));
    writeLine(indent, OD_T("Columns"),                  toString((int) pTable->numColumns()));
    
    OdDbTableStylePtr pStyle = pTable->tableStyle().openObject();
    if (!pStyle.isNull())
      writeLine(indent, OD_T("Table Style"),               toString(pStyle->getName()));
    else
      writeLine(indent, OD_T("Table Style"),               L"Null");

    dumpEntityData(pTable, indent);
  }
};
/************************************************************************/
/* Text Dumper                                                          */
/************************************************************************/
class OdDbText_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbTextPtr pText = pEnt;
    writeLine();
    writeLine(indent, toString(pText->isA()),   toString(pText->getDbHandle()));
    dumpTextData(pText, indent);   
  }
};
/************************************************************************/
/* Trace Dumper                                                         */
/************************************************************************/
class OdDbTrace_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbTracePtr pTrace = pEnt;
    writeLine(indent++, toString(pTrace->isA()),  toString(pTrace->getDbHandle()));
   
    for (int i = 0; i < 4; i++)
    {
      OdGePoint3d pt;
      pTrace->getPointAt(i, pt);
      writeLine(indent, toString(OD_T("Point %d"), i),  toString(pt));
    }
    dumpEntityData(pTrace, indent);
  }
};
/************************************************************************/
/* Viewport Dumper                                                       */
/************************************************************************/
class OdDbViewport_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbViewportPtr pVport = pEnt;
    writeLine(indent++, toString(pEnt->isA()),  toString(pEnt->getDbHandle()));
    writeLine(indent, OD_T("Back Clip Distance"),     toString(pVport->backClipDistance()));
    writeLine(indent, OD_T("Back Clip On"),           toString(pVport->isBackClipOn()));
    writeLine(indent, OD_T("Center Point"),           toString(pVport->centerPoint()));
    writeLine(indent, OD_T("Circle sides"),           toString(pVport->circleSides()));
    writeLine(indent, OD_T("Custom Scale"),           toString(pVport->customScale()));
    writeLine(indent, OD_T("Elevation"),              toString(pVport->elevation()));
    writeLine(indent, OD_T("Front Clip at Eye"),      toString(pVport->isFrontClipAtEyeOn()));
    writeLine(indent, OD_T("Front Clip Distance"),    toString(pVport->frontClipDistance()));
    writeLine(indent, OD_T("Front Clip On"),          toString(pVport->isFrontClipOn()));
    writeLine(indent, OD_T("Plot style sheet"),       toString(pVport->effectivePlotStyleSheet()));

    OdDbObjectIdArray layerIds;
    pVport->getFrozenLayerList(layerIds);
    if (layerIds.length())
    {
      writeLine(indent, OD_T("Frozen Layers:"));
      for (int i = 0; i < (int) layerIds.length(); i++)
      {
         writeLine(indent+1, toString(i),       toString(layerIds[i])) ;
      }
    }
    else
    {
      writeLine(indent, OD_T("Frozen Layers"), OD_T("None"));
    }
    
    OdGePoint3d origin;
    OdGeVector3d xAxis;
    OdGeVector3d yAxis;
    pVport->getUcs(origin, xAxis, yAxis);
    writeLine(indent, OD_T("UCS origin"),             toString(origin));        
    writeLine(indent, OD_T("UCS x-Axis"),             toString(xAxis));      
    writeLine(indent, OD_T("UCS y-Axis"),             toString(yAxis));
    writeLine(indent, OD_T("Grid Increment"),         toString(pVport->gridIncrement()));
    writeLine(indent, OD_T("Grid On"),                toString(pVport->isGridOn()));
    writeLine(indent, OD_T("Height"),                 toString(pVport->height()));
    writeLine(indent, OD_T("Lens Length"),            toString(pVport->lensLength()));
    writeLine(indent, OD_T("Locked"),                 toString(pVport->isLocked()));
    writeLine(indent, OD_T("Non-Rectangular Clip"),   toString(pVport->isNonRectClipOn()));

    if (!pVport->nonRectClipEntityId().isNull())
    {
      writeLine(indent, OD_T("Non-rectangular Clipper"), toString(pVport->nonRectClipEntityId().getHandle()));
    }
    writeLine(indent, OD_T("Render Mode"),            toString(pVport->renderMode()));
    writeLine(indent, OD_T("Remove Hidden Lines"),    toString(pVport->hiddenLinesRemoved()));
    writeLine(indent, OD_T("Shade Plot"),             toString(pVport->shadePlot()));
    writeLine(indent, OD_T("Snap Isometric"),         toString(pVport->isSnapIsometric()));
    writeLine(indent, OD_T("Snap On"),                toString(pVport->isSnapOn()));
    writeLine(indent, OD_T("Transparent"),            toString(pVport->isTransparent()));
    writeLine(indent, OD_T("UCS Follow"),             toString(pVport->isUcsFollowModeOn()));
    writeLine(indent, OD_T("UCS Icon at Origin"),     toString(pVport->isUcsIconAtOrigin()));

    OdDb::OrthographicView orthoUCS;       
    writeLine(indent, OD_T("UCS Orthographic"),       toString(pVport->isUcsOrthographic(orthoUCS)));
    writeLine(indent, OD_T("Orthographic UCS"),              toString(orthoUCS));
    writeLine(indent, OD_T("UCS Saved with VP"),      toString(pVport->isUcsSavedWithViewport()));
    
    if (!pVport->ucsName().isNull())
    {
      OdDbUCSTableRecordPtr pUCS = pVport->ucsName().safeOpenObject();
      writeLine(indent, OD_T("UCS Name"), toString(pUCS->getName()));
    }
    else
    {
      writeLine(indent, OD_T("UCS Name"), OD_T("Null"));
    }

    writeLine(indent, OD_T("View Center"),            toString(pVport->viewCenter()));
    writeLine(indent, OD_T("View Height"),            toString(pVport->viewHeight()));
    writeLine(indent, OD_T("View Target"),            toString(pVport->viewTarget()));
    writeLine(indent, OD_T("Width"),                  toString(pVport->width()));
    dumpEntityData(pEnt, indent);
  }
};

/************************************************************************/
/* Wipeout Dumper                                                  */
/************************************************************************/
class OdDbWipeout_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbWipeoutPtr pWipeout = pEnt;  
    writeLine(indent++, toString(pWipeout->isA()),  toString(pWipeout->getDbHandle()));
    dumpRasterImageData(pWipeout, indent);
  }
}; 

/************************************************************************/
/* Xline Dumper                                                         */
/************************************************************************/
class OdDbXline_Dumper : public OdDbEntity_Dumper
{
public:

  void dump(OdDbEntity* pEnt, int indent) const 
  {
    OdDbXlinePtr pXline = pEnt;
    writeLine(indent++, toString(pXline->isA()),  toString(pXline->getDbHandle()));
    writeLine(indent, OD_T("Base Point"),               toString(pXline->basePoint()));
    writeLine(indent, OD_T("Unit Direction"),           toString(pXline->unitDir()));
    dumpCurveData(pXline, indent);
  }
};

class Dumpers
{
  OdStaticRxObject<    OdDb2LineAngularDimension_Dumper   >    m_2LineAngularDimensionDumper;
  OdStaticRxObject<    OdDb2dPolyline_Dumper              >    m_2dPolylineDumper;
  OdStaticRxObject<    OdDb3PointAngularDimension_Dumper  >    m_3PointAngularDimensionDumper;
  OdStaticRxObject<    OdDb3dPolyline_Dumper              >    m_3dPolylineDumper;
  OdStaticRxObject<    OdDb3dSolid_Dumper                 >    m_3dSolidDumper;
  OdStaticRxObject<    OdDbAlignedDimension_Dumper        >    m_alignedDimensionDumper;
  OdStaticRxObject<    OdDbArcAlignedText_Dumper          >    m_arcAlignedTextDumper;
  OdStaticRxObject<    OdDbArcDimension_Dumper            >    m_arcDimensionDumper;
  OdStaticRxObject<    OdDbArc_Dumper                     >    m_arcDumper;
  OdStaticRxObject<    OdDbBlockReference_Dumper          >    m_blockReference;
  OdStaticRxObject<    OdDbBody_Dumper                    >    m_bodyDumper;
  OdStaticRxObject<    OdDbCircle_Dumper                  >    m_circleDumper;
  OdStaticRxObject<    OdDbDiametricDimension_Dumper      >    m_diametricDimensionDumper;
  OdStaticRxObject<    OdDbEllipse_Dumper                 >    m_ellipseDumper;
  OdStaticRxObject<    OdDbEntity_Dumper                  >    m_entityDumper;
  OdStaticRxObject<    OdDbFace_Dumper                    >    m_faceDumper;
  OdStaticRxObject<    OdDbFcf_Dumper                     >    m_fcfDumper;
  OdStaticRxObject<    OdDbHatch_Dumper                   >    m_hatchDumper;
  OdStaticRxObject<    OdDbLeader_Dumper                  >    m_leaderDumper;
  OdStaticRxObject<    OdDbLine_Dumper                    >    m_lineDumper;
  OdStaticRxObject<    OdDbMInsertBlock_Dumper            >    m_mInsertBlock;
  OdStaticRxObject<    OdDbMText_Dumper                   >    m_mTextDumper;
  OdStaticRxObject<    OdDbMline_Dumper                   >    m_mlineDumper;
  OdStaticRxObject<    OdDbOle2Frame_Dumper               >    m_ole2FrameDumper;
  OdStaticRxObject<    OdDbOrdinateDimension_Dumper       >    m_ordinateDimensionDumper;
  OdStaticRxObject<    OdDbPoint_Dumper                   >    m_pointDumper;
  OdStaticRxObject<    OdDbPolyFaceMesh_Dumper            >    m_polyFaceMeshDumper;
  OdStaticRxObject<    OdDbPolygonMesh_Dumper             >    m_polygonMesh;
  OdStaticRxObject<    OdDbPolyline_Dumper                >    m_polylineDumper;
  OdStaticRxObject<    OdDbProxyEntity_Dumper             >    m_proxyEntityDumper;
  OdStaticRxObject<    OdDbRadialDimension_Dumper         >    m_radialDimensionDumper;
  OdStaticRxObject<    OdDbRasterImage_Dumper             >    m_imageDumper;
  OdStaticRxObject<    OdDbRay_Dumper                     >    m_rayDumper;
  OdStaticRxObject<    OdDbRegion_Dumper                  >    m_regionDumper;
  OdStaticRxObject<    OdDbRotatedDimension_Dumper        >    m_rotatedDimensionDumper;
  OdStaticRxObject<    OdDbShape_Dumper                   >    m_shapeDumper;
  OdStaticRxObject<    OdDbSolid_Dumper                   >    m_solidDumper;
  OdStaticRxObject<    OdDbSpline_Dumper                  >    m_splineDumper;
  OdStaticRxObject<    OdDbTable_Dumper                   >    m_tableDumper;
  OdStaticRxObject<    OdDbText_Dumper                    >    m_textDumper;
  OdStaticRxObject<    OdDbTrace_Dumper                   >    m_traceDumper;
  OdStaticRxObject<    OdDbViewport_Dumper                >    m_viewportDumper;
  OdStaticRxObject<    OdDbWipeout_Dumper                 >    m_wipeoutDumper;
  OdStaticRxObject<    OdDbXline_Dumper                   >    m_xlineDumper;

public:
  /**********************************************************************/
  /* Add Protocol Extensions                                            */
  /**********************************************************************/
  void addXs()
  {
    OdDb2LineAngularDimension  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_2LineAngularDimensionDumper);
    OdDb2dPolyline             ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_2dPolylineDumper);
    OdDb3PointAngularDimension ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_3PointAngularDimensionDumper);
    OdDb3dPolyline             ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_3dPolylineDumper);
    OdDb3dSolid                ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_3dSolidDumper);
    OdDbAlignedDimension       ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_alignedDimensionDumper);
    OdDbArc                    ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_arcDumper);
    OdDbArcAlignedText         ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_arcAlignedTextDumper);
    OdDbArcDimension           ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_arcDimensionDumper);
    OdDbBlockReference         ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_blockReference);
    OdDbBody                   ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_bodyDumper);
    OdDbCircle                 ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_circleDumper);
    OdDbDiametricDimension     ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_diametricDimensionDumper);
    OdDbEllipse                ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_ellipseDumper);
    OdDbEntity                 ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_entityDumper);
    OdDbFace                   ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_faceDumper);
    OdDbFcf                    ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_fcfDumper);
    OdDbHatch                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_hatchDumper);
    OdDbLeader                 ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_leaderDumper);                    
    OdDbLine                   ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_lineDumper);                    
    OdDbMInsertBlock           ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_mInsertBlock);
    OdDbMText                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_mTextDumper);
    OdDbMline                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_mlineDumper);
    OdDbOle2Frame              ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_ole2FrameDumper);
    OdDbOrdinateDimension      ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_ordinateDimensionDumper);
    OdDbPoint                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_pointDumper);
    OdDbPolyFaceMesh           ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_polyFaceMeshDumper);
    OdDbPolygonMesh            ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_polygonMesh);
    OdDbPolyline               ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_polylineDumper);
    OdDbProxyEntity            ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_proxyEntityDumper);
    OdDbProxyObject            ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_proxyEntityDumper);
    OdDbRadialDimension        ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_radialDimensionDumper);
    OdDbRasterImage            ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_imageDumper);
    OdDbRay                    ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_rayDumper);
    OdDbRegion                 ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_regionDumper);
    OdDbRotatedDimension       ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_rotatedDimensionDumper);
    OdDbShape                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_shapeDumper);
    OdDbSolid                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_solidDumper);
    OdDbSpline                 ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_splineDumper);
    OdDbTable                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_tableDumper);
    OdDbText                   ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_textDumper);
    OdDbTrace                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_traceDumper);
    OdDbViewport               ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_viewportDumper);
    OdDbWipeout                ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_wipeoutDumper);
    OdDbXline                  ::desc()->addX(OdDbEntity_Dumper::desc(),   &m_xlineDumper);
  } // end addXs

  /**********************************************************************/
  /* Delete Protocol Extensions                                         */
  /**********************************************************************/
  void delXs()
  {
    OdDb2LineAngularDimension  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDb2dPolyline             ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDb3PointAngularDimension ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDb3dPolyline             ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDb3dSolid                ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbAlignedDimension       ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbArc                    ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbArcAlignedText         ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbArcDimension           ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbBlockReference         ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbBody                   ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbCircle                 ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbDiametricDimension     ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbEllipse                ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbEntity                 ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbFace                   ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbFcf                    ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbHatch                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbLeader                 ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbLine                   ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbMInsertBlock           ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbMText                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbMline                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbOle2Frame              ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbOrdinateDimension      ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbPoint                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbPolyFaceMesh           ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbPolygonMesh            ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbPolyline               ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbProxyEntity            ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbProxyObject            ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbRadialDimension        ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbRasterImage            ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbRay                    ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbRegion                 ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbRotatedDimension       ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbShape                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbSolid                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbSpline                 ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbTable                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbText                   ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbTrace                  ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbViewport               ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbWipeout                ::desc()->delX(OdDbEntity_Dumper::desc());
    OdDbXline                  ::desc()->delX(OdDbEntity_Dumper::desc());
  }
};


/************************************************************************/
/* Initialize protocol extensions                                       */
/************************************************************************/
void ExProtocolExtension::initialize()
{
  // Register OdDbEntity_Dumper with Teigha
  OdDbEntity_Dumper::rxInit();
  OdDbObject_Dumper::rxInit();///
  m_pDumpers = new Dumpers;
  m_pDumpers->addXs();
}

/************************************************************************/
/* Uninitialize protocol extensions                                     */
/************************************************************************/
void ExProtocolExtension::uninitialize()
{
  if (m_pDumpers)
  {
    m_pDumpers->delXs();
    OdDbEntity_Dumper::rxUninit();
    OdDbObject_Dumper::rxUninit();
    delete m_pDumpers;
    m_pDumpers = 0;
  }
}


