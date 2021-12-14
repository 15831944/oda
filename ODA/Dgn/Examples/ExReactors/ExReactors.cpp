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
/* This console application reads a DGN file and modifies elements      */
/* to demonstrate using of database reactors.                           */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/* ExReactors <source filename>                                         */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"

#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "RxDynamicModule.h"
#include "DgDatabase.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "ExDgElementReactor.h"
#include "ExDgDatabaseReactor.h"
#include "DgLine.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

// Auxiliary function to display different messages
//void oddgPrintConsoleString(const wchar_t* fmt, ...);

/************************************************************************/
/* Auxiliary class to display reactor messages                          */
/************************************************************************/
template <class TReactor>
class OdExReactorMsg : public TReactor
{
public:
  // TReactor overridden
  virtual void printMessage(const OdString& msg) const
  { 
    oddgPrintConsoleString(msg);
  }
};


/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* OdExDgnSystemServices and OdExDgnHostAppServices                     */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};


#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
ODRX_END_STATIC_MODULE_MAP()
#endif


/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if ( argc < 2 )
  {
    oddgPrintConsoleString(L"\nExReactors sample program. Copyright (c) 2019, Open Design Alliance\n");
    oddgPrintConsoleString(L"\tusage: ExReactors <source file>\n");
    oddgPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }
  OdString srcFileName = argv[1];

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Create a Services object                                           */
  /**********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  odrxInitialize(&svcs);

  try 
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /********************************************************************/
    /* Read the DGN file into the OdDgDatabase object                   */
    /********************************************************************/
    OdDgDatabasePtr pDb = svcs.readFile( srcFileName );

    /********************************************************************/
    /* Add a reactor to the database                                    */
    /********************************************************************/
    OdStaticRxObject<OdExReactorMsg<OdExDgDatabaseReactor> > dbReactor;
    pDb->addReactor(&dbReactor);

    /********************************************************************/
    /* Add reactor to the graphics elements                             */
    /********************************************************************/
    OdStaticRxObject<OdExReactorMsg<OdExDgElementReactor> > elmReactor;
    OdDgElementId elementId;
    {
      OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForRead);
      pModel->addReactor(&elmReactor);
      OdDgElementIteratorPtr pIterElem = pModel->createGraphicsElementsIterator();
      if ( !pIterElem->done() )
      {
        elementId = pIterElem->item();
        OdDgGraphicsElementPtr pElm = elementId.safeOpenObject(OdDg::kForRead);
        pElm->addReactor(&elmReactor);
      }
    }

    /********************************************************************/
    /* Modify graphics elements.                                        */
    /********************************************************************/
    // modify property
    {
      OdDgGraphicsElementPtr pElm = elementId.safeOpenObject(OdDg::kForWrite);
      pElm->setColorIndex(pElm->getColorIndex() & 0xff ^ 0xff);
    }
    oddgPrintConsoleString(L"\n");
    // erase
    {
      OdDgGraphicsElementPtr pElm = elementId.safeOpenObject(OdDg::kForWrite);
      pElm->erase();
    }
    oddgPrintConsoleString(L"\n");
    // unerase
    {
      OdDgGraphicsElementPtr pElm = elementId.safeOpenObject(OdDg::kForWrite, true);
      pElm->erase(false);
    }
    oddgPrintConsoleString(L"\n");
    // append a new element to model
    {
      OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
      if ( pModel->getModelIs3dFlag() )
      {
        OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
        pLine3d->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
        pLine3d->setEndPoint( OdGePoint3d( 10., 10., 10. ) );
        pModel->addElement(pLine3d);
      }
      else
      {
        OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
        pLine2d->setStartPoint( OdGePoint2d( 0., 0. ) );
        pLine2d->setEndPoint( OdGePoint2d( 10., 10. ) );
        pModel->addElement(pLine2d);
      }
      pModel = 0;
    }

    /********************************************************************/
    /* Remove reactor from the database                                 */
    /********************************************************************/
    pDb->removeReactor(&dbReactor);

    /********************************************************************/
    /* Remove reactor from the graphics elements                        */
    /********************************************************************/
    {
      OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForRead);
      pModel->removeReactor(&elmReactor);
      OdDgElementIteratorPtr pIterElem = pModel->createGraphicsElementsIterator();
      for ( ; !pIterElem->done(); pIterElem->step() )
      {
        OdDgElementPtr pElm = pIterElem->item().openObject(OdDg::kForRead);
        if ( pElm.get() )
        {
          pElm->removeReactor(&elmReactor);
        }
      }
    }
  }
  /********************************************************************/
  /* Display the error                                                */
  /********************************************************************/
  catch (OdError& e)
  {
    oddgPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    oddgPrintConsoleString(L"\nUnknown Error.\n");
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  odrxUninitialize();

  return 0;
}
