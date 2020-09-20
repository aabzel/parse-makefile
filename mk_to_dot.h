#ifndef __MK_TO_DOT_H
#define __MK_TO_DOT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

bool discard_path (char *in_file_path, int rank, char *out_file_path);
bool calc_mk_node (char *in_file_name, char *out_root_node_name);
bool proc_mk_file (char *fileName, char *outFileName);

bool parse_mk (char *fileStr, char *tempStr, int outStrLen);
bool parse_c (char *in_file_str, char *out_temp_str, int out_temp_str_len);
int match (const char *string, char *pattern);
bool is_char_of_file (char ch);
bool is_allowed_char_file (char ch);

#ifdef __cplusplus
}
#endif

#endif /* __MK_TO_DOT_H */
