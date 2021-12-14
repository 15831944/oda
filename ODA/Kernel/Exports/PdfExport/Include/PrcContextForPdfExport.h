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


#ifndef _PRC_CONTEXT_FOR_PDF_EXPORT_INCLUDED_
#define _PRC_CONTEXT_FOR_PDF_EXPORT_INCLUDED_

#include "Gi/GiDrawableImpl.h"
#include "Gs/Gs.h"
#include "PdfExportDef.h"
#include "Gi/GiCommonDraw.h"
#include "RxObject.h"
#include "RxDictionary.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/** \details 
A class that implements PRC context for exporting to PDF format.

<group OdExport_Classes>
*/
class PDFEXPORT_DLL OdPrcContextForPdfExport : public OdRxObject
{
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdPrcContextForPdfExport);
//DOM-IGNORE-END

private:
  /** \details
  User-defined data container that can be used in implementations of the shouldExportAsPRC() method.
  */
  OdRxObjectPtr m_userData;

  /** 
  The flag that determines whether the writing to a .pdf file is needed.
  The flag value is equal to false if it is needed to get only a PRC stream as the export result;  
  otherwise, its value is equal to true. In this case, an output .pdf file is created.
  */
  bool m_writePdfFile;

public:

  /** \details 
  Creates a new context object for the export to the .pdf format with PRC support.
  \remarks
  By default, a .pdf file is created as a result of the export.
  */
  OdPrcContextForPdfExport() :
    m_writePdfFile(true)
  {};
  
  /** \details
  Determines whether the entity should be exported to a PDF document as a PRC object.
  \param pDrawable   [in] A raw pointer to an entity that is checked whether it can/should be exported as PRC object or not.
  \param entity      [in] A raw pointer to a path node of the entity.
  \param pView       [in] A raw pointer to an object that contains information about the GS client view object.
  \param viewportidx [out] A placeholder for the index of the viewport (with the PRC content) in the PDF document where the entity should be exported to.
  \returns true if the entity should be exported to PDF document as a PRC object; otherwise, the method returns false.
  \remarks 
  The default implementation returns false.
  */
  virtual bool shouldExportAsPRC(
    const OdGiDrawable *pDrawable,
    const OdGiPathNode *entity,
    const OdGsClientViewInfo *pView,
    OdUInt32 &viewportidx) const
  {
    return false;
  }
  /** \details
  Gets the extents of a specified entity.
  \param pDrawable        [in] A raw pointer to the drawable entity whose extents should be returned.
  \param viewportObjectId [in] A raw pointer to an OdDbStub object that represents the identifier of the viewport in which the entity should be drawn.
  \param pGiContext       [in] A raw pointer to an OdGiContext object associated with the context object.
  \param cameraView       [in] A camera transformation matrix.
  \param modelToWorld     [in] A model to world transformation matrix.
  \param extents          [out] A placeholder for the extents of the entity.
  \remarks 
  The method fills the passed OdGeExtents3d object with extents information and returns it to a calling subroutine.
  */
  virtual void getExtents(
    const OdGiDrawable * pDrawable,
    OdDbStub * viewportObjectId,
    OdGiContext * pGiContext,
    const OdGeMatrix3d & cameraView,
    const OdGeMatrix3d & modelToWorld,
    OdGeExtents3d & extents) const;

  /** \details
  Retrieves the user data from the context object.
  \returns
  Returns A smart pointer to an OdRxObject object that contains user data.
  */
  virtual OdRxObjectPtr& userData(void) { return m_userData; }

  /** \details
  Sets a new user data for the context object.
  \param userData [in] A user-defined data container to be set.
  */
  virtual void setUserData (const OdRxObjectPtr &userData) { m_userData = userData; }

  /** \details
  Retrieves the current value of the flag that determines whether the writing to a .pdf file is needed.
  \returns
  The current value of the file writing flag.
  \remarks 
  The flag value is equal to false if it is needed to get only a PRC stream as the export result;  
  otherwise, its value is equal to true. In this case, an output .pdf file is created.
  */
  virtual bool getWritePdfFile() const { return m_writePdfFile; }

  /** \details
  Sets a new value of the flag that determines whether the writing to a .pdf file is needed.
  \param flag [in] A new flag value to be set.
  \remarks 
  The flag value is equal to false if it is needed to get only a PRC stream as the export result;  
  otherwise, its value is equal to true. In this case, an output .pdf file is created.
  */
  virtual void setWritePdfFile(bool flag) { m_writePdfFile = flag; }

protected:
  /** \details
  Gets the entity's extents.
  \param pDrawable        [in] A raw pointer to the entity whose extents are retrieved.
  \param viewportObjectId [in] An identifier of the viewport that is used for the entity rendering.
  \param pGiContext       [in] A raw pointer to an OdGiContext instance associated with the object.
  \param cameraView       [in] A camera transformation matrix.
  \param modelToWorld     [in] A model to world transformation matrix.
  \param extents          [out] A placeholder for the extents of the entity.
  \param regenType        [in] The regeneration type for the viewport.
  */
  void getExtentsWithParams(
    const OdGiDrawable * pDrawable,
    OdDbStub * viewportObjectId,
    OdGiContext * pGiContext,
    const OdGeMatrix3d & cameraView,
    const OdGeMatrix3d & modelToWorld,
    OdGeExtents3d & extents,
    OdGiRegenType regenType) const;
};

/** \details 
A data type that represents a smart pointer to an OdPrcContextForPdfExport object.
*/
typedef OdSmartPtr<OdPrcContextForPdfExport> OdPrcContextForPdfExportPtr;

/** \details
A data type that represents a map of stream buffers.
*/
typedef std::map<OdUInt32, OdStreamBufPtr> PRCStreamsMap;

/** \details
A wrapper class for PRC context used within PDF export functionality.

<group OdExport_Classes>
*/
class PDFEXPORT_DLL OdPrcContextForPdfExportWrapper : public OdPrcContextForPdfExport
{
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdPrcContextForPdfExportWrapper);
//DOM-IGNORE-END

  private:
    /** \details
    A smart pointer to the export context object.
    */
    OdPrcContextForPdfExportPtr m_userContext;
    
    /** \details
    A map that contains a collection of pairs &quot;32-bit unsigned integer key - smart pointer to an OdStreamBuf object&quot;.
    This collection represents PRC streams used for the export.
    */
    PRCStreamsMap m_outStreamsPRC;

  public:

    /** \details 
    Retrieves the current map of output PRC data streams.
    \returns A PRCStreamsMap object that contains a collection of pairs &quot;32-bit unsigned integer key - smart pointer to an OdStreamBuf object&quot;.
    */
    PRCStreamsMap &getOutputPRC()
    {
      return m_outStreamsPRC;
    }

    /** \details 
    Sets a new map of output PRC data streams.
    \param streamsPRC [in] A new map to be set.
    */
    void setOutputPRC(const PRCStreamsMap &streamsPRC)
    {
      m_outStreamsPRC = streamsPRC;
    }

    /** \details
    Retrieves the current user PRC context for PDF export.
    \returns A smart pointer to an OdPrcContextForPdfExport object.
    */
    const OdPrcContextForPdfExportPtr &getUserContext() const
    {
      return m_userContext;
    }

    /** \details
    Sets a user PRC context for PDF export.
    \param userContext [in] A new context object to be set.
    */
    void setUserContext(const OdPrcContextForPdfExportPtr &userContext)
    {
      m_userContext = userContext;
    }

    /** \details
    Determines whether the entity should be exported to a PDF document as a PRC object.
    \param pDrawable   [in] A raw pointer to an entity that is checked whether it can/should be exported as PRC object or not.
    \param entity      [in] A raw pointer to a path node of the entity.
    \param pView       [in] A raw pointer to an object that contains information about the GS client view object.
    \param viewportidx [out] A placeholder for the index of the viewport (with the PRC content) in the PDF document where the entity should be exported to.
    \returns true if the entity should be exported to PDF document as a PRC object; otherwise, the method returns false.
    \remarks
    The default implementation returns false.
    */
    virtual bool shouldExportAsPRC (const OdGiDrawable *pDrawable,
      const OdGiPathNode *entity,
      const OdGsClientViewInfo *pView,
      OdUInt32 &viewportidx) const
    {
      return m_userContext.isNull() ? OdPrcContextForPdfExport::shouldExportAsPRC(pDrawable, entity, pView, viewportidx) : m_userContext->shouldExportAsPRC(pDrawable, entity, pView, viewportidx);
    }

    /** \details
    Retrieves the user data from the context object.
    \returns
    Returns A smart pointer to an OdRxObject object that contains user data.
    */
    virtual OdRxObjectPtr& userData(void)
    {
      return m_userContext.isNull() ? OdPrcContextForPdfExport::userData() : m_userContext->userData();
    }

    /** \details
    Sets a new user data for the context object.
    \param userData [in] A user-defined data container to be set.
    */
    virtual void setUserData(const OdRxObjectPtr &userData)
    {
      m_userContext.isNull() ? OdPrcContextForPdfExport::setUserData(userData) : m_userContext->setUserData(userData);
    }

    /** \details
    Retrieves the current value of the flag that determines whether the writing to a .pdf file is needed.
    \returns
    The current value of the file writing flag.
    \remarks 
    The flag value is equal to false if it is needed to get only a PRC stream as the export result;  
    otherwise, its value is equal to true. In this case, an output .pdf file is created.
    */
    virtual bool getWritePdfFile() const
    {
      return m_userContext.isNull() ? OdPrcContextForPdfExport::getWritePdfFile() : m_userContext->getWritePdfFile();
    }
};

/** \details
A data type that represents a smart pointer to an OdPrcContextForPdfExportWrapper object.
*/
typedef OdSmartPtr<OdPrcContextForPdfExportWrapper> OdPrcContextForPdfExportWrapperPtr;

/** \details 
Creates base PRC context for &quot;all in single view&quot; mode.
\returns A smart pointer to an OdRxObject instance that represents the context object.
*/
inline OdRxObjectPtr odCreatePrcAllInSingleViewContextBase()
{
  OdRxObjectPtr pObj;
  OdRxClassPtr pClass = odrxClassDictionary()->getAt("OdPrcContextForPdfExport_AllInSingleView_Base");
  if (!pClass.isNull())
  {
    pObj = pClass->create();
  }
  return pObj;
}
#endif
