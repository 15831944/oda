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


///////////////////////////////////////////////////////////////////////////////
//
// PdfPublishImage.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_IMAGE_INCLUDED_
#define _PDF_PUBLISH_IMAGE_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "Gi/GiRasterImage.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
   This class implements an image entity in .pdf documents created with Publish SDK.
*/
class PDFPUBLISH_TOOLKIT OdImage : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdImage();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdImage);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys an image object.
  */
  virtual ~OdImage();

  /** \details 
    Sets the full filename of the image.
    Use IsValid() to check the status of the operation.
   
    \param filename [in] A new filename value.
  */
  void setFile(const OdString& filename);

  /** \details 
    Sets a raster image object as the source for the image.
   
    \param pRasterImage [in] A smart pointer to a raster image object.
  */
  void setImage(const OdGiRasterImagePtr& pRasterImage);

  /** \details 
    Sets the image size in pixels.
   
    \param width  [in] A width value in pixels.
    \param height [in] A height value in pixels.
  */
  void setSize(const OdInt32 width, const OdInt32 height);

  /** \details 
    Returns the current image full filename.
    The method fills a passed string object and returns it to the calling subroutine.
   
    \param out_filename [out] A placeholder for the full filename.
  */
  void getFile(OdString& out_filename) const;

  /** \details 
    Returns the current raster image object.
    The method fills a passed raster image object and returns it to the calling subroutine.
   
    \param pOutRasterImage [out] A placeholder for the raster image object.
  */  
  void getImage(OdGiRasterImagePtr& pOutRasterImage) const;

  /** \details 
    Returns the current image size (width and height in pixels).
    The method fills the passed integer values representing width and height of the image and returns them to the calling subroutine.
   
    \param width  [out] A placeholder for the width value.
    \param height [out] A placeholder for the height value.
  */
  void getSize(OdInt32& width, OdInt32& height) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdImage, OdImage> object.
*/
SMARTPTR(OdImage);

}
#endif // _PDF_PUBLISH_IMAGE_INCLUDED_

