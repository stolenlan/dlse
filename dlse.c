#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include "getopt.h"
#include "dlse.h"


/**
 * Print provided data as a string of hexadecimal characters
 * 
 * FOR TESTING
 */
static void printx(unsigned char *data, unsigned int len) {

	int n_items = 0;

	for(int i = 0; i < len; i += 16) {
		for(int ii = 0; ii < 16 && i+ii < len; ii++) {
			printf("%02x ", data[i+ii]);
		}

		uint16_t v = *(uint16_t *)&data[i];

		printf("(%u)\n", v);

		if(v > 0) n_items++;
	}

	printf("Total Items: %i\n", n_items);
}


void print_usage() {
	printf("Usage dlse [OPTIONS] <save file path>\n");
	printf("See: dlse --help for more information.\n");
}

void print_help() {

	printf("%s\nSET CHARACTER PROPERTIES\n", help_text);

	// Iterate over the defined structure, and 
	for(int i = 0; i < NUM_OFFSETS_V15; i++) {
		struct st_offset inst = offset_v15[i];
		if(inst.key == NULL) continue;
		char set_opt_param_help[32];
		memset(set_opt_param_help, '\0', 32);
		sprintf(set_opt_param_help, "--set-%-10s<value>", inst.key);
		printf(
		"      %-28s%s\n", set_opt_param_help, inst.label);
	}

	printf("\n");
}


int get_offset_index(const char *key) {
	// Return the first occurence of <key>
	// -1 if not found
	for(int i = 0; i < NUM_OFFSETS_V15; i++) {
		struct st_offset inst = offset_v15[i];
		if(inst.key == NULL) continue;
		if(strcmp(inst.key, key) == 0) {
			return i;
		}
	}

	return -1;
}

char * get_save_name(const char *filename) {
	FILE *sfile_stream = fopen(filename, "r");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return NULL;
	}

	// Save and date are at the start of the file
	fseek(sfile_stream, SAVE_NAME_OFFSET, SEEK_SET);

	char *save_name = malloc(sizeof(char)*65);
	memset(save_name, '\0', 65);
	int nb = fread(save_name, sizeof(char), 64, sfile_stream);
	if(nb > 0) return save_name;

	// else
	free(save_name);
	return NULL;
}

char * get_save_date(const char *filename) {
	FILE *sfile_stream = fopen(filename, "r");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return NULL;
	}

	// Save and date are at the start of the file
	fseek(sfile_stream, SAVE_DATE_OFFSET, SEEK_SET);

	char *save_date = malloc(sizeof(char)*65);
	memset(save_date, '\0', 65);
	int nb = fread(save_date, sizeof(char), 64, sfile_stream);
	if(nb > 0) return save_date;

	// else
	free(save_date);
	return NULL;
}

// Return allocated string
char * get_property(const char *filename, const char *key) {

	FILE *sfile_stream = fopen(filename, "r");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return NULL;
	}

	fseek(sfile_stream, 0, SEEK_END);
	size_t sfile_size = ftell(sfile_stream);

	int idx = get_offset_index(key);
	if(idx < 0) {
		printf("%s is not a valid save file property\n", key);
		return NULL;
	}

	struct st_offset inst = offset_v15[idx];

	char *buff = malloc(sizeof(char)*64);
	memset(buff, '\0', 64);

	bool success = false;

	if(inst.type == DT_CH58) {
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		fread(buff, sizeof(char), 58, sfile_stream);
		success = true;
	}


	if(inst.type == DT_SINT) {
		uint8_t val;
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		fread(&val, sizeof(uint8_t), 1, sfile_stream);
		sprintf(buff, "%u", val);
		success = true;
	}


	if(inst.type == DT_NINT) {
		int val;
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		fread(&val, sizeof(int), 1, sfile_stream);
		sprintf(buff, "%i", val);
		success = true;
	}


	if(inst.type == DT_UINT) {
		uint32_t val;
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		fread(&val, sizeof(uint32_t), 1, sfile_stream);
		sprintf(buff, "%u", val);
		success = true;
	}

	if(inst.type == DT_LINT) {
		long int val;
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		fread(&val, sizeof(val), 1, sfile_stream);
		sprintf(buff, "%li", val);
		success = true;
	}


	fclose(sfile_stream);

	if(success) {
		return buff;
	} else {
		free(buff);
		return NULL;
	}

	
}


int set_property(const char *filename, 
	const char *set_key, const char *str_val) {

	int idx = get_offset_index(set_key);
	if(idx < 0) {
		printf("%s is not a valid save file property\n", set_key);
		return 1;
	}

	FILE *sfile_stream = fopen(filename, "r+");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return 1;
	}

	fseek(sfile_stream, 0, SEEK_END);
	size_t sfile_size = ftell(sfile_stream);
	
	// The instance of the offset property
	struct st_offset inst = offset_v15[idx];


	// Normal int
	if(inst.type == DT_NINT) {
		int val = atoi(str_val);
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		int bw = fwrite(&val, sizeof(int), 1, sfile_stream);
		if(bw > 0) {
			printf("Set %s to %i\n", inst.label, val);
			fclose(sfile_stream);
			return 0;
		} else {
			printf("FAILED setting %s to %i\n", inst.label, val);
			fclose(sfile_stream);
			return 1;
		}
	}

	// Uint
	if(inst.type == DT_UINT) {
		uint32_t val = (uint32_t)atoi(str_val);
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		int bw = fwrite(&val, sizeof(int), 1, sfile_stream);
		if(bw > 0) {
			printf("Set %s to %u\n", inst.label, val);
			fclose(sfile_stream);
			return 0;
		} else {
			printf("FAILED setting %s to %u\n", inst.label, val);
			fclose(sfile_stream);
			return 1;
		}
	}

	// Short int
	if(inst.type == DT_SINT) {
		uint8_t val = (uint8_t)atoi(str_val);
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		int bw = fwrite(&val, sizeof(uint8_t), 1, sfile_stream);
		if(bw > 0) {
			printf("Set %s to %u\n", inst.label, val);
			fclose(sfile_stream);
			return 0;
		} else {
			printf("FAILED setting %s to %u\n", inst.label, val);
			fclose(sfile_stream);
			return 1;
		}
	}

	fclose(sfile_stream);
	return 1;
}


int list_character_data(char *filename) {
	int nwidth = 40;
	int kwidth = 8;

	char *save_name = get_save_name(filename);
	char *save_date = get_save_date(filename);

	printf("Save Name: %s\n", save_name);
	printf("save Date: %s\n", save_date);
	printf("---\n");


	for(int i = 0; i < NUM_OFFSETS_V15; i++) {
		struct st_offset inst = offset_v15[i];

		if(inst.key == NULL) continue;

		char *str_val = get_property(filename, inst.key);

		printf("%-*s %-*s %s\n", 12, inst.key, 
			32, inst.label, str_val);
	}

	printf("---\n");
	printf("Save Name: %s\n", save_name);
	printf("save Date: %s\n", save_date);
	free(save_name);
	free(save_date);

	return 0;
}


int set_all_protections(const char *filename, const char *str_val) {
	// HACK Set max protections
	FILE *sfile_stream = fopen(filename, "r+");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return 1;
	}

	fseek(sfile_stream, 0, SEEK_END);
	size_t sfile_size = ftell(sfile_stream);
	
	// WARN TODO: for my version only v1.5
	for(int i = 18; i <= 29; i++) {
		struct st_offset inst = offset_v15[i];
		uint8_t val = (uint8_t)atoi(str_val);
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		int bw = fwrite(&val, sizeof(uint8_t), 1, sfile_stream);
		if(bw > 0) printf("Set %s to %u\n", inst.label, val);
	}

	fclose(sfile_stream);
	return 0;
}

int set_all_skills(const char *filename, const char *str_val) {
	// HACK Set all skills
	FILE *sfile_stream = fopen(filename, "r+");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return 1;
	}

	fseek(sfile_stream, 0, SEEK_END);
	size_t sfile_size = ftell(sfile_stream);
	
	// WARN TODO: for my version only v1.5 35 start of all skills
	// 69 Start of demonic ones
	for(int i = 35; i <= 77; i++) {
		struct st_offset inst = offset_v15[i];
		uint8_t val = (uint8_t)atoi(str_val);
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		int bw = fwrite(&val, sizeof(uint8_t), 1, sfile_stream);
		if(bw > 0) printf("Set %s to %u\n", inst.label, val);
	}

	fclose(sfile_stream);
	return 0;

}




int replace_items(const char *filename, const unsigned char *src_file) {

	// Load the items from the source file
	FILE *replace_source_stream = fopen(src_file, "r");
	if(!replace_source_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		print_usage();
		return 1;
	}
	// Get the size of the source file
	fseek(replace_source_stream, 0, SEEK_END);
	size_t rpfile_size = ftell(replace_source_stream);
	// Remember OFFSET is an offset from the end of the file, thus
	// subtract the start from the end (they are reversed)
	uint32_t items_size = OFFSET_ITEMS_START-OFFSET_ITEMS_END;
	fseek(replace_source_stream, rpfile_size-OFFSET_ITEMS_START, SEEK_SET);
	unsigned char source_items[items_size];
	// For now just print the hex data
	fread(source_items, items_size, 1, replace_source_stream);
	fclose(replace_source_stream);


	// Save the items to the dest file
	FILE *sfile_stream = fopen(filename, "r+");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return 1;
	}
	fseek(sfile_stream, 0, SEEK_END);
	size_t sfile_size = ftell(sfile_stream);
	// Remember that start and end are referenced from the end of the file
	// so to get the size we need to subtract the end from the start
	size_t items_len = OFFSET_ITEMS_START - OFFSET_ITEMS_END;
	printf("Copy/Replace all items %s --> %s (%li bytes)\n", 
		src_file, filename, items_len);
	fseek(sfile_stream, sfile_size-OFFSET_ITEMS_START, SEEK_SET);
	int nwritten = fwrite(source_items, items_len, 1, sfile_stream);
	fclose(sfile_stream);
	if(nwritten > 0) return 0;
	return 1;
}


int transfer_to(const char *filename, const char *src_file) {
	// Open the src_file, and for every skill that is non-zero, set the
	// abblicable property
	FILE *target_savefile_stream = fopen(src_file, "r+");
	if(!target_savefile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", src_file, errstr);
		return 1;
	}

	fseek(target_savefile_stream, 0, SEEK_END);
	size_t sfile_size = ftell(target_savefile_stream);

	for(int i = 0; i < NUM_OFFSETS_V15; i++) {
		struct st_offset inst = offset_v15[i];

		if(inst.key == NULL) continue;
		// Skip the character name...
		if(strcmp(inst.key, "name") == 0) continue;

		char *str_val = get_property(src_file, inst.key);
		if(str_val == NULL) {
			printf("ERROR getting property %s\n", inst.key);
			continue;
		}

		if(inst.type != DT_CH58) {
			// All the rest are numerical types
			long long int tv = strtoll(str_val, NULL, 10);
			if(tv == 0) {
				free(str_val);
				continue;
			}
		}

		set_property(filename, inst.key, str_val);
		free(str_val);
	}
	fclose(target_savefile_stream);

	return replace_items(filename, src_file);
}


int main(int argc, char *argv[]) {
	char *sfile_path = NULL;
	char *replace_items_source_path = NULL;
	char *previous_char_source_path = NULL;
	bool verbose_mode = false;
	bool enable_list_character_data = false;
	bool enable_set_all_prot = false;
	bool enable_set_all_skills = false;
	char *set_key = NULL;
	char *str_val = NULL;

	if(argc < 2) {
		print_usage();
		return -2;
	}

	int c, i, loptind = 0;

	int n_settable_props = 0;
	for(int i = 0; i < NUM_OFFSETS_V15; i++) {
		struct st_offset inst = offset_v15[i];
		if(inst.key == NULL) continue;
		n_settable_props++;
	}

	const char *shortops = "hp:s:r:t:l";
	// The available options.

	struct option static_longops[N_STATIC_OPS] = {
		{"help",                       no_argument,        0,            'h' },
		{"set-all-prot",               required_argument,  0,            'p' },
		{"set-all-skills",             required_argument,  0,            's' },
		{"replace-items",              required_argument,  0,            'r' },
		{"transfer-character",         required_argument,  0,            't' },
		{"list",                       no_argument,        0,            'l' },
		{"verbose",                    no_argument,        0,             0  }
	};



	struct option longops[N_STATIC_OPS+n_settable_props+1];
	for(int i = 0; i < N_STATIC_OPS; i++) {
		longops[i] = static_longops[i];
	}

	int opt_idx = 0;
	char set_opt_names[n_settable_props][32];

	for(int i = 0; i < NUM_OFFSETS_V15; i++) {
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

	// If there are remaining items not processed by getopt they are hopefully
	// commands to be executed
	for (int i = optind; i < argc; i++) {
		FILE *fname_test_stream = fopen(argv[i], "r");
		if(!fname_test_stream) {
			char *errstr = strerror(errno);
			printf("%s: %s\n", argv[i], errstr);
			print_usage();
			return 1;
		}
		fclose(fname_test_stream);

		sfile_path = argv[i];
	}

	if(sfile_path == NULL) {
		printf("No save filename provided\n");
		return 1;
	}


	if(replace_items_source_path != NULL) {
		return replace_items(sfile_path, replace_items_source_path);
	}

	if(previous_char_source_path != NULL) {
		return transfer_to(sfile_path, previous_char_source_path);
	}

	if(enable_list_character_data) {
		return list_character_data(sfile_path);
	}

	// Set 
	if(set_key != NULL) {
		if(str_val == NULL) {
			printf("You must specify --value with --set\n");
			print_usage();
			return 1;
		}

		int retval = set_property(sfile_path, set_key, str_val);
		free(set_key);
		
		if(retval != 0) {
			printf("WARNING: Set property --set-%s failed.\n", set_key);
		}

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
