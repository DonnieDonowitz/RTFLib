#ifndef RTFLIB_H
#define RTFLIB_H

#include "RTFErrors.h"
#include "RTFGlobals.h"
#include "RTFStructures.h"

#define _c(x) const_cast<char*>(x)

class RTFLib 
{
public:
    RTFLib() {};
    ~RTFLib() {};

    int openRTF(const char* filename, const char* fonts, const char* colors);
    int closeRTF();
    inline DOCUMENT_FORMAT* getDocumentFormat() { return &documentFormat; }

protected:
    int writeHeading();    
    int init();
    void setFontTable(const char* fonts);
    void setColorTable(const char* colors);


    DOCUMENT_FORMAT documentFormat;
    SECTION_FORMAT sectionFormat;
    PARAGRAPH_FORMAT paragraphFormat;
    TABLE_ROW_FORMAT tableRowFormat;
    TABLE_CELL_FORMAT tableCellFormat;
    FOOTER_FORMAT footerFormat;
    HEADER_FORMAT headerFormat;
};

#endif 