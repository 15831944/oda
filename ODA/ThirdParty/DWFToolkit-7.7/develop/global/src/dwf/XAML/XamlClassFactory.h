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
#if !defined XAML_CLASSFACTORY_HEADER
#define XAML_CLASSFACTORY_HEADER

///
/// \file       XAML/XamlClassFactory.h
///


#include "XAML/XamlCore.h"

#include "whiptk/whip_toolkit.h"
#include "whiptk/class_factory.h"
//#include "XAML/pch.h"

/// \class WT_XAML_Class_Factory
/// Class factory for creating DWFx 2D graphics streams.  We use 
/// the XAML moniker here, but technically our DWFx 2D streams are
/// based on the WPF subset of XAML, not actually "full XAML".
///
/// \see WT_Class_Factory
class XAMLTK_API WT_XAML_Class_Factory : public WT_Class_Factory
{
public:
    WT_XAML_Class_Factory(void);
public:
    virtual ~WT_XAML_Class_Factory(void);

    //Create__XAML_Class_Factoryhere

    ///
    ///\copydoc WT_Class_Factory::Create_File()
    ///
    virtual WT_File*        Create_File() 
        ;

	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL()
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL() ;	
	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(WT_Attribute &attribute)
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &attribute) ;
	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(WT_Attribute &attribute, WT_URL_Item &item)
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &attribute, WT_URL_Item &item) 
		;
	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(WT_Attribute &attribute,WT_Integer32 index,WT_Unsigned_Integer16 const * address, WT_Unsigned_Integer16 const * friendly_name )
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &attribute,
												WT_Integer32 index, 
												WT_Unsigned_Integer16 const * address, 
												WT_Unsigned_Integer16 const * friendly_name ) 
												;

	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(const WT_Attribute_URL& url )
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL( WT_Attribute_URL const& url)
        ;

    //Code Page
    ///
    ///\copydoc WT_Class_Factory::Create_Code_Page()
    ///
    virtual WT_Code_Page*    Create_Code_Page() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Code_Page( WT_Integer32 page_number )
    ///
    virtual WT_Code_Page*    Create_Code_Page( WT_Integer32 page_number ) 
        ;

	///
    ///\copydoc WT_Class_Factory::Create_Code_Page( const WT_Code_Page& code_page ) 
    ///
	virtual WT_Code_Page*    Create_Code_Page( WT_Code_Page const& code_page )  
        ;

    //Color
    ///
    ///\copydoc WT_Class_Factory::Create_Color()
    ///
    virtual WT_Color*        Create_Color() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color(WT_RGBA32 color, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping)
    ///
    virtual WT_Color*        Create_Color(WT_RGBA32 color, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color(int red, int green, int blue, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping)
    ///
    virtual WT_Color*        Create_Color(int red, int green, int blue, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color(int red, int green, int blue, int alpha, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping)
    ///
    virtual WT_Color*        Create_Color(int red, int green, int blue, int alpha, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color(WT_Color_Index index, WT_Color_Map const &color_map)
    ///
    virtual WT_Color*        Create_Color(WT_Color_Index index, WT_Color_Map const &color_map) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Color(WT_Color const &color) 
    ///
    virtual WT_Color*		Create_Color(WT_Color const & /*color*/)
        ;

    //Color_Map    
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(int file_revision_being_used=REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED)
    ///
    virtual WT_Color_Map*    Create_Color_Map(int file_revision_being_used=REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(int count, WT_RGBA32 const *map, WT_File &file)
    ///
    virtual WT_Color_Map*    Create_Color_Map(int count, WT_RGBA32 const *map, WT_File &file) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(int count, WT_RGB const *map, WT_File &file)
    ///
    virtual WT_Color_Map*    Create_Color_Map(int count, WT_RGB const *map, WT_File &file) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(WT_Color_Map const &cmap)
    ///
    virtual WT_Color_Map*    Create_Color_Map(WT_Color_Map const &cmap) 
        ;

    //WT_Contour_Set
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set() 
    ///
    virtual WT_Contour_Set*         Create_Contour_Set() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set(WT_File &file, WT_Integer32 contours, WT_Integer32 const *counts, WT_Integer32 point_count, WT_Logical_Point const *points, WT_Boolean copy) 
    ///
    virtual WT_Contour_Set*         Create_Contour_Set(WT_File &file, WT_Integer32 contours, WT_Integer32 const *counts, WT_Integer32 point_count, WT_Logical_Point const *points, WT_Boolean copy)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set(WT_File &file, WT_Integer32 point_count, WT_Logical_Point const *points)
    ///
     virtual WT_Contour_Set*        Create_Contour_Set(WT_File &file, WT_Integer32 point_count, WT_Logical_Point const *points)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set(WT_Contour_Set const &source, WT_Boolean copy=WD_True)
    ///
     virtual WT_Contour_Set*        Create_Contour_Set(WT_Contour_Set const &source, WT_Boolean copy=WD_True)
        ;

	///
	 ///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color() 
    ///
	virtual WT_Contrast_Color*		Create_Contrast_Color() 
		;
	  
	///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color(WT_RGBA32 color) 
    ///
	virtual WT_Contrast_Color*		Create_Contrast_Color(WT_RGBA32 color)
		;
	
	///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color(int red,int green,int blue,int alpha = 255) 
    ///
    virtual WT_Contrast_Color*		Create_Contrast_Color(int red,int green,int blue,int alpha = 255) 
		;
	
	///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color(const WT_Contrast_Color& color ) 
    ///
	virtual WT_Contrast_Color* Create_Contrast_Color(WT_Contrast_Color const& color  ) 
        ;

    //Dash_Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Dash_Pattern()
    ///
    virtual WT_Dash_Pattern*    Create_Dash_Pattern() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Dash_Pattern(WT_Integer32 id_number, WT_Integer16 length, WT_Integer16 const *pArray)
    ///
    virtual WT_Dash_Pattern*    Create_Dash_Pattern(WT_Integer32 id_number, WT_Integer16 length, WT_Integer16 const *pArray)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Dash_Pattern(WT_Dash_Pattern const &r)
    ///
    virtual WT_Dash_Pattern*    Create_Dash_Pattern(WT_Dash_Pattern const &r)
        ;

	//Delineate
	///
    ///\copydoc WT_Class_Factory::Create_Delineate() 
    ///
	virtual WT_Delineate*		Create_Delineate() 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Delineate(WT_Boolean delineate_state) 
    ///
    virtual WT_Delineate*		Create_Delineate(WT_Boolean delineate_state) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Delineate(const WT_Delineate& delineate) 
    ///
	virtual WT_Delineate*        Create_Delineate(WT_Delineate const& delineate) 
        ;

	//DWF header
	///
    ///\copydoc WT_Class_Factory::Create_DWF_Header() 
    ///
	virtual WT_DWF_Header*				Create_DWF_Header() 
        ;

	//End of DWF
	///
    ///\copydoc WT_Class_Factory::Create_End_Of_DWF() 
    ///
	virtual	WT_End_Of_DWF*				Create_End_Of_DWF() 
		;

    //Ellipse
    ///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse() 
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0) 
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse(const WT_Filled_Ellipse& e)
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse(const WT_Filled_Ellipse& e)
        ;
    
    //Outline Ellipse
    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse()
    ///
    virtual WT_Outline_Ellipse*            Create_Outline_Ellipse()
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
    ///
     virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
    ///
     virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse(const WT_Outline_Ellipse& e)
    ///
     virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(const WT_Outline_Ellipse& e) 
		 ;

    //Embed
    ///
    ///\copydoc WT_Class_Factory::Create_Embed() 
    ///
    virtual WT_Embed*                    Create_Embed() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Embed(WT_Embed const &embed)  
    ///
    virtual WT_Embed*                    Create_Embed(WT_Embed const &embed) 
        ; 

    ///
    ///\copydoc WT_Class_Factory::Create_Embedded_Font()
    ///    
    virtual WT_Embedded_Font*           Create_Embedded_Font()
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_Embedded_Font(int,int,int,WT_Integer32,WT_Byte*,WT_Integer32,WT_Byte*,WT_Integer32,WT_Byte*,WT_Boolean)
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
        ;

	//Fill
	///
    ///\copydoc WT_Class_Factory::Create_Fill() 
    ///
	virtual WT_Fill*					Create_Fill() 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Fill(WT_Boolean fill) 
    ///
	virtual WT_Fill*					Create_Fill(WT_Boolean fill) 
        ;

	///
    ///\copydoc WT_Class_Factory::Create_Fill(WT_Fill const& fill) 
    ///
	virtual WT_Fill*					Create_Fill(WT_Fill const& fill) 
        ;

    //Fill Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Fill_Pattern() 
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Fill_Pattern(WT_Fill_Pattern::WT_Pattern_ID n)
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern(WT_Fill_Pattern::WT_Pattern_ID n)
        ;

	///
    ///\copydoc WT_Class_Factory::Create_Fill_Pattern(WT_Fill_Pattern const& pattern) 
    ///
	 virtual WT_Fill_Pattern*            Create_Fill_Pattern(WT_Fill_Pattern const& pattern) 
        ;

    //Font
    ///
    ///\copydoc WT_Class_Factory::Create_Font()  
    ///
    virtual WT_Font*                    Create_Font() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Font()  
    ///
    virtual WT_Font*                    Create_Font(WT_Font const& font) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Font(WT_String const &name, WT_Boolean bold, WT_Boolean italic, WT_Boolean underline,WT_Byte charset, WT_Byte pitch, WT_Byte family, WT_Integer32 height, 
    ///                                                   WT_Unsigned_Integer16 rotation, WT_Unsigned_Integer16 width_scale, 
    ///                                                   WT_Unsigned_Integer16 spacing, WT_Unsigned_Integer16 oblique, 
    ///                                                   WT_Integer32 flags=0)
    ///
    virtual WT_Font*                    Create_Font(WT_String const &name, WT_Boolean bold, WT_Boolean italic, WT_Boolean underline, 
                                                   WT_Byte charset, WT_Byte pitch, WT_Byte family, WT_Integer32 height, 
                                                   WT_Unsigned_Integer16 rotation, WT_Unsigned_Integer16 width_scale, 
                                                   WT_Unsigned_Integer16 spacing, WT_Unsigned_Integer16 oblique, 
                                                   WT_Integer32 flags=0)
        ;

    //Font Extension											   
    ///
    ///\copydoc WT_Class_Factory::Create_Font_Extension() 
    ///
    virtual WT_Font_Extension* Create_Font_Extension() 
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_Font_Extension(WT_String const &, WT_String const &)
    ///
    virtual WT_Font_Extension* Create_Font_Extension(WT_String const & logfont_name, WT_String const & cannonical_name) 
        ;

	//Font Options
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Charset() 
    ///
	virtual WT_Font_Option_Charset*		Create_Font_Option_Charset() 
		;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Charset(WT_Byte charset) 
    ///
 	virtual WT_Font_Option_Charset*		Create_Font_Option_Charset(WT_Byte charset) 
		;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Family() 
    ///
	virtual WT_Font_Option_Family*		Create_Font_Option_Family() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Family(WT_Byte family) 
    ///
 	virtual WT_Font_Option_Family*		Create_Font_Option_Family(WT_Byte family) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Flags() 
    ///
 	virtual	WT_Font_Option_Flags*		Create_Font_Option_Flags() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Flags(WT_Integer32 flags) 
    ///
 	virtual WT_Font_Option_Flags*		Create_Font_Option_Flags(WT_Integer32 flags) 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Font_Name() 
    ///
	virtual WT_Font_Option_Font_Name*	Create_Font_Option_Font_Name() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Font_Name(WT_String const & name) 
    ///
 	virtual WT_Font_Option_Font_Name*	Create_Font_Option_Font_Name(WT_String const & name) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Height() 
    ///
	virtual WT_Font_Option_Height*		Create_Font_Option_Height() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Height(WT_Integer32 height) 
    ///
 	virtual WT_Font_Option_Height*		Create_Font_Option_Height(WT_Integer32 height) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Oblique() 
    ///
	virtual WT_Font_Option_Oblique*		Create_Font_Option_Oblique() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Oblique(WT_Unsigned_Integer16 oblique) 
    ///
 	virtual WT_Font_Option_Oblique*		Create_Font_Option_Oblique(WT_Unsigned_Integer16 oblique) 
		;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Pitch() 
    ///
 	virtual WT_Font_Option_Pitch*		Create_Font_Option_Pitch() 
        ; 	
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Pitch(WT_Byte pitch) 
    ///
	virtual WT_Font_Option_Pitch*		Create_Font_Option_Pitch(WT_Byte pitch) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Rotation() 
    ///
	virtual	WT_Font_Option_Rotation*	Create_Font_Option_Rotation() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Rotation(WT_Unsigned_Integer16 rotation) 
    ///
 	virtual WT_Font_Option_Rotation*	Create_Font_Option_Rotation(WT_Unsigned_Integer16 rotation) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Spacing() 
    ///
	virtual	WT_Font_Option_Spacing*		Create_Font_Option_Spacing() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Spacing(WT_Unsigned_Integer16 spacing) 
    ///
 	virtual WT_Font_Option_Spacing*		Create_Font_Option_Spacing(WT_Unsigned_Integer16 spacing) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Style(WT_Boolean bold=WD_False, WT_Boolean italic=WD_False, 
	///															WT_Boolean underlined=WD_False) 
    ///
	virtual WT_Font_Option_Style*		Create_Font_Option_Style(WT_Boolean bold=WD_False, WT_Boolean italic=WD_False, 
										WT_Boolean underlined=WD_False) 
        ;
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Width_Scale() 
    ///
	virtual WT_Font_Option_Width_Scale*	Create_Font_Option_Width_Scale() 
        ;
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Width_Scale(WT_Unsigned_Integer16 width_scale) 
    ///
 	virtual WT_Font_Option_Width_Scale* Create_Font_Option_Width_Scale(WT_Unsigned_Integer16 width_scale) 
        ;

    //Gouraud Polyline
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polyline() 
    ///
    virtual WT_Gouraud_Polyline*        Create_Gouraud_Polyline() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polyline(int count, WT_Logical_Point const *points, 
    ///                                                          WT_RGBA32 const *colors, WT_Boolean copy)
    ///
    virtual WT_Gouraud_Polyline*        Create_Gouraud_Polyline(int count, WT_Logical_Point const *points, 
                                                              WT_RGBA32 const *colors, WT_Boolean copy)
        ;     

	///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polyline(WT_Gouraud_Polyline const& pline) 
    ///
	 virtual WT_Gouraud_Polyline*		Create_Gouraud_Polyline(WT_Gouraud_Polyline const& pline)
        ;

    //Gouraud Polytriangle
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polytriangle() 
    ///
    virtual WT_Gouraud_Polytriangle*    Create_Gouraud_Polytriangle() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polytriangle(int count, WT_Logical_Point const *points, 
    ///                                                              WT_RGBA32 const *colors, WT_Boolean copy)
    ///
    virtual WT_Gouraud_Polytriangle*    Create_Gouraud_Polytriangle(int count, WT_Logical_Point const *points, 
                                                                  WT_RGBA32 const *colors, WT_Boolean copy)
        ;
      
	///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polytriangle(WT_Gouraud_Polytriangle const& polytri) 
    ///
	virtual WT_Gouraud_Polytriangle*		Create_Gouraud_Polytriangle(WT_Gouraud_Polytriangle const& /*polytri*/)
        ;

    //Image
    ///
    ///\copydoc WT_Class_Factory::Create_Image() 
    ///
    virtual WT_Image*                    Create_Image() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Image(int image_type) 
    ///
    virtual WT_Image*                    Create_Image(int image_type) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_Image(WT_Unsigned_Integer16 rows, 
    ///                                                 WT_Unsigned_Integer16 columns, 
    ///                                                 WT_Image::WT_Image_Format format, WT_Integer32 identifier, 
    ///                                                 WT_Color_Map const *color_map, WT_Integer32 data_size, 
    ///                                                 WT_Byte *data, WT_Logical_Point const &min_corner, 
    ///                                                 WT_Logical_Point const &max_corner, WT_Boolean copy)
    ///
    virtual WT_Image*                    Create_Image(WT_Unsigned_Integer16 rows, 
                                                     WT_Unsigned_Integer16 columns, 
                                                     WT_Image::WT_Image_Format format, WT_Integer32 identifier, 
                                                     WT_Color_Map const *color_map, WT_Integer32 data_size, 
                                                     WT_Byte *data, WT_Logical_Point const &min_corner, 
                                                     WT_Logical_Point const &max_corner, WT_Boolean copy,
                                                     WT_Integer32 dpi=-1)
        ;

    //Layer
    ///
    ///\copydoc WT_Class_Factory::Create_Layer() 
    ///
    virtual WT_Layer*                    Create_Layer() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Layer(WT_File &file, WT_Integer32 layer_num, char const *layer_name=NULL)
    ///
    virtual WT_Layer*                    Create_Layer(WT_File &file, WT_Integer32 layer_num, char const *layer_name=NULL)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Layer(WT_File &file, WT_Integer32 layer_num, WT_Unsigned_Integer16 const *layer_name)
    ///
    virtual WT_Layer*                    Create_Layer(WT_File &file, WT_Integer32 layer_num, WT_Unsigned_Integer16 const *layer_name)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Layer(WT_Layer const &layer)
    ///
    virtual WT_Layer*                    Create_Layer(WT_Layer const &layer)
        ;
     
    //Line Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Line_Pattern() 
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Line_Pattern(WT_Line_Pattern::WT_Pattern_ID id) 
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern(WT_Line_Pattern::WT_Pattern_ID id)
        ;

	///
    ///\copydoc WT_Class_Factory::Create_Line_Pattern(WT_Line_Pattern const& pat) 
	///
	virtual WT_Line_Pattern*            Create_Line_Pattern(WT_Line_Pattern const& pat) 
        ;

    //Line Style    
    ///
    ///\copydoc WT_Class_Factory::Create_Line_Style() 
    ///
    virtual WT_Line_Style*                Create_Line_Style()
        ;

	///
    ///\copydoc WT_Class_Factory::Create_Line_Style(WT_Line_Style const &) 
	///
    virtual WT_Line_Style*                Create_Line_Style(WT_Line_Style const & /*style*/) 
        ;

	//Line Weight
	///
    ///\copydoc WT_Class_Factory::Create_Line_Weight() 
	///
	virtual WT_Line_Weight*				Create_Line_Weight() 
		;
 	///
    ///\copydoc WT_Class_Factory::Create_Line_Weight(WT_Integer32 weight) 
	///
 	virtual WT_Line_Weight*				Create_Line_Weight(WT_Integer32 weight) 
		;

    ///
    ///\copydoc WT_Class_Factory::Create_Line_Weight(WT_Line_Weight const &) 
    ///
    virtual WT_Line_Weight*             Create_Line_Weight(WT_Line_Weight const & /*weight*/) 
        ;

    //WT_Macro_Definition
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Definition()
    ///
    virtual WT_Macro_Definition*        Create_Macro_Definition()
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Definition( WT_Unsigned_Integer16 index,WT_Integer32 scale_units = 1 )
    ///
    virtual WT_Macro_Definition*        Create_Macro_Definition( WT_Unsigned_Integer16 index,WT_Integer32 scale_units = 1 )
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Draw()
    ///
    virtual WT_Macro_Draw*                Create_Macro_Draw() ;
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Draw(int count, WT_Logical_Point const * points,WT_Boolean copy )
    ///
    virtual WT_Macro_Draw*                Create_Macro_Draw(int count, WT_Logical_Point const * points,WT_Boolean copy )
        ; 

	///
	///\copydoc WT_Class_Factory::Create_Macro_Draw(WT_Macro_Draw const& draw) 
	///
	virtual WT_Macro_Draw*				Create_Macro_Draw(WT_Macro_Draw const& /*draw*/ )
        ;

    //Macro_Index
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Index()
    ///
    virtual WT_Macro_Index*                Create_Macro_Index()
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Index(WT_Integer32 macro)
    ///
    virtual WT_Macro_Index*                Create_Macro_Index(WT_Integer32 macro)
        ;

	///
	///\copydoc WT_Class_Factory::Create_Macro_Index(WT_Macro_Index const& draw) 
	///
	 virtual WT_Macro_Index*                Create_Macro_Index(WT_Macro_Index const& /*index*/) 
        ;

    //Macro_Scale
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Scale()
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale()
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Scale(WT_Integer32 macro_scale)
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale(WT_Integer32 macro_scale)
        ;

	///
	///\copydoc WT_Class_Factory::Create_Macro_Scale(WT_Macro_Scale const& scale) 
	///
	 virtual WT_Macro_Scale*                Create_Macro_Scale(WT_Macro_Scale const& /*scale*/) 
        ;

    //Merge_Control
    ///
    ///\copydoc WT_Class_Factory::Create_Merge_Control()
    ///
    virtual WT_Merge_Control*            Create_Merge_Control()
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Merge_Control(WT_Merge_Control::WT_Merge_Format merge)
    ///
    virtual WT_Merge_Control*            Create_Merge_Control(WT_Merge_Control::WT_Merge_Format merge)
        ;

	//Merge_Control
	///
	///\copydoc WT_Class_Factory::Create_Merge_Control(WT_Merge_Control const& merge_control) 
	///
	virtual WT_Merge_Control*            Create_Merge_Control(WT_Merge_Control const& merge_control) 
        ;

	//Named_View_List
	///
	///\copydoc WT_Class_Factory::Create_Named_View_List(void) 
	///
	virtual	WT_Named_View_List*				Create_Named_View_List(void) 
		;
 	///
	///\copydoc WT_Class_Factory::Create_Named_View(WT_Named_View_List const& named_view_list) 
	///
 	virtual WT_Named_View_List*				Create_Named_View_List(WT_Named_View_List const &named_view_list) 
		;

    //Named_View
    ///
    ///\copydoc WT_Class_Factory::Create_Named_View(void)
    ///
    virtual    WT_Named_View*                Create_Named_View(void)
        ;
     ///
    ///\copydoc WT_Class_Factory::Create_Named_View(WT_Named_View const &named_view)
    ///
    virtual WT_Named_View*                Create_Named_View(WT_Named_View const &named_view)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Named_View(WT_Logical_Box const &view, const char *name=NULL)
    ///
    virtual WT_Named_View*                Create_Named_View(WT_Logical_Box const &view, const char *name=NULL)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Named_View(WT_Logical_Box const &view, WT_Unsigned_Integer16 const *name)
    ///
    virtual WT_Named_View*                Create_Named_View(WT_Logical_Box const &view, WT_Unsigned_Integer16 const *name)
        ;

    //Object_Node
    ///
    ///\copydoc WT_Class_Factory::Create_Object_Node() 
    ///
    virtual WT_Object_Node*                Create_Object_Node() 
        ;
     ///
    ///\copydoc WT_Class_Factory::Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
    ///                                                      char const *object_node_name=NULL)
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
                                                          char const *object_node_name=NULL)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
    ///                                                     WT_Unsigned_Integer16 const *object_node_name)
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
                                                         WT_Unsigned_Integer16 const *object_node_name)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Object_Node(WT_Object_Node const &object_node)
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_Object_Node const &object_node)
        ;

    //Overpost
    ///
    ///\copydoc WT_Class_Factory::Create_Overpost() 
    ///
    virtual WT_Overpost*                Create_Overpost() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Overpost( WT_Overpost::WT_AcceptMode acceptMode,WT_Boolean renderEntities,WT_Boolean addExtents)
    ///
    virtual WT_Overpost*                Create_Overpost( WT_Overpost::WT_AcceptMode acceptMode,WT_Boolean renderEntities,WT_Boolean addExtents)
        ;

    //Pen_Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern() 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id) 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id) 
         ;
     ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id, 
    ///                                                      WT_Unsigned_Integer32 screening_percentage, 
    ///                                                      WT_Boolean colormap_flag, WT_Color_Map const *color_map, 
    ///                                                      WT_Boolean copy) 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id, 
                                                          WT_Unsigned_Integer32 screening_percentage, 
                                                          WT_Boolean colormap_flag, WT_Color_Map const *color_map, 
                                                          WT_Boolean copy) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern(WT_Pen_Pattern const &pattern) 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern const &pattern) 
         ;

    //PNG_Group4_Image
    ///
    ///\copydoc WT_Class_Factory::Create_PNG_Group4_Image()
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_PNG_Group4_Image(int image_type)
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image(int image_type) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_PNG_Group4_Image(int image_typeWT_Unsigned_Integer16 columns, 
    ///                                                          WT_PNG_Group4_Image::WT_PNG_Group4_Image_Format format, 
    ///                                                          WT_Integer32 identifier, 
    ///                                                          WT_Color_Map const *color_map, 
    ///                                                          WT_Integer32 data_size, 
    ///                                                          WT_Byte *data, 
    ///                                                          WT_Logical_Point const &min_corner, 
    ///                                                          WT_Logical_Point const &max_corner,     
    ///                                                          WT_Boolean copy)
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image(WT_Unsigned_Integer16 rows, 
                                                              WT_Unsigned_Integer16 columns, 
                                                              WT_PNG_Group4_Image::WT_PNG_Group4_Image_Format format, 
                                                              WT_Integer32 identifier, 
                                                              WT_Color_Map const *color_map, 
                                                              WT_Integer32 data_size, 
                                                              WT_Byte *data, 
                                                              WT_Logical_Point const &min_corner, 
                                                              WT_Logical_Point const &max_corner,     
                                                              WT_Boolean copy,
                                                              WT_Integer32 dpi=-1)  
        ;

    //Polygon
    ///
    ///\copydoc WT_Class_Factory::Create_Polygon() 
    ///
    virtual WT_Polygon*                    Create_Polygon() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Polygon(int count, WT_Logical_Point const *points, WT_Boolean copy) 
    ///
    virtual WT_Polygon*                    Create_Polygon(int count, WT_Logical_Point const *points, WT_Boolean copy) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_Polygon(const WT_Polygon &pline) 
    ///
    virtual WT_Polygon*                    Create_Polygon(const WT_Polygon &pline) 
         ;

    //Polyline
    ///
    ///\copydoc WT_Class_Factory::Create_Polyline() 
    ///
    virtual WT_Polyline*    Create_Polyline() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Polyline(
    ///                            int                         count, /**< The number of points in the array. */
    ///                            WT_Logical_Point const *    points, /**< Pointer to the array of points. */
    ///                            WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
    ///                            )
    ///
    virtual WT_Polyline*    Create_Polyline(
                                int                         count, /**< The number of points in the array. */
                                WT_Logical_Point const *    points, /**< Pointer to the array of points. */
                                WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
                                ) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Polyline(const WT_Polyline & pline) 
    ///
    virtual WT_Polyline*    Create_Polyline(const WT_Polyline & pline) 
        ;

    // Polymarker
    ///
    ///\copydoc WT_Class_Factory::Create_Polymarker()
    ///
    virtual WT_Polymarker* Create_Polymarker()
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_Polymarker( int, WT_Logical_Point const *, WT_Boolean )
    ///
    virtual WT_Polymarker* Create_Polymarker(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        )
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_Polymarker( WT_Polymarker const& )
    ///
	virtual WT_Polymarker* Create_Polymarker(WT_Polymarker const& marker) 
        ;

    //Polytriangle
    ///
    ///\copydoc WT_Class_Factory::Create_Polytriangle() 
    ///
    virtual WT_Polytriangle*            Create_Polytriangle() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Polytriangle(int count, WT_Logical_Point const *points, WT_Boolean copy)
    ///
    virtual WT_Polytriangle*            Create_Polytriangle(int count, WT_Logical_Point const *points, WT_Boolean copy)
        ; 

	///
	///\copydoc WT_Class_Factory::Create_Polytriangle(WT_Polytriangle const& polytriangle) 
	///
	virtual WT_Polytriangle*            Create_Polytriangle(WT_Polytriangle const& polytriangle) 
        ;

    //Projection
    ///
    ///\copydoc WT_Class_Factory::Create_Projection() 
    ///
    virtual WT_Projection*                Create_Projection() ;
     ///
    ///\copydoc WT_Class_Factory::Create_Projection(const WT_Projection::WT_Projection_Type projection_type) 
    ///
     virtual WT_Projection*                Create_Projection(const WT_Projection::WT_Projection_Type projection_type) 
        ;
 
	///
	///\copydoc WT_Class_Factory::Create_Projection(WT_Projection const& projection) 
	///
	 virtual WT_Projection*                Create_Projection(WT_Projection const& projection) 
        ;
    //Text
    ///
    ///\copydoc WT_Class_Factory::Create_Text() 
    ///
    virtual WT_Text*                    Create_Text() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Text(WT_Text const &) 
    ///
    virtual WT_Text*                    Create_Text(WT_Text const &) 
         ;
     ///
    ///\copydoc WT_Class_Factory::Create_Text(WT_Logical_Point const &position, 
    ///                                                WT_String const &string, 
    ///                                                WT_Logical_Point const *bbox, 
    ///                                                WT_Unsigned_Integer16 overscore_count, 
    ///                                                WT_Unsigned_Integer16 const *overscore_pos, 
    ///                                                WT_Unsigned_Integer16 underscore_count, 
    ///                                                WT_Unsigned_Integer16 const *underscore_pos)
    ///
    virtual WT_Text*                    Create_Text(WT_Logical_Point const &position, 
                                                    WT_String const &string, 
                                                    WT_Logical_Point const *bbox, 
                                                    WT_Unsigned_Integer16 overscore_count, 
                                                    WT_Unsigned_Integer16 const *overscore_pos, 
                                                    WT_Unsigned_Integer16 underscore_count, 
                                                    WT_Unsigned_Integer16 const *underscore_pos)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Text(WT_Logical_Point const &position, WT_String const &string)  
    ///
    virtual WT_Text*                    Create_Text(WT_Logical_Point const &position, WT_String const &string) 
         ;

    //Text_Background
    ///
    ///\copydoc WT_Class_Factory::Create_Text_Background() 
    ///
    virtual WT_Text_Background*            Create_Text_Background() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Text_Background(WT_Text_Background::eBackground background,WT_Integer32 offset)
    ///
    virtual WT_Text_Background*            Create_Text_Background(WT_Text_Background::eBackground background,WT_Integer32 offset)
        ;

	///
	///\copydoc WT_Class_Factory::Create_Text_Background(WT_Text_Background const& background) 
	///
	virtual WT_Text_Background*            Create_Text_Background(WT_Text_Background const& background)  
        ;

    //Text_HAlign
    ///
    ///\copydoc WT_Class_Factory::Create_Text_HAlign() 
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Text_HAlign(WT_Text_HAlign::WT_HAlign hAlign) 
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign(WT_Text_HAlign::WT_HAlign hAlign) 
        ;

	///
	///\copydoc WT_Class_Factory::Create_Text_HAlign(WT_Text_HAlign const& hAlign) 
	///
	virtual WT_Text_HAlign*                Create_Text_HAlign(WT_Text_HAlign const& hAlign) 
        ;

    //Text_VAlign
    ///
    ///\copydoc WT_Class_Factory::Create_Text_VAlign() 
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Text_VAlign(WT_Text_VAlign::WT_VAlign vAlign) 
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign(WT_Text_VAlign::WT_VAlign vAlign) 
        ;

	///
	///\copydoc WT_Class_Factory::Create_Text_VAlign(WT_Text_VAlign const& vAlign) 
	///
	virtual WT_Text_VAlign*                Create_Text_VAlign(WT_Text_VAlign const& vAlign) 
        ;

    //WT_Units
    ///
    ///\copydoc WT_Class_Factory::Create_Units() 
    ///
    virtual WT_Units*                    Create_Units() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Units(const WT_Units& u) 
    ///
    virtual WT_Units*                    Create_Units(WT_Units const& u)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Units(WT_Matrix const &xform, WT_String const &units) 
    ///
    virtual WT_Units*                    Create_Units(WT_Matrix const &xform, WT_String const &units) 
         ;

    //WT_URL
    ///
    ///\copydoc WT_Class_Factory::Create_URL() 
    ///
    virtual WT_URL*                        Create_URL() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_URL(WT_URL_Item &item) 
    ///
    virtual WT_URL*                        Create_URL(WT_URL_Item &item) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_URL(WT_Integer32 index, WT_Unsigned_Integer16 const *address, WT_Unsigned_Integer16 const *friendly_name)
    ///
    virtual WT_URL*                        Create_URL(WT_Integer32 index, WT_Unsigned_Integer16 const *address, WT_Unsigned_Integer16 const *friendly_name)
        ;
  
	///
	///\copydoc WT_Class_Factory::Create_URL(WT_URL const& url) 
	///
	 virtual WT_URL*                        Create_URL(WT_URL const& url) 
        ;

    //WT_User_Data
    ///
    ///\copydoc WT_Class_Factory::Create_UserData() 
    ///
    virtual WT_UserData*                Create_UserData() 
        ; 
    ///
    ///\copydoc WT_Class_Factory::Create_UserData(WT_String const &data_description)  
    ///
    virtual WT_UserData*                Create_UserData(WT_String const &data_description) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_UserData(WT_String const &data_description, WT_Integer32 data_size, 
    ///                                                    WT_Byte *data, WT_Boolean copy)  
    ///
    virtual WT_UserData*                Create_UserData(WT_String const &data_description, WT_Integer32 data_size, 
                                                        WT_Byte *data, WT_Boolean copy) 
        ;

    //User_Fill_Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern()   
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_Integer16 pattern_num)    
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 pattern_num) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_Integer16 pattern_num, WT_Unsigned_Integer16 rows,WT_Unsigned_Integer16 columns,
    ///                                                          WT_Unsigned_Integer32 data_size,WT_Byte const *data)    
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 pattern_num, WT_Unsigned_Integer16 rows,WT_Unsigned_Integer16 columns,
                                                              WT_Unsigned_Integer32 data_size,WT_Byte const *data)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_Integer16 pattern_num,WT_User_Fill_Pattern::Fill_Pattern * fill_pattern)    
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 pattern_num,WT_User_Fill_Pattern::Fill_Pattern * fill_pattern)
        ; 
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_User_Fill_Pattern const &)
    ///
    virtual    WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_User_Fill_Pattern const &)
        ;

    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern()
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern()
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern(WT_Integer16 hashpatnum) 
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_Integer16 hashpatnum) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern(WT_Integer16 hashpatnum,WT_Unsigned_Integer16 xsize,WT_Unsigned_Integer16 ysize)
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_Integer16 hashpatnum,WT_Unsigned_Integer16 xsize,WT_Unsigned_Integer16 ysize)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern(WT_User_Hatch_Pattern const & pattern)  
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_User_Hatch_Pattern const & pattern) 
        ;

    //WT_View
    ///
    ///\copydoc WT_Class_Factory::Create_View() 
    ///
    virtual WT_View*                    Create_View() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_View(WT_Logical_Box view)  
    ///
    virtual WT_View*                    Create_View(WT_Logical_Box view) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_View(char *name) 
    ///
    virtual WT_View*                    Create_View(char *name) 
         ;
    ///
    ///\copydoc WT_Class_Factory::Create_View(WT_View const &view) 
    ///
    virtual WT_View*                    Create_View(WT_View const &view) 
         ;

    //Viewport     
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport() 
    ///
    virtual WT_Viewport*                Create_Viewport() 
        ;
     ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_Viewport const &vport)  
    ///
    virtual WT_Viewport*                Create_Viewport(WT_Viewport const &vport) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_File &file, char const *name, 
    ///                                                    int num_points, WT_Logical_Point const *points, 
    ///                                                    WT_Boolean copy=WD_True)  
    ///
    virtual WT_Viewport*                Create_Viewport(WT_File &file, char const *name, 
                                                        int num_points, WT_Logical_Point const *points, 
                                                        WT_Boolean copy=WD_True)
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_File &file, WT_Unsigned_Integer16 const *name, 
    ///                                                    int num_points, WT_Logical_Point const *points, 
    ///                                                    WT_Boolean copy=WD_True)
    ///
    virtual WT_Viewport*                Create_Viewport(WT_File &file, WT_Unsigned_Integer16 const *name, 
                                                        int num_points, WT_Logical_Point const *points, 
                                                        WT_Boolean copy=WD_True) 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_File &file, WT_String const &name, 
    ///                                                    WT_Contour_Set const &boundary, WT_Boolean copy=WD_True) 
    ///
    virtual WT_Viewport*                Create_Viewport(WT_File &file, WT_String const &name, 
                                                        WT_Contour_Set const &boundary, WT_Boolean copy=WD_True)
        ;

    //Visibility
    ///
    ///\copydoc WT_Class_Factory::Create_Visibility() 
    ///
    virtual WT_Visibility*                Create_Visibility() 
        ;
    ///
    ///\copydoc WT_Class_Factory::Create_Visibility(WT_Boolean visible) 
    ///
    virtual WT_Visibility*                Create_Visibility(WT_Boolean visible) 
         ;

	///
	///\copydoc WT_Class_Factory::Create_Visibility(WT_Visibility const& visibility) 
	///
	virtual WT_Visibility*                Create_Visibility(WT_Visibility const& visibility) 
        ;

    ///
	///\copydoc WT_Class_Factory::Create_Unknown() 
	///
    virtual WT_Unknown*                   Create_Unknown() 
        ;


    //Destruction methods
    virtual void            Destroy(WT_Object*      pObject);
    virtual void            Destroy(WT_Attribute*   pAttribute);
    virtual void            Destroy(WT_File*        pFile);
    virtual void            Destroy(WT_Dash_Pattern* pDashPat);
    virtual void            Destroy(WT_Layer*       pLayer);
    virtual void            Destroy(WT_Named_View*  pNamedView);
    virtual void            Destroy(WT_Object_Node* pObjectNode);

};




#endif // XAML_CLASSFACTORY_HEADER
