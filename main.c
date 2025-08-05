/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 Tyler Anderson <stolenlan@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dlse.h"

int main(int argc, char *argv[]) {
	char *sfile_path = NULL;
	char *replace_items_source_path = NULL;
	char *previous_char_source_path = NULL;
	bool enable_list_character_data = false;
	bool enable_set_all_prot = false;
	bool enable_set_all_skills = false;
	char *set_key = NULL;
	char *str_val = NULL;
	int n_offsets = offset_count();
	int c, i, loptind = 0;
	int n_settable_props = 0;

	if(argc < 2) {
		print_usage();
		interactive_mode();
		return -2;
	}

	for(int i = 0; i < n_offsets; i++) {
		struct st_offset inst = offset_v15[i];
		if(inst.key == NULL) continue;
		n_settable_props++;
	}

	const char *shortops = "hp:s:r:t:l";

	struct option static_longops[] = {
		{"help",                       no_argument,        0,            'h' },
		{"set-all-prot",               required_argument,  0,            'p' },
		{"set-all-skills",             required_argument,  0,            's' },
		{"replace-items",              required_argument,  0,            'r' },
		{"transfer-character",         required_argument,  0,            't' },
		{"list",                       no_argument,        0,            'l' },
		{"verbose",                    no_argument,        0,             0  },
		{0,                            0,                  0,             0  }
	};

	int N_STATIC_OPS = 0;
	for(int i = 0; i < 1000; i++) {
		if(static_longops[i].name == 0 && static_longops[i].val == 0) break;
		N_STATIC_OPS++;
	}

	struct option longops[N_STATIC_OPS+n_settable_props+1];
	for(int i = 0; i < N_STATIC_OPS; i++) {
		longops[i] = static_longops[i];
	}

	int opt_idx = 0;
	char set_opt_names[n_settable_props][32];

	for(int i = 0; i < n_offsets; i++) {
		struct st_offset inst = offset_v15[i];
		if(inst.key == NULL) continue;
		// char opt_name[4+strlen(inst.key)+1];
		memset(set_opt_names[opt_idx], '\0', 32);
		sprintf(set_opt_names[opt_idx], "set-%s", inst.key);
		struct option opt = {
			set_opt_names[opt_idx], required_argument, 0, 0
		};
		longops[opt_idx+N_STATIC_OPS] = opt;
		opt_idx++;
	}

	struct option last_opt = {0,0,0,0};
	longops[opt_idx+N_STATIC_OPS] = last_opt;

	for(i = 1; i < argc; i++) {
		c = getopt_long(argc, argv, shortops, longops, &loptind);
		switch(c) {
			case 0:
				if(strcmp(longops[loptind].name, "verbose") == 0) {
					verbose_mode = true;
				}
				// grab the first-four characters
				char opt_prefix[5];
				memset(opt_prefix, '\0', 5);
				memcpy(opt_prefix, longops[loptind].name, 4);
				if(strcmp("set-", opt_prefix) == 0) {
					// We have a set param
					int opt_len = strlen(longops[loptind].name);
					char set_opt_key[opt_len-4+1];
					memset(set_opt_key,  '\0', opt_len-4+1);
					memcpy(set_opt_key, &longops[loptind].name[4], opt_len-3);

					if(set_key != NULL) {
						printf("WARNING: Ignoring --set-%s (one at a time)\n",
							set_opt_key);
						break;
					}

					// Finally Set this shit up
					set_key = strdup(set_opt_key);
					str_val = optarg;
				}
				break;
			case 'h': // Help - print help text and exit immediatly
				print_usage();
				print_help();
				return 0;
				break;
			case 'p':
				enable_set_all_prot = true;
				str_val = optarg;
				break;
			case 's':
				enable_set_all_skills = true;
				str_val = optarg;
				break;
			case 'r':
				replace_items_source_path = optarg;
				break;
			case 't':
				previous_char_source_path = optarg;
			case 'l':
				enable_list_character_data = true;
				break;
			case '?': // Unknown
				// Just exit now
				printf("Invalid argument(s) provided\n");
				return 1;
			default:
				break;
		}
	}

	if(argc-optind > 1) {
		printf("Invalid options, see help\n");
		return 2;
	}

	// If there are remaining items not processed by getopt it better be the
	// target save file
	for (int i = optind; i < argc; i++) {
		FILE *fname_test_stream = fopen(argv[i], "r");
		if(!fname_test_stream) {
			char *errstr = strerror(errno);
			printf("%s: %s\n", argv[i], errstr);
			print_usage();
			return 1;
		}
		fclose(fname_test_stream);
		int save_version = get_save_major_vers(argv[i]);
		if (save_version != 150) {
			printf("ERROR: Unsupported save/game version.\n");
			return 2;
		}
		sfile_path = argv[i];
	}

	if(sfile_path == NULL) {
		printf("No save filename provided\n");
		return 1;
	}

	if(replace_items_source_path != NULL) {
		int save_version = get_save_major_vers(replace_items_source_path);
		if (save_version != 150) {
			printf("ERROR: Unsupported save/game version for file: %s.\n", 
				replace_items_source_path);
			return 2;
		}
		return replace_items(sfile_path, replace_items_source_path);
	}

	if(previous_char_source_path != NULL) {
		int save_version = get_save_major_vers(previous_char_source_path);
		if (save_version != 150) {
			printf("ERROR: Unsupported save/game version for file: %s.\n", 
				previous_char_source_path);
			return 2;
		}
		return transfer_to(sfile_path, previous_char_source_path);
	}

	if(enable_list_character_data) {
		return list_character_data(sfile_path);
	}

	if(set_key != NULL) {
		if(str_val == NULL) {
			printf("You must specify --value with --set\n");
			print_usage();
			return 1;
		}
		int retval = set_property(sfile_path, set_key, str_val);
		if(retval != 0) {
			printf("WARNING: Set property --set-%s failed.\n", set_key);
		}
		free(set_key);
		return retval;
	}

	if(enable_set_all_prot) {
		return set_all_protections(sfile_path, str_val);
	}

	if(enable_set_all_skills) {
		return set_all_skills(sfile_path, str_val);
	}

	return list_character_data(sfile_path);
}
