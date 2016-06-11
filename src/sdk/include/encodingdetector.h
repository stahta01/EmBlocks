/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */
/*
    This file is part of Em::Blocks.

    Copyright (c) 2011-2013 Em::Blocks

    Em::Blocks is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Em::Blocks is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Em::Blocks.  If not, see <http://www.gnu.org/licenses/>.

	@version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:
*/

#ifndef ENCODINGDETECTOR_H
#define ENCODINGDETECTOR_H

#include "settings.h"
#include "filemanager.h"
#include <wx/fontmap.h>
#include "nsUniversalDetector.h"

class wxString;

/** Try to detect the encoding of a file on disk. */
class DLLIMPORT EncodingDetector :
            public nsUniversalDetector
{
	public:
		EncodingDetector(const wxString& filename, bool useLog=true);
		EncodingDetector(LoaderBase* fileLdr, bool useLog=true);
		EncodingDetector(const wxByte* buffer, size_t size, bool useLog=true);
		EncodingDetector(const EncodingDetector& rhs, bool useLog=true);
		~EncodingDetector();

        const wxString& DoIt(const char* aBuf, PRUint32 aLen);
        /** @return True if file was read, false if not. */
        bool IsOK() const;
        /** @return True if the file contains a BOM (Byte Order Mark), false if not. */
		bool UsesBOM() const;
		/** @return The BOM size in bytes. Returns zero if no BOM. */
		int GetBOMSizeInBytes() const;
		/** @return The detected encoding. Currently ISO8859-1 is returned if no BOM is present. */
		wxFontEncoding GetFontEncoding() const;
		wxString GetWxStr() const;
	protected:
        /** @return True if succeeded, false if not (e.g. file didn't exist). */
        bool DetectEncoding(const wxString& filename, bool ConvertToWxString = true);
        bool DetectEncoding(const wxByte* buffer, size_t size, bool ConvertToWxString = true);
        void Report(const char* aCharset);

        bool m_IsOK;
        bool m_UseBOM;
        bool m_UseLog;
        int m_BOMSizeInBytes;
        wxFontEncoding m_Encoding;
	private:
        wxString mResult;
        wxString m_ConvStr;
        bool ConvertToWxStr(const wxByte* buffer, size_t size);
//        static inline bool IsUTF8Tail(wxByte b) { return ((b & 0xC0) == 0x80); };
//        bool DetectUTF8(const wxByte* byt, size_t size);
        bool IsTextUTF16BE(const wxByte *text, size_t size);
        bool IsTextUTF16LE(const wxByte *text, size_t size);
        bool DetectUTF16(const wxByte* byt, size_t size);
        bool IsTextUTF32BE(const wxByte *text, size_t size);
        bool IsTextUTF32LE(const wxByte *text, size_t size);
        bool DetectUTF32(const wxByte* byt, size_t size);
};

#endif // ENCODINGDETECTOR_H
