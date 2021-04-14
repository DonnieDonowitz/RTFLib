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

    ERROR_TYPE openRTF(const char* filename, const char* fonts, const char* colors);
    ERROR_TYPE closeRTF();

    inline DOCUMENT_FORMAT* getDocumentFormat()             { return &documentFormat; }
    inline void setDocumentFormat(DOCUMENT_FORMAT df)       { documentFormat = df; }
    
    inline SECTION_FORMAT* getSectionFormat()               { return &sectionFormat; }
    inline void setSectionFormat(SECTION_FORMAT sf)         { sectionFormat = sf; }
    ERROR_TYPE startSection();

    inline PARAGRAPH_FORMAT* getParagraphFormat()           { return &paragraphFormat; }
    inline void setParagraphFormat(PARAGRAPH_FORMAT pf)     { paragraphFormat = pf; }
    ERROR_TYPE startParagraph(const char* text, bool isNew);

    inline TABLE_ROW_FORMAT* getTableRowFormat()            { return &tableRowFormat; }
    inline void setTableRowFormat(TABLE_ROW_FORMAT trf)     { tableRowFormat = trf; }
    ERROR_TYPE startTableRow(bool autofit);
    ERROR_TYPE endTableRow();

    inline TABLE_CELL_FORMAT* getTableCellFormat()          { return &tableCellFormat; }
    inline void setTableCellFormat(TABLE_CELL_FORMAT tcf)   { tableCellFormat = tcf; }
    ERROR_TYPE startTableCell(int colNum = 0, int rightMargin = 0);
    ERROR_TYPE endTableCell();

    ERROR_TYPE loadImage(const char* path, int width, int height);

    inline FOOTER_FORMAT* getFooterFormat()                 { return &footerFormat; }
    inline void setFooterFormat(FOOTER_FORMAT ff)           { footerFormat = ff; }
    ERROR_TYPE setFooter(const char* footerText);

    inline HEADER_FORMAT* getHeaderFormat()                 { return &headerFormat; }
    inline void setHeaderFormat(HEADER_FORMAT hf)           { headerFormat = hf; } 
    ERROR_TYPE setHeader(const char* headerText);

    ERROR_TYPE createInternalHyperlink(const char* bookmark, const char* linkName, bool isNewPar);
    ERROR_TYPE createExternalHyperlink(const char* URL, const char* linkName, bool isNewPar);

    ERROR_TYPE startBookmark(const char* bookName);
    ERROR_TYPE endBookmark(const char* bookName);

protected:
    ERROR_TYPE writeHeading();    
    ERROR_TYPE init();
    ERROR_TYPE writeDocumentFormat();
    ERROR_TYPE writeSectionFormat();
    ERROR_TYPE writeParagraphFormat();
    ERROR_TYPE writeFooterFormat();
    ERROR_TYPE writeHeaderFormat();

    void setFontTable(const char* fonts);
    void setColorTable(const char* colors);
    void setDefaultFormats();

    const char* binaryToHex(unsigned char* binary, int size);
    const char* getShadingName(int shadingType, bool isInTableCell);
    const char* getBorderName(int borderType);

    DOCUMENT_FORMAT documentFormat;
    SECTION_FORMAT sectionFormat;
    PARAGRAPH_FORMAT paragraphFormat;
    TABLE_ROW_FORMAT tableRowFormat;
    TABLE_CELL_FORMAT tableCellFormat;
    FOOTER_FORMAT footerFormat;
    HEADER_FORMAT headerFormat;

    FILE* file;
    char fontTable[4096];
    char colorTable[4096];
};

#endif 