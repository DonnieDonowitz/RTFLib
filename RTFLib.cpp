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
    char text[4096];

    strcat(text, "\n\\tcelld");

    switch(tableCellFormat.textAlignment)
    {
        case TABLE_CELL_TEXT_ALIGN_TOP:         strcat(text, "\\clvertalt"); break;
        case TABLE_CELL_TEXT_ALIGN_CENTER:      strcat(text, "\\clvertalc"); break;
        case TABLE_CELL_TEXT_ALIGN_BOTTOM:      strcat(text, "\\clvertalb"); break;
        default:                                return TABLE_CELL_ERROR;
    }

    switch(tableCellFormat.textDirection)
    {
        case TABLE_CELL_TEXT_DIRECTION_LRTB:        strcat(text, "\\cltxlrtb"); break;
        case TABLE_CELL_TEXT_DIRECTION_RLTB:        strcat(text, "\\cltxtbrl"); break;
        case TABLE_CELL_TEXT_DIRECTION_LRBT:        strcat(text, "\\cltxbtlr"); break;
        case TABLE_CELL_TEXT_DIRECTION_LRTBV:       strcat(text, "\\cltxlrtbv"); break;
        case TABLE_CELL_TEXT_DIRECTION_RLTBV:       strcat(text, "\\cltxttbrlv"); break;   
        default:                                    return TABLE_CELL_ERROR;     
    }

    if(tableCellFormat.borderBottom.hasBorder)
    {
        sprintf(text, "\\clbrdrb%s\\brdrw%d\\brsp%d\\brdrcf%d", getBorderName(tableCellFormat.borderBottom.border.type), tableCellFormat.borderBottom.border.width, tableCellFormat.borderBottom.border.space, tableCellFormat.borderBottom.border.color);
    }

    if(tableCellFormat.borderTop.hasBorder)
    {
        sprintf(text, "\\clbrdrt%s\\brdrw%d\\brsp%d\\brdrcf%d", getBorderName(tableCellFormat.borderTop.border.type), tableCellFormat.borderTop.border.width, tableCellFormat.borderTop.border.space, tableCellFormat.borderTop.border.color);
    }

    if(tableCellFormat.borderLeft.hasBorder)
    {
        sprintf(text, "\\clbrdrl%s\\brdrw%d\\brsp%d\\brdrcf%d", getBorderName(tableCellFormat.borderLeft.border.type), tableCellFormat.borderLeft.border.width, tableCellFormat.borderLeft.border.space, tableCellFormat.borderLeft.border.color);
    }

    if(tableCellFormat.borderRight.hasBorder)
    {
        sprintf(text, "\\clbrdrr%s\\brdrw%d\\brsp%d\\brdrcf%d", getBorderName(tableCellFormat.borderRight.border.type), tableCellFormat.borderRight.border.width, tableCellFormat.borderRight.border.space, tableCellFormat.borderRight.border.color);
    }

    if(tableCellFormat.hasShading)
    {
        sprintf(text, "%s\\clshdgn%d\\clcfpat%d\\clcbpat%d", getShadingName(tableCellFormat.shading.type, true), tableCellFormat.shading.intensity, tableCellFormat.shading.fillColor, tableCellFormat.shading.backgroundColor);
    }

    sprintf(text, "\\cellx%d", colNum == 0 ? colNum : rightMargin);

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return TABLE_CELL_ERROR;
    }

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
    FILE* f = fopen(path, "rb");
    unsigned char info[54];

    if(f == NULL)
    {
        return OPEN_ERROR;
    }

    fread(info, sizeof(unsigned char), 54, f);

    int w = *(int*) &info[18];
    int h = *(int*) &info[22];

    int size = 3 * w * h;

    unsigned char* data = new unsigned char[size];

    fread(data, sizeof(unsigned char), size, f);
    fclose(f);

    for(int i = 0; i < size; ++i)
    {
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
    }

    const char* hex = binaryToHex(data, size);
    char text[4096];

    sprintf(text, "{\\pict\\picw%d\\pich%d\\picwgoa%d\\pichgoal%d\\pngblip", w, h, width, height);

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return IMAGE_ERROR;
    }

    if(fwrite(hex, 1, strlen(hex), file) < strlen(hex))
    {
        return IMAGE_ERROR;
    }

    if(fwrite("}", 1, 1, file) < 1)
    {
        return IMAGE_ERROR;
    }

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

ERROR_TYPE RTFLib::writeHeaderFormat()
{
    char text[4096];

    strcat(text, "\\par");

    switch(headerFormat.alignment)
    {
        case HEADER_ALIGN_LEFT:         strcat(text, "\\ql"); break;
        case HEADER_ALIGN_CENTER:       strcat(text, "\\qc"); break;
        case HEADER_ALIGN_RIGHT:        strcat(text, "\\qr"); break;
        case HEADER_ALIGN_JUSTIFY:      strcat(text, "\\qj"); break;
        default:                        return HEADER_ERROR;
    }

    if(headerFormat.hasTabs)
    {
        switch(headerFormat.tabs.kind)
        {
            case HEADER_TAB_KIND_NONE:          strcat(text, ""); break;        
            case HEADER_TAB_KIND_CENTER:        strcat(text, "\\tqc"); break;
            case HEADER_TAB_KIND_RIGHT:         strcat(text, "\\tqr"); break;
            case HEADER_TAB_KIND_DECIMAL:       strcat(text, "\\tqdec"); break;
            default:                            return HEADER_ERROR;
        }

        switch(headerFormat.tabs.lead)
        {
            case HEADER_TAB_LEAD_NONE:          strcat(text, ""); break;
            case HEADER_TAB_LEAD_DOT:           strcat(text, "\\tldot"); break;
            case HEADER_TAB_LEAD_MDOT:          strcat(text, "\\tlmdot"); break;
            case HEADER_TAB_LEAD_HYPH:          strcat(text, "\\tlhyph"); break;
            case HEADER_TAB_LEAD_UNDERLINE:     strcat(text, "\\tlul"); break;
            case HEADER_TAB_LEAD_THICKLINE:     strcat(text, "\\tlth"); break;
            case HEADER_TAB_LEAD_EQUAL:         strcat(text, "\\tleq"); break;
            default:                            return HEADER_ERROR;
        }

        sprintf(text, "\\tx%d", headerFormat.tabs.position);
    }

    if(headerFormat.hasShading)
    {
        sprintf(text, "\\shading%d%s\\cfpat%d\\cbpat%d", headerFormat.shading.intensity, getShadingName(headerFormat.shading.type, false), headerFormat.shading.fillColor, headerFormat.shading.backgroundColor);
    }

    sprintf(text, "\\animtext%d\\expndtw%d\\kerning%d\\charscalex%d\\f%d\\fs%d\\cf%d", headerFormat.character.animated,headerFormat.character.expand, headerFormat.character.kerning, headerFormat.character.scale, headerFormat.character.fontNumber, headerFormat.character.fontSize, headerFormat.character.foregroundColor);

    if(headerFormat.character.isBold)
    {
        strcat(text, "\\b");
    }

    if(headerFormat.character.isCapital)
    {
        strcat(text, "\\caps");
    }

    if(headerFormat.character.isDoubleStrike)
    {
        strcat(text, "\\striked");
    }

    if(headerFormat.character.isEmboss)
    {
        strcat(text, "\\embo");
    }

    if(headerFormat.character.isEngrave)
    {
        strcat(text, "\\impr");
    }

    if(headerFormat.character.isItalic)
    {
        strcat(text, "\\i");
    }

    if(headerFormat.character.isOutline)
    {
        strcat(text, "\\outl");
    }

    if(headerFormat.character.isShadow)
    {
        strcat(text, "\\shad");
    }

    if(headerFormat.character.isSmallCapital)
    {
        strcat(text, "\\scaps");
    }

    if(headerFormat.character.isStrike)
    {
        strcat(text, "\\strike");
    }

    if(headerFormat.character.isSubscript)
    {
        strcat(text, "\\sub");
    }

    if(headerFormat.character.isSuperscript)
    {
        strcat(text, "\\super");
    }

    switch(headerFormat.character.underline)
    {
        case UNDERLINE_TYPE_NONE:                   strcat(text, "\\ulnone"); break; 
        case UNDERLINE_TYPE_CONTINUOUS:             strcat(text, "\\ul"); break;  
        case UNDERLINE_TYPE_DOT:                    strcat(text, "\\uld"); break;     
        case UNDERLINE_TYPE_DASH:                   strcat(text, "\\uldash"); break;     
        case UNDERLINE_TYPE_DASHDOT:                strcat(text, "\\uldashd"); break;     
        case UNDERLINE_TYPE_DASHDOTDOT:             strcat(text, "\\uldashdd"); break;         
        case UNDERLINE_TYPE_DOUBLE:                 strcat(text, "\\uldb"); break;         
        case UNDERLINE_TYPE_HWAVE:                  strcat(text, "\\ulhwave"); break;     
        case UNDERLINE_TYPE_LDASH:                  strcat(text, "\\ulldash"); break; 
        case UNDERLINE_TYPE_THICK:                  strcat(text, "\\ulth"); break;     
        case UNDERLINE_TYPE_THICKDOT:               strcat(text, "\\ulthd"); break; 
        case UNDERLINE_TYPE_THICKDASH:              strcat(text, "\\ulthdash"); break; 
        case UNDERLINE_TYPE_THICKDASHDOT:           strcat(text, "\\ulthdashd"); break; 
        case UNDERLINE_TYPE_THICKDASHDOTDOT:        strcat(text, "\\ulthdashdd"); break;         
        case UNDERLINE_TYPE_THICKLDASH:             strcat(text, "\\ulthldash"); break;     
        case UNDERLINE_TYPE_DOUBLEWAVE:             strcat(text, "\\ululdbwave"); break;   
        case UNDERLINE_TYPE_WORD:                   strcat(text, "\\ulw"); break; 
        case UNDERLINE_TYPE_WAVE:                   strcat(text, "\\ulwave"); break;     
        default:                                    return HEADER_ERROR;
    }

    if(headerFormat.isTabbed)
    {
        sprintf(text, " \\tab %s", headerFormat.text);
    }
    else
    {
        sprintf(text, "\\li%d\\ri%d %s", headerFormat.leftIndent, headerFormat.rightIndent, headerFormat.text);
    }

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return HEADER_ERROR;
    }


    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeParagraphFormat()
{
    char text[4096];

    strcat(text, "\n");

    if(paragraphFormat.isNew)
    {
        strcat(text, "\\par");
    }

    if(paragraphFormat.isDefault)
    {
        strcat(text, "\\pard");
    }

    if(paragraphFormat.isInTable)
    {
        strcat(text, "\\intbl");
    }
    else
    {
        strcat(text, "\\plain");
    }

    switch(paragraphFormat.breakType)
    {
        case PARAGRAPH_BREAK_NONE:      strcat(text, ""); break;      
        case PARAGRAPH_BREAK_PAGE:      strcat(text, "\\page"); break;
        case PARAGRAPH_BREAK_COLUMN:    strcat(text, "\\column"); break;
        case PARAGRAPH_BREAK_LINE:      strcat(text, "\\line"); break;
        default:                        return PARAGRAPH_FORMAT_ERROR;      
    }

    switch(paragraphFormat.alignment)
    {
        case PARAGRAPH_ALIGN_LEFT:      strcat(text, "\\ql"); break;        
        case PARAGRAPH_ALIGN_CENTER:    strcat(text, "\\qc"); break; 
        case PARAGRAPH_ALIGN_RIGHT:     strcat(text, "\\qr"); break; 
        case PARAGRAPH_ALIGN_JUSTIFY:   strcat(text, "\\qj"); break; 
        default:                        return PARAGRAPH_FORMAT_ERROR;      
    }

    if(paragraphFormat.hasTabs)
    {
        switch(paragraphFormat.tabs.kind)
        {
            case PARAGRAPH_TAB_KIND_NONE:       strcat(text, ""); break;        
            case PARAGRAPH_TAB_KIND_CENTER:     strcat(text, "\\tqc"); break;
            case PARAGRAPH_TAB_KIND_RIGHT:      strcat(text, "\\tqr"); break;
            case PARAGRAPH_TAB_KIND_DECIMAL:    strcat(text, "\\tqdec"); break;
            default:                            return PARAGRAPH_FORMAT_ERROR;
        }

        switch(paragraphFormat.tabs.lead)
        {
            case PARAGRAPH_TAB_LEAD_NONE:           strcat(text, ""); break;
            case PARAGRAPH_TAB_LEAD_DOT:            strcat(text, "\\tldot"); break;
            case PARAGRAPH_TAB_LEAD_MDOT:           strcat(text, "\\tlmdot"); break;
            case PARAGRAPH_TAB_LEAD_HYPH:           strcat(text, "\\tlhyph"); break;
            case PARAGRAPH_TAB_LEAD_UNDERLINE:      strcat(text, "\\tlul"); break;
            case PARAGRAPH_TAB_LEAD_THICKLINE:      strcat(text, "\\tlth"); break;
            case PARAGRAPH_TAB_LEAD_EQUAL:          strcat(text, "\\tleq"); break;
            default:                                return PARAGRAPH_FORMAT_ERROR;
        }

        sprintf(text, "\\tx%d", paragraphFormat.tabs.position);
    }

    if(paragraphFormat.hasNums)
    {
        sprintf(text, "{\\*\\pn\\pnlvl%d\\pnsp%d\\pntxtb %c}", paragraphFormat.nums.level, paragraphFormat.nums.space, paragraphFormat.nums.charNum);
    }

    if(paragraphFormat.hasBorders)
    {
        switch(paragraphFormat.borders.kind)
        {
            case PARAGRAPH_BORDER_KIND_NONE:    strcat(text, ""); break; 
            case PARAGRAPH_BORDER_KIND_TOP:     strcat(text, "\\brdrt"); break; 
            case PARAGRAPH_BORDER_KIND_BOTTOM:  strcat(text, "\\brdrb"); break; 
            case PARAGRAPH_BORDER_KIND_LEFT:    strcat(text, "\\brdrl"); break; 
            case PARAGRAPH_BORDER_KIND_RIGHT:   strcat(text, "\\brdrr"); break; 
            case PARAGRAPH_BORDER_KIND_BOX:     strcat(text, "\\box"); break; 
            default:                    return PARAGRAPH_FORMAT_ERROR;
        }

        strcat(text, getBorderName(paragraphFormat.borders.type));
        sprintf(text, "\\brdrw%d\\brsp%d\\brdrcf%d", paragraphFormat.borders.width, paragraphFormat.borders.space, paragraphFormat.borders.color);
    }

    if(paragraphFormat.hasShading)
    {
        sprintf(text, "\\shading%d%s\\cfpat%d\\cbpat%d", paragraphFormat.shading.intensity, getShadingName(paragraphFormat.shading.type, false), paragraphFormat.shading.fillColor, paragraphFormat.shading.backgroundColor);
    }

    sprintf(text, "\\animtext%d\\expndtw%d\\kerning%d\\charscalex%d\\f%d\\fs%d\\cf%d", paragraphFormat.character.animated,paragraphFormat.character.expand, paragraphFormat.character.kerning, paragraphFormat.character.scale, paragraphFormat.character.fontNumber, paragraphFormat.character.fontSize, paragraphFormat.character.foregroundColor);

    if(paragraphFormat.character.isBold)
    {
        strcat(text, "\\b");
    }

    if(paragraphFormat.character.isCapital)
    {
        strcat(text, "\\caps");
    }

    if(paragraphFormat.character.isDoubleStrike)
    {
        strcat(text, "\\striked");
    }

    if(paragraphFormat.character.isEmboss)
    {
        strcat(text, "\\embo");
    }

    if(paragraphFormat.character.isEngrave)
    {
        strcat(text, "\\impr");
    }

    if(paragraphFormat.character.isItalic)
    {
        strcat(text, "\\i");
    }

    if(paragraphFormat.character.isOutline)
    {
        strcat(text, "\\outl");
    }

    if(paragraphFormat.character.isShadow)
    {
        strcat(text, "\\shad");
    }

    if(paragraphFormat.character.isSmallCapital)
    {
        strcat(text, "\\scaps");
    }

    if(paragraphFormat.character.isStrike)
    {
        strcat(text, "\\strike");
    }

    if(paragraphFormat.character.isSubscript)
    {
        strcat(text, "\\sub");
    }

    if(paragraphFormat.character.isSuperscript)
    {
        strcat(text, "\\super");
    }

    switch(paragraphFormat.character.underline)
    {
        case UNDERLINE_TYPE_NONE:                   strcat(text, "\\ulnone"); break; 
        case UNDERLINE_TYPE_CONTINUOUS:             strcat(text, "\\ul"); break;  
        case UNDERLINE_TYPE_DOT:                    strcat(text, "\\uld"); break;     
        case UNDERLINE_TYPE_DASH:                   strcat(text, "\\uldash"); break;     
        case UNDERLINE_TYPE_DASHDOT:                strcat(text, "\\uldashd"); break;     
        case UNDERLINE_TYPE_DASHDOTDOT:             strcat(text, "\\uldashdd"); break;         
        case UNDERLINE_TYPE_DOUBLE:                 strcat(text, "\\uldb"); break;         
        case UNDERLINE_TYPE_HWAVE:                  strcat(text, "\\ulhwave"); break;     
        case UNDERLINE_TYPE_LDASH:                  strcat(text, "\\ulldash"); break; 
        case UNDERLINE_TYPE_THICK:                  strcat(text, "\\ulth"); break;     
        case UNDERLINE_TYPE_THICKDOT:               strcat(text, "\\ulthd"); break; 
        case UNDERLINE_TYPE_THICKDASH:              strcat(text, "\\ulthdash"); break; 
        case UNDERLINE_TYPE_THICKDASHDOT:           strcat(text, "\\ulthdashd"); break; 
        case UNDERLINE_TYPE_THICKDASHDOTDOT:        strcat(text, "\\ulthdashdd"); break;         
        case UNDERLINE_TYPE_THICKLDASH:             strcat(text, "\\ulthldash"); break;     
        case UNDERLINE_TYPE_DOUBLEWAVE:             strcat(text, "\\ululdbwave"); break;   
        case UNDERLINE_TYPE_WORD:                   strcat(text, "\\ulw"); break; 
        case UNDERLINE_TYPE_WAVE:                   strcat(text, "\\ulwave"); break;     
        default:                                    return PARAGRAPH_FORMAT_ERROR;
    }

    if(paragraphFormat.isTabbed)
    {
        sprintf(text, " \\tab %s", paragraphFormat.text);
    }
    else
    {
        sprintf(text, "\\fi%d\\li%d\\ri%d\\sb%d\\sa%d\\sl%d %s", paragraphFormat.firstLineIndent, paragraphFormat.leftIndent, paragraphFormat.rightIndent, paragraphFormat.spaceBefore, paragraphFormat.spaceAfter, paragraphFormat.lineSpacing, paragraphFormat.text);
    }

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return PARAGRAPH_FORMAT_ERROR;
    }

    return SUCCESS;
}  

ERROR_TYPE RTFLib::writeFooterFormat()
{
    char text[4096];

    strcat(text, "\\par");

    switch(footerFormat.alignment)
    {
        case HEADER_ALIGN_LEFT:         strcat(text, "\\ql"); break;
        case HEADER_ALIGN_CENTER:       strcat(text, "\\qc"); break;
        case HEADER_ALIGN_RIGHT:        strcat(text, "\\qr"); break;
        case HEADER_ALIGN_JUSTIFY:      strcat(text, "\\qj"); break;
        default:                        return FOOTER_ERROR;
    }

    if(footerFormat.hasTabs)
    {
        switch(footerFormat.tabs.kind)
        {
            case HEADER_TAB_KIND_NONE:          strcat(text, ""); break;        
            case HEADER_TAB_KIND_CENTER:        strcat(text, "\\tqc"); break;
            case HEADER_TAB_KIND_RIGHT:         strcat(text, "\\tqr"); break;
            case HEADER_TAB_KIND_DECIMAL:       strcat(text, "\\tqdec"); break;
            default:                            return FOOTER_ERROR;
        }

        switch(footerFormat.tabs.lead)
        {
            case HEADER_TAB_LEAD_NONE:          strcat(text, ""); break;
            case HEADER_TAB_LEAD_DOT:           strcat(text, "\\tldot"); break;
            case HEADER_TAB_LEAD_MDOT:          strcat(text, "\\tlmdot"); break;
            case HEADER_TAB_LEAD_HYPH:          strcat(text, "\\tlhyph"); break;
            case HEADER_TAB_LEAD_UNDERLINE:     strcat(text, "\\tlul"); break;
            case HEADER_TAB_LEAD_THICKLINE:     strcat(text, "\\tlth"); break;
            case HEADER_TAB_LEAD_EQUAL:         strcat(text, "\\tleq"); break;
            default:                            return FOOTER_ERROR;
        }

        sprintf(text, "\\tx%d", footerFormat.tabs.position);
    }

    if(footerFormat.hasShading)
    {
        sprintf(text, "\\shading%d%s\\cfpat%d\\cbpat%d", footerFormat.shading.intensity, getShadingName(footerFormat.shading.type, false), footerFormat.shading.fillColor, footerFormat.shading.backgroundColor);
    }

    sprintf(text, "\\animtext%d\\expndtw%d\\kerning%d\\charscalex%d\\f%d\\fs%d\\cf%d", footerFormat.character.animated,footerFormat.character.expand, footerFormat.character.kerning, footerFormat.character.scale, footerFormat.character.fontNumber, footerFormat.character.fontSize, footerFormat.character.foregroundColor);

    if(footerFormat.character.isBold)
    {
        strcat(text, "\\b");
    }

    if(footerFormat.character.isCapital)
    {
        strcat(text, "\\caps");
    }

    if(footerFormat.character.isDoubleStrike)
    {
        strcat(text, "\\striked");
    }

    if(footerFormat.character.isEmboss)
    {
        strcat(text, "\\embo");
    }

    if(footerFormat.character.isEngrave)
    {
        strcat(text, "\\impr");
    }

    if(footerFormat.character.isItalic)
    {
        strcat(text, "\\i");
    }

    if(footerFormat.character.isOutline)
    {
        strcat(text, "\\outl");
    }

    if(footerFormat.character.isShadow)
    {
        strcat(text, "\\shad");
    }

    if(footerFormat.character.isSmallCapital)
    {
        strcat(text, "\\scaps");
    }

    if(footerFormat.character.isStrike)
    {
        strcat(text, "\\strike");
    }

    if(footerFormat.character.isSubscript)
    {
        strcat(text, "\\sub");
    }

    if(footerFormat.character.isSuperscript)
    {
        strcat(text, "\\super");
    }

    switch(footerFormat.character.underline)
    {
        case UNDERLINE_TYPE_NONE:                   strcat(text, "\\ulnone"); break; 
        case UNDERLINE_TYPE_CONTINUOUS:             strcat(text, "\\ul"); break;  
        case UNDERLINE_TYPE_DOT:                    strcat(text, "\\uld"); break;     
        case UNDERLINE_TYPE_DASH:                   strcat(text, "\\uldash"); break;     
        case UNDERLINE_TYPE_DASHDOT:                strcat(text, "\\uldashd"); break;     
        case UNDERLINE_TYPE_DASHDOTDOT:             strcat(text, "\\uldashdd"); break;         
        case UNDERLINE_TYPE_DOUBLE:                 strcat(text, "\\uldb"); break;         
        case UNDERLINE_TYPE_HWAVE:                  strcat(text, "\\ulhwave"); break;     
        case UNDERLINE_TYPE_LDASH:                  strcat(text, "\\ulldash"); break; 
        case UNDERLINE_TYPE_THICK:                  strcat(text, "\\ulth"); break;     
        case UNDERLINE_TYPE_THICKDOT:               strcat(text, "\\ulthd"); break; 
        case UNDERLINE_TYPE_THICKDASH:              strcat(text, "\\ulthdash"); break; 
        case UNDERLINE_TYPE_THICKDASHDOT:           strcat(text, "\\ulthdashd"); break; 
        case UNDERLINE_TYPE_THICKDASHDOTDOT:        strcat(text, "\\ulthdashdd"); break;         
        case UNDERLINE_TYPE_THICKLDASH:             strcat(text, "\\ulthldash"); break;     
        case UNDERLINE_TYPE_DOUBLEWAVE:             strcat(text, "\\ululdbwave"); break;   
        case UNDERLINE_TYPE_WORD:                   strcat(text, "\\ulw"); break; 
        case UNDERLINE_TYPE_WAVE:                   strcat(text, "\\ulwave"); break;     
        default:                                    return FOOTER_ERROR;
    }

    if(footerFormat.isTabbed)
    {
        sprintf(text, " \\tab %s", footerFormat.text);
    }
    else
    {
        sprintf(text, "\\li%d\\ri%d %s", footerFormat.leftIndent, footerFormat.rightIndent, footerFormat.text);
    }

    if(fwrite(text, 1, strlen(text), file) < strlen(text))
    {
        return FOOTER_ERROR;
    }

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
    pf.character = { 0, 0, 0, 100, 0, 24, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false };

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
    ff.character = { 0, 0, 0, 100, 0, 24, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false };

    HEADER_FORMAT hf = { FOOTER_ALIGN_LEFT, 0, 0, "", false, false, false };
    hf.tabs = { 0, PARAGRAPH_TAB_KIND_NONE, PARAGRAPH_TAB_LEAD_NONE };
    hf.shading = { 0, PARAGRAPH_SHADING_TYPE_FILL, 0, 0 };
    hf.character = { 0, 0, 0, 100, 0, 24, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false };

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
