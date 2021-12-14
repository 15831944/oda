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
/* This console application reads a PC3 file and writes it              */
/* to an XML file.                                                      */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdPC3Read <file.pc3> <file.xml>                                   */
/*                                                                      */
/************************************************************************/
#include "Pc3Parser.h"
#include <zlib.h>
#include <iostream>
#include <fstream>
#include <strstream>


#ifdef OD_HAVE_MEM_H_FILE
#include <mem.h>  // for memset
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

const int PC3_HEADER_LENGTH = 52;

int main(int argc, char* argv[])
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand( &argv );
#endif

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if ( argc < 3 ) 
  {
    std::cout << "usage: OdPC3Read <file.pc3> <file.xml>" << std::endl;
    return 1;
  }

  /**********************************************************************/
  /* Open the input file                                                */
  /**********************************************************************/
  std::ifstream inFile( argv[1], std::ios::in | std::ios::binary );
  
  /**********************************************************************/
  /* Skip the header                                                    */
  /**********************************************************************/
  inFile.seekg( PC3_HEADER_LENGTH );
  
  /**********************************************************************/
  /* Read the decompressed stream size                                  */
  /**********************************************************************/
  unsigned int nUncompSize; 
  inFile.read( reinterpret_cast<char*>(&nUncompSize), sizeof( nUncompSize ) );
  
  /**********************************************************************/
  /* Read the compressed stream size                                    */
  /**********************************************************************/
  unsigned int nCompSize; 
  inFile.read( reinterpret_cast<char*>(&nCompSize), sizeof( nCompSize ) );
  if ( nUncompSize == 0 || nCompSize == 0 )
  {
    std::cout << "Invalid PC3 header" << std::endl;
    return 2;
  }
  
  /**********************************************************************/
  /* Read the compressed stream into compBuf                            */
  /**********************************************************************/
  std::auto_ptr<unsigned char> compBuf( new unsigned char[nCompSize] );
  inFile.read( reinterpret_cast<char*>( compBuf.get() ), nCompSize );


  /**********************************************************************/
  /* Initialize zlib                                                    */
  /**********************************************************************/
  z_stream d_stream;
  memset( &d_stream, 0, sizeof(z_stream) );
  std::auto_ptr<unsigned char> uncompBuf( new unsigned char[nUncompSize] );
  d_stream.next_in  = compBuf.get();
  d_stream.next_out = uncompBuf.get();
  if( inflateInit( &d_stream ) != Z_OK )
  {
    std::cout << "Error initializing inflate engine" << std::endl;
    return 3; 
  }

  /**********************************************************************/
  /* Decompress compBuf                                                 */
  /**********************************************************************/
  bool bSuccess = true;
  while ( d_stream.total_out < nUncompSize && d_stream.total_in < nCompSize )
  {
    d_stream.avail_in = d_stream.avail_out = 1;
    int status = inflate( &d_stream, Z_NO_FLUSH );
    if ( status != Z_OK )
    {
      if ( status != Z_STREAM_END ) bSuccess = false;
      break;
    }
  }
  inflateEnd( &d_stream );
  if ( !bSuccess )
  {
    std::cout << "Error inflating buffer" << std::endl;
    return 4;
  }

  /**********************************************************************/
  /* Parse the uncompressed data, ans write the XML file                */
  /**********************************************************************/
  std::istrstream instr( reinterpret_cast<char*>( uncompBuf.get() ), nUncompSize );
  std::ofstream out( argv[2] );
  pc3::Parser p;
  if ( p.loadText( instr ) ) 
    p.saveXml( out );
  
  return 0;
}
