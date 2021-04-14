#include "RTFLib.h"

ERROR_TYPE RTFLib::openRTF(const char* filename, const char* fonts, const char* colors)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::closeRTF()
{
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
    return SUCCESS;
}  

ERROR_TYPE RTFLib::init()
{
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
