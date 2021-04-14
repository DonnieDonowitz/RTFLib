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
    sectionFormat.isNew = true;

    if(!writeSectionFormat())
    {
        return SECTION_FORMAT_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::startParagraph(const char* text, bool isNew)
{
    paragraphFormat.text = text;
    paragraphFormat.isNew = isNew;

    if(!writeParagraphFormat())
    {
        return PARAGRAPH_FORMAT_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::startTableRow(bool autofit)
{
    char text[1024] = "\n\\trowd\\trgaph115\\trkeep";

    if(autofit)
    {
        strcat(text, "\\trautofit1");
    }

    switch(tableRowFormat.alignment)
    {
        case TABLE_ROW_TEXT_ALIGN_LEFT:     strcat(text, "\\trql"); break;
        case TABLE_ROW_TEXT_ALIGN_CENTER:   strcat(text, "\\trqc"); break;
        case TABLE_ROW_TEXT_ALIGN_RIGHT:    strcat(text, "\\trqr"); break;
        default:                            return TABLE_ROW_ERROR;    
    }

    sprintf(text, "\\trleft%d\\trrh%d\\trpaddb%d\\trpaddfb3\\trpaddl%d\\trpaddfl3\\trpaddr%d\\trpaddfr3\\trpaddt%d\\trpaddft3", tableRowFormat.leftIndent, tableRowFormat.height, tableRowFormat.marginTop, tableRowFormat.marginBottom, tableRowFormat.marginLeft, tableRowFormat.marginRight);
    
    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return TABLE_ROW_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::endTableRow()
{
    char text[] = "\n\\trgaph115\\row\\pard";

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return TABLE_ROW_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::startTableCell(int colNum, int rightMargin)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::endTableCell()
{
    char text[] = "\n\\cell";

     if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return TABLE_CELL_ERROR;
    }


    return SUCCESS;
}

ERROR_TYPE RTFLib::loadImage(const char* path, int width, int height)
{
    return SUCCESS;
}

ERROR_TYPE RTFLib::setFooter(const char* footerText)
{
    char footer[] = "{\\footer";

    if(fwrite(footer, 1, strlen(footer), file) < strlen(footer))
    {
        return FOOTER_ERROR;
    }

    if(!writeFooterFormat())
    {
        return FOOTER_ERROR;
    }

    strcat(_c(footerText), "{\\field{\\*\\fldinst PAGE}{\\fldrslt 1}}\\pard\\par}");

    if(fwrite(footerText, 1, strlen(footerText), file) < strlen(footerText))
    {
        return FOOTER_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::setHeader(const char* headerText)
{
    char header[] = "{\\header";

    if(fwrite(header, 1, strlen(header), file) < strlen(header))
    {
        return HEADER_ERROR;
    }

    if(!writeHeaderFormat())
    {
        return HEADER_ERROR;
    }

    strcat(_c(headerText), "\\pard\\par}");

    if(fwrite(headerText, 1, strlen(headerText), file) < strlen(headerText))
    {
        return HEADER_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::createInternalHyperlink(const char* bookmark, const char* linkName, bool isNewPar)
{
    char link[1024];

    sprintf(link, "{\\field {\\*\\fldinst HYPERLINK \\\\l \"%s\"}{\\fldrslt %s}}", bookmark, linkName);

    return startParagraph(link, isNewPar) == SUCCESS ? SUCCESS : HYPERLINK_ERROR ;
}

ERROR_TYPE RTFLib::createExternalHyperlink(const char* URL, const char* linkName, bool isNewPar)
{
    char link[1024];

    sprintf(link, "{\\field {\\*\\fldinst HYPERLINK \"%s\"}{\\fldrslt %s}}", URL, linkName);

    return startParagraph(link, isNewPar) == SUCCESS ? SUCCESS : HYPERLINK_ERROR ;
}

ERROR_TYPE RTFLib::startBookmark(const char* bookName)
{
    char book[1024];
    
    sprintf(book, "{\\*\\bkmkstart %s}", bookName);

    if(fwrite(book, 1, strlen(book), file) < strlen(book))
    {
        return BOOKMARK_ERROR;
    }

    return SUCCESS;
}

ERROR_TYPE RTFLib::endBookmark(const char* bookName)
{
    char book[1024];
    
    sprintf(book, "{\\*\\bkmkend %s}", bookName);

    if(fwrite(book, 1, strlen(book), file) < strlen(book))
    {
        return BOOKMARK_ERROR;
    }

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
    char text[1024];

    sprintf(text, "\\viewkind%d\\viewscale%d\\paperw%d\\paperh%d\\margl%d\\margr%d\\margt%d\\margb%d\\gutter%d", documentFormat.viewKind, documentFormat.viewScale, documentFormat.paperWidth, documentFormat.paperHeight, documentFormat.marginLeft, documentFormat.marginRight, documentFormat.marginTop, documentFormat.marginBottom, documentFormat.gutterWidth);

    if(documentFormat.facingPages)
    {
        strcat(text, "\\facingp");
    }

    if(documentFormat.readOnly)
    {
        strcat(text, "\\annotprot");
    }

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return DOCUMENT_FORMAT_ERROR;
    }

    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeSectionFormat()
{
    char text[4096];

    strcat(text, "\n");

    if(sectionFormat.isNew)
    {
        strcat(text, "\\sect");
    }

    if(sectionFormat.isDefault)
    {
        strcat(text, "\\sectd");
    }

    switch(sectionFormat.breakType)
    {
        case SECTION_BREAK_CONTINUOUS:  strcat(text, "\\sbknone"); break;
        case SECTION_BREAK_COLUMN:      strcat(text, "\\sbkcol"); break;
        case SECTION_BREAK_PAGE:        strcat(text, "\\sbkpage"); break;
        case SECTION_BREAK_EVENPAGE:    strcat(text, "\\sbkeven"); break;
        case SECTION_BREAK_ODDPAGE:     strcat(text, "\\sbkodd"); break;
        default:                        return SECTION_FORMAT_ERROR;
    }

    if(sectionFormat.hasCols)
    {
        sprintf(text, "\\cols%d\\colsx%d", sectionFormat.colsNumber, sectionFormat.colsDistance);

        if(sectionFormat.hasColsLineBetween)
        {
            strcat(text, "\\linebetcol");
        }
    }

    sprintf(text, "\\pgwsxn%d\\pghsxn%d\\marglsxn%d\\margrsxn%d\\margtsxn%d\\margbsxn%d\\guttersxn%d\\headery%d\\footery%d", sectionFormat.width, sectionFormat.height, sectionFormat.marginLeft, sectionFormat.marginRight, sectionFormat.marginTop, sectionFormat.marginBottom, sectionFormat.gutterWidth, sectionFormat.headerOffset, sectionFormat.footerOffset);

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return SECTION_FORMAT_ERROR;
    }

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
    char sep[] = ";";
    char fontTableEntry[4096];
    int fontCount = 0;
    
    char* token = strtok(_c(fonts), sep);

    while(token != NULL)
    {
        sprintf(fontTableEntry, "{\\f%d\\fnil\\fcharset0\\cpg1252 %s", fontCount, token);
        strcat(fontTable, fontTableEntry);
        token = strtok(NULL, sep);
        fontCount++;
    }
}  

void RTFLib::setColorTable(const char* colors)
{
    char sep[] = ";";
    char colorTableEntry[4096];
    int colCount = 0;

    char* token = strtok(_c(colors), sep);

    while(token != NULL)
    {
        sprintf(colorTableEntry, "\\red%s", token);
        token = strtok(NULL, sep);

        if(token != NULL)
        {
            sprintf(colorTableEntry, "\\green%s", token);
        }

        token = strtok(NULL, sep);
        
        if(token != NULL)
        {
            sprintf(colorTableEntry, "\\blue%s;", token);
        }

        strcat(colorTable, colorTableEntry);
        token = strtok(NULL, sep);
        colCount++;
    }
} 

void RTFLib::setDefaultFormats()
{
    DOCUMENT_FORMAT df = { DOCUMENT_VIEWKIND_PAGE, 100, 12240, 15840, 1800, 1800, 1440, 1440, 0, false, false };

    SECTION_FORMAT sf = { SECTION_BREAK_CONTINUOUS, 12240, 15840, 1800, 1800, 1440, 1440, 0, 720, 720, 1, 720, false, true, false, false };

    PARAGRAPH_FORMAT pf = { PARAGRAPH_BREAK_NONE, PARAGRAPH_ALIGN_LEFT, 0, 0, 0, 0, 0, 0, "", false, true, false, false, false, false, false, false };
    pf.tabs = { 0, PARAGRAPH_TAB_KIND_NONE, PARAGRAPH_TAB_LEAD_NONE };
    pf.shading = { 0, PARAGRAPH_SHADING_TYPE_FILL, 0, 0 };
    pf.nums = { 11, 360, char(0x95) };
    pf.borders = { PARAGRAPH_BORDER_KIND_NONE, PARAGRAPH_BORDER_TYPE_STHICK, 0, 0, 0 };
    pf.character = { 0, 0, 100, 0, 24, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false };

    TABLE_ROW_FORMAT trf = { TABLE_ROW_TEXT_ALIGN_LEFT, 0, 0, 0, 0, 0, 0 };

    TABLE_CELL_FORMAT tcf = { TABLE_CELL_TEXT_ALIGN_CENTER, 0, 0, 0, 0, TABLE_CELL_TEXT_DIRECTION_LRTB, false };
    tcf.shading = { 0, PARAGRAPH_SHADING_TYPE_FILL, 0, 0 };
    tcf.borderBottom.hasBorder = false;
    tcf.borderBottom.border = { PARAGRAPH_BORDER_KIND_NONE, PARAGRAPH_BORDER_TYPE_STHICK, 5, 0, 0 };
    tcf.borderTop.hasBorder = false;
    tcf.borderTop.border = { PARAGRAPH_BORDER_KIND_NONE, PARAGRAPH_BORDER_TYPE_STHICK, 5, 0, 0 };
    tcf.borderLeft.hasBorder = false;
    tcf.borderLeft.border = { PARAGRAPH_BORDER_KIND_NONE, PARAGRAPH_BORDER_TYPE_STHICK, 5, 0, 0 };
    tcf.borderRight.hasBorder = false;
    tcf.borderRight.border = { PARAGRAPH_BORDER_KIND_NONE, PARAGRAPH_BORDER_TYPE_STHICK, 5, 0, 0 };

    FOOTER_FORMAT ff = { FOOTER_ALIGN_LEFT, 0, 0, "", false, false, false };
    ff.tabs = { 0, PARAGRAPH_TAB_KIND_NONE, PARAGRAPH_TAB_LEAD_NONE };
    ff.shading = { 0, PARAGRAPH_SHADING_TYPE_FILL, 0, 0 };
    ff.character = { 0, 0, 100, 0, 24, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false };

    HEADER_FORMAT hf = { FOOTER_ALIGN_LEFT, 0, 0, "", false, false, false };
    hf.tabs = { 0, PARAGRAPH_TAB_KIND_NONE, PARAGRAPH_TAB_LEAD_NONE };
    hf.shading = { 0, PARAGRAPH_SHADING_TYPE_FILL, 0, 0 };
    hf.character = { 0, 0, 100, 0, 24, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false };

    setDocumentFormat(df);
    setSectionFormat(sf);
    setParagraphFormat(pf);
    setTableRowFormat(trf);
    setTableCellFormat(tcf);
    setFooterFormat(ff);
    setHeaderFormat(hf);
} 

const char* RTFLib::binaryToHex(unsigned char* binary, int size)
{
    char* hex = new char[2 * size];
    char p1,p2;

    for(int i = 0; i < size; ++i)
    {
        p1 = binary[i] / 16;
        if(p1 < 10)
        {
            p1 += 48;
        }
        else
        {
            switch(p1)
            {
                case 10:    p1 = 'a'; break;
                case 11:    p1 = 'b'; break;
                case 12:    p1 = 'c'; break;
                case 13:    p1 = 'd'; break;
                case 14:    p1 = 'e'; break;
                case 15:    p1 = 'f'; break;
                default:    break;
            }
        }

        p2 = binary[i] % 16;
        if(p2 < 10)
        {
            p2 += 48;
        }
        else
        {
            switch(p2)
            {
                case 10:    p2 = 'a'; break;
                case 11:    p2 = 'b'; break;
                case 12:    p2 = 'c'; break;
                case 13:    p2 = 'd'; break;
                case 14:    p2 = 'e'; break;
                case 15:    p2 = 'f'; break;
                default:    break;
            }
        }

        hex[2 * i] = p1;
        hex[2 * i + 1] = p2;
    }

    strcat(hex, "\0");
    
    return hex;
} 

const char* RTFLib::getShadingName(int shadingType, bool isInTableCell)
{
    char* shading = new char[20];

    if(!isInTableCell)
    {
        switch(shadingType)
        {
            case PARAGRAPH_SHADING_TYPE_FILL:       strcpy(shading,""); break;     
            case PARAGRAPH_SHADING_TYPE_HORIZ:      strcpy(shading,"bghoriz"); break;      
            case PARAGRAPH_SHADING_TYPE_VERT:       strcpy(shading,"bgvert"); break;     
            case PARAGRAPH_SHADING_TYPE_FDIAG:      strcpy(shading,"bgfdiag"); break;      
            case PARAGRAPH_SHADING_TYPE_BDIAG:      strcpy(shading,"bgbdiag"); break;      
            case PARAGRAPH_SHADING_TYPE_CROSS:      strcpy(shading,"bgcross"); break;      
            case PARAGRAPH_SHADING_TYPE_CROSSD:     strcpy(shading,"bgdcross"); break;       
            case PARAGRAPH_SHADING_TYPE_DHORIZ:     strcpy(shading,"bgdkhoriz"); break;       
            case PARAGRAPH_SHADING_TYPE_DVERT:      strcpy(shading,"bgdkvert"); break;      
            case PARAGRAPH_SHADING_TYPE_DFDIAG:     strcpy(shading,"bgdkfdiag"); break;       
            case PARAGRAPH_SHADING_TYPE_DBDIAG:     strcpy(shading,"bgdkbdiag"); break;       
            case PARAGRAPH_SHADING_TYPE_DCROSS:     strcpy(shading,"bgdkcross"); break;       
            case PARAGRAPH_SHADING_TYPE_DCROSSD:    strcpy(shading,"bgdkdcross"); break;
            default:                                return nullptr;                
        }
    }
    else
    {
        switch(shadingType)
        {
            case TABLE_CELL_SHADING_TYPE_FILL:      strcpy(shading,""); break;     
            case TABLE_CELL_SHADING_TYPE_HORIZ:     strcpy(shading,"clbghoriz"); break;     
            case TABLE_CELL_SHADING_TYPE_VERT:      strcpy(shading,"clbgvert"); break;     
            case TABLE_CELL_SHADING_TYPE_FDIAG:     strcpy(shading,"clbgfdiag"); break;     
            case TABLE_CELL_SHADING_TYPE_BDIAG:     strcpy(shading,"clbgbdiag"); break;     
            case TABLE_CELL_SHADING_TYPE_CROSS:     strcpy(shading,"clbgcross"); break;     
            case TABLE_CELL_SHADING_TYPE_CROSSD:    strcpy(shading,"clbgdcross"); break;    
            case TABLE_CELL_SHADING_TYPE_DHORIZ:    strcpy(shading,"clbgdkhoriz"); break;   
            case TABLE_CELL_SHADING_TYPE_DVERT:     strcpy(shading,"clbgdkvert"); break;    
            case TABLE_CELL_SHADING_TYPE_DFDIAG:    strcpy(shading,"clbgdkfdiag"); break;   
            case TABLE_CELL_SHADING_TYPE_DBDIAG:    strcpy(shading,"clbgdkbdiag"); break;   
            case TABLE_CELL_SHADING_TYPE_DCROSS:    strcpy(shading,"clbgdkcross"); break;   
            case TABLE_CELL_SHADING_TYPE_DCROSSD:   strcpy(shading,"clbgdkdcross"); break;
            default:                                return nullptr;                
        }
    }

    return shading;
} 

const char* RTFLib::getBorderName(int borderType)
{
    char* border = new char[20];

    switch(borderType)
    {
        case PARAGRAPH_BORDER_TYPE_STHICK:          strcpy(border,"\\brdrs"); break; 	
        case PARAGRAPH_BORDER_TYPE_DTHICK:  	    strcpy(border,"\\brdrth"); break;
        case PARAGRAPH_BORDER_TYPE_SHADOW:          strcpy(border,"\\brdrsh"); break;
        case PARAGRAPH_BORDER_TYPE_DOUBLE:	        strcpy(border,"\\brdrdb"); break;     
        case PARAGRAPH_BORDER_TYPE_DOT:		        strcpy(border,"\\brdrdot"); break;
        case PARAGRAPH_BORDER_TYPE_DASH:		    strcpy(border,"\\brdrdash"); break;        
        case PARAGRAPH_BORDER_TYPE_HAIRLINE:	    strcpy(border,"\\brdrhair"); break;   
        case PARAGRAPH_BORDER_TYPE_INSET:		    strcpy(border,"\\brdrinset"); break;
        case PARAGRAPH_BORDER_TYPE_SDASH:		    strcpy(border,"\\brdrdashsm"); break;   
        case PARAGRAPH_BORDER_TYPE_DOTDASH:	        strcpy(border,"\\brdrdashd"); break;   
        case PARAGRAPH_BORDER_TYPE_DOTDOTDASH:      strcpy(border,"\\brdrdashdd"); break;
        case PARAGRAPH_BORDER_TYPE_OUTSET:	        strcpy(border,"\\brdroutset"); break;
        case PARAGRAPH_BORDER_TYPE_TRIPLE:	        strcpy(border,"\\brdrtriple"); break;   
        case PARAGRAPH_BORDER_TYPE_WAVY:            strcpy(border,"\\brdrwavy"); break;   
        case PARAGRAPH_BORDER_TYPE_DWAVY:		    strcpy(border,"\\brdrwavydb"); break;   
        case PARAGRAPH_BORDER_TYPE_STRIPED:	        strcpy(border,"\\brdrdashdotstr"); break;
        case PARAGRAPH_BORDER_TYPE_EMBOSS:	        strcpy(border,"\\brdremboss"); break;          
        case PARAGRAPH_BORDER_TYPE_ENGRAVE:	        strcpy(border,"\\brdrengrave"); break;   
        default:                                    return nullptr;    
    }

    return border;
} 
