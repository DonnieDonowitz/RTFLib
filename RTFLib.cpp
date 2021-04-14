#include "RTFLib.h"

ERROR_TYPE RTFLib::openRTF(const char* filename, const char* fonts, const char* colors)
{
    if(fonts && strcmp(fonts, "") != 0) 
    {
        setFontTable(fonts);
    }

    if(colors && strcmp(colors, "") != 0)
    {
        setColorTable(colors);
    }

    file = fopen(filename, "w");

    if(file)
    {
        if(!writeHeading())
        {
            return HEADING_FORMAT_ERROR;
        }
        
        if(!writeDocumentFormat())
        {
            return DOCUMENT_FORMAT_ERROR;
        }

        if(!writeSectionFormat())
        {
            return SECTION_FORMAT_ERROR;
        }
    }
    else
    {
        return OPEN_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::closeRTF()
{
    char text[1024] = "\n\\par}";

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return CLOSE_ERROR;
    }

    if(fclose(file))
    {
        return CLOSE_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::startSection()
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::startParagraph(const char* text, bool isNew)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::startTableRow(bool autofit)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::endTableRow()
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::startTableCell(int colNum, int rightMargin)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::endTableCell()
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::loadImage(const char* path, int width, int height)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::setFooter(const char* footerText)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::setHeader(const char* headerText)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::createInternalHyperlink(const char* bookmark, const char* linkName, bool isNewPar)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::createExternalHyperlink(const char* URL, const char* linkName, bool isNewPar)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::startBookmark(const char* bookName)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::endBookmark(const char* bookName)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::writeHeading()
{
    char text[1024];

    strcpy(text, "{\\rtf1\\ansi\\ansicpg1252\\deff0{\\fonttbl");
    strcat(text, fontTable);
    strcat(text, "}{\\colortbl");
    strcat(text, colorTable);
    strcat(text, "}{\\*\\generator RTFLib v1.0;}\n{\\info{\\author DonnieDonowitz}{\\company Angelo Marino Carmollingo}}");

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return HEADING_FORMAT_ERROR;
    }

    return SUCCESS;
}  

ERROR_TYPE RTFLib::init()
{
    strcpy(fontTable, "{\\f0\\froman\\fcharset0\\cpg1252 Times New Roman}{\\f1\\fswiss\\fcharset0\\cpg1252 Arial}{\\f2\\fmodern\\fcharset0\\cpg1252 Courier New}{\\f3\\fscript\\fcharset0\\cpg1252 Cursive}{\\f4\\fdecor\\fcharset0\\cpg1252 Old English}{\\f5\\ftech\\fcharset0\\cpg1252 Symbol}{\\f6\\fbidi\\fcharset0\\cpg1252 Miriam}");
    strcpy(colorTable,"\\red0\\green0\\blue0;\\red255\\green0\\blue0;\\red0\\green255\\blue0;\\red0\\green0\\blue255;\\red255\\green255\\blue0;\\red255\\green0\\blue255;\\red0\\green255\\blue255;\\red255\\green255\\blue255;\\red128\\green0\\blue0;\\red0\\green128\\blue0;\\red0\\green0\\blue128;\\red128\\green128\\blue0;\\red128\\green0\\blue128;\\red0\\green128\\blue128;\\red128\\green128\\blue128;");
    
    setDefaultFormats();
    if(strcmp(fontTable, "") == 0 || strcmp(colorTable, "") == 0)
    {
        return INIT_ERROR;
    }
    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeDocumentFormat()
{
    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeSectionFormat()
{
    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeParagraphFormat()
{
    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeFooterFormat()
{
    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeHeaderFormat()
{
    return SUCCESS;
}  

void RTFLib::setFontTable(const char* fonts)
{

}  

void RTFLib::setColorTable(const char* colors)
{
  
} 

void RTFLib::setDefaultFormats()
{
    DOCUMENT_FORMAT df = { DOCUMENT_VIEWKIND_PAGE, 100, 12240, 15840, 1800, 1800, 1440, 1440, 0, false, false };

    SECTION_FORMAT sf = { SECTION_BREAK_CONTINUOUS, 12240, 15840, 1800, 1800, 1440, 1440, 0, 720, 720, 1, 720, false, true, false, false };

    PARAGRAPH_FORMAT pf = { PARAGRAPH_BREAK_NONE, PARAGRAPH_ALIGN_LEFT, 0, 0, 0, 0, 0, 0, "", false, true, false, false, false, false, false, false };
    pf.tabs = {}
} 

const char* RTFLib::binaryToHex(unsigned char* binary, int size)
{
    return nullptr;
} 

const char* RTFLib::getShadingName(int shadingType, bool isInTableCell)
{
    return nullptr;
} 

const char* RTFLib::getBorderName(int borderType)
{
    return nullptr;
} 
