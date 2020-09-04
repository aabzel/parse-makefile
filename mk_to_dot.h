#ifndef __MK_TO_DOT_H
#define __MK_TO_DOT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

bool proc_mk_file (char *fileName, char *outFileName, char *edge_color,char *root_node_name);

bool test_parse_mk (void);
bool test_parse_c (void);
bool parse_mk (char *fileStr, char *tempStr, int outStrLen);
bool parse_c (char *in_file_str, char *out_temp_str, int out_temp_str_len);
int match (const char *string, char *pattern);
bool is_char_of_file (char ch);
bool is_allowed_char_file (char ch);

#ifdef __cplusplus
}
#endif

#endif /* __MK_TO_DOT_H */
