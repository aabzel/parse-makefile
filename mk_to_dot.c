#include "mk_to_dot.h"

#include "fifo_char.h"
#include "str_ops.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <regex.h>

bool proc_mk_file (char *fileName, char *outFileName, char *edge_color,char *root_node_name) {
    FILE *filePrt = NULL;
    FILE *outFilePrt = NULL;
    bool res = false;
    char childMkFile[500];
    char child_c_file_node[500];
    char child_c_file[500];
    char childMkNode[500];
    char rootMkFile[500];
    char curFileStr[500];
    char rootMknodeName[500];

    res = parse_mk (fileName, rootMkFile, sizeof (rootMkFile));
    if (false == res) {
        return false;
    }
    strncpy (rootMknodeName, rootMkFile, sizeof (rootMknodeName));
    replace_char (rootMknodeName, '.', '_');

    outFilePrt = fopen (outFileName, "a");
    if (NULL == outFilePrt) {
        return false;
    }
    fprintf (outFilePrt, "\n\n%s [ label = \"%s\"];", root_node_name, rootMkFile);

    filePrt = fopen (fileName, "r");
    if (filePrt) {
        int line = 0;
        while (NULL != fgets (curFileStr, sizeof (curFileStr), filePrt)) {

            memset (childMkFile, 0x00, sizeof (childMkFile));
            memset (childMkNode, 0x00, sizeof (childMkNode));

            bool mk_res = parse_mk (curFileStr, childMkFile, sizeof (childMkFile));
            if (true == mk_res ) {
                strncpy (childMkNode, childMkFile, sizeof (childMkNode));
                replace_char (childMkNode, '.', '_');
                fprintf (outFilePrt, "\n%s [ label = \"%s\", style=filled, fillcolor=lightskyblue];", childMkNode, childMkFile);
                fprintf (outFilePrt, "\n%s->%s [color=%s];", root_node_name, childMkNode, edge_color);
            } else {
                // printf ("\nUnable to parse line: %d %s ", line, curFileStr);
                line++;
            }
            if (false == mk_res) {
            	memset (child_c_file, 0x00, sizeof (child_c_file));
                bool c_res = parse_c (curFileStr, child_c_file, sizeof(child_c_file));
                if (true==c_res) {
                	strncpy(child_c_file_node, child_c_file,sizeof(child_c_file_node));
                	replace_char (child_c_file_node, '.', '_');
                	fprintf (outFilePrt, "\n%s [ label = \"%s\", style=filled, fillcolor=cornsilk];", child_c_file_node, child_c_file);
                	fprintf (outFilePrt, "\n%s->%s [color=%s];", root_node_name, child_c_file_node, edge_color);
                	printf ("\n %s", child_c_file);
                }
            }
        }



        res = true;
        fclose (filePrt);
    } else {
        printf ("Error");
    }
    if (outFilePrt) {
        fclose (outFilePrt);
    }
    return res;
}

bool parse_c(char *in_file_str, char *out_temp_str, int out_temp_str_len) {
	bool res = false;
	if (NULL != in_file_str) {
		int inStrLen = strlen(in_file_str);
		if (0 < inStrLen) {
			if (NULL != strstr(in_file_str, ".c")) {
				char fifoArray[1000];
				Fifo_array_t outfilefifo;
				fifo_init(&outfilefifo, sizeof(fifoArray), fifoArray);
				for (int i = 0; i < inStrLen; i++) {
					if (true == is_allowed_char_file(in_file_str[i])) {
						if ('#' == in_file_str[i]) {
							i = inStrLen;
							break;
						}
						if (('\n' != in_file_str[i]) && ('\r' != in_file_str[i])
								&& (' ' != in_file_str[i])) {
							fifo_push(&outfilefifo, in_file_str[i]);
						}
					} else {
						fifo_reset(&outfilefifo);
					}
				}
				uint16_t fileNameLen = 0;
				fifo_peek_array(&outfilefifo, out_temp_str, &fileNameLen);
				if (0 < fileNameLen) {
					if (NULL != strstr(out_temp_str, ".c")) {
						// printf ("\n mk file: [%s]", tempStr);
						if(NULL == strstr(out_temp_str, ".cmm")){
						    res = true;
						}
					} else {
						fifo_reset(&outfilefifo);
						memset(out_temp_str, 0x00, out_temp_str_len);
					}
				}
			}

		}
	}

	return res;
}

//
bool parse_mk (char *in_file_str, char *tempStr, int outStrLen) {
    bool res = false;
    uint16_t fileNameLen = 0;
    char fifoArray[1000];
    if (NULL!=in_file_str) {
        int inStrLen = strlen (in_file_str);
        if (0 < inStrLen) {
            if (NULL != strstr (in_file_str, ".mk")) {
                Fifo_array_t outfilefifo;
                fifo_init (&outfilefifo, sizeof (fifoArray), fifoArray);
                for (int i = 0; i < inStrLen; i++) {
                    if (true == is_allowed_char_file (in_file_str[i])) {
                        if ('#' == in_file_str[i]) {
                            i = inStrLen;
                            break;
                        }
                        if (('\n' != in_file_str[i]) && ('\r' != in_file_str[i]) && (' ' != in_file_str[i])) {
                            fifo_push (&outfilefifo, in_file_str[i]);
                        }
                    } else {
                        fifo_reset (&outfilefifo);
                    }
                }
                fifo_peek_array (&outfilefifo, tempStr, &fileNameLen);
                if (0 < fileNameLen) {
                    if (NULL != strstr (tempStr, ".mk")) {
                        // printf ("\n mk file: [%s]", tempStr);
                        res = true;
                    } else {
                        fifo_reset (&outfilefifo);
                        memset (tempStr, 0x00, outStrLen);
                    }
                }
            }
        }
    }
    return res;
}

bool test_parse_c (void) {
    int cmp_res;

    char tempStr[1000];
    strncpy (tempStr, "", sizeof (tempStr));
    parse_c ("CSRC += $(PATH_BOARD)/src/tsts_v1_driver.c\n", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("tsts_v1_driver.c", tempStr);
    if (0 != cmp_res) {
        return false;
    }

    strncpy (tempStr, "", sizeof (tempStr));
    parse_c ("    CSRC += $(PATH_BOARD)/src/can_led_config.c", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("can_led_config.c", tempStr);
    if (0 != cmp_res) {
        return false;
    }

    strncpy (tempStr, "", sizeof (tempStr));
    parse_c ("    CSRC += $(PATH_BOARD)/src/tsts_v1_msg_handler.c\n", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("tsts_v1_msg_handler.c", tempStr);
    if (0 != cmp_res) {
        return false;
    }

    strncpy (tempStr, "", sizeof (tempStr));
    parse_c ("%.cmm: $(OBJDIR_)", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("", tempStr);
    if (0 != cmp_res) {
        return false;
    }

    return true;
}


// grep "    include $(ROOT)/components/toolboxes/io_toolbox/io_toolbox.mk" -E "\w+.mk"
bool test_parse_mk (void) {
    int cmp_res;

    char tempStr[1000];
    strncpy (tempStr, "", sizeof (tempStr));
    parse_mk ("    include $(ROOT)//components//toolboxes//io_toolbox//io_toolbox.mk\n", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("io_toolbox.mk", tempStr);
    if (0 != cmp_res) {
        return false;
    }

    strncpy (tempStr, "", sizeof (tempStr));
    parse_mk ("include $(ROOT)/components/lib/spc58_mcan/spc58_mcan.mk", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("spc58_mcan.mk", tempStr);
    if (0 != cmp_res) {
        return false;
    }

    strncpy (tempStr, "", sizeof (tempStr));
    parse_mk ("CAN_MANAGER_GENERATED_MK_NAME   = can_generated_patched.mk\n", tempStr, sizeof (tempStr));
    cmp_res = strcmp ("can_generated_patched.mk", tempStr);
    if (0 != cmp_res) {
        return false;
    }
    return true;
}

bool is_allowed_char_file (char ch) {
    if ('=' == ch) {
        return false;
    }

    if ('/' == ch) {
        return false;
    }
    if ('\\' == ch) {
        return false;
    }
    if ('%' == ch) {
        return false;
    }
    if ('$' == ch) {
        return false;
    }
    if ('(' == ch) {
        return false;
    }
    if (')' == ch) {
        return false;
    }
    return true;
}

bool is_char_of_file (char ch) {
    if ('a' <= ch && (ch <= 'z')) {
        return true;
    }
    if ('A' <= ch && (ch <= 'Z')) {
        return true;
    }
    if ('_' == ch) {
        return true;
    }
    if ('.' == ch) {
        return true;
    }
    return false;
}
