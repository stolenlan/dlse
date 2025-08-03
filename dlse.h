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

// TODO set these to v15 offset items...
#define OFFSET_ITEMS_START 31355
#define OFFSET_ITEMS_END 26255

// NOTE: These offsets are from the START of the file
#define SAVE_NAME_OFFSET 100
#define SAVE_DATE_OFFSET 180

enum dtype {
	DT_SINT,
	DT_NINT,
	DT_UINT,
	DT_LINT,
	DT_CH20
};


struct st_offset{
	long int offset;
	int type;
	const char *label;
	const char *key;
	long int max;
	const char *cat;
};

extern struct st_offset offset_v15[];
extern char *categories[];
extern bool verbose_mode;
extern char *help_text;
extern char *interactive_help;

/**
 * 
 * WORKING NOTES
 * 
 * Weapons start 2134 from end of last character name
 * 20 slots, with 48 bytes between each slot, and 48 between each inventory
 * category
 * 
 * armor starts 3094 after first letter of last occurence of character name
 * jewelry starts 4054 bytes after first letter of ...
 * potions and scrolls start 5014 bytes ...
 * keys and quest items start 5974 bytes...
 * junk bag 6934
 */
int offset_count();
void print_usage();
void interactive_mode();
void print_help();
int get_offset_index(const char *key);
int get_save_major_vers(const char *filename);
char * get_save_name(const char *filename);
char * get_save_date(const char *filename);
char * get_property(const char *filename, const char *key);
int set_property(const char *filename, 
	const char *set_key, const char *str_val);
int list_character_data(char *filename);
int set_all_protections(const char *filename, const char *str_val);
int set_all_skills(const char *filename, const char *str_val);
int replace_items(const char *filename, const unsigned char *src_file);
int transfer_to(const char *filename, const char *src_file);