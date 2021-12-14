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
/*
  Project : Dump for OdDg
  Created  : 09/13/2007
*/

#pragma once

#include "OdaCommon.h"
#include "Gi/GiDrawable.h"
#include "DgDatabase.h"
#include "DgReferenceAttach.h"


extern FILE* DumpStream;     //the file for output to


/************************************************************************/
/* This class is the base class for dumpers of all objects.             */
/*                                                                      */
/* Every object class has this extension.                               */
/* Also, dumper is available (OdDgDumper) for all such dumpers.         */
/************************************************************************/


class OdDgObject_Dumper : public OdRxObject
{

  ODRX_DECLARE_MEMBERS( OdDgObject_Dumper );
public:

  OdDgObject_Dumper();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;
  
  virtual void      dump( OdRxObjectPtr object ) = 0; //each object class has to describe itself via that method
  virtual OdString  getClassName() = 0;               //each object class has to define its own name

  //some useful methods for the other dump()'s
  void startDescription( OdRxObjectPtr object );
  void finishDescription();
  void describeSubElements( OdDgElementIteratorPtr iterator );
  void writeShift();
  void writeGraphicsElementInfo( OdDgGraphicsElementPtr element );

  //output for different types
  void writeFieldValue( const OdString & name, OdString const& value );
  void writeFieldValue( const OdString & name, OdUInt16 value );
  void writeFieldValueHex( const OdString & name, OdUInt16 value );
  void writeFieldValue( const OdString & name, OdUInt32 value );
  void writeFieldValueHex( const OdString & name, OdUInt32 value );
  void writeFieldValue( const OdString & name, double value );
  void writeFieldValue( const OdString & name, OdGePoint2d const& value );
  void writeFieldValue( const OdString & name, OdGePoint3d const& value );
  void writeFieldValue( const OdString & name, OdGeVector3d const& value );
  void writeFieldValue( const OdString & name, OdUInt64 value );
  void writeFieldValue( const OdString & name, OdUInt8 value );
  void writeFieldValueHex( const OdString & name, OdUInt8 value );
  void writeFieldValue( const OdString & name, bool value );
  void writeFieldValue( const OdString & name, OdInt16 value );
  void writeFieldValue( const OdString & name, OdInt32 value );
#if !(defined(ODA_UNIXOS) && (OD_SIZEOF_LONG == 8))
  void writeFieldValue( const OdString & name, int value );
#endif
  void writeFieldValue( const OdString & name, OdInt64 value );
  void writeFieldValue( const OdString & name, OdGeMatrix3d const& value );
 

  OdRxObjectPtr getObject(){ return m_object; }
  //it is common for all dumpers
  static int                  m_nesting;
private:

  OdRxObjectPtr m_object;
  //methods used only by this class
  void writeFieldName( const OdString& fieldName );

  long        m_references;     //used by each object separately

  //it is common for all dumpers
  static OdArray< OdString >  m_nestedTypes;

};


/************************************************************************/
/* Dumpers for all objects                                              */
/************************************************************************/


class OdDgDatabase_Dumper : public OdDgObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

private:
  void dumpSummaryInfo( OdDgDatabase* pDb );
};


class OdDgModel_Dumper : public OdDgObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

class OdDgReferenceHeader_Dumper : public OdDgObject_Dumper
{

public:

  virtual void      dump( OdRxObjectPtr object );
  virtual OdString  getClassName();

};

/************************************************************************/
/* This manages all the dumpers                                         */
/************************************************************************/
class OdDgDumper
{

public:

  OdDgDumper();
  virtual ~OdDgDumper();

  void rootDump( OdRxObjectPtr database ) const;  //root dumper

private:

  OdDgDatabase_Dumper                                 m_databaseDumper;
  OdDgModel_Dumper                                    m_modelDumper;
  OdDgReferenceHeader_Dumper                          m_referenceAttachmentDumper;

};
