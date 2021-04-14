#include <stdint.h>

#define ERROR_TYPE uint8_t

#define OPEN_ERROR                  1   
#define CLOSE_ERROR                 2
#define INIT_ERROR                  3
#define HEADING_ERROR               4
#define SECTION_FORMAT_ERROR        5
#define PARAGRAPH_FORMAT_ERROR      6
#define IMAGE_ERROR                 7
#define TABLE_ROW_ERROR             8
#define TABLE_CELL_ERROR            9
#define WRITE_ERROR                 10
#define FOOTER_ERROR                11
#define HEADER_ERROR                12
#define HYPERLINK_ERROR             13
#define BOOKMARK_ERROR              14
#define SUCCESS                     255