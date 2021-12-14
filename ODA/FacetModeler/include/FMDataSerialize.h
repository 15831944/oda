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
#ifndef _FM_DATA_SERIALIZE_
#define _FM_DATA_SERIALIZE_

#include <OdaCommon.h>
#include "OdPlatformSettings.h"
#include "Modeler/FMMdlBody.h"
#include "Modeler/FMMdlVertex.h"
#include <map>

namespace FacetModeler
{
  /** Key type. */
  typedef OdUInt64 KeyType;

  /** \details
     Represents a vertex entity to save.
  */
  struct VertexImage
  {
    /** \details 
      Constructor.
    */
    VertexImage() : pVtx(NULL), pPrev(0), pNext(0), pKey(0)
    {}
    Vertex* pVtx;  // Vertex pointer.
    KeyType pKey;  // Key.
    KeyType pPrev; // Previous key.
    KeyType pNext; // Next key.
  };

  /** \details
     Represents an edge entity to save.
  */
  struct EdgeImage
  {
    /** \details 
      Constructor.
    */
    EdgeImage() : pEdge(NULL), pFaceKey(0), pLoopKey(0),
      pVertexKey(0), pPrevKey(0), pNextKey(0), pPairKey(0), pPartnerKey(0)
    {}

    Edge* pEdge; // Edge pointer.
    KeyType pFaceKey; // Face key.
    KeyType pLoopKey; // Loop key.
    KeyType pVertexKey; // Vertex key.
    KeyType pPrevKey; // Previous key.
    KeyType pNextKey; // Next key.
    KeyType pPairKey; // Pair key.
    KeyType pPartnerKey; // Partner key.
  };

  /** \details
     Represents a loop entity to save.
  */
  struct LoopImage
  {
    /** \details 
      Constructor.
    */
    LoopImage() : pLoopKey(0)
    {}

    KeyType pLoopKey; // Loop key.
    std::vector<KeyType> edgeKeys; // Edge key array.
  };

  /** Array of loop image entities.*/
  typedef std::vector<LoopImage> LoopImages;

  /** \details
     Represents a face entity to save.
  */
  struct FaceImage
  {
    /** \details 
      Constructor.
    */
    FaceImage() : pFace(NULL),pFaceKey(0),  pPrevKey(0), pNextKey(0)
    {}

    Face* pFace; // Face pointer.
    KeyType pFaceKey; // Face key.
    KeyType pPrevKey; // Previous key.
    KeyType pNextKey; // Next key.

    LoopImages loopImages; // Loop images.
  };

  /** Array of vertex image entities.*/
  typedef std::vector<VertexImage> VertexImages;

  /** Map of pointers and edge image entities.*/
  typedef std::map<KeyType, EdgeImage> EdgeImages;

  /** Array of face image entities.*/
  typedef std::vector<FaceImage> FaceImages;

  /** \details
     Provides functionality for reading and writing binary data to a file.
  */
  class FMGEOMETRY_API BinaryStream
  {
  public:
    
    /** \details
      Defines types of object markers.
    */
    enum OdaObjectMarker
    {
      /** OdGePoint2d marker. */
      ePoint2dMarker = 1,

      /** Contour2D marker. */
      eContour2dMarker = 2,

      /** Profile2D marker. */
      eProfile2dMarker = 3,

      /** Body marker. */
      eBodyMarker = 4,

      /** Face marker. */
      eFaceMarker = 5,

      /** OdGePoint3d marker. */
      ePoint3dMarker = 6
    };

    /** \details
      Defines Facet Modeler file or binary stream format versions.
    */
    enum Version
    {
      /** Force old-style 64-bit. */
      v64bit = -2,

      /** Force old-style 32-bit. */
      v32bit = -1,

      /** Version from the file. */
      vAuto = 0,

      /** Version where saving unified for different bitness. */
      vKeyTypeAdded = 1,

      /** Version where added saving of Face color fields. */
      vSaveColorsAdded = 2,

      /** Current version. */
      vCurrent = 2,
    };

    static const OdUInt32 iSigLen = 4; // Signature length in bytes
    static const OdUInt32 iVerLen = 2; // Version length in bytes

    /** \details
       Default constructor.
    */
    BinaryStream();
    
    /** \details 
      Destructor.
    */
    ~BinaryStream();

    /** \details
       Opens a file from the file system for writing.
      
       \param szFile [in] Filename string.
       \returns true if the file is opened successfully, or false otherwise.
    */
    bool Create(const OdString& szFile);

    /** \details
       Opens a file from the file system for reading in the specified format version.

       \param szFile [in] Filename string.
       \param ver    [in] File version to read.
       \returns true if the file is opened successfully, or false otherwise.
    */
    bool Open(const OdString& szFile, Version ver = vAuto);

    
    /** \details
       Opens a stream buffer for reading in the specified format version.
      
       \param pFileBuff [in] Pointer to the OdStreamBuf object.
       \param ver       [in] Format version to use for reading.
       \returns true if the stream buffer is opened successfully, or false otherwise.
    */
    bool Open(OdStreamBuf* pFileBuff, Version ver = vAuto);

    /** \details
       Opens a stream buffer for writing a binary stream to.

       \param pFileBuff [in] Pointer to the OdStreamBuf object.
       \returns true if the stream buffer is opened successfully, or false otherwise.
    */
    bool Create(OdStreamBuf* pFileBuff);

    /** \details
       Resets the stream of a file pointer to the start and sets a format version.
      
       \param ver [in] Format version.
       \returns true if the stream buffer is reset successfully, or false otherwise.
    */
    bool Reset(Version ver = vAuto);

    /** \details 
      Gets version of this stream object.
    
      \returns Current stream version from Version enum.
    */
    inline Version getVersion() const
    {
      return m_iVersion;
    };

    /** \details
       Writes the specified number of bytes of data.
      
       \param pData  [in] Pointer to data to write.
       \param length [in] Count of bytes to write.
    */
    void Write(const void* pData, OdUInt32 length);

    /** \details
       Reads the specified number of bytes.
      
       \param pData [out] Pointer to the read data.
       \param length [in] Number of bytes to read.
    */
    void Read(void* pData, OdUInt32 length);

    /** \details
       Writes data with a defined size.
      
       \param val [in] Data to write.
    */
    template<typename T>
    void Write(const T val)
    {
      Write(&val, sizeof(val));
    }

    /** \details
       Writes the 2D point.
      
       \param val [in] OdGePoint2d object to write.
    */
    void Write(const OdGePoint2d& val);
    
	/** \details
       Writes the contour.
      
       \param val [in] Contour2D object to write.
    */
    void Write(const Contour2D& val);
    
	/** \details
       Writes the profile.
      
       \param val [in] Profile2D object to write.
    */
    void Write(const Profile2D& val);

    /** \details
       Writes the 3D point.
      
       \param val [in] OdGePoint3d object to write.
    */
    void Write(const OdGePoint3d& val);
    
	/** \details
       Writes the face.
      
       \param val [in] Face object to write.
    */
    void Write(const Face& val);
    
	/** \details
       Writes the body.
      
       \param val [in] Body object to write.
    */
    void Write(const Body& val);

    /** \details
       Writes the specified number of vertices from the vertex list.
      
       \param pVertexList [in] Vertex list pointer.
       \param uVtxCount   [in] Vertices number to write.
    */
    void WriteVertexList(Vertex* pVertexList, OdUInt32 uVtxCount);
    
	/** \details
       Writes the vertex.
      
       \param pVertex [in] Pointer to vertex to write.
    */
    void WriteVertex(Vertex* pVertex);
    
	/** \details
       Writes the body edges list.
      
       \param val [in] Body to write.
    */
    void WriteEdgesList(const Body& val);
    
	/** \details
       Writes the edge.
      
       \param pEdge [in] Edge to write.
    */
    void WriteEdge(Edge* pEdge);

    /** \details
       Reads an object.
      
       \param val [out] Read object.
    */
    template<typename T>
    void Read(T& val)
    {
      Read(&val, sizeof(val));
    }

    /** \details
       Reads a point object.
      
       \param val [out] Read OdGePoint2d object.
    */
    void Read(OdGePoint2d& val);
    
	/** \details
       Reads a contour object.
      
       \param val [out] Read Contour2D object.
    */
    void Read(Contour2D& val);
    /** \details
       Reads a profile object.
      
       \param val [out] Read Profile2d object.
    */
    void Read(Profile2D& val);

    /** \details
       Reads a point.
      
       \param val [out] Read OdGePoint3d object.
    */
    void Read(OdGePoint3d& val);

    /** \details
       Reads a Face object to the specified body.
      
       \param val [in/out] Body object to assign the face to.
    */
    void ReadFaceToBody(Body& val);

    /** \details
       Reads a Body object.
      
       \param val [out] Read Body object.
    */
    void Read(Body& val);

    /** \details
       Reads a KeyType object.
      
       \param val [out] Read KeyType value.
    */
    void ReadKey(KeyType& val);

    /** \details
       Reads a VertexImages array from the specified body.
      
       \param vertexImages [out] VertexImages output array.
       \param pBody         [in] Body to read from.
    */
    void ReadVertexImages(VertexImages& vertexImages, Body* pBody);

    /** \details
       Sets a vertex list for the specified body using a VertexImages array.
      
       \param vertexImages [in] VertexImages array to set.
       \param val      [in/out] Body to set.
    */
    void SetVertexListForBody(VertexImages& vertexImages, Body& val);

    /** \details
       Reads an EdgeImages array.
      
       \param edgeImages [out] Read EdgeImages array.
    */
    void ReadEdgeImages(EdgeImages& edgeImages);

    /** \details
       Reads a FaceImages array.
      
       \param faceImages [out] Read FaceImages array.
    */
    void ReadFaceImages(FaceImages& faceImages);
  private:
    OdStreamBufPtr m_pStream; // Pointer to the stream buffer
    Version m_iVersion;       // Current version
  };

} // ::
#endif //_FM_DATA_SERIALIZE_
