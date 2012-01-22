/*
 *  Client/Text/Font.cpp
 *  This file is part of the "Dream" project, and is licensed under the GNU GPLv3.
 *
 *  Created by Samuel Williams on 8/05/07.
 *  Copyright 2007 Samuel Williams. All rights reserved.
 *
 */

#include "Font.h"

#include "../Resources/Loader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "FontFace.h"
#include "TextBlock.h"

namespace Dream
{
	namespace Text
	{
		
		const char * ft2_error_message(int code);
		
		FT_Library freetypeLibrary ()
		{
			using namespace std;
			static FT_Library library = NULL;
			
			if (library == NULL) {
				FT_Error err = FT_Init_FreeType(&library);
				
				if (err) {
					cerr << "Error loading freetype: " << err << endl;
				}
			}
			
			return library;
		}
		
		using namespace std;
		
		void Font::Loader::registerLoaderTypes (ILoader * loader) {
			loader->setLoaderForExtension(this, "ttf");
			loader->setLoaderForExtension(this, "dfont");
		}
		
		REF(Object) Font::Loader::initWithPath(const Path & p) {
			return new Font(p);
		}
		
		REF(Object) Font::Loader::loadFromData(const PTR(IData) data, const ILoader * loader) {
			return new Font(data);
		}
		
		Font::Font (const Path & p) : m_face(NULL)
		{
			FT_Face face;
			FT_Error err = FT_New_Face(freetypeLibrary(), p.toLocalPath().c_str(), 0, &face);
			
			if (err) {
				throw LoadError(StringT("Error loading freetype font (") + p.toLocalPath() + "): " + ft2_error_message(err));
			} else {
				m_face = new Detail::FontFace(face, ALPHA);
			}
			
			setPixelSize(12);
		}
		
		Font::Font (const PTR(IData) data) : m_face(NULL)
		{
			Shared<Buffer> buffer = data->buffer();
			
			FT_Face face;
			FT_Error err = FT_New_Memory_Face(freetypeLibrary(), buffer->begin(), buffer->size(), 0, &face);
			m_fontData = data;
			
			if (err) {
				throw LoadError(StringT("Error loading freetype font:") + ft2_error_message(err));
			} else {
				m_face = new Detail::FontFace(face, ALPHA);
			}
			
			setPixelSize(12);
		}
		
		Font::~Font ()
		{
			if (m_face) {
				delete m_face;
				m_face = NULL;
			}
		}
		
		Detail::FontFace * Font::fontFace ()
		{
			return m_face;
		}
		
		const Detail::FontFace * Font::fontFace () const 
		{
			return m_face;
		}
		
		void Font::setPixelSize(unsigned sz)
		{
			ensure(m_face != NULL);
			
			FT_Error err = FT_Set_Pixel_Sizes(m_face->face(), sz, sz);
			
			ensure(err == 0);
		}
		
		Vector<2, unsigned> Font::computeBoundingBox (const std::wstring &text) const
		{
			ensure(m_face != NULL);
			
			return m_face->processText(text, REF(Image)());
		}
		
		REF(Image) Font::renderText (const std::wstring & text)
		{
			ensure(m_face != NULL);
			
			REF(Image) img = new Image(computeBoundingBox(text) << 1U, m_face->pixelFormat(), UBYTE);
			img->zero();
			
			m_face->processText(text, img);
			
			return img;
		}
		
		IndexT Font::singleLineOffset ()
		{
			return m_face->lineOffset();
		}
		
		// uses a method described in fterrors.h to build an error translation function
#undef __FTERRORS_H__
#define FT_ERRORDEF(e, v, s)			case v: return s;
#define FT_ERROR_START_LIST
#define FT_ERROR_END_LIST
		
		const char * ft2_error_message(int code) {
			switch (code) {
#include FT_ERRORS_H
			}
			
			std::cerr << __PRETTY_FUNCTION__ << "unknown error code: " << code << std::endl;
			return "unknown error";
		}
		
	}
}