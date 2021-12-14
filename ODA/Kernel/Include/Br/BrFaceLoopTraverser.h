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



#ifndef _INC_DDBRFACELOOPTRAVERSER_3F82E16B0109_INCLUDED
#define _INC_DDBRFACELOOPTRAVERSER_3F82E16B0109_INCLUDED

#include "Br/BrFace.h"
#include "Br/BrLoop.h"
#include "Br/BrTraverser.h"

class OdBrShellFaceTraverser;

#include "TD_PackPush.h"

/** \details
    This class is the interface class for face loop traversers.

    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrFaceLoopTraverser : public OdBrTraverser
{
public:
  /** \details
    Default constructor.
  */
  OdBrFaceLoopTraverser();
#ifdef OD_ADD_MOVE_CTOR_HAVE_DEF_DEL_FUNC
  //DOM-IGNORE-BEGIN
  OdBrFaceLoopTraverser(const OdBrFaceLoopTraverser&) = default;
  OdBrFaceLoopTraverser& operator=(const OdBrFaceLoopTraverser&) = default;
  OdBrFaceLoopTraverser(OdBrFaceLoopTraverser&&) = default;
  OdBrFaceLoopTraverser& operator=(OdBrFaceLoopTraverser&&) = default;
  //DOM-IGNORE-END
#endif

  /** \details
    Sets this Traverser object to a specific face loop list.
    \param face [in]  Owner of the loop list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setFace( const OdBrFace& face );

  /** \details
    Sets this Traverser object to a specific face loop list.
    \param shellFaceTrav [in]  Shell-face traverser.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setFace(const OdBrShellFaceTraverser& shellFaceTrav);

  /** \details
    Sets the starting position of this Traverser object.
    \param loop [in]  Defines the starting position.
  */
  OdBrErrorStatus setLoop(const OdBrLoop& loop);

  /** \details
    Sets this Traverser object to a specific face loop list
    and starting position.
    \param loop [in]  Defines the face loop list and starting position.
  */
  OdBrErrorStatus setFaceAndLoop(const OdBrLoop& loop);

  /** \details
    Returns the loop object at the current Traverser position.
  */
  OdBrLoop getLoop() const;

 /** \details
    Returns the owner of the loop list associated with this Traverser object.
 */
  OdBrFace getFace() const;
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRFACELOOPTRAVERSER_3F82E16B0109_INCLUDED */

