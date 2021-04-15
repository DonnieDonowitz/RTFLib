struct DOCUMENT_FORMAT
{
    int viewKind;
    int viewScale;
    int paperWidth;
    int paperHeight;
    int marginLeft;
    int marginRight;
    int marginTop;
    int marginBottom;
    int gutterWidth;

    bool facingPages;
    bool readOnly;
};

struct SECTION_FORMAT
{
    int breakType;
    int width;
    int height;
    int marginLeft;
    int marginRight;
    int marginTop;
    int marginBottom;
    int gutterWidth;
    int headerOffset;
    int footerOffset;
    int colsNumber;
    int colsDistance;

    bool isNew;
    bool isDefault;
    bool hasCols;
    bool hasColsLineBetween;
};

struct TAB_FORMAT
{
    int position;
    int kind;
    int lead;
};

struct NUM_FORMAT
{
    int level;
    int space;

    char charNum;
};

struct BORDER_FORMAT
{
    int kind;
    int type;
    int width;
    int color;
    int space;
};

struct SHADING_FORMAT
{
    int intensity;
    int type;
    int fillColor;
    int backgroundColor;
};

struct CHARACTER_FORMAT
{
    int animated;
    int backgroundColor;
    int foregroundColor;
    int scale;
    int expand;
    int fontNumber;
    int fontSize;
    int kerning;
    int underline;

    bool isBold;
    bool isCapital;
    bool isEmboss;
    bool isItalic;
    bool isEngrave;
    bool isOutline;
    bool isSmallCapital;
    bool isShadow;
    bool isStrike;
    bool isDoubleStrike;
    bool isSubscript;
    bool isSuperscript;
};

struct PARAGRAPH_FORMAT
{
    int breakType;
    int alignment;
    int firstLineIndent;
    int leftIndent;
    int rightIndent;
    int spaceBefore;
    int spaceAfter;
    int lineSpacing;
    
    const char* text;

    bool isNew;
    bool isDefault;
    bool isTabbed;
    bool isInTable;
    bool hasTabs;
    bool hasNums;
    bool hasBorders;
    bool hasShading;

    TAB_FORMAT tabs;
    BORDER_FORMAT borders;
    NUM_FORMAT nums;
    SHADING_FORMAT shading;
    CHARACTER_FORMAT character;
};

struct TABLE_BORDER_FORMAT
{
    bool hasBorder;

    BORDER_FORMAT border;
};

struct TABLE_ROW_FORMAT
{
    int alignment;
    int height;
    int marginLeft;
    int marginRight;
    int marginTop;
    int marginBottom;
    int leftIndent;
};

struct TABLE_CELL_FORMAT
{
    int textAlignment;
    int marginLeft;
    int marginRight;
    int marginTop;
    int marginBottom;
    int textDirection;

    bool hasShading;

    SHADING_FORMAT shading;
    TABLE_BORDER_FORMAT borderLeft;
    TABLE_BORDER_FORMAT borderRight;
    TABLE_BORDER_FORMAT borderTop;
    TABLE_BORDER_FORMAT borderBottom;
};

struct FOOTER_FORMAT
{
    int alignment;
    int leftIndent;
    int rightIndent;
    
    const char* text;

    bool isTabbed;
    bool hasTabs;
    bool hasShading;

    TAB_FORMAT tabs;
    SHADING_FORMAT shading;
    CHARACTER_FORMAT character;
};

struct HEADER_FORMAT
{
    int alignment;
    int leftIndent;
    int rightIndent;
    
    const char* text;

    bool isTabbed;
    bool hasTabs;
    bool hasShading;

    TAB_FORMAT tabs;
    SHADING_FORMAT shading;
    CHARACTER_FORMAT character;
};
