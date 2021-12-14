//
//  Copyright (c) 2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//
#if !defined ABSTRACT_CLASSFACTORY_HEADER
#define ABSTRACT_CLASSFACTORY_HEADER

///
///\file        dwf/whiptk/class_factory.h
///

#include "whiptk/whip_toolkit.h"
#include "whiptk/allclass.h"
#include "whiptk/color.h"
#include "whiptk/fillpat.h"
#include "dwfcore/Exception.h"

using namespace DWFCore;

/// Abstract Class Factory to create all rendition attributes and WT_Objects.

/// \class WT_Class_Factory
/// Abstract class factory for creating Whip objects, subclassed for 
/// different file formats.  The "classic" W2D objects are created using
/// the WT_W2D_Class_Factory subclass.
///
class WHIPTK_API WT_Class_Factory
{
protected:
    //@{ \name Construction / destruction.
    /// Constructs a WT_Class_Factory object.
    WT_Class_Factory()
    {}

public:
    /// Destroys a WT_Class_Factory object.
    virtual ~WT_Class_Factory()
    {}

    ///
    ///         Creates WT_Attribute_URL object
    ///
    ///\return  WT_Attribute_URL object.
    ///\throw   DWFNotImplementedException
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL()
	{
		_DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
	}
	///
    ///         Creates a WT_Attribute_URL object with the given dependent attribute.
    ///
    ///\return  WT_Attribute_URL object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &/*attribute*/)
	{
		_DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
	}
	///
    ///         Creates a WT_Attribute_URL object with the given dependent attribute and initial URL item.
    ///
    ///\return  WT_Attribute_URL object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &/*attribute*/, WT_URL_Item &/*item*/)
	{
		_DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
	}
	///
    ///         Creates a WT_Attribute_URL object with the given initial data.
    ///
    ///\return  WT_Attribute_URL object.
    ///\throw   DWFNotImplementedException
    ///	
    virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &/*attribute*/,
												WT_Integer32 /*index*/, 
												WT_Unsigned_Integer16 const * /*address*/, 
												WT_Unsigned_Integer16 const * /*friendly_name*/ ) 
												
	{
		_DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
	}

	///
    ///        Creates a WT_Attribute_URL object from another attribute_url.
    ///
    ///\return  WT_Attribute_URL object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute_URL const& /*url*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///         Creates WT_File object
    ///
    ///\return  WT_File object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_File*        Create_File()  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///         Creates WT_Code_Page object
    ///
    ///\return  WT_Code_Page object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Code_Page*    Create_Code_Page()  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///         Creates WT_Code_Page object
    ///
    /// page_number Constructs a WT_Code_Page object with the given page number.       
    ///\return  WT_Code_Page object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Code_Page*    Create_Code_Page( WT_Integer32 /*page_number*/ )  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///         Creates WT_Code_Page object from another code_page
    ///
    ///\return  WT_Code_Page object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Code_Page*    Create_Code_Page( WT_Code_Page const& /*code_page*/ )  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///         Creates a WT_Color object. (Defaults to white.).
    ///
    ///\return  WT_Color object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Color*        Create_Color()  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///         Creates a WT_Color object with the given color and optional mapping mode.
    ///
    /// color given color(WT_RGBA32) 
    /// m      mapping mode (WT_Color::WT_Color_Map_Mode)
    ///\return  WT_Color object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Color*        Create_Color(WT_RGBA32 /*color*/, WT_Color::WT_Color_Map_Mode =WT_Color::No_Mapping)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///         Creates a WT_Color object with the given color and optional mapping mode.
    ///
    ///\return  WT_Color object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Color*        Create_Color(int /*red*/, int /*green*/, int /*blue*/, WT_Color::WT_Color_Map_Mode =WT_Color::No_Mapping)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///         Creates a WT_Color object with the given color and optional mapping mode.
    ///
    ///\return  WT_Color object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Color*        Create_Color(int /*red*/, int /*green*/, int /*blue*/, int /*alpha*/, WT_Color::WT_Color_Map_Mode =WT_Color::No_Mapping)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Color object from the given index and corresponding color map. 
    ///
    ///\return  WT_Color object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Color*		Create_Color(WT_Color_Index /*index*/, WT_Color_Map const & /*color_map*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Color object from another color.
    ///
    ///\return  WT_Color object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Color*		Create_Color(WT_Color const & /*color*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Color_Map object for the optional specific file revision. Can be used a default constructor.
    ///
    ///\return  WT_Color_Map object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Color_Map*    Create_Color_Map(int =REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Color_Map object for the given size and map, for the given file.
    ///
    ///\return  WT_Color_Map object.
    ///\throw   DWFNotImplementedException
    ///     
    virtual WT_Color_Map*	Create_Color_Map(int /*count*/, WT_RGBA32 const * /*map*/, WT_File & /*file*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Color_Map object for the given size and map, for the given file.
    ///
    ///\return  WT_Color_Map object.
    ///\throw   DWFNotImplementedException
    ///     
    virtual WT_Color_Map*	Create_Color_Map(int /*count*/, WT_RGB const * /*map*/, WT_File & /*file*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Color_Map object. Copy constructor. 
    ///
    ///\return  WT_Color_Map object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Color_Map*	Create_Color_Map(WT_Color_Map const & /*cmap*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Constructs a WT_Compressed_Data_Moniker object. 
    ///
    ///\return  WT_Compressed_Data_Moniker object.
    ///\throw   DWFNotImplementedException
    ///     
    virtual WT_Compressed_Data_Moniker*        Create_Compressed_Data_Moniker(int =REVISION_WHEN_ZLIB_COMPRESSION_IS_SUPPORTED) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Contour_Set object. 
    ///
    ///\return  WT_Contour_Set object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contour_Set*        Create_Contour_Set() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Contour_Set object with the given data. 
    ///
    ///\return  WT_Contour_Set object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contour_Set*		Create_Contour_Set(WT_File & /*file*/, WT_Integer32 /*contours*/, WT_Integer32 const * /*counts*/, WT_Integer32 /*point_count*/, WT_Logical_Point const * /*points*/, WT_Boolean /*copy*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Contour_Set object with the given data for one countour (i.e. a polygon). Copies the points locally.      
    ///
    ///\return  WT_Contour_Set object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contour_Set*		Create_Contour_Set(WT_File & /*file*/, WT_Integer32 /*point_count*/, WT_Logical_Point const * /*points*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Contour_Set object with the given data. Can be used as a copy constructor. 
    ///
    ///\return  WT_Contour_Set object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contour_Set*		Create_Contour_Set(WT_Contour_Set const & /*source*/, WT_Boolean =WD_True)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///        Creates a WT_Contrast_Color object with the given data. Can be used as a copy constructor. 
    ///
    ///\return  WT_Contrast_Color object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contrast_Color* Create_Contrast_Color() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Contrast_Color object with the given data. Can be used as a copy constructor. 
    ///
    ///\return  WT_Contrast_Color object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contrast_Color* Create_Contrast_Color(WT_RGBA32 /*color*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///        Creates a WT_Contrast_Color object with the given data. Can be used as a copy constructor. 
    ///
    ///\return  WT_Contrast_Color object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contrast_Color* Create_Contrast_Color( int /*red*/, int /*green*/, int /*blue*/, int /*alpha*/ =255 ) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///        Creates a WT_Contrast_Color object from another constrast color 
    ///
    ///\return  WT_Contrast_Color object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Contrast_Color* Create_Contrast_Color( WT_Contrast_Color const& /*color*/  ) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///        Creates a WT_Dash_Pattern object. 
    ///
    ///\return  WT_Dash_Pattern object.
    ///\throw   DWFNotImplementedException
    ///     
    virtual WT_Dash_Pattern*    Create_Dash_Pattern() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Dash_Pattern object with the given data. 
    ///
    ///\return  WT_Dash_Pattern object.
    ///\throw   DWFNotImplementedException
    ///     
    virtual WT_Dash_Pattern*	Create_Dash_Pattern(WT_Integer32 /*id_number*/, WT_Integer16 /*length*/, WT_Integer16 const* /*pArray*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Dash_Pattern object. Copy constructor.  
    ///
    ///\return  WT_Dash_Pattern object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Dash_Pattern*	Create_Dash_Pattern(WT_Dash_Pattern const & /*r*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Dash_Pattern_List object.  
    ///
    ///\return  WT_Dash_Pattern_List object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Dash_Pattern_List*    Create_Dash_Pattern_List() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates a WT_Dash_Pattern_List object with the given data. 
    ///
    ///\return  WT_Dash_Pattern_List object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Dash_Pattern_List*    Create_Dash_Pattern_List(WT_Dash_Pattern* /*pPat*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Constructs WT_Delineate object 
    ///
    ///\return  WT_Delineate object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Delineate*        Create_Delineate() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///        Creates WT_Delineate object with given data
    ///
    ///\return  WT_Delineate object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Delineate*        Create_Delineate(WT_Boolean /*delineate_state*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
	///
    ///        Creates WT_Delineate object from another Delineate object
    ///
    ///\return  WT_Delineate object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Delineate*        Create_Delineate(WT_Delineate const& /*delineate*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_DWF_Header object
    ///
    ///\return WT_DWF_Header object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_DWF_Header*      Create_DWF_Header()
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Ellipse object.
    ///
    ///\return  WT_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Filled_Ellipse*            Create_Filled_Ellipse() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Ellipse object with the given data.
    ///
    ///\return  WT_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Filled_Ellipse*            Create_Filled_Ellipse(WT_Integer32 /*x*/, 
        WT_Integer32 /*y*/, 
        WT_Integer32 /*major*/, 
        WT_Integer32 /*minor*/, 
        WT_Unsigned_Integer16 /*start*/ =0, 
        WT_Unsigned_Integer16 /*end*/ =0, 
        WT_Unsigned_Integer16 /*tilt*/ =0) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Ellipse object with the given data.
    ///
    ///\return  WT_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Filled_Ellipse*			Create_Filled_Ellipse(WT_Logical_Point const & /*pos*/, 
        WT_Integer32 /*major*/, 
        WT_Integer32 /*minor*/, 
        WT_Unsigned_Integer16 /*start*/ =0, 
        WT_Unsigned_Integer16 /*end*/ =0, 
        WT_Unsigned_Integer16 /*tilt*/ =0) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Outline_Ellipse object from an existing ellipse.
    ///
    ///\return  WT_Outline_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///        
    virtual WT_Filled_Ellipse*         Create_Filled_Ellipse(const WT_Filled_Ellipse& /*e*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Outline_Ellipse object.
    ///
    ///\return  WT_Outline_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///        
    virtual WT_Outline_Ellipse*            Create_Outline_Ellipse() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Outline_Ellipse object from an existing ellipse.
    ///
    ///\return  WT_Outline_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///        
    virtual WT_Outline_Ellipse*         Create_Outline_Ellipse(const WT_Outline_Ellipse& /*e*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Outline_Ellipse object with the given data.
    ///
    ///\return  WT_Outline_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(WT_Integer32 /*x*/, WT_Integer32 /*y*/, 
        WT_Integer32 /*major*/, WT_Integer32 /*minor*/, 
        WT_Unsigned_Integer16 /*start*/ =0, 
        WT_Unsigned_Integer16 /*end*/ =0, 
        WT_Unsigned_Integer16 /*tilt*/ =0) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Ellipse object with the given data.
    ///
    ///\return  WT_Outline_Ellipse object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Outline_Ellipse*			Create_Outline_Ellipse(WT_Logical_Point const & /*pos*/, 
        WT_Integer32 /*major*/, 
        WT_Integer32 /*minor*/, 
        WT_Unsigned_Integer16 /*start*/ =0, 
        WT_Unsigned_Integer16 /*end*/ =0, 
        WT_Unsigned_Integer16 /*tilt*/ =0) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Embed object.
    ///
    ///\return  WT_Embed object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Embed*                    Create_Embed() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Embed object with given data.
    ///
    ///\return  WT_Embed object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Embed*					Create_Embed(WT_Embed const & /*embed*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Embedded_Font object.
    ///
    ///\return  WT_Embedded_Font object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Embedded_Font*           Create_Embedded_Font()
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Embedded_Font object.
    ///
    ///\return  WT_Embedded_Font object.
    ///\throw   DWFNotImplementedException
    ///    
    virtual WT_Embedded_Font*           Create_Embedded_Font(
        int /*request_type*/,
        int /*privilege_type*/,
        int /*character_set_type*/,
        WT_Integer32 /*data_size*/,
        WT_Byte * /*data*/,
        WT_Integer32 /*font_type_face_name_length*/,
        WT_Byte * /*font_type_face_name_string*/,
        WT_Integer32 /*font_logfont_name_length*/,
        WT_Byte * /*font_logfont_name_string*/,
        WT_Boolean /*copy*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_End_Of_DWF object.
    ///
    ///\return  WT_End_Of_DWF object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_End_Of_DWF*                Create_End_Of_DWF() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Fill object.
    ///
    ///\return  WT_Fill object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Fill*                    Create_Fill() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Fill object with given fill setting.
    ///
    ///\return  WT_Fill object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Fill*                    Create_Fill(WT_Boolean /*fill*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Fill object from another fill.
    ///
    ///\return  WT_Fill object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Fill*                    Create_Fill(WT_Fill const& /*fill*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Fill_Pattern object.
    ///
    ///\return  WT_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Fill_Pattern with the given ID.
    ///
    ///\return  WT_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern(WT_Fill_Pattern::WT_Pattern_ID /*n*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Fill_Pattern object from another fill_pattern object.
    ///
    ///\return  WT_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern(WT_Fill_Pattern const& /*pattern*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Font object.
    ///
    ///\return  WT_Font object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font*                    Create_Font() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font from the given data.
    ///
    ///\return  WT_Font object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font*                    Create_Font(WT_String const &/*name*/, WT_Boolean /*bold*/, WT_Boolean /*italic*/, 
        WT_Boolean /*underline*/, 
        WT_Byte /*charset*/, WT_Byte /*pitch*/, WT_Byte /*family*/, WT_Integer32 /*height*/, 
        WT_Unsigned_Integer16 /*rotation*/, WT_Unsigned_Integer16 /*width_scale*/, 
        WT_Unsigned_Integer16 /*spacing*/, WT_Unsigned_Integer16 /*oblique*/, 
        WT_Integer32 /*flags*/ =0) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	 ///
    ///       Creates a WT_Font from another font.
    ///
    ///\return  WT_Font object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font*                    Create_Font(WT_Font const& /*font*/)
		
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Extension object.
    ///
    ///\return  WT_Font_Extension object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Extension*            Create_Font_Extension() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Extension object with the given data. 
    ///
    ///\return  WT_Font_Extension object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Extension*            Create_Font_Extension(WT_String const & /*logfont_name*/, WT_String const & /*cannonical_name*/)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Charset object.
    ///
    ///\return  WT_Font_Option_Charset object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Charset*        Create_Font_Option_Charset() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Charset object with the given character set code. 
    ///
    ///\return  WT_Font_Option_Charset object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Charset*        Create_Font_Option_Charset(WT_Byte /*charset*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Family object.
    ///
    ///\return  WT_Font_Option_Family object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Family*        Create_Font_Option_Family() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Family object with the given font family value.
    ///
    ///\return  WT_Font_Option_Family object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Family*        Create_Font_Option_Family(WT_Byte /*family*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Flags object.
    ///
    ///\return  WT_Font_Option_Flags object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_Font_Option_Flags*        Create_Font_Option_Flags() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Flags object with the given flags. 
    ///
    ///\return  WT_Font_Option_Flags object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Flags*        Create_Font_Option_Flags(WT_Integer32 /*flags*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Font_Name object.
    ///
    ///\return  WT_Font_Option_Font_Name object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Font_Name*    Create_Font_Option_Font_Name() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Font_Name object with the given name. 
    ///
    ///\return  WT_Font_Option_Font_Name object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Font_Name*    Create_Font_Option_Font_Name(WT_String const & /*name*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Height object.
    ///
    ///\return  WT_Font_Option_Height object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Height*        Create_Font_Option_Height() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Height object with the given height (in drawing units.
    ///
    ///\return  WT_Font_Option_Height object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Height*        Create_Font_Option_Height(WT_Integer32 /*height*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Oblique object.
    ///
    ///\return  WT_Font_Option_Oblique object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Oblique*        Create_Font_Option_Oblique() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Oblique object with the given oblique angle. 
    ///
    ///\return  WT_Font_Option_Oblique object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Oblique*        Create_Font_Option_Oblique(WT_Unsigned_Integer16 /*oblique*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Pitch object. 
    ///
    ///\return  WT_Font_Option_Pitch object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Pitch*        Create_Font_Option_Pitch() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Pitch object with the given pitch value. 
    ///
    ///\return  WT_Font_Option_Pitch object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Pitch*        Create_Font_Option_Pitch(WT_Byte /*pitch*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Rotation object.
    ///
    ///\return  WT_Font_Option_Rotation object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_Font_Option_Rotation*    Create_Font_Option_Rotation() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Rotation object with the given rotation angle.
    ///
    ///\return  WT_Font_Option_Rotation object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Rotation*    Create_Font_Option_Rotation(WT_Unsigned_Integer16 /*rotation*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Spacing object.
    ///
    ///\return  WT_Font_Option_Spacing object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_Font_Option_Spacing*        Create_Font_Option_Spacing() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Spacing object with the given spacing.
    ///
    ///\return  WT_Font_Option_Spacing object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Spacing*        Create_Font_Option_Spacing(WT_Unsigned_Integer16 /*spacing*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Style object with the given settings (can be used as a default constructor.). 
    ///
    ///\return  WT_Font_Option_Style object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Style*        Create_Font_Option_Style(WT_Boolean /*bold*/=WD_False, WT_Boolean /*italic*/=WD_False, 
        WT_Boolean /*underlined*/=WD_False) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Width_Scale object. 
    ///
    ///\return  WT_Font_Option_Width_Scale object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Width_Scale*    Create_Font_Option_Width_Scale() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Font_Option_Width_Scale object with the given width scale.
    ///
    ///\return  WT_Font_Option_Width_Scale object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Font_Option_Width_Scale* Create_Font_Option_Width_Scale(WT_Unsigned_Integer16 /*width_scale*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Gouraud_Polyline object.
    ///
    ///\return  v object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Gouraud_Polyline*        Create_Gouraud_Polyline() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Gouraud_Polyline object with the given data.
    ///
    ///\return  WT_Gouraud_Polyline object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Gouraud_Polyline*		Create_Gouraud_Polyline(int /*count*/, WT_Logical_Point const* /*points*/, 
        WT_RGBA32 const* /*colors*/, WT_Boolean /*copy*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Gouraud_Polyline object from another Gouraud Polyline object
    ///
    ///\return  WT_Gouraud_Polyline object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Gouraud_Polyline*		Create_Gouraud_Polyline(WT_Gouraud_Polyline const& /*pline*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Gouraud_Polytriangle object.
    ///
    ///\return WT_Gouraud_Polytriangle object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Gouraud_Polytriangle*    Create_Gouraud_Polytriangle() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Gouraud_Polytriangle object with the given data.
    ///
    ///\return WT_Gouraud_Polytriangle object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Gouraud_Polytriangle*	Create_Gouraud_Polytriangle(int /*count*/, WT_Logical_Point const* /*points*/, 
        WT_RGBA32 const* /*colors*/, WT_Boolean /*copy*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Gouraud_Polytriangle object from another Gouraud Polyline object
    ///
    ///\return  WT_Gouraud_Polytriangle object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Gouraud_Polytriangle*		Create_Gouraud_Polytriangle(WT_Gouraud_Polytriangle const& /*polytri*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Image object.
    ///
    ///\return WT_Image object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Image*                    Create_Image() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Image object of the given type (one of enum WT_Image_Format.).
    ///
    ///\return WT_Image object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Image*                    Create_Image(int /*image_type*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Image object with the given data. 
    ///
    ///\return WT_Image object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Image*                    Create_Image(WT_Unsigned_Integer16 /*rows*/,
        WT_Unsigned_Integer16 /*columns*/, 
        WT_Image::WT_Image_Format /*format*/, WT_Integer32 /*identifier*/, 
        WT_Color_Map const* /*color_map*/, WT_Integer32 /*data_size*/, 
        WT_Byte* /*data*/, WT_Logical_Point const & /*min_corner*/, 
        WT_Logical_Point const & /*max_corner*/, WT_Boolean /*copy*/,
        WT_Integer32 /*dpi*/=-1)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Layer object
    ///
    ///\return WT_Layer object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Layer*                    Create_Layer() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Layer object wth the given data.
    ///
    ///\return WT_Layer object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Layer*					Create_Layer(WT_File & /*file*/, WT_Integer32 /*layer_num*/, char const * /*layer_name*/=WD_Null)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Layer object wth the given data.
    ///
    ///\return WT_Layer object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Layer*                    Create_Layer(WT_File & /*file*/, WT_Integer32 /*layer_num*/, 
        WT_Unsigned_Integer16 const* /*layer_name*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Layer object. Copy constructor.
    ///
    ///\return WT_Layer object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Layer*					Create_Layer(WT_Layer const & /*layer*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Line_Pattern object.
    ///
    ///\return WT_Line_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Line_Pattern object with the given predefined pattern ID.
    ///
    ///\return WT_Line_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern(WT_Line_Pattern::WT_Pattern_ID /*id*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	 ///
    ///       Creates a WT_Line_Pattern object from another WT_Line_Pattern object.
    ///
    ///\return WT_Line_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern(WT_Line_Pattern const& /*pat*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Line_Style object.
    ///
    ///\return WT_Line_Style object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Style*                Create_Line_Style() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Line_Style object. (Copy constructor)
    ///
    ///\return WT_Line_Style object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Style*                Create_Line_Style(WT_Line_Style const & /*style*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Line_Weight object.
    ///
    ///\return WT_Line_Weight object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Weight*                Create_Line_Weight() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Line_Weight object with the given line weight.
    ///
    ///\return WT_Line_Weight object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Weight*                Create_Line_Weight(WT_Integer32 /*weight*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Line_Weight object with the given line weight.
    ///
    ///\return WT_Line_Weight object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Line_Weight*                Create_Line_Weight(WT_Line_Weight const & /*weight*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Macro_Definition object.
    ///
    ///\return WT_Macro_Definition object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Definition*        Create_Macro_Definition() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Macro_Definition object with given data.
    ///
    ///\return WT_Macro_Definition object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Definition*        Create_Macro_Definition( WT_Unsigned_Integer16 /*index*/,WT_Integer32 /*scale_units*/=1 ) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Macro_Draw object.
    ///
    ///\return WT_Macro_Draw object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Draw*                Create_Macro_Draw() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Macro_Draw object.
    ///
    ///\return WT_Macro_Draw object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Draw*				Create_Macro_Draw(int /*count*/, WT_Logical_Point const * /*points*/,WT_Boolean /*copy*/ )
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
	 ///
    ///       Creates a WT_Macro_Draw object from anoter WT_Macro_Draw object.
    ///
    ///\return WT_Macro_Draw object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Draw*				Create_Macro_Draw(WT_Macro_Draw const& /*draw*/ )
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Macro_Index object.
    ///
    ///\return WT_Macro_Index object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Index*                Create_Macro_Index() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Macro_Index object using the Macro index. if the macro dosent exist
    ///        it will be ignored
    ///
    ///\return WT_Macro_Index object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Index*                Create_Macro_Index(WT_Integer32 /*macro*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Macro_Index object from another WT_Macro_Index.
    ///
    ///\return WT_Macro_Index object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Index*                Create_Macro_Index(WT_Macro_Index const& /*index*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Macro_Scale object.
    ///
    ///\return WT_Macro_Scale object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Macro_Scale object with the given size (in drawing units.)
    ///
    ///\return WT_Macro_Scale object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale(WT_Integer32 /*macro_scale*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Macro_Scale object from another WT_Macro_Scale object.
    ///
    ///\return WT_Macro_Scale object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale(WT_Macro_Scale const& /*scale*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Merge_Control object.
    ///
    ///\return WT_Merge_Control object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Merge_Control*            Create_Merge_Control() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Merge_Control object with the given format.
    ///
    ///\return WT_Merge_Control object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Merge_Control*            Create_Merge_Control(WT_Merge_Control::WT_Merge_Format /*merge*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Merge_Control object from another merge_control object.
    ///
    ///\return WT_Merge_Control object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Merge_Control*            Create_Merge_Control(WT_Merge_Control const& /*merge_control*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Named_View_List object.
    ///
    ///\return WT_Named_View_List object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_Named_View_List*                Create_Named_View_List(void) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	 ///
    ///       Creates a WT_Named_View_List object from another object.
    ///
    ///\return WT_Named_View_List object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_Named_View_List*                Create_Named_View(WT_Named_View_List const& /*named_view_list*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Named_View object.
    ///
    ///\return WT_Named_View object.
    ///\throw   DWFNotImplementedException
    ///
    virtual    WT_Named_View*                Create_Named_View(void) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Named_View object. Copy constructor.
    ///
    ///\return WT_Named_View object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Named_View*				Create_Named_View(WT_Named_View const & /*named_view*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Named_View object with the given view and name.
    ///
    ///\return WT_Named_View object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Named_View*				Create_Named_View(WT_Logical_Box const & /*view*/, const char * /*name*/=WD_Null) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Named_View object with the given view and name.
    ///
    ///\return WT_Named_View object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Named_View*				Create_Named_View(WT_Logical_Box const & /*view*/, WT_Unsigned_Integer16 const* /*name*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Object_Node object.
    ///
    ///\return WT_Object_Node object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Object_Node*                Create_Object_Node() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Object_Node object with the given data.
    ///
    ///\return WT_Object_Node object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_File & /*file*/, WT_Integer32 /*object_node_num*/, 
        char const * /*object_node_name*/=WD_Null)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Object_Node object with the given data.
    ///
    ///\return WT_Object_Node object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_File & /*file*/, WT_Integer32 /*object_node_num*/, 
        WT_Unsigned_Integer16 const* /*object_node_name*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Object_Node object. Copy constructor.
    ///
    ///\return WT_Object_Node object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Object_Node*				Create_Object_Node(WT_Object_Node const & /*object_node*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Object_Stream object.
    ///
    ///\return WT_Object_Stream object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Object_Stream*            Create_Object_Stream() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Object_Stream object from another Object_Stream.
    ///
    ///\return WT_Object_Stream object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Object_Stream*            Create_Object_Stream(WT_Object_Stream const& /*object_stream*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Overpost object.
    ///
    ///\return WT_Overpost object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Overpost*                Create_Overpost() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Overpost object with given data.
    ///
    ///\return WT_Overpost object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Overpost*                Create_Overpost( WT_Overpost::WT_AcceptMode /*acceptMode*/,
        WT_Boolean /*renderEntities*/,WT_Boolean /*addExtents*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
	
    ///
    ///       Creates a WT_Pen_Pattern object.
    ///
    ///\return WT_Pen_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Pen_Pattern object with the given predefined pattern ID.
    ///
    ///\return WT_Pen_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID /*pattern_id*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Pen_Pattern object with the given data.
    ///
    ///\return WT_Pen_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID /*pattern_id*/, 
        WT_Unsigned_Integer32 /*screening_percentage*/,
        WT_Boolean /*colormap_flag*/, WT_Color_Map const* /*color_map*/, 
        WT_Boolean /*copy*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Pen_Pattern object. Copy constructor.
    ///
    ///\return WT_Pen_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Pen_Pattern*				Create_Pen_Pattern(WT_Pen_Pattern const & /*pattern*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_PenPat_Options object.
    ///
    ///\return WT_PenPat_Options object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_PenPat_Options*            Create_PenPat_Options() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_PenPat_Options object with the given data.
    ///
    ///\return WT_PenPat_Options object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_PenPat_Options*			Create_PenPat_Options(WT_Boolean const /*scale_pen_width*/, 
        WT_Boolean const /*map_colors_to_gray_scale*/, 
        WT_Boolean const /*use_alternate_fill_rule*/, 
        WT_Boolean const /*use_error_diffusion_for_DWF_Rasters*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_PenPat_Options object. Copy constructor. 
    ///
    ///\return WT_PenPat_Options object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_PenPat_Options*			Create_PenPat_Options(WT_PenPat_Options const & /*other*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Plot_Optimized object.
    ///
    ///\return WT_Plot_Optimized object.
    ///\throw   DWFNotImplementedException
    ///
     virtual WT_Plot_Optimized*            Create_Plot_Optimized() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Plot_Optimized object with the given state.
    ///
    ///\return WT_Plot_Optimized object.
    ///\throw   DWFNotImplementedException
    ///
     virtual WT_Plot_Optimized*            Create_Plot_Optimized(WT_Boolean /*plot_optimized*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
	 ///
    ///       Creates a WT_Plot_Optimized object froma another object.
    ///
    ///\return WT_Plot_Optimized object.
    ///\throw   DWFNotImplementedException
    ///
     virtual WT_Plot_Optimized*            Create_Plot_Optimized(WT_Plot_Optimized const& /*plot_optimized*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_PNG_Group4_Image object.
    ///
    ///\return WT_PNG_Group4_Image object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_PNG_Group4_Image object with the given data.
    ///
    ///\return WT_PNG_Group4_Image object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image(int /*image_type*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_PNG_Group4_Image object with the given data.
    ///
    ///\return WT_PNG_Group4_Image object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image(WT_Unsigned_Integer16 /*rows*/, 
        WT_Unsigned_Integer16 /*columns*/, 
        WT_PNG_Group4_Image::WT_PNG_Group4_Image_Format /*format*/, 
        WT_Integer32 /*identifier*/, 
        WT_Color_Map const* /*color_map*/, 
        WT_Integer32 /*data_size*/, 
        WT_Byte* /*data*/, 
        WT_Logical_Point const & /*min_corner*/, 
        WT_Logical_Point const & /*max_corner*/, 
        WT_Boolean /*copy*/,
        WT_Integer32 /* dpi */=-1) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Polygon object. 
    ///
    ///\return WT_Polygon object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polygon*                    Create_Polygon() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Polygon object from the given data.
    ///
    ///\return WT_Polygon object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polygon*					Create_Polygon(int /*count*/, WT_Logical_Point const* /*points*/, WT_Boolean /*copy*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Polygon object. Copy constructor. 
    ///
    ///\return WT_Polygon object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polygon*					Create_Polygon(const WT_Polygon & /*pline*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Polyline object.
    ///
    ///\return WT_Polyline object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polyline*    Create_Polyline() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Polyline object from the given data.
    ///
    ///\return WT_Polyline object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polyline*    Create_Polyline(
        int                         /*count*/, /**< The number of points in the array.*/
        WT_Logical_Point const *    /*points*/, /**< Pointer to the array of points.*/
        WT_Boolean                  /*copy*/ /**< Whether the points should be copied or if their addresses should be used directly from the array.*/
        ) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Polyline object. Copy constructor.
    ///
    ///\return WT_Polyline object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polyline*	Create_Polyline(const WT_Polyline & /*pline*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    /// Creates a WT_Polymarker object.
    ///
    ///\return WT_Polymarker object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polymarker* Create_Polymarker()
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    /// Creates a WT_Polymarker object from the given data.
    ///
    ///\return WT_Polymarker object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polymarker* Create_Polymarker(
        int                         /*count*/, /**< The number of points in the array. */
        WT_Logical_Point const *    /*points*/, /**< Pointer to the array of points. */
        WT_Boolean                  /*copy*/ /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        )
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
	/// Creates a WT_Polymarker object from another object.
    ///
    ///\return WT_Polymarker object.
    ///\throw   DWFNotImplementedException
    ///
	virtual WT_Polymarker* Create_Polymarker(WT_Polymarker const& /*marker*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Polytriangle object.
    ///
    ///\return WT_Polytriangle object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polytriangle*            Create_Polytriangle() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Polytriangle object from the given data.
    ///
    ///\return WT_Polytriangle object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polytriangle*			Create_Polytriangle(int /*count*/, WT_Logical_Point const* /*points*/, WT_Boolean /*copy*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Polytriangle object from another object.
    ///
    ///\return WT_Polytriangle object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Polytriangle*            Create_Polytriangle(WT_Polytriangle const& /*polytriangle*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Projection object.
    ///
    ///\return WT_Projection object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Projection*                Create_Projection() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Projection object with the given projection type.
    ///
    ///\return WT_Projection object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Projection*				Create_Projection(const WT_Projection::WT_Projection_Type /*projection_type*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Projection object from another projection object.
    ///
    ///\return WT_Projection object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Projection*                Create_Projection(WT_Projection const& /*projection*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Text object.
    ///
    ///\return WT_Text object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text*                    Create_Text() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text object.Overrides default copy constructor.
    ///
    ///\return WT_Text object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text*					Create_Text(WT_Text const &)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text object with the given data.
    ///
    ///\return WT_Text object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text*					Create_Text(WT_Logical_Point const & /*position*/, 
        WT_String const & /*string*/, 
        WT_Logical_Point const* /*bbox*/, 
        WT_Unsigned_Integer16 /*overscore_count*/, 
        WT_Unsigned_Integer16 const* /*overscore_pos*/, 
        WT_Unsigned_Integer16 /*underscore_count*/, 
        WT_Unsigned_Integer16 const* /*underscore_pos*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text object with the given data.
    ///
    ///\return WT_Text object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text*					Create_Text(WT_Logical_Point const & /*position*/, WT_String const & /*string*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text_Background object
    ///
    ///\return WT_Text_Background object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_Background*            Create_Text_Background()  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text_Background object with the given background style and color.
    ///
    ///\return WT_Text_Background object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_Background*            Create_Text_Background(WT_Text_Background::eBackground /*background*/,WT_Integer32 /*offset*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Text_Background object from another text background
    ///
    ///\return WT_Text_Background object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_Background*            Create_Text_Background(WT_Text_Background const& /*background*/)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text_HAlign object.
    ///
    ///\return WT_Text_HAlign object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign()  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text_HAlign object with the given alignment.
    ///
    ///\return WT_Text_HAlign object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign(WT_Text_HAlign::WT_HAlign /*hAlign*/)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Text_HAlign object from another Text_HAlign.
    ///
    ///\return WT_Text_HAlign object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign(WT_Text_HAlign const& /*hAlign*/)  
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Text_VAlign object.
    ///
    ///\return WT_Text_VAlign object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Text_VAlign object with the given alignment.
    ///
    ///\return WT_Text_VAlign object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign(WT_Text_VAlign::WT_VAlign /*vAlign*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	 ///
    ///       Creates a WT_Text_VAlign object from another Text_VAlign.
    ///
    ///\return WT_Text_VAlign object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign(WT_Text_VAlign const& /*vAlign*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

    ///
    ///       Creates a WT_Units object.
    ///
    ///\return WT_Units object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Units*                    Create_Units() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Units object.
    ///
    ///\return WT_Units object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Units*					Create_Units(WT_Units const& /*u*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Units object with the given data.
    ///
    ///\return WT_Units object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_Units*					Create_Units(WT_Matrix const & /*xform*/, WT_String const & /*units*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_URL object.
    ///
    ///\return WT_URL object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_URL*                        Create_URL() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_URL object with the given initial URL item.
    ///
    ///\return WT_URL object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_URL*                        Create_URL(WT_URL_Item & /*item*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_URL object with the given initial hyperlink data.
    ///
    ///\return WT_URL object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_URL*						Create_URL(WT_Integer32 /*index*/, WT_Unsigned_Integer16 const* /*address*/, 
        WT_Unsigned_Integer16 const* /*friendly_name*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_URL object.
    ///
    ///\return WT_URL object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_URL*                        Create_URL(WT_URL const& /*url*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_URL_List object.
    ///
    ///\return WT_URL_List object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_URL_List*                Create_URL_List() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_URL_List object. Copy constructor. 
    ///
    ///\return WT_URL_List object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_URL_List*				Create_URL_List(WT_URL_List const & /*list*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_UserData object. 
    ///
    ///\return WT_UserData object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_UserData*                Create_UserData() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_UserData object with the given description.
    ///
    ///\return WT_UserData object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_UserData*				Create_UserData(WT_String const & /*data_description*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_UserData object with the given data.
    ///
    ///\return WT_UserData object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_UserData*				Create_UserData(WT_String const & /*data_description*/, 
        WT_Integer32 /*data_size*/, WT_Byte* /*data*/, WT_Boolean /*copy*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Fill_Pattern object.
    ///
    ///\return WT_User_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Fill_Pattern with the given ID.
    ///
    ///\return WT_User_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 /*pattern_num*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Fill_Pattern with a user defined bitonal bitmap passing in all the parameters .
    ///
    ///\return WT_User_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 /*pattern_num*/, WT_Unsigned_Integer16 /*rows*/,
        WT_Unsigned_Integer16 /*columns*/,
        WT_Unsigned_Integer32 /*data_size*/,WT_Byte const* /*data*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Fill_Pattern with a user defined bitonal bitmap using the reference counted Fill_Pattern class.
    ///
    ///\return WT_User_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 /*pattern_num*/,
        WT_User_Fill_Pattern::Fill_Pattern * /*fill_pattern*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Fill_Pattern. Copy Constructor for the WT_User_Fill_Pattern.
    ///
    ///\return WT_User_Fill_Pattern object.
    ///\throw   DWFNotImplementedException
    ///
    virtual	WT_User_Fill_Pattern*		Create_User_Fill_Pattern(WT_User_Fill_Pattern const &)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Hatch_Pattern object. 
    ///
    ///\return WT_User_Hatch_Pattern object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Hatch_Pattern object. Constructor using a previously defined hashpatnum. 
    ///
    ///\return WT_User_Hatch_Pattern object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_Integer16 /*hashpatnum*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Hatch_Pattern object. Constructor using a unique hashpatnum along with the X and Y size
    ///       of the line patterns in points. 
    ///
    ///\return WT_User_Hatch_Pattern object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_Integer16 /*hashpatnum*/,
        WT_Unsigned_Integer16 /*xsize*/,WT_Unsigned_Integer16 /*ysize*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_User_Hatch_Pattern object. Copy constructor
    ///
    ///\return WT_User_Hatch_Pattern object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_User_Hatch_Pattern*		Create_User_Hatch_Pattern(WT_User_Hatch_Pattern const & /*pattern*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_View object.
    ///
    ///\return WT_View object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_View*                    Create_View() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_View object with the given bounds.
    ///
    ///\return WT_View object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_View*                    Create_View(WT_Logical_Box /*view*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_View object with the given name.
    ///
    ///\return WT_View object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_View*                    Create_View(char* /*name*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_View object. Copy constructor.
    ///
    ///\return WT_View object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_View*					Create_View(WT_View const & /*view*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Viewport object.
    ///
    ///\return WT_Viewport object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Viewport*                Create_Viewport() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Viewport object. Copy constructor.
    ///
    ///\return WT_Viewport object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Viewport*				Create_Viewport(WT_Viewport const & /*vport*/)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Viewport object with the given data.
    ///
    ///\return WT_Viewport object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Viewport*				Create_Viewport(WT_File & /*file*/, char const* /*name*/, 
        int /*num_points*/, WT_Logical_Point const* /*points*/, 
        WT_Boolean /*copy*/=WD_True) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Viewport object with the given data.
    ///
    ///\return WT_Viewport object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Viewport*				Create_Viewport(WT_File & /*file*/, WT_Unsigned_Integer16 const* /*name*/, 
        int /*num_points*/, WT_Logical_Point const* /*points*/, 
        WT_Boolean /*copy*/=WD_True) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Viewport object with the given data.
    ///
    ///\return WT_Viewport object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Viewport*				Create_Viewport(WT_File & /*file*/, WT_String const & /*name*/, 
        WT_Contour_Set const & /*boundary*/, WT_Boolean /*copy*/=WD_True)
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Visibility object.
    ///
    ///\return WT_Visibility object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Visibility*                Create_Visibility() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }
    ///
    ///       Creates a WT_Visibility object with the given value. 
    ///
    ///\return WT_Visibility object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Visibility*                Create_Visibility(WT_Boolean /*visible*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }

	///
    ///       Creates a WT_Visibility object.
    ///
    ///\return WT_Visibility object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Visibility*                Create_Visibility(WT_Visibility const& /*visibility*/) 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }


    ///
    ///       Creates a WT_Unknown object.
    ///
    ///\return WT_Unknown object.
    ///\throw   DWFNotImplementedException
    /// 
    virtual WT_Unknown*                Create_Unknown() 
        
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This opcode not implemented in the factory..." );
    }


    //Destroy methods for all base classes
    virtual void            Destroy(WT_Object*      pObject);
    virtual void            Destroy(WT_Attribute*   pAttribute);
    virtual void            Destroy(WT_File*        pFile);
    virtual void            Destroy(WT_Dash_Pattern* pDashPat);
    virtual void            Destroy(WT_Layer*       pLayer);
    virtual void            Destroy(WT_Named_View*  pNamedView);
    virtual void            Destroy(WT_Object_Node* pObjectNode);
};

#endif //ABSTRACT_CLASSFACTORY_HEADER

