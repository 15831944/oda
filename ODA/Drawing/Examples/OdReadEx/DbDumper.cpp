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

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "DbDumper.h"
#include "toString.h"
#include "DbProxyObject.h"
#include "DbProxyExt.h"
#include "summinfo.h"

/************************************************************************/
/* Dump the Header Variables                                            */
/************************************************************************/
void DbDumper::dumpHeader(OdDbDatabase* pDb, int indent)
{
  writeLine();  
  writeLine(indent, OD_T("Filename:"),         shortenPath(pDb->getFilename()));
  writeLine(indent, OD_T("File DWG Version:"), toString(pDb->originalFileVersion()));

  writeLine();  
  writeLine(indent++, OD_T("Header Variables:"));
 
  writeLine();
  writeLine(indent, OD_T("TDCREATE:"), toString(pDb->getTDCREATE()));
  writeLine(indent, OD_T("TDUPDATE:"), toString(pDb->getTDUPDATE()));

  writeLine();
  writeLine(indent, OD_T("ANGBASE"),       toString(pDb->getANGBASE()));        
  writeLine(indent, OD_T("ANGDIR"),        toString(pDb->getANGDIR()));               
  writeLine(indent, OD_T("ATTMODE"),       toString(pDb->getATTMODE()));                   
  writeLine(indent, OD_T("AUNITS"),        toString(pDb->getAUNITS()));
  writeLine(indent, OD_T("AUPREC"),        toString(pDb->getAUPREC()));                   
  writeLine(indent, OD_T("CECOLOR"),       toString(pDb->getCECOLOR()));            
  writeLine(indent, OD_T("CELTSCALE"),     toString(pDb->getCELTSCALE()));            
  writeLine(indent, OD_T("CHAMFERA"),      toString(pDb->getCHAMFERA()));
  writeLine(indent, OD_T("CHAMFERB"),      toString(pDb->getCHAMFERB()));         
  writeLine(indent, OD_T("CHAMFERC"),      toString(pDb->getCHAMFERC()));         
  writeLine(indent, OD_T("CHAMFERD"),      toString(pDb->getCHAMFERD ()));
  writeLine(indent, OD_T("CMLJUST"),       toString(pDb->getCMLJUST()));               
  writeLine(indent, OD_T("CMLSCALE"),      toString(pDb->getCMLSCALE()));         
  writeLine(indent, OD_T("DIMADEC"),       toString(pDb->dimadec()));
  writeLine(indent, OD_T("DIMALT"),        toString(pDb->dimalt()));
  writeLine(indent, OD_T("DIMALTD"),       toString(pDb->dimaltd()));
  writeLine(indent, OD_T("DIMALTF"),       toString(pDb->dimaltf()));
  writeLine(indent, OD_T("DIMALTRND"),     toString(pDb->dimaltrnd()));
  writeLine(indent, OD_T("DIMALTTD"),      toString(pDb->dimalttd()));
  writeLine(indent, OD_T("DIMALTTZ"),      toString(pDb->dimalttz()));
  writeLine(indent, OD_T("DIMALTU"),       toString(pDb->dimaltu()));
  writeLine(indent, OD_T("DIMALTZ"),       toString(pDb->dimaltz()));
  writeLine(indent, OD_T("DIMAPOST"),      toString(pDb->dimapost()));
  writeLine(indent, OD_T("DIMASZ"),        toString(pDb->dimasz()));
  writeLine(indent, OD_T("DIMATFIT"),      toString(pDb->dimatfit()));
  writeLine(indent, OD_T("DIMAUNIT"),      toString(pDb->dimaunit()));
  writeLine(indent, OD_T("DIMAZIN"),       toString(pDb->dimazin()));
  writeLine(indent, OD_T("DIMBLK"),        toString(pDb->dimblk()));
  writeLine(indent, OD_T("DIMBLK1"),       toString(pDb->dimblk1()));
  writeLine(indent, OD_T("DIMBLK2"),       toString(pDb->dimblk2()));
  writeLine(indent, OD_T("DIMCEN"),        toString(pDb->dimcen()));
  writeLine(indent, OD_T("DIMCLRD"),       toString(pDb->dimclrd()));
  writeLine(indent, OD_T("DIMCLRE"),       toString(pDb->dimclre()));
  writeLine(indent, OD_T("DIMCLRT"),       toString(pDb->dimclrt()));
  writeLine(indent, OD_T("DIMDEC"),        toString(pDb->dimdec()));
  writeLine(indent, OD_T("DIMDLE"),        toString(pDb->dimdle()));
  writeLine(indent, OD_T("DIMDLI"),        toString(pDb->dimdli()));
  writeLine(indent, OD_T("DIMDSEP"),       toString(pDb->dimdsep()));
  writeLine(indent, OD_T("DIMEXE"),        toString(pDb->dimexe()));
  writeLine(indent, OD_T("DIMEXO"),        toString(pDb->dimexo()));
  writeLine(indent, OD_T("DIMFRAC"),       toString(pDb->dimfrac()));
  writeLine(indent, OD_T("DIMGAP"),        toString(pDb->dimgap()));
  writeLine(indent, OD_T("DIMJUST"),       toString(pDb->dimjust()));
  writeLine(indent, OD_T("DIMLDRBLK"),     toString(pDb->dimldrblk()));
  writeLine(indent, OD_T("DIMLFAC"),       toString(pDb->dimlfac()));
  writeLine(indent, OD_T("DIMLIM"),        toString(pDb->dimlim()));
  writeLine(indent, OD_T("DIMLUNIT"),      toString(pDb->dimlunit()));
  writeLine(indent, OD_T("DIMLWD"),        toString(pDb->dimlwd()));
  writeLine(indent, OD_T("DIMLWE"),        toString(pDb->dimlwe()));
  writeLine(indent, OD_T("DIMPOST"),       toString(pDb->dimpost()));
  writeLine(indent, OD_T("DIMRND"),        toString(pDb->dimrnd()));
  writeLine(indent, OD_T("DIMSAH"),        toString(pDb->dimsah()));
  writeLine(indent, OD_T("DIMSCALE"),      toString(pDb->dimscale()));
  writeLine(indent, OD_T("DIMSD1"),        toString(pDb->dimsd1()));
  writeLine(indent, OD_T("DIMSD2"),        toString(pDb->dimsd2()));
  writeLine(indent, OD_T("DIMSE1"),        toString(pDb->dimse1()));
  writeLine(indent, OD_T("DIMSE2"),        toString(pDb->dimse2()));
  writeLine(indent, OD_T("DIMSOXD"),       toString(pDb->dimsoxd()));
  writeLine(indent, OD_T("DIMTAD"),        toString(pDb->dimtad()));
  writeLine(indent, OD_T("DIMTDEC"),       toString(pDb->dimtdec()));
  writeLine(indent, OD_T("DIMTFAC"),       toString(pDb->dimtfac()));
  writeLine(indent, OD_T("DIMTIH"),        toString(pDb->dimtih()));
  writeLine(indent, OD_T("DIMTIX"),        toString(pDb->dimtix()));
  writeLine(indent, OD_T("DIMTM"),         toString(pDb->dimtm()));
  writeLine(indent, OD_T("DIMTOFL"),       toString(pDb->dimtofl()));
  writeLine(indent, OD_T("DIMTOH"),        toString(pDb->dimtoh()));
  writeLine(indent, OD_T("DIMTOL"),        toString(pDb->dimtol()));
  writeLine(indent, OD_T("DIMTOLJ"),       toString(pDb->dimtolj()));
  writeLine(indent, OD_T("DIMTP"),         toString(pDb->dimtp()));
  writeLine(indent, OD_T("DIMTSZ"),        toString(pDb->dimtsz()));
  writeLine(indent, OD_T("DIMTVP"),        toString(pDb->dimtvp()));
  writeLine(indent, OD_T("DIMTXSTY"),      toString(pDb->dimtxsty()));
  writeLine(indent, OD_T("DIMTXT"),        toString(pDb->dimtxt()));
  writeLine(indent, OD_T("DIMTZIN"),       toString(pDb->dimtzin()));
  writeLine(indent, OD_T("DIMUPT"),        toString(pDb->dimupt()));
  writeLine(indent, OD_T("DIMZIN"),        toString(pDb->dimzin()));
  writeLine(indent, OD_T("DISPSILH"),      toString(pDb->getDISPSILH()));               
  writeLine(indent, OD_T("DRAWORDERCTL"),  toString(pDb->getDRAWORDERCTL()));      
  writeLine(indent, OD_T("ELEVATION"),     toString(pDb->getELEVATION()));      
  writeLine(indent, OD_T("EXTMAX"),        toString(pDb->getEXTMAX()));          
  writeLine(indent, OD_T("EXTMIN"),        toString(pDb->getEXTMIN()));          
  writeLine(indent, OD_T("FACETRES"),      toString(pDb->getFACETRES()));         
  writeLine(indent, OD_T("FILLETRAD"),     toString(pDb->getFILLETRAD()));       
  writeLine(indent, OD_T("FILLMODE"),      toString(pDb->getFILLMODE()));                
  writeLine(indent, OD_T("INSBASE"),       toString(pDb->getINSBASE()));         
  writeLine(indent, OD_T("ISOLINES"),      toString(pDb->getISOLINES()));           
  writeLine(indent, OD_T("LIMCHECK"),      toString(pDb->getLIMCHECK()));                
  writeLine(indent, OD_T("LIMMAX"),        toString(pDb->getLIMMAX()));        
  writeLine(indent, OD_T("LIMMIN"),        toString(pDb->getLIMMIN()));          
  writeLine(indent, OD_T("LTSCALE"),       toString(pDb->getLTSCALE()));           
  writeLine(indent, OD_T("LUNITS"),        toString(pDb->getLUNITS()));               
  writeLine(indent, OD_T("LUPREC"),        toString(pDb->getLUPREC()));                   
  writeLine(indent, OD_T("MAXACTVP"),      toString(pDb->getMAXACTVP()));           
  writeLine(indent, OD_T("MIRRTEXT"),      toString(pDb->getMIRRTEXT()));                
  writeLine(indent, OD_T("ORTHOMODE"),     toString(pDb->getORTHOMODE()));               
  writeLine(indent, OD_T("PDMODE"),        toString(pDb->getPDMODE()));           
  writeLine(indent, OD_T("PDSIZE"),        toString(pDb->getPDSIZE()));             
  writeLine(indent, OD_T("PELEVATION"),    toString(pDb->getPELEVATION()));            
  writeLine(indent, OD_T("PELLIPSE"),      toString(pDb->getPELLIPSE()));               
  writeLine(indent, OD_T("PEXTMAX"),       toString(pDb->getPEXTMAX()));         
  writeLine(indent, OD_T("PEXTMIN"),       toString(pDb->getPEXTMIN()));          
  writeLine(indent, OD_T("PINSBASE"),      toString(pDb->getPINSBASE()));           
  writeLine(indent, OD_T("PLIMCHECK"),     toString(pDb->getPLIMCHECK()));               
  writeLine(indent, OD_T("PLIMMAX"),       toString(pDb->getPLIMMAX()));
  writeLine(indent, OD_T("PLIMMIN"),       toString(pDb->getPLIMMIN()));         
  writeLine(indent, OD_T("PLINEGEN"),      toString(pDb->getPLINEGEN()));                
  writeLine(indent, OD_T("PLINEWID"),      toString(pDb->getPLINEWID()));           
  writeLine(indent, OD_T("PROXYGRAPHICS"), toString(pDb->getPROXYGRAPHICS()));                
  writeLine(indent, OD_T("PSLTSCALE"),     toString(pDb->getPSLTSCALE()));               
  writeLine(indent, OD_T("PUCSNAME"),      toString(pDb->getPUCSNAME()));         
  writeLine(indent, OD_T("PUCSORG"),       toString(pDb->getPUCSORG()));
  writeLine(indent, OD_T("PUCSXDIR"),      toString(pDb->getPUCSXDIR()));         
  writeLine(indent, OD_T("PUCSYDIR"),      toString(pDb->getPUCSYDIR()));          
  writeLine(indent, OD_T("QTEXTMODE"),     toString(pDb->getQTEXTMODE()));               
  writeLine(indent, OD_T("REGENMODE"),     toString(pDb->getREGENMODE()));               
  writeLine(indent, OD_T("SHADEDGE"),      toString(pDb->getSHADEDGE()));           
  writeLine(indent, OD_T("SHADEDIF"),      toString(pDb->getSHADEDIF()));           
  writeLine(indent, OD_T("SKETCHINC"),     toString(pDb->getSKETCHINC()));       
  writeLine(indent, OD_T("SKPOLY"),        toString(pDb->getSKPOLY()));               
  writeLine(indent, OD_T("SPLFRAME"),      toString(pDb->getSPLFRAME()));
  writeLine(indent, OD_T("SPLINESEGS"),    toString(pDb->getSPLINESEGS()));           
  writeLine(indent, OD_T("SPLINETYPE"),    toString(pDb->getSPLINETYPE()));           
  writeLine(indent, OD_T("SURFTAB1"),      toString(pDb->getSURFTAB1()));           
  writeLine(indent, OD_T("SURFTAB2"),      toString(pDb->getSURFTAB2()));           
  writeLine(indent, OD_T("SURFTYPE"),      toString(pDb->getSURFTYPE()));           
  writeLine(indent, OD_T("SURFU"),         toString(pDb->getSURFU()));           
  writeLine(indent, OD_T("SURFV"),         toString(pDb->getSURFV()));           
  writeLine(indent, OD_T("TEXTQLTY"),      toString(pDb->getTEXTQLTY()));          
  writeLine(indent, OD_T("TEXTSIZE"),      toString(pDb->getTEXTSIZE()));          
  writeLine(indent, OD_T("THICKNESS"),     toString(pDb->getTHICKNESS()));
  writeLine(indent, OD_T("TILEMODE"),      toString(pDb->getTILEMODE()));                
  writeLine(indent, OD_T("TRACEWID"),      toString(pDb->getTRACEWID()));       
  writeLine(indent, OD_T("TREEDEPTH"),     toString(pDb->getTREEDEPTH()));               
  writeLine(indent, OD_T("UCSNAME"),       toString(pDb->getUCSNAME()));      
  writeLine(indent, OD_T("UCSORG"),        toString(pDb->getUCSORG()));       
  writeLine(indent, OD_T("UCSXDIR"),       toString(pDb->getUCSXDIR()));      
  writeLine(indent, OD_T("UCSYDIR"),       toString(pDb->getUCSYDIR()));      
  writeLine(indent, OD_T("UNITMODE"),      toString(pDb->getUNITMODE()));           
  writeLine(indent, OD_T("USERI1"),        toString(pDb->getUSERI1()));           
  writeLine(indent, OD_T("USERI2"),        toString(pDb->getUSERI2()));
  writeLine(indent, OD_T("USERI3"),        toString(pDb->getUSERI3()));           
  writeLine(indent, OD_T("USERI4"),        toString(pDb->getUSERI4()));           
  writeLine(indent, OD_T("USERI5"),        toString(pDb->getUSERI5()));           
  writeLine(indent, OD_T("USERR1"),        toString(pDb->getUSERR1()));         
  writeLine(indent, OD_T("USERR2"),        toString(pDb->getUSERR2()));         
  writeLine(indent, OD_T("USERR3"),        toString(pDb->getUSERR3()));         
  writeLine(indent, OD_T("USERR4"),        toString(pDb->getUSERR4()));         
  writeLine(indent, OD_T("USERR5"),        toString(pDb->getUSERR5()));         
  writeLine(indent, OD_T("USRTIMER"),      toString(pDb->getUSRTIMER()));               
  writeLine(indent, OD_T("VISRETAIN"),     toString(pDb->getVISRETAIN()));               
  writeLine(indent, OD_T("WORLDVIEW"),     toString(pDb->getWORLDVIEW()));
}

/************************************************************************/
/* Dump Summary Info                                           */
/************************************************************************/
void DbDumper::dumpSummaryInfo(OdDbDatabase* pDb, int indent)
{
  writeLine();  
  OdDbDatabaseSummaryInfoPtr sumInfo = oddbGetSummaryInfo(pDb);
  OdString tmp;
  writeLine(indent, "SummaryInfo:");

  writeLine(indent, "Title: ", toString(sumInfo->getTitle()));
  writeLine(indent, "Subject: ", toString(sumInfo->getSubject()));
  writeLine(indent, "Author: ", toString(sumInfo->getAuthor()));
  writeLine(indent, "Comments: ", toString(sumInfo->getComments()));
  writeLine(indent, "Keywords: ", toString(sumInfo->getKeywords()));
  writeLine(indent, "LastSavedBy: ", toString(sumInfo->getLastSavedBy()));
  writeLine(indent, "RevisionNumber: ", toString(sumInfo->getRevisionNumber()));

  int numCustoms = sumInfo->numCustomInfo();
  writeLine(indent, "Number of custom info: ", toString(numCustoms));
  for (int i = 0; i < numCustoms; i++)
  {
    OdString key = "";;
    OdString value = "";;
    sumInfo->getCustomSummaryInfo(i, key, value);
    writeLine(indent, "Key: ", key);
    writeLine(indent, "Value: ", value);
  }
}

/************************************************************************/
/* Dump a Symbol Table Record                                           */
/************************************************************************/
void DbDumper::dumpSymbolTableRecord (OdDbSymbolTableRecordPtr pRecord, int indent)
{
  writeLine(indent, OD_T("Xref dependent"),   toString(pRecord->isDependent()));
  if (pRecord->isDependent())
  {
    writeLine(indent, OD_T("Resolved"),       toString(pRecord->isResolved()));
  }
}
/************************************************************************/
/* Dump the LayerTable                                                  */
/************************************************************************/
void DbDumper::dumpLayers(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the LayerTable                               */
  /**********************************************************************/
  OdDbLayerTablePtr pTable = pDb->getLayerTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the LayerTable                                        */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /********************************************************************/
    /* Open the LayerTableRecord for Reading                            */
    /********************************************************************/
    OdDbLayerTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /********************************************************************/
    /* Dump the LayerTableRecord                                        */
    /********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"),             toString(pRecord->getName()));
    writeLine(indent, OD_T("In Use"),           toString(pRecord->isInUse()));
    writeLine(indent, OD_T("On"),               toString(!pRecord->isOff()));
    writeLine(indent, OD_T("Frozen"),           toString(pRecord->isFrozen()));
    writeLine(indent, OD_T("Locked"),           toString(pRecord->isLocked()));
    writeLine(indent, OD_T("Color"),            toString(pRecord->color()));
    writeLine(indent, OD_T("Linetype"),         toString(pRecord->linetypeObjectId()));
    writeLine(indent, OD_T("Lineweight"),       toString(pRecord->lineWeight()));
    writeLine(indent, OD_T("Plotstyle"),        toString(pRecord->plotStyleName()));
    writeLine(indent, OD_T("Plottable"),        toString(pRecord->isPlottable()));
    writeLine(indent, OD_T("New VP Freeze"),    toString(pRecord->VPDFLT()));
    dumpSymbolTableRecord(pRecord, indent);
  }
}

/************************************************************************/
/* Dump the LinetypeTable                                               */
/************************************************************************/
void DbDumper::dumpLinetypes(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the LinetypeTable                            */
  /**********************************************************************/
  OdDbLinetypeTablePtr pTable = pDb->getLinetypeTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the LinetypeTable                                     */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /*********************************************************************/
    /* Open the LinetypeTableRecord for Reading                          */
    /*********************************************************************/
    OdDbLinetypeTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /********************************************************************/
    /* Dump the LinetypeTableRecord                                      */
    /********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    /********************************************************************/
    /* Dump the first line of record as in ACAD.LIN                     */
    /********************************************************************/
    OdString buffer;
    buffer = OdString(OD_T("*")) + pRecord->getName();
    if (pRecord->comments() != OD_T("")) 
    {
      buffer += OD_T(",") + pRecord->comments();
    }
    writeLine(indent, buffer);

    /********************************************************************/
    /* Dump the second line of record as in ACAD.LIN                    */
    /********************************************************************/
    if (pRecord->numDashes()) 
    {
      buffer = (pRecord->isScaledToFit() ? OD_T("S") : OD_T("A"));
      for (int i=0; i < pRecord->numDashes(); i++) {
          buffer += OD_T(",") + toString(pRecord->dashLengthAt(i));
          OdUInt16 shapeNumber  = pRecord->shapeNumberAt(i);
          OdString text = pRecord->textAt(i);
         
          /**************************************************************/
          /* Dump the Complex Line                                      */
          /**************************************************************/
          if (shapeNumber || text != OD_T("")) 
          {
            OdDbTextStyleTableRecordPtr pTextStyle = pRecord->shapeStyleAt(i).openObject();
            if (shapeNumber) 
            {
              buffer += OD_T(",[") +  toString(shapeNumber) + OD_T(",");
            if (!pTextStyle.isNull())
              buffer += pTextStyle->fileName();
            else
              buffer += L"NULL style";
            }
            else 
            {
              buffer += OD_T(",[") + toString(text) + OD_T(",");
              if (!pTextStyle.isNull())
                buffer += pTextStyle->getName();
              else
                buffer += L"NULL style";
            }
            if (pRecord->shapeScaleAt(i)) 
            {
              buffer += OD_T(",S") + toString(pRecord->shapeScaleAt(i));
            }
            if (pRecord->shapeRotationAt(i)) 
            {
              buffer += OD_T(",R") + toDegreeString(pRecord->shapeRotationAt(i));
            }
            if (pRecord->shapeOffsetAt(i).x) 
            {
              buffer += OD_T(",X") + toString(pRecord->shapeOffsetAt(i).x);
            }
            if (pRecord->shapeOffsetAt(i).y) 
            {
              buffer += OD_T(",Y") + toString(pRecord->shapeOffsetAt(i).y);
            }
            buffer += OD_T("]");
          }
        }
        writeLine(indent, buffer);
     }
    dumpSymbolTableRecord(pRecord, indent);
  }
}
/************************************************************************/
/* Dump the TextStyleTable                                              */
/************************************************************************/
void DbDumper::dumpTextStyles(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the TextStyleTable                            */
  /**********************************************************************/
  OdDbTextStyleTablePtr pTable = pDb->getTextStyleTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the TextStyleTable                                    */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /*********************************************************************/
    /* Open the TextStyleTableRecord for Reading                         */
    /*********************************************************************/
    OdDbTextStyleTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /*********************************************************************/
    /* Dump the TextStyleTableRecord                                      */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"),              toString(pRecord->getName()));
    writeLine(indent, OD_T("Shape File"),        toString(pRecord->isShapeFile()));
    writeLine(indent, OD_T("Text Height"),       toString(pRecord->textSize()));
    writeLine(indent, OD_T("Width Factor"),      toString(pRecord->xScale()));
    writeLine(indent, OD_T("Obliquing Angle"),   toDegreeString(pRecord->obliquingAngle()));
    writeLine(indent, OD_T("Backwards"),         toString(pRecord->isBackwards()));
    writeLine(indent, OD_T("Vertical"),          toString(pRecord->isVertical()));
    writeLine(indent, OD_T("Upside Down"),       toString(pRecord->isUpsideDown()));
    writeLine(indent, OD_T("Filename"),          shortenPath(pRecord->fileName()));
    writeLine(indent, OD_T("BigFont Filename"),  shortenPath(pRecord->bigFontFileName()));

    OdString typeface;
    bool bold;
    bool italic;
    int charset;
    int pitchAndFamily;
    pRecord->font(typeface, bold, italic, charset, pitchAndFamily); 
    writeLine(indent, OD_T("Typeface"),               toString(typeface));
    writeLine(indent, OD_T("Character Set"),          toString(charset));
    writeLine(indent, OD_T("Bold"),                   toString(bold));
    writeLine(indent, OD_T("Italic"),                 toString(italic));
    writeLine(indent, OD_T("Font Pitch & Family"),  toHexString(pitchAndFamily));
    dumpSymbolTableRecord(pRecord, indent);
  }
} 
/************************************************************************/
/* Dump the DimStyleTable                                               */
/************************************************************************/
void DbDumper::dumpDimStyles(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the DimStyleTable                            */
  /**********************************************************************/
  OdDbDimStyleTablePtr pTable = pDb->getDimStyleTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the DimStyleTable                                    */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /*********************************************************************/
    /* Open the DimStyleTableRecord for Reading                         */
    /*********************************************************************/
    OdDbDimStyleTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /*********************************************************************/
    /* Dump the DimStyleTableRecord                                      */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"),              toString(pRecord->getName()));
    writeLine(indent, OD_T("Arc Symbol"), toArcSymbolTypeString(pRecord->getArcSymbolType()));
    
    OdCmColor bgrndTxtColor;
    OdUInt16 bgrndTxtFlags = pRecord->getBgrndTxtColor(bgrndTxtColor);
    writeLine(indent, OD_T("Background Text Color"),           toString(bgrndTxtColor));
    writeLine(indent, OD_T("BackgroundText Flags"),            toString(bgrndTxtFlags));
    writeLine(indent, OD_T("Extension Line 1 Linetype"),       toString(pRecord->getDimExt1Linetype()));
    writeLine(indent, OD_T("Extension Line 2 Linetype"),       toString(pRecord->getDimExt2Linetype()));
    writeLine(indent, OD_T("Dimension Line Linetype"),         toString(pRecord->getDimExt2Linetype()));
    writeLine(indent, OD_T("Extension Line Fixed Len"),        toString(pRecord->getExtLineFixLen()));
    writeLine(indent, OD_T("Extension Line Fixed Len Enable"), toString(pRecord->getExtLineFixLenEnable()));
    writeLine(indent, OD_T("Jog Angle"),                       toDegreeString(pRecord->getJogAngle()));
    writeLine(indent, OD_T("Modified For Recompute"),          toString(pRecord->isModifiedForRecompute()));
    writeLine(indent, OD_T("DIMADEC"),       toString(pRecord->dimadec()));
    writeLine(indent, OD_T("DIMALT"),        toString(pRecord->dimalt()));
    writeLine(indent, OD_T("DIMALTD"),       toString(pRecord->dimaltd()));
    writeLine(indent, OD_T("DIMALTF"),       toString(pRecord->dimaltf()));
    writeLine(indent, OD_T("DIMALTRND"),     toString(pRecord->dimaltrnd()));
    writeLine(indent, OD_T("DIMALTTD"),      toString(pRecord->dimalttd()));
    writeLine(indent, OD_T("DIMALTTZ"),      toString(pRecord->dimalttz()));
    writeLine(indent, OD_T("DIMALTU"),       toString(pRecord->dimaltu()));
    writeLine(indent, OD_T("DIMALTZ"),       toString(pRecord->dimaltz()));
    writeLine(indent, OD_T("DIMAPOST"),      toString(pRecord->dimapost()));
    writeLine(indent, OD_T("DIMASZ"),        toString(pRecord->dimasz()));
    writeLine(indent, OD_T("DIMATFIT"),      toString(pRecord->dimatfit()));
    writeLine(indent, OD_T("DIMAUNIT"),      toString(pRecord->dimaunit()));
    writeLine(indent, OD_T("DIMAZIN"),       toString(pRecord->dimazin()));
    writeLine(indent, OD_T("DIMBLK"),        toString(pRecord->dimblk()));
    writeLine(indent, OD_T("DIMBLK1"),       toString(pRecord->dimblk1()));
    writeLine(indent, OD_T("DIMBLK2"),       toString(pRecord->dimblk2()));
    writeLine(indent, OD_T("DIMCEN"),        toString(pRecord->dimcen()));
    writeLine(indent, OD_T("DIMCLRD"),       toString(pRecord->dimclrd()));
    writeLine(indent, OD_T("DIMCLRE"),       toString(pRecord->dimclre()));
    writeLine(indent, OD_T("DIMCLRT"),       toString(pRecord->dimclrt()));
    writeLine(indent, OD_T("DIMDEC"),        toString(pRecord->dimdec()));
    writeLine(indent, OD_T("DIMDLE"),        toString(pRecord->dimdle()));
    writeLine(indent, OD_T("DIMDLI"),        toString(pRecord->dimdli()));
    writeLine(indent, OD_T("DIMDSEP"),       toString(pRecord->dimdsep()));
    writeLine(indent, OD_T("DIMEXE"),        toString(pRecord->dimexe()));
    writeLine(indent, OD_T("DIMEXO"),        toString(pRecord->dimexo()));
    writeLine(indent, OD_T("DIMFRAC"),       toString(pRecord->dimfrac()));
    writeLine(indent, OD_T("DIMGAP"),        toString(pRecord->dimgap()));
    writeLine(indent, OD_T("DIMJUST"),       toString(pRecord->dimjust()));
    writeLine(indent, OD_T("DIMLDRBLK"),     toString(pRecord->dimldrblk()));
    writeLine(indent, OD_T("DIMLFAC"),       toString(pRecord->dimlfac()));
    writeLine(indent, OD_T("DIMLIM"),        toString(pRecord->dimlim()));
    writeLine(indent, OD_T("DIMLUNIT"),      toString(pRecord->dimlunit()));
    writeLine(indent, OD_T("DIMLWD"),        toString(pRecord->dimlwd()));
    writeLine(indent, OD_T("DIMLWE"),        toString(pRecord->dimlwe()));
    writeLine(indent, OD_T("DIMPOST"),       toString(pRecord->dimpost()));
    writeLine(indent, OD_T("DIMRND"),        toString(pRecord->dimrnd()));
    writeLine(indent, OD_T("DIMSAH"),        toString(pRecord->dimsah()));
    writeLine(indent, OD_T("DIMSCALE"),      toString(pRecord->dimscale()));
    writeLine(indent, OD_T("DIMSD1"),        toString(pRecord->dimsd1()));
    writeLine(indent, OD_T("DIMSD2"),        toString(pRecord->dimsd2()));
    writeLine(indent, OD_T("DIMSE1"),        toString(pRecord->dimse1()));
    writeLine(indent, OD_T("DIMSE2"),        toString(pRecord->dimse2()));
    writeLine(indent, OD_T("DIMSOXD"),       toString(pRecord->dimsoxd()));
    writeLine(indent, OD_T("DIMTAD"),        toString(pRecord->dimtad()));
    writeLine(indent, OD_T("DIMTDEC"),       toString(pRecord->dimtdec()));
    writeLine(indent, OD_T("DIMTFAC"),       toString(pRecord->dimtfac()));
    writeLine(indent, OD_T("DIMTIH"),        toString(pRecord->dimtih()));
    writeLine(indent, OD_T("DIMTIX"),        toString(pRecord->dimtix()));
    writeLine(indent, OD_T("DIMTM"),         toString(pRecord->dimtm()));
    writeLine(indent, OD_T("DIMTOFL"),       toString(pRecord->dimtofl()));
    writeLine(indent, OD_T("DIMTOH"),        toString(pRecord->dimtoh()));
    writeLine(indent, OD_T("DIMTOL"),        toString(pRecord->dimtol()));
    writeLine(indent, OD_T("DIMTOLJ"),       toString(pRecord->dimtolj()));
    writeLine(indent, OD_T("DIMTP"),         toString(pRecord->dimtp()));
    writeLine(indent, OD_T("DIMTSZ"),        toString(pRecord->dimtsz()));
    writeLine(indent, OD_T("DIMTVP"),        toString(pRecord->dimtvp()));
    writeLine(indent, OD_T("DIMTXSTY"),      toString(pRecord->dimtxsty()));
    writeLine(indent, OD_T("DIMTXT"),        toString(pRecord->dimtxt()));
    writeLine(indent, OD_T("DIMTZIN"),       toString(pRecord->dimtzin()));
    writeLine(indent, OD_T("DIMUPT"),        toString(pRecord->dimupt()));
    writeLine(indent, OD_T("DIMZIN"),        toString(pRecord->dimzin()));
    
    dumpSymbolTableRecord(pRecord, indent);
  }
}

/************************************************************************/
/* Dump the RegAppTable                                              */
/************************************************************************/
void DbDumper::dumpRegApps(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the RegAppTable                            */
  /**********************************************************************/
  OdDbRegAppTablePtr pTable = pDb->getRegAppTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the RegAppTable                                    */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /*********************************************************************/
    /* Open the RegAppTableRecord for Reading                         */
    /*********************************************************************/
    OdDbRegAppTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /*********************************************************************/
    /* Dump the RegAppTableRecord                                      */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"), toString(pRecord->getName()));
  }
}

/************************************************************************/
/* Dump the AbstractViewTableRecord                                     */
/*************************************************************************/
void DbDumper::dumpAbstractViewTableRecord(OdDbAbstractViewTableRecordPtr pView, int indent) {
  
    /*********************************************************************/
    /* Dump the AbstractViewTableRecord                                  */
    /*********************************************************************/
    writeLine(indent, OD_T("Back Clip Dist"),      toString(pView->backClipDistance()));
    writeLine(indent, OD_T("Back Clip Enabled"),   toString(pView->backClipEnabled()));
    writeLine(indent, OD_T("Front Clip Dist"),     toString(pView->frontClipDistance()));
    writeLine(indent, OD_T("Front Clip Enabled"),  toString(pView->frontClipEnabled()));
    writeLine(indent, OD_T("Front Clip at Eye"),   toString(pView->frontClipAtEye()));
    writeLine(indent, OD_T("Elevation"),           toString(pView->elevation()));
    writeLine(indent, OD_T("Height"),              toString(pView->height()));
    writeLine(indent, OD_T("Width"),               toString(pView->width()));
    writeLine(indent, OD_T("Lens Length"),         toString(pView->lensLength()));
    writeLine(indent, OD_T("Render Mode"),         toString(pView->renderMode()));
    writeLine(indent, OD_T("Perspective"),         toString(pView->perspectiveEnabled()));
    writeLine(indent, OD_T("UCS Name"),            toString(pView->ucsName()));  

    OdDb::OrthographicView orthoUCS;       
    writeLine(indent, OD_T("UCS Orthographic"),    toString(pView->isUcsOrthographic(orthoUCS)));
    writeLine(indent, OD_T("Orthographic UCS"),    toString(orthoUCS));
    if (pView->isUcsOrthographic(orthoUCS))
    {
      OdGePoint3d origin;
      OdGeVector3d xAxis;
      OdGeVector3d yAxis;
      pView->getUcs(origin, xAxis, yAxis);
      writeLine(indent, OD_T("UCS Origin"),          toString(origin));        
      writeLine(indent, OD_T("UCS x-Axis"),          toString(xAxis));      
      writeLine(indent, OD_T("UCS y-Axis"),          toString(yAxis));
    }
    writeLine(indent, OD_T("Target"),              toString(pView->target()));       
    writeLine(indent, OD_T("View Direction"),      toString(pView->viewDirection()));       
    writeLine(indent, OD_T("Twist Angle"),         toDegreeString(pView->viewTwist()));
    dumpSymbolTableRecord(pView, indent);
}

/************************************************************************/
/* Dump the ViewportTable                                              */
/************************************************************************/
void DbDumper::dumpViewports(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the ViewportTable                            */
  /**********************************************************************/
  OdDbViewportTablePtr pTable = pDb->getViewportTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the ViewportTable                                    */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /*********************************************************************/
    /* Open the ViewportTableRecord for Reading                          */
    /*********************************************************************/
    OdDbViewportTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /*********************************************************************/
    /* Dump the ViewportTableRecord                                      */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"),              toString(pRecord->getName()));
    writeLine(indent, OD_T("Circle Sides"),      toString(pRecord->circleSides()));
    writeLine(indent, OD_T("Fast Zooms Enabled"),toString(pRecord->fastZoomsEnabled()));
    writeLine(indent, OD_T("Grid Enabled"),      toString(pRecord->gridEnabled()));
    writeLine(indent, OD_T("Grid Increments"),   toString(pRecord->gridIncrements()));
    writeLine(indent, OD_T("Icon at Origin"),    toString(pRecord->iconAtOrigin()));
    writeLine(indent, OD_T("Icon Enabled"),      toString(pRecord->iconEnabled()));
    writeLine(indent, OD_T("Iso snap Enabled"),  toString(pRecord->isometricSnapEnabled()));
    writeLine(indent, OD_T("Iso Snap Pair"),     toString(pRecord->snapPair()));
    writeLine(indent, OD_T("UCS Saved w/Vport"), toString(pRecord->isUcsSavedWithViewport()));
    writeLine(indent, OD_T("UCS follow"),        toString(pRecord->ucsFollowMode()));
    writeLine(indent, OD_T("Lower-Left Corner"), toString(pRecord->lowerLeftCorner()));
    writeLine(indent, OD_T("Upper-Right Corner"),toString(pRecord->upperRightCorner()));
    writeLine(indent, OD_T("Snap Angle"),        toDegreeString(pRecord->snapAngle()));
    writeLine(indent, OD_T("Snap Base"),         toString(pRecord->snapBase()));
    writeLine(indent, OD_T("Snap Enabled"),      toString(pRecord->snapEnabled()));
    writeLine(indent, OD_T("Snap Increments"),   toString(pRecord->snapIncrements()));
    dumpAbstractViewTableRecord(pRecord, indent);
  }
}

/************************************************************************/
/* Dump the ViewTable                                                   */
/************************************************************************/
void DbDumper::dumpViews(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the ViewTable                                */
  /**********************************************************************/
  OdDbViewTablePtr pTable = pDb->getViewTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the ViewTable                                         */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /*********************************************************************/
    /* Open the ViewTableRecord for Reading                              */
    /*********************************************************************/
    OdDbViewTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /*********************************************************************/
    /* Dump the ViewTableRecord                                          */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"),              toString(pRecord->getName()));
    writeLine(indent, OD_T("Category Name"),     toString(pRecord->getCategoryName()));
    writeLine(indent, OD_T("Layer State"),       toString(pRecord->getLayerState()));
    
    OdString layoutName(OD_T(""));
    if (!pRecord->getLayout().isNull()) 
    {
      OdDbLayoutPtr pLayout = pRecord->getLayout().safeOpenObject();
      layoutName = pLayout->getLayoutName();
    }
    writeLine(indent, OD_T("Layout Name"),        toString(layoutName));
    writeLine(indent, OD_T("PaperSpace View"),    toString(pRecord->isPaperspaceView()));
    writeLine(indent, OD_T("Associated UCS"),     toString(pRecord->isUcsAssociatedToView()));
    writeLine(indent, OD_T("PaperSpace View"),    toString(pRecord->isViewAssociatedToViewport()));
    dumpAbstractViewTableRecord(pRecord, indent);
  }
} 

/************************************************************************/
/* Dump the MLeaderStyle Dictionary                                       */
/************************************************************************/
void DbDumper::dumpMLeaderStyles(OdDbDatabase* pDb, int indent)
{
  OdDbDictionaryPtr pDictionary = pDb->getMLeaderStyleDictionaryId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pDictionary->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new DictionaryIterator                     */
  /**********************************************************************/
  OdDbDictionaryIteratorPtr pIter = pDictionary->newIterator(); 

  /**********************************************************************/
  /* Step through the MlineStyle dictionary                             */
  /**********************************************************************/
  for (; !pIter->done(); pIter->next())
  {
    OdDbObjectId id = pIter->objectId();
    OdDbMLeaderStylePtr pEntry = OdDbMLeaderStyle::cast(id.safeOpenObject());
    if (pEntry.isNull())
      continue;

    /*********************************************************************/
    /* Dump the MLeaderStyle dictionary entry                              */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pEntry->desc()));
    writeLine(indent, OD_T("Name"),                              toString(pEntry->getName()));
    writeLine(indent, OD_T("Description"),                       toString(pEntry->description()));
    writeLine(indent, OD_T("Bit flags"),                         toString((int)pEntry->bitFlags()));
    writeLine(indent, OD_T("Content type"),                      toString(pEntry->contentType()));
    writeLine(indent, OD_T("Draw order"),                        toString(pEntry->drawMLeaderOrderType()));
    writeLine(indent, OD_T("Lider line draw order"),             toString(pEntry->drawLeaderOrderType()));
    writeLine(indent, OD_T("Max number of segment points"),      toString(pEntry->maxLeaderSegmentsPoints()));
    writeLine(indent, OD_T("First segment angle constraint"),    toString(pEntry->firstSegmentAngleConstraint()));
    writeLine(indent, OD_T("Second segment angle constraint"),   toString(pEntry->secondSegmentAngleConstraint()));
    writeLine(indent, OD_T("Lider line type"),                   toString(pEntry->leaderLineType()));
    writeLine(indent, OD_T("Lider line color"),                  toString(pEntry->leaderLineColor()));
    writeLine(indent, OD_T("Lider line linetype"),               toString(pEntry->leaderLineTypeId()));
    writeLine(indent, OD_T("Lider line lineweight"),             toString(pEntry->leaderLineWeight()));
    writeLine(indent, OD_T("Lider line landing"),                toString(pEntry->enableLanding()));
    writeLine(indent, OD_T("Landing gap"),                       toString(pEntry->landingGap()));
    writeLine(indent, OD_T("The \"dog leg\" flag"),              toString(pEntry->enableDogleg()));
    writeLine(indent, OD_T("The \"dog leg\" length"),            toString(pEntry->doglegLength()));
    writeLine(indent, OD_T("Arrow symbol"),                      toString(pEntry->arrowSymbolId()));
    writeLine(indent, OD_T("Arrow size"),                        toString(pEntry->arrowSize()));
    writeLine(indent, OD_T("Text style ID"),                     toString(pEntry->textStyleId()));
    writeLine(indent, OD_T("Text attachment direction"),         toString(pEntry->textAttachmentDirection()));
    writeLine(indent, OD_T("Text attachment for left dir."),     toString(pEntry->textAttachmentType(OdDbMLeaderStyle::kLeftLeader)));
    writeLine(indent, OD_T("Text attachment for right dir."),    toString(pEntry->textAttachmentType(OdDbMLeaderStyle::kRightLeader)));
    writeLine(indent, OD_T("Text attachment for top dir."),      toString(pEntry->textAttachmentType(OdDbMLeaderStyle::kTopLeader)));
    writeLine(indent, OD_T("Text attachment for bottom dir."),   toString(pEntry->textAttachmentType(OdDbMLeaderStyle::kBottomLeader)));
    writeLine(indent, OD_T("Text angle type"),                   toString(pEntry->textAngleType()));
    writeLine(indent, OD_T("Text alignment type"),               toString(pEntry->textAlignmentType()));
    writeLine(indent, OD_T("Text alignment always left"),        toString(pEntry->textAlignAlwaysLeft()));
    writeLine(indent, OD_T("Text color"),                        toString(pEntry->textColor()));
    writeLine(indent, OD_T("Text height"),                       toString(pEntry->textHeight()));
    writeLine(indent, OD_T("Text frame enabled"),                toString(pEntry->enableFrameText()));
    writeLine(indent, OD_T("Alignment space"),                   toString(pEntry->alignSpace()));
    writeLine(indent, OD_T("Block ID"),                          toString(pEntry->blockId()));
    writeLine(indent, OD_T("Block color"),                       toString(pEntry->blockColor()));
    writeLine(indent, OD_T("Block scale"),                       toString(pEntry->blockScale()));
    writeLine(indent, OD_T("Block scale enabled"),               toString(pEntry->enableBlockScale()));
    writeLine(indent, OD_T("Block rotation"),                    toString(pEntry->blockRotation()));
    writeLine(indent, OD_T("Block rotation enabled"),            toString(pEntry->enableBlockRotation()));
    writeLine(indent, OD_T("Block connection type"),             toString(pEntry->blockConnectionType()));
    writeLine(indent, OD_T("Scale factor"),                      toString(pEntry->scale()));
    writeLine(indent, OD_T("Properties were modified"),          toString(pEntry->overwritePropChanged()));
    writeLine(indent, OD_T("Annotative"),                        toString(pEntry->annotative()));
    writeLine(indent, OD_T("Break size"),                        toString(pEntry->breakSize()));
    writeLine(indent, OD_T("Extend to text"),                    toString(pEntry->extendLeaderToText()));
  }
}

/************************************************************************/
/* Dump the MlineStyle Dictionary                                       */
/************************************************************************/
void DbDumper::dumpMLineStyles(OdDbDatabase* pDb, int indent)
{
  OdDbDictionaryPtr pDictionary = pDb->getMLStyleDictionaryId().safeOpenObject();
  
  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pDictionary->desc()));

  /**********************************************************************/
  /* Get a SmartPointer to a new DictionaryIterator                     */
  /**********************************************************************/
  OdDbDictionaryIteratorPtr pIter = pDictionary->newIterator(); 
  
  /**********************************************************************/
  /* Step through the MlineStyle dictionary                             */
  /**********************************************************************/
  for (; !pIter->done(); pIter->next())
  {
    OdDbObjectId id = pIter->objectId();
    OdDbMlineStylePtr pEntry = OdDbMlineStyle::cast(id.safeOpenObject());
    if (pEntry.isNull())
      continue;

    /*********************************************************************/
    /* Dump the MLineStyle dictionary entry                              */
    /*********************************************************************/
    writeLine();
    writeLine(indent, toString(pEntry->desc()));
    writeLine(indent, OD_T("Name"),               toString(pEntry->name()));
    writeLine(indent, OD_T("Description"),        toString(pEntry->description()));
    writeLine(indent, OD_T("Start Angle"),        toDegreeString(pEntry->startAngle()));
    writeLine(indent, OD_T("End Angle"),          toDegreeString(pEntry->endAngle()));
    writeLine(indent, OD_T("Start Inner Arcs"),   toString(pEntry->startInnerArcs()));
    writeLine(indent, OD_T("End Inner Arcs"),     toString(pEntry->endInnerArcs()));
    writeLine(indent, OD_T("Start Round Cap"),    toString(pEntry->startRoundCap()));
    writeLine(indent, OD_T("End Round Cap"),      toString(pEntry->endRoundCap()));
    writeLine(indent, OD_T("Start Square Cap"),   toString(pEntry->startSquareCap()));
    writeLine(indent, OD_T("End Square Cap"),     toString(pEntry->endSquareCap()));
    writeLine(indent, OD_T("Show Miters"),        toString(pEntry->showMiters()));
    /*********************************************************************/
    /* Dump the elements                                                 */
    /*********************************************************************/
    if (pEntry->numElements()) 
    {
      writeLine(indent, OD_T("Elements:"));
    }
    for (int i = 0; i < pEntry->numElements(); i++)
    {
      double offset;
      OdCmColor color;
      OdDbObjectId linetypeId;
      pEntry->getElementAt(i, offset, color,linetypeId);
      writeLine(indent,     OD_T("Index"),            toString(i));
      writeLine(indent + 1, OD_T("Offset"),           toString(offset));
      writeLine(indent + 1, OD_T("Color"),            toString(color));
      writeLine(indent + 1, OD_T("Linetype"),         toString(linetypeId));
    }
  }
}
/************************************************************************/
/* Dump the UCSTable                                                    */
/************************************************************************/
void DbDumper::dumpUCSTable(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the UCSTable                               */
  /**********************************************************************/
  OdDbUCSTablePtr pTable = pDb->getUCSTableId().safeOpenObject();

  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));
  
  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the UCSTable                                          */
  /**********************************************************************/
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    /********************************************************************/
    /* Open the UCSTableRecord for Reading                            */
    /********************************************************************/
    OdDbUCSTableRecordPtr pRecord = pIter->getRecordId().safeOpenObject();

    /********************************************************************/
    /* Dump the UCSTableRecord                                        */
    /********************************************************************/
    writeLine();
    writeLine(indent, toString(pRecord->desc()));
    writeLine(indent, OD_T("Name"),              toString(pRecord->getName()));
    writeLine(indent, OD_T("UCS Origin"),         toString(pRecord->origin()));       
    writeLine(indent, OD_T("UCS x-Axis"),         toString(pRecord->xAxis()));      
    writeLine(indent, OD_T("UCS y-Axis"),         toString(pRecord->yAxis()));
    dumpSymbolTableRecord(pRecord, indent);
  }
} 

/************************************************************************/
/* Dump the Entity                                                      */
/************************************************************************/
void DbDumper::dumpEntity(OdDbObjectId id, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the Entity                                   */
  /**********************************************************************/
  OdDbEntityPtr pEnt = OdDbEntity::cast( id.openObject() );
  if (pEnt.isNull())
    return;

  /**********************************************************************/
  /* Retrieve the Protocol Extension registered for this object type    */
  /**********************************************************************/
  OdSmartPtr<OdDbEntity_Dumper> pEntDumper = pEnt;

  /**********************************************************************/
  /* Dump the entity                                                    */
  /**********************************************************************/
  writeLine();
  pEntDumper->dump(pEnt, indent);

  /**********************************************************************/
  /* Dump the Xdata                                                     */
  /**********************************************************************/
  dumpXdata(pEnt->xData(), indent);    

  /**********************************************************************/
  /* Dump the Extension Dictionary                                      */
  /**********************************************************************/
  if (!pEnt->extensionDictionary().isNull())
  {
    dumpObject(pEnt->extensionDictionary(), OD_T("ACAD_XDICTIONARY"), indent);
  }
} 

/************************************************************************/
/* Dump the BlockTable                                                  */
/************************************************************************/
void DbDumper::dumpBlocks(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the BlockTable                               */
  /**********************************************************************/
  OdDbBlockTablePtr pTable = pDb->getBlockTableId().safeOpenObject();
  
  /**********************************************************************/
  /* Dump the Description                                               */
  /**********************************************************************/
  writeLine();
  writeLine(indent++, toString(pTable->desc()));
  
  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pBlkIter = pTable->newIterator();
  
  /**********************************************************************/
  /* Step through the BlockTable                                        */
  /**********************************************************************/
  for (pBlkIter->start(); ! pBlkIter->done(); pBlkIter->step())
  {
    /********************************************************************/
    /* Open the BlockTableRecord for Reading                            */
    /********************************************************************/
    OdDbBlockTableRecordPtr pBlock = pBlkIter->getRecordId().safeOpenObject();
    
    /********************************************************************/
    /* Dump the BlockTableRecord                                        */
    /********************************************************************/
    writeLine();
    writeLine(indent, toString(pBlock->desc()));
    writeLine(indent+1, OD_T("Name"),                         toString(pBlock->getName()));
    writeLine(indent+1, OD_T("Anonymous"),                    toString(pBlock->isAnonymous()));
    writeLine(indent+1, OD_T("Comments"),                     toString(pBlock->comments()));
    writeLine(indent+1, OD_T("Origin"),                       toString(pBlock->origin()));
    writeLine(indent+1, OD_T("Block Insert Units"),           toString(pBlock->blockInsertUnits()));
    writeLine(indent+1, OD_T("Block Scaling"),                toString(pBlock->blockScaling()));
    writeLine(indent+1, OD_T("Explodable"),                   toString(pBlock->explodable()));

    OdGeExtents3d extents;
    if (eOk == pBlock->getGeomExtents(extents)) {
      writeLine(indent+1, OD_T("Min Extents"),                toString(extents.minPoint()));
      writeLine(indent+1, OD_T("Max Extents"),                toString(extents.maxPoint()));
    }
    writeLine(indent+1, OD_T("Layout"),                       toString(pBlock->isLayout()));
    writeLine(indent+1, OD_T("Has Attribute Definitions"),    toString(pBlock->hasAttributeDefinitions()));
    writeLine(indent+1, OD_T("Xref Status"),                  toString(pBlock->xrefStatus()));
    if (pBlock->xrefStatus()!=OdDb::kXrfNotAnXref)
    {
      writeLine(indent+1, OD_T("Xref Path"),                  toString(pBlock->pathName()));
      writeLine(indent+1, OD_T("From Xref Attach"),           toString(pBlock->isFromExternalReference()));
      writeLine(indent+1, OD_T("From Xref Overlay"),          toString(pBlock->isFromOverlayReference()));
      writeLine(indent+1, OD_T("Xref Unloaded"),              toString(pBlock->isUnloaded()));
    }
    /********************************************************************/
    /* Get a SmartPointer to a new ObjectIterator                       */
    /********************************************************************/
    OdDbObjectIteratorPtr pEntIter = pBlock->newIterator();
    
    /********************************************************************/
    /* Step through the BlockTableRecord                                */
    /********************************************************************/
    for (; !pEntIter->done(); pEntIter->step())
    {
     /********************************************************************/
     /* Dump the Entity                                                  */
     /********************************************************************/
     dumpEntity(pEntIter->objectId(), indent+1);
    }
  }
} 

/************************************************************************/
/* Dump Xdata                                                           */
/************************************************************************/
void DbDumper::dumpXdata(OdResBuf* resbuf, int indent)
{
  if (!resbuf)
    return;
  
  dumpResBuf(resbuf, OD_T("Xdata:"), indent);
}

void DbDumper::dumpResBuf(OdResBuf* resbuf, const OdString& caption, int indent)
{
  if (!resbuf)
    return;

  writeLine(indent++, caption); 

  /**********************************************************************/
  /* Step through the ResBuf chain                                      */
  /**********************************************************************/
  for (; resbuf != 0; resbuf = resbuf->next())
  {
    int code = resbuf->restype();
    
    /********************************************************************/
    /* Convert resbuf->ResVal to a string                               */
    /********************************************************************/
    OdString rightString(OD_T("???"));
    switch (OdDxfCode::_getType(code))
    {
      case OdDxfCode::Name:
      case OdDxfCode::String:
      case OdDxfCode::LayerName:
        rightString = toString(resbuf->getString());
        break;

      case OdDxfCode::Bool:
        rightString = toString(resbuf->getBool());
        break;

      case OdDxfCode::Integer8:
        rightString = toString(resbuf->getInt8());
        break;

      case OdDxfCode::Integer16:
        rightString = toString(resbuf->getInt16());
        break;

      case OdDxfCode::Integer32:
        rightString = toString((int)resbuf->getInt32());
        break;

      case OdDxfCode::Double:
        rightString = toString(resbuf->getDouble());
        break;

      case OdDxfCode::Angle:
        rightString = toDegreeString(resbuf->getDouble());
        break;

      case OdDxfCode::Point:
        {
          rightString = toString(resbuf->getPoint3d());
        }
        break;

      case OdDxfCode::BinaryChunk:
        rightString = OD_T("<Binary Data>");
        break;

      case OdDxfCode::ObjectId:
      case OdDxfCode::SoftPointerId:
      case OdDxfCode::HardPointerId:
      case OdDxfCode::SoftOwnershipId:
      case OdDxfCode::HardOwnershipId:
      case OdDxfCode::Handle:
        rightString = toString(resbuf->getHandle());
        break;

      case OdDxfCode::Unknown:
      default:
        rightString = OD_T("Unknown");
        break;
    }
    writeLine(indent,toString(code),rightString);
  }
}

/************************************************************************/
/* Dump the Xref the full path to the Osnap entity                      */
/************************************************************************/
void DbDumper::dumpXrefFullSubentPath(OdDbXrefFullSubentPath& subEntPath, int indent)
{
  writeLine(indent, OD_T("Subentity Index"), toString((int)subEntPath.subentId().index()));
  writeLine(indent, OD_T("Subentity Type"), toString(subEntPath.subentId().type()));
  for (OdUInt32 j = 0; j < subEntPath.objectIds().size(); j++)
  {
    OdDbEntityPtr pEnt = subEntPath.objectIds()[j].openObject();
    if (!pEnt.isNull())
    {
      writeLine(indent, toString(pEnt->isA()), toString(pEnt->getDbHandle()));
    }
  }
}

/************************************************************************/
/* Dump Object Snap Point Reference for an Associative Dimension        */
/************************************************************************/
void DbDumper::dumpOsnapPointRef(OdDbOsnapPointRefPtr pRef,  int index, int indent)
{
  writeLine(indent++, toString(pRef->isA()), toString(index));
  writeLine(indent, OD_T("Osnap Mode"), toString(pRef->osnapType()));
  writeLine(indent, OD_T("Near Osnap"),  toString(pRef->nearPointParam()));
  writeLine(indent, OD_T("Osnap Point"), toString(pRef->point()));

  writeLine(indent, OD_T("Main Entity"));
  dumpXrefFullSubentPath(pRef->mainEntity(), indent+1);

  writeLine(indent, OD_T("Intersect Entity"));
  dumpXrefFullSubentPath(pRef->intersectEntity(), indent + 1);

  if (pRef->lastPointRef())
  {
    writeLine(indent, OD_T("Last Point Referenced"));
    dumpOsnapPointRef(pRef->lastPointRef(), index, indent + 1);
  }
  else
  {
    writeLine(indent, OD_T("Last Point Referenced"), OD_T("Null"));
  }
}

/************************************************************************/
/* Dump an Associative dimension                                        */
/************************************************************************/
void DbDumper::dumpDimAssoc(OdDbObjectPtr pObject, const int indent)
{
  OdDbDimAssocPtr pDimAssoc = pObject;
  writeLine(indent, OD_T("Associative"), 
      toString ((OdDbDimAssoc::AssocFlags) pDimAssoc->assocFlag()));
  writeLine(indent, OD_T("TransSpatial"), toString(pDimAssoc->isTransSpatial()));
  writeLine(indent, OD_T("Rotated Type"), toString(pDimAssoc->rotatedDimType()));

  for (int i = 0; i < OdDbDimAssoc::kMaxPointRefs; i++)
  {
    OdDbOsnapPointRefPtr pRef = pDimAssoc->pointRef(i);
    if (!pRef.isNull())
    {
      dumpOsnapPointRef(pRef, i, indent);
    }      
    else
    {
      break;
    }
  }  
}

/************************************************************************/
/* Dump the object.                                                      */
/*                                                                      */
/* Dictionary objects are recursively dumped.                           */
/* XRecord objects are dumped.                                          */
/* DimAssoc objects are dumped.                                         */
/************************************************************************/
void DbDumper::dumpObject(
                          OdDbObjectId id,
                          OdString itemName,
                          int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the object                                   */
  /**********************************************************************/
  OdDbObjectPtr pObject = id.openObject();
  
  if (pObject.isNull())
  {
    writeLine(indent++, itemName, L"*Erased*");
    return;
  }

  /**********************************************************************/
  /* Dump the item name and class name                                  */
  /**********************************************************************/
  if (pObject->isKindOf(OdDbDictionary::desc()))
  {
    writeLine();
  }

  OdString str = toString(pObject->isA());
  const OdString treeNode = OD_T("<AeccDbTreeNode>");
  bool isStrEq = str == treeNode;

  writeLine(indent++, itemName, str);
  
  /**********************************************************************/
  /* Dispatch                                                           */
  /**********************************************************************/
  if ( pObject->isKindOf( OdDbDictionary::desc() ) )
  {
    /********************************************************************/
    /* Dump the dictionary                                               */
    /********************************************************************/
    OdDbDictionaryPtr pDic = pObject;

    /********************************************************************/
    /* Get a SmartPointer to a new DictionaryIterator                   */
    /********************************************************************/
    OdDbDictionaryIteratorPtr pIter = pDic->newIterator(OdRx::kDictSorted);

    /********************************************************************/
    /* Step through the Dictionary                                      */
    /********************************************************************/
    for (; !pIter->done(); pIter->next())
    {
      /******************************************************************/
      /* Dump the Dictionary object                                     */
      /******************************************************************/
      // Dump each item in the dictionary.
      dumpObject(pIter->objectId(), pIter->name(), indent);
    }
  }
  else if (pObject->isKindOf(OdDbXrecord::desc()))
  {
    /********************************************************************/
    /* Dump an Xrecord                                                  */
    /********************************************************************/
    OdDbXrecordPtr pXRec = pObject;
    dumpXdata(pXRec->rbChain(), indent);
  }
  else if (pObject->isKindOf(OdDbDimAssoc::desc()))
  {
    /********************************************************************/
    /* Dump an Associative dimension                                    */
    /********************************************************************/
    dumpDimAssoc(pObject, indent);
  }
  else if (pObject->isKindOf(OdDbProxyObject::desc()))
  {
      dumpProxyObject(id, itemName, indent);
  }
  else
  {
    OdSmartPtr<OdDbObject_Dumper> pObjDumper = pObject->isA()->getX(OdDbObject_Dumper::desc());

    /***********************************************************************/
    /* Dump the object                                                    */
    /**********************************************************************/
    writeLine();
    if (!pObjDumper.isNull())
    {
      pObjDumper->dump(pObject, indent);
    }
  }
}

/************************************************************************/
/* Dump proxy object                                                    */
/************************************************************************/
void DbDumper::dumpProxyObject(
                          OdDbObjectId id, 
                          const OdString& itemName,
                          int indent)
{
    /**********************************************************************/
    /* Get a SmartPointer to the object                                   */
    /**********************************************************************/
    OdDbObjectPtr pObject = id.safeOpenObject();

    OdSmartPtr<OdDbProxyExt> proxyEntExt = pObject;

    writeLine(indent, OD_T("Proxy HANDLE:"), id.getHandle().ascii());
    writeLine(indent, OD_T("Proxy OriginalClassName"), toString(proxyEntExt->originalClassName(pObject)));
    writeLine(indent, OD_T("Proxy ApplicationDescription"), toString(proxyEntExt->applicationDescription(pObject)));
    writeLine(indent, OD_T("Proxy OriginalDxfName"), toString(proxyEntExt->originalDxfName(pObject)));
}

/************************************************************************/
/* Dump the database                                                    */
/************************************************************************/
void DbDumper::dump(OdDbDatabase* pDb, int indent)
{
  dumpSummaryInfo (pDb, indent);
  dumpHeader      (pDb, indent);
  dumpLayers      (pDb, indent);
  dumpLinetypes   (pDb, indent);
  dumpTextStyles  (pDb, indent);
  dumpDimStyles   (pDb, indent);
  dumpMLeaderStyles(pDb, indent);
  dumpRegApps     (pDb, indent);
  dumpViewports   (pDb, indent);
  dumpViews       (pDb, indent);
  dumpMLineStyles (pDb, indent);
  dumpUCSTable    (pDb, indent);
  dumpBlocks(pDb, indent);
  dumpObject(pDb->getNamedObjectsDictionaryId(), OD_T("Named Objects Dictionary"), indent);
}
