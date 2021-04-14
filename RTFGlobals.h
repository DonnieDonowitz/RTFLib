#include <stdio.h>
#include <cstring>
#include <stdlib.h>


//PARAGRAPH GLOBAL DEFINITIONS
#define PARAGRAPH_BREAK_NONE                    0
#define PARAGRAPH_BREAK_PAGE                    1
#define PARAGRAPH_BREAK_COLUMN                  2
#define PARAGRAPH_BREAK_LINE                    3

#define PARAGRAPH_ALIGN_LEFT				    0
#define PARAGRAPH_ALIGN_CENTER			        1
#define PARAGRAPH_ALIGN_RIGHT			        2
#define PARAGRAPH_ALIGN_JUSTIFY			        3

#define PARAGRAPH_TAB_KIND_NONE			        0
#define PARAGRAPH_TAB_KIND_CENTER	        	1
#define PARAGRAPH_TAB_KIND_RIGHT		       	2
#define PARAGRAPH_TAB_KIND_DECIMAL      		3

#define PARAGRAPH_TAB_LEAD_NONE			        0
#define PARAGRAPH_TAB_LEAD_DOT			        1
#define PARAGRAPH_TAB_LEAD_MDOT			        2
#define PARAGRAPH_TAB_LEAD_HYPH			        3
#define PARAGRAPH_TAB_LEAD_UNDERLINE		    4
#define PARAGRAPH_TAB_LEAD_THICKLINE		    5
#define PARAGRAPH_TAB_LEAD_EQUAL			    6

#define PARAGRAPH_BORDER_KIND_NONE	        	0
#define PARAGRAPH_BORDER_KIND_TOP			    1
#define PARAGRAPH_BORDER_KIND_BOTTOM		    2
#define PARAGRAPH_BORDER_KIND_LEFT		        3
#define PARAGRAPH_BORDER_KIND_RIGHT		        4
#define PARAGRAPH_BORDER_KIND_BOX			    5

#define PARAGRAPH_BORDER_TYPE_STHICK		    0
#define PARAGRAPH_BORDER_TYPE_DTHICK		    1
#define PARAGRAPH_BORDER_TYPE_SHADOW		    2
#define PARAGRAPH_BORDER_TYPE_DOUBLE		    3
#define PARAGRAPH_BORDER_TYPE_DOT			    4
#define PARAGRAPH_BORDER_TYPE_DASH		        5
#define PARAGRAPH_BORDER_TYPE_HAIRLINE	        6
#define PARAGRAPH_BORDER_TYPE_INSET		        7
#define PARAGRAPH_BORDER_TYPE_SDASH		        8
#define PARAGRAPH_BORDER_TYPE_DOTDASH		    9
#define PARAGRAPH_BORDER_TYPE_DOTDOTDASH	    10
#define PARAGRAPH_BORDER_TYPE_OUTSET		    11
#define PARAGRAPH_BORDER_TYPE_TRIPLE		    12
#define PARAGRAPH_BORDER_TYPE_WAVY		        13
#define PARAGRAPH_BORDER_TYPE_DWAVY		        14
#define PARAGRAPH_BORDER_TYPE_STRIPED		    15
#define PARAGRAPH_BORDER_TYPE_EMBOSS		    16
#define PARAGRAPH_BORDER_TYPE_ENGRAVE		    17

#define PARAGRAPH_SHADING_TYPE_FILL		        0
#define PARAGRAPH_SHADING_TYPE_HORIZ		    1
#define PARAGRAPH_SHADING_TYPE_VERT		        2
#define PARAGRAPH_SHADING_TYPE_FDIAG		    3
#define PARAGRAPH_SHADING_TYPE_BDIAG		    4
#define PARAGRAPH_SHADING_TYPE_CROSS		    5
#define PARAGRAPH_SHADING_TYPE_CROSSD		    6
#define PARAGRAPH_SHADING_TYPE_DHORIZ		    7
#define PARAGRAPH_SHADING_TYPE_DVERT		    8
#define PARAGRAPH_SHADING_TYPE_DFDIAG		    9
#define PARAGRAPH_SHADING_TYPE_DBDIAG		    10
#define PARAGRAPH_SHADING_TYPE_DCROSS		    11
#define PARAGRAPH_SHADING_TYPE_DCROSSD	        12


//SECTION GLOBAL DEFINITIONS
#define SECTION_BREAK_CONTINUOUS			    0
#define SECTION_BREAK_COLUMN				    1
#define SECTION_BREAK_PAGE				        2
#define SECTION_BREAK_EVENPAGE			        3
#define SECTION_BREAK_ODDPAGE			        4


//TABLE ROW GLOBAL DEFINITIONS
#define TABLE_ROW_TEXT_ALIGN_LEFT				0
#define TABLE_ROW_TEXT_ALIGN_CENTER				1
#define TABLE_ROW_TEXT_ALIGN_RIGHT				2


//TABLE CELL GLOBAL DEFINITIONS
#define TABLE_CELL_TEXT_ALIGN_TOP				0
#define TABLE_CELL_TEXT_ALIGN_CENTER			1
#define TABLE_CELL_TEXT_ALIGN_BOTTOM			2

#define TABLE_CELL_TEXT_DIRECTION_LRTB			0
#define TABLE_CELL_TEXT_DIRECTION_RLTB			1
#define TABLE_CELL_TEXT_DIRECTION_LRBT			2
#define TABLE_CELL_TEXT_DIRECTION_LRTBV			3
#define TABLE_CELL_TEXT_DIRECTION_RLTBV			4

#define TABLE_CELL_SHADING_TYPE_FILL			0
#define TABLE_CELL_SHADING_TYPE_HORIZ			1
#define TABLE_CELL_SHADING_TYPE_VERT			2
#define TABLE_CELL_SHADING_TYPE_FDIAG			3
#define TABLE_CELL_SHADING_TYPE_BDIAG			4
#define TABLE_CELL_SHADING_TYPE_CROSS			5
#define TABLE_CELL_SHADING_TYPE_CROSSD			6
#define TABLE_CELL_SHADING_TYPE_DHORIZ			7
#define TABLE_CELL_SHADING_TYPE_DVERT			8
#define TABLE_CELL_SHADING_TYPE_DFDIAG			9
#define TABLE_CELL_SHADING_TYPE_DBDIAG			10
#define TABLE_CELL_SHADING_TYPE_DCROSS			11
#define TABLE_CELL_SHADING_TYPE_DCROSSD			12


//DOCUMENT GLOBAL DEFINITIONS
#define DOCUMENT_VIEWKIND_NONE			        0
#define DOCUMENT_VIEWKIND_PAGE			        1
#define DOCUMENT_VIEWKIND_OUTLINE		        2
#define DOCUMENT_VIEWKIND_MASTER			    3
#define DOCUMENT_VIEWKIND_NORMAL			    4


//HEADER GLOBAL DEFINITIONS
#define HEADER_ALIGN_LEFT				        0
#define HEADER_ALIGN_CENTER			            1
#define HEADER_ALIGN_RIGHT			            2
#define HEADER_ALIGN_JUSTIFY			        3

#define HEADER_TAB_KIND_NONE			        0
#define HEADER_TAB_KIND_CENTER	        	    1
#define HEADER_TAB_KIND_RIGHT		       	    2
#define HEADER_TAB_KIND_DECIMAL      		    3

#define HEADER_TAB_LEAD_NONE			        0
#define HEADER_TAB_LEAD_DOT			            1
#define HEADER_TAB_LEAD_MDOT			        2
#define HEADER_TAB_LEAD_HYPH			        3
#define HEADER_TAB_LEAD_UNDERLINE		        4
#define HEADER_TAB_LEAD_THICKLINE		        5
#define HEADER_TAB_LEAD_EQUAL			        6

#define HEADER_SHADING_TYPE_FILL		        0
#define HEADER_SHADING_TYPE_HORIZ		        1
#define HEADER_SHADING_TYPE_VERT		        2
#define HEADER_SHADING_TYPE_FDIAG		        3
#define HEADER_SHADING_TYPE_BDIAG		        4
#define HEADER_SHADING_TYPE_CROSS		        5
#define HEADER_SHADING_TYPE_CROSSD		        6
#define HEADER_SHADING_TYPE_DHORIZ		        7
#define HEADER_SHADING_TYPE_DVERT		        8
#define HEADER_SHADING_TYPE_DFDIAG		        9
#define HEADER_SHADING_TYPE_DBDIAG		        10
#define HEADER_SHADING_TYPE_DCROSS		        11
#define HEADER_SHADING_TYPE_DCROSSD	            12


//FOOTER GLOBAL DEFINITIONS
#define FOOTER_ALIGN_LEFT				        0
#define FOOTER_ALIGN_CENTER			            1
#define FOOTER_ALIGN_RIGHT			            2
#define FOOTER_ALIGN_JUSTIFY			        3

#define FOOTER_TAB_KIND_NONE			        0
#define FOOTER_TAB_KIND_CENTER	        	    1
#define FOOTER_TAB_KIND_RIGHT		       	    2
#define FOOTER_TAB_KIND_DECIMAL      		    3

#define FOOTER_TAB_LEAD_NONE			        0
#define FOOTER_TAB_LEAD_DOT			            1
#define FOOTER_TAB_LEAD_MDOT			        2
#define FOOTER_TAB_LEAD_HYPH			        3
#define FOOTER_TAB_LEAD_UNDERLINE		        4
#define FOOTER_TAB_LEAD_THICKLINE		        5
#define FOOTER_TAB_LEAD_EQUAL			        6

#define FOOTER_SHADING_TYPE_FILL		        0
#define FOOTER_SHADING_TYPE_HORIZ		        1
#define FOOTER_SHADING_TYPE_VERT		        2
#define FOOTER_SHADING_TYPE_FDIAG		        3
#define FOOTER_SHADING_TYPE_BDIAG		        4
#define FOOTER_SHADING_TYPE_CROSS		        5
#define FOOTER_SHADING_TYPE_CROSSD		        6
#define FOOTER_SHADING_TYPE_DHORIZ		        7
#define FOOTER_SHADING_TYPE_DVERT		        8
#define FOOTER_SHADING_TYPE_DFDIAG		        9
#define FOOTER_SHADING_TYPE_DBDIAG		        10
#define FOOTER_SHADING_TYPE_DCROSS		        11
#define FOOTER_SHADING_TYPE_DCROSSD	            12

