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

#include "DbDimension.h"
#include "DbCurve.h"
#include "DbSurface.h"

#include "Db3dSolid.h"
#include "DbBody.h"
#include "DbFace.h"
#include "DbHatch.h"
#include "DbMline.h"
#include "DbMPolygon.h"
#include "DbOle2Frame.h"
#include "DbPoint.h"
#include "DbPolyFaceMesh.h"
#include "DbPolygonMesh.h"
#include "DbRasterImage.h"
#include "DbRegion.h"
#include "DbSection.h"
#include "DbShape.h"
#include "DbSolid.h"
#include "DbSubDMesh.h"
#include "DbTable.h"
#include "DbTrace.h"
#include "DbUnderlayReference.h"
#include "DbViewport.h"

#include "DbArcAlignedText.h"
#include "DbAttribute.h" 
#include "DbAttributeDefinition.h" 
#include "DbBlockReference.h"
#include "DbFcf.h"
#include "DbMText.h"
#include "GiWorldDrawParser.h"
#include "StaticRxObject.h"

#include "toString.h"

/************************************************************************/
/* Construction/Destruction                                             */
/************************************************************************/

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbEntity_Parser, OdRxObject)

ExProtocolExtension::ExProtocolExtension()
{
}

ExProtocolExtension::~ExProtocolExtension()
{
  if(m_pParsers)
    uninitialize();
}

/************************************************************************/
/* Parse the common data and WorldDraw information for all              */
/* entities                                                             */
/************************************************************************/
class OdDbGeometry_Parser : public OdDbEntity_Parser
{
public:
  
  OdDbGeometry_Parser()
  {
    m_wd.setContext(&m_ctx);
  }

  void setIndent(int nInd)  {m_wd.setIndent(nInd);}
  void setDatabase(OdDbDatabase* pDb) {m_ctx.setDatabase(pDb);}


private:
  OdGiContextParser   m_ctx;
  OdGiWorldDrawParser m_wd;

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    writeLine();
    writeLine(indent++, toString(pEnt->isA()),toString(pEnt->getDbHandle()));
    writeLine(indent, L"(Geometry)");
   
    setIndent(indent);
    /**********************************************************************/
    /* Call worldDraw()                                                   */
    /**********************************************************************/
    pEnt->worldDraw(&m_wd);
  }
};

/************************************************************************/
/* Get text data                                                       */
/************************************************************************/
void getTextData(const OdDbText* pText, int indent)
{
  writeLine(indent, L"Text String",          toString(pText->textString()));
};


/************************************************************************/
/*          Dummy Parser                                                */
/************************************************************************/
class OdDbDummy_Parser : public OdDbEntity_Parser
{
public:
  void extractText(const OdDbEntity* pEnt, int indent) {}
};

/************************************************************************/
/* Arc Aligned Text Parser                                              */
/************************************************************************/
class OdDbArcAlignedText_Parser : public OdDbEntity_Parser
{
public:

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    writeLine();
    OdDbArcAlignedTextPtr pArcText = pEnt;
    writeLine(indent++, toString(pEnt->isA()), toString(pEnt->getDbHandle()));
    writeLine(indent, L"Text String",          toString(pArcText->textString()));  
  }
};

/************************************************************************/
/* Attribute Definition Parser                                          */
/************************************************************************/
class OdDbAttributeDefinition_Parser : public OdDbEntity_Parser
{
public:

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    writeLine();
    OdDbAttributeDefinitionPtr pAttdef = pEnt;
    writeLine(indent++, toString(pAttdef->isA()), toString(pEnt->getDbHandle()));
    writeLine(indent, L"Tag", toString(pAttdef->tag()));
    writeLine(indent, L"Prompt", toString(pAttdef->prompt()));
    getTextData(pAttdef, indent);
  }
};

/************************************************************************/
/* Block Reference Parser                                                */
/************************************************************************/
class OdDbBlockReference_Parser : public OdDbEntity_Parser
{
public:

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    OdDbBlockReferencePtr   pBlkRef = pEnt;
    OdDbBlockTableRecordPtr pRecord = pBlkRef->blockTableRecord().openObject();
    /**********************************************************************/
   /* Parse the attributes                                               */
   /**********************************************************************/
   OdDbObjectIteratorPtr pIter = pBlkRef->attributeIterator();
   for (int i=0; !pIter->done(); i++, pIter->step())
   {
     OdDbAttributePtr pAttr = pIter->entity();
     if (!pAttr.isNull())
     {
       writeLine();
       writeLine(indent++, toString(pAttr->isA()), toString(i));
       writeLine(indent, L"Tag", toString(pAttr->tag()));
       getTextData(pAttr, indent);
      }
    }
  }
};

/************************************************************************/
/* FCF Parser                                                           */
/************************************************************************/
class OdDbFcf_Parser : public OdDbEntity_Parser
{
public:

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    writeLine();
    OdDbFcfPtr pFcf = pEnt;
    writeLine(indent++, toString(pFcf->isA()), toString(pFcf->getDbHandle()));
    writeLine(indent, L"Text String",          toString(pFcf->text()));
  }
};

/************************************************************************/
/* MText Parser                                                         */
/************************************************************************/
class OdDbMText_Parser : public OdDbEntity_Parser
{
public:

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    writeLine();
    OdDbMTextPtr pMText = pEnt;
    writeLine(indent++, toString(pMText->isA()), toString(pMText->getDbHandle()));
    writeLine(indent, L"Text String",          toString(pMText->text())); // return text without formate codes
    // writeLine(indent, L"Text String",          toString(pMText->contents()));
   }
};


/************************************************************************/
/* Text Parser                                                          */
/************************************************************************/
class OdDbText_Parser : public OdDbEntity_Parser
{
public:

  void extractText(const OdDbEntity* pEnt, int indent)
  {
    OdDbTextPtr pText = pEnt;
    writeLine();
    writeLine(indent++, toString(pText->isA()),   toString(pText->getDbHandle()));
    getTextData(pText, indent);   
  }
};

class Parsers
{
  OdStaticRxObject<    OdDbDummy_Parser                   >    m_dummyParser;
  OdStaticRxObject<    OdDbGeometry_Parser                >    m_geometryParser;
  OdStaticRxObject<    OdDbArcAlignedText_Parser          >    m_arcAlignedTextParser;
  OdStaticRxObject<    OdDbAttributeDefinition_Parser     >    m_attributeDefinitionParser;
  OdStaticRxObject<    OdDbBlockReference_Parser          >    m_blockReference;
  OdStaticRxObject<    OdDbFcf_Parser                     >    m_fcfParser;
  OdStaticRxObject<    OdDbMText_Parser                   >    m_mTextParser;
  OdStaticRxObject<    OdDbText_Parser                    >    m_textParser;

 
public:
  /**********************************************************************/
  /* Add Protocol Extensions in constructor                             */
  /**********************************************************************/
  Parsers()
  {
    // Parser using worldDraw() is for base OdDbEntity class
    OdDbEntity                 ::desc()->addX(OdDbEntity_Parser::desc(),   &m_geometryParser);

    // Specific parsers for classes containing text
    OdDbArcAlignedText         ::desc()->addX(OdDbEntity_Parser::desc(),   &m_arcAlignedTextParser);
    OdDbAttributeDefinition    ::desc()->addX(OdDbEntity_Parser::desc(),   &m_attributeDefinitionParser);
    OdDbBlockReference         ::desc()->addX(OdDbEntity_Parser::desc(),   &m_blockReference);
    OdDbFcf                    ::desc()->addX(OdDbEntity_Parser::desc(),   &m_fcfParser);
    OdDbMText                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_mTextParser);
    OdDbText                   ::desc()->addX(OdDbEntity_Parser::desc(),   &m_textParser);

    // Dummy parsers for classes which can't contain text
    OdDbDimension              ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbCurve                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbSurface                ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);

    OdDb3dSolid                ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbBody                   ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbFace                   ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbHatch                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbMline                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbMPolygon               ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbOle2Frame              ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbPoint                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbPolyFaceMesh           ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbPolygonMesh            ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbRasterImage            ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbRegion                 ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbSection                ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbShape                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbSolid                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbSubDMesh               ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbTable                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser); // Table block *T will be parsed
    OdDbTrace                  ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbUnderlayReference      ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
    OdDbViewport               ::desc()->addX(OdDbEntity_Parser::desc(),   &m_dummyParser);
  }



  /**********************************************************************/
  /* Delete Protocol Extensions in destructor                           */
  /**********************************************************************/
 ~Parsers()
  {
    OdDbDimension              ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbCurve                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbSurface                ::desc()->delX(OdDbEntity_Parser::desc());

    OdDb3dSolid                ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbBody                   ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbFace                   ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbHatch                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbMline                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbMPolygon               ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbOle2Frame              ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbPoint                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbPolyFaceMesh           ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbPolygonMesh            ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbRasterImage            ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbRegion                 ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbSection                ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbShape                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbSolid                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbSubDMesh               ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbTable                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbTrace                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbUnderlayReference      ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbViewport               ::desc()->delX(OdDbEntity_Parser::desc());

    OdDbText                   ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbMText                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbFcf                    ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbBlockReference         ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbAttributeDefinition    ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbArcAlignedText         ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbCurve                  ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbDimension              ::desc()->delX(OdDbEntity_Parser::desc());
    OdDbEntity                 ::desc()->delX(OdDbEntity_Parser::desc());
  }

  void setDatabase(OdDbDatabase* pDb)
  {
    m_geometryParser.setDatabase(pDb);
  }
};

/************************************************************************/
/* Initialize protocol extensions                                       */
/************************************************************************/
void ExProtocolExtension::initialize()
{
  // Register OdDbEntity_Dumper with the Drawings SDK
  OdDbEntity_Parser::rxInit();
  m_pParsers = new Parsers;
//  m_pParsers->addXs();
}

/************************************************************************/
/* Uninitialize protocol extensions                                     */
/************************************************************************/
void ExProtocolExtension::uninitialize()
{
//  m_pParsers->delXs();
  delete m_pParsers;
  m_pParsers = 0;
  OdDbEntity_Parser::rxUninit();
}

void ExProtocolExtension::setDatabase(OdDbDatabase* pDb)
{
  m_pParsers->setDatabase(pDb);
}
