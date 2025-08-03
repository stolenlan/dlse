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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <libgen.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>
#include <dirent.h>

#include "dlse.h"

bool verbose_mode = false;

struct st_offset offset_v15[] = {
	{33463, DT_CH20, "Character Name", 			"name", 	-1, "None"},
	{33404, DT_SINT, "Stat Strength", 			"str", 		-1, "Stat"},
	{33402, DT_SINT, "Stat Strength2", 			"str2", 	-1, "Stat"},
	{33400, DT_SINT, "Stat Intelligence", 		"int", 		-1, "Stat"},
	{33398, DT_SINT, "Stat Intelligence2", 		"int2", 	-1, "Stat"},
	{33396, DT_SINT, "Stat Dexterity", 			"dex", 		-1, "Stat"},
	{33394, DT_SINT, "Stat Dexterity2", 		"dex2", 	-1, "Stat"},
	{33392, DT_SINT, "Stat Agility", 			"agl", 		-1, "Stat"},
	{33390, DT_SINT, "Stat Agility2", 			"agl2", 	-1, "Stat"},
	{33388, DT_SINT, "Stat Vitality", 			"vit", 		-1, "Stat"},
	{33386, DT_SINT, "Stat Vitality2", 			"vit2", 	-1, "Stat"},
	{33384, DT_SINT, "Stat Honor", 				"hnr", 		-1, "Stat"},
	{33382, DT_SINT, "Stat Honor2", 			"hnr2", 	-1, "Stat"},
	{33380, DT_SINT, "Life", 					"life", 	-1, "None"},
	{33378, DT_SINT, "Life2", 					"life2", 	-1, "None"},
	{33376, DT_SINT, "Strike", 					"strike", 	-1, "None"},
	{33372, DT_SINT, "Perry", 					"perry", 	-1, "None"},
	{33368, DT_SINT, "Influence", 				"infl", 	-1, "None"},
	{33326, DT_SINT, "Prot Magic Natural", 		"npmagic", 	-1, "Prot"},
	{33324, DT_SINT, "Prot Magic Enhanced", 	"epmagic", 	-1, "Prot"},
	{33322, DT_SINT, "Prot Fire Natural", 		"npfire", 	-1, "Prot"},
	{33320, DT_SINT, "Prot Fire Enhanced", 		"epfire", 	-1, "Prot"},
	{33318, DT_SINT, "Prot Petrif. Natural", 	"nppetrif", -1, "Prot"},
	{33316, DT_SINT, "Prot Petrif. Enhanced", 	"eppetrif", -1, "Prot"},
	{33314, DT_SINT, "Prot Poison Natural", 	"nppoison", -1, "Prot"},
	{33312, DT_SINT, "Prot Poison Enhanced", 	"eppoison", -1, "Prot"},
	{33310, DT_SINT, "Prot Ice Natural", 		"npice", 	-1, "Prot"},
	{33308, DT_SINT, "Prot Ice Enhanced", 		"epice", 	-1, "Prot"},
	{33306, DT_SINT, "Prot Gas Natural", 		"npgas", 	-1, "Prot"},
	{33304, DT_SINT, "Prot Gas Enhanced", 		"epgas", 	-1, "Prot"},
	{33302, DT_NINT, "Gold", 					"gold", 	-1, "None"},
	{33298, DT_NINT, "Experience Points", 		"exp", 		-1, "None"},
	{33294, DT_UINT, "Advancement Points", 		"adv", 		-1, "None"},
	{33248, DT_NINT, "Kills", 					"kills", 	-1, "None"},
	{33244, DT_NINT, "Deaths", 					"deaths", 	-1, "None"},
	{33216, DT_SINT, "Weaponry Light", 			"swlw", 	-1, "Weaponry"},
    {33208, DT_SINT, "Weaponry Medium", 		"swmw", 	-1, "Weaponry"},
    {33200, DT_SINT, "Weaponry Heavy", 			"swhw", 	-1, "Weaponry"},
    {33192, DT_SINT, "Weaponry Light Pole", 	"swlp", 	-1, "Weaponry"},
    {33184, DT_SINT, "Weaponry Heavy Pole", 	"swhp", 	-1, "Weaponry"},
    {33176, DT_SINT, "Weaponry L Dual Weild", 	"swld", 	-1, "Weaponry"},
    {33168, DT_SINT, "Weaponry M. Dual Wield", 	"swmd", 	-1, "Weaponry"},
    {33160, DT_SINT, "Weaponry Thrown", 		"swtw", 	-1, "Weaponry"},
    {33152, DT_SINT, "Weaponry Archery", 		"swar", 	-1, "Weaponry"},
    {33144, DT_SINT, "Defence Parry", 			"sdpa", 	-1, "Defence"},
    {33136, DT_SINT, "Defence Light Armor", 	"sdla", 	-1, "Defence"},
    {33128, DT_SINT, "Defence Medium Armor", 	"sdma", 	-1, "Defence"},
    {33120, DT_SINT, "Defence Heavy Armor", 	"sdha", 	-1, "Defence"},
    {33112, DT_SINT, "Defence Light Shield", 	"sdls", 	-1, "Defence"},
    {33104, DT_SINT, "Defence Medium Shield", 	"sdms", 	-1, "Defence"},
    {33096, DT_SINT, "Defence Heavy Shield", 	"sdhs", 	-1, "Defence"},
    {33088, DT_SINT, "General Athletics", 		"sgat", 	-1, "General"},
    {33080, DT_SINT, "General Scout", 			"sgsc", 	-1, "General"},
    {33072, DT_SINT, "General Bargain", 		"sgba", 	-1, "General"},
    {33064, DT_SINT, "General Repair", 			"sgre", 	-1, "General"},
    {32992, DT_SINT, "Magic Arcane Magic", 		"smam", 	-1, "Magic"},
    {32984, DT_SINT, "Magic Celestial Magic", 	"smcm", 	-1, "Magic"},
    {32976, DT_SINT, "Magic Nether Magic", 		"smnm", 	-1, "Magic"},
    {32968, DT_SINT, "Magic Rune Matic", 		"smrm", 	-1, "Magic"},
    {32960, DT_SINT, "Magic Magic Weapons", 	"smwm", 	-1, "Magic"},
    {33008, DT_SINT, "Magic Identify Items", 	"smid", 	-1, "Magic"},
    {32952, DT_SINT, "Magic Magic Scribe", 		"smsc", 	-1, "Magic"},
    {32944, DT_SINT, "Magic Magic Channel", 	"smch", 	-1, "Magic"},
    {32936, DT_SINT, "Magic Alchemy", 			"smal", 	-1, "Magic"},
    {33048, DT_SINT, "Theif Pick Locks", 		"stpl", 	-1, "Theif"},
    {33040, DT_SINT, "Theif Inspect", 			"stin", 	-1, "Theif"},
    {33032, DT_SINT, "Theif Disarm Trap", 		"stdt", 	-1, "Theif"},
    {33024, DT_SINT, "Theif Sneak", 			"stsn", 	-1, "Theif"},
    {33016, DT_SINT, "Theif Steal", 			"stst", 	-1, "Theif"},
    {32928, DT_SINT, "Diabolic Inflict Wounds", "sdiw", 	-1, "Diabolic"},
    {32920, DT_SINT, "Diabolic Crushing Blow", 	"sdcb", 	-1, "Diabolic"},
    {32912, DT_SINT, "Diabolic Crit. Strike", 	"sdcs", 	-1, "Diabolic"},
    {32904, DT_SINT, "Diabolic Hawkeye", 		"sdhe", 	-1, "Diabolic"},
    {32896, DT_SINT, "Diabolic Spellfire", 		"sdsp", 	-1, "Diabolic"},
    {32888, DT_SINT, "Diabolic Drain Life",		"sddl", 	-1, "Diabolic"},
    {33224, DT_SINT, "Diabolic Ninjutsu", 		"sdni", 	-1, "Diabolic"},
    {33000, DT_SINT, "Diabolic Backstab", 		"sdba", 	-1, "Diabolic"},
    {33056, DT_SINT, "Diabolic Iron Will", 		"sdir", 	-1, "Diabolic"},
    {   -1,      -1,                 NULL,        NULL,     -1,      NULL }
};


char *categories[] = {
	"None", 
	"Stat", 
	"Prot", 
	"Weaponry", 
	"General", 
	"Magic", 
	"Theif", 
	"Diabolic",
	NULL
};

char *help_text  = "\n"
" Dungeon Lords Save Editor\n"
" (c) 2025 Tyler Anderson - <stolenlan@gmail.com>\n\n"

"  Simply specifying a <save file> will list the corresponding character\n"
" properties, thus --list is mostly redundant, except that it can be appended\n"
" for convenience and will prevent all other options from having an effect.\n"
" Generally you can use --set-<property key> <value> to set the value for\n"
" the given <save file>.  Finally the --replace-items, and -t options both\n"
" require a parameter of a save file path to be copied FROM. eg.\n"
" dlse C:\\path\\to_target_save -t C:\\path\\to_source_save.\n\n"

" Maybe I'll get around to making a win32 GUI or something... later.\n\n"
" Bitcoin: bc1qmgv9thga0ff0le4hg6kaf6y272g5j7gm3jw6hq (may help with the GUI :)"
"\n\n"
"GENERAL OPTIONS\n"
"  -h, --help                      Display this help\n"
"  -p, --set-all-prot   <value>    Set all protections to <value>\n"
"  -s, --set-all-skills <value>    Set all skills to <value>\n"
"  -r, --replace-items  <file>     Use items from provided save file\n"
"  -t, --transfer-character <file> Migrate character stats, and equipment to\n"
"                                  a new character, use this to actually\n"
"                                  continue playing from endgame *ish level.\n"
"                                  NOTE: You should save just after killing\n"
"                                  the soul-eater, BEFORE releasing Galdren\n"
"                                  then use that save to transfer to a new\n"
"                                  character.\n"
"  -l, --list                      List property values\n"
"      --verbose                   Be more verbose\n";

char *interactive_help = "\n"
" Dungeon Lords Save Editor\n"
" (c) 2025 Tyler Anderson - <stolenlan@gmail.com>\n"
" Bitcoin: bc1qmgv9thga0ff0le4hg6kaf6y272g5j7gm3jw6hq\n"
"          May help with the GUI :)\n\n"

" Interactive mode commands:\n"
" --------------------------\n"
" help            Show This help\n"
" quit            Exit the program.\n"
" show(games)     Show available save games (default install path).\n\n"

" load <save file/index>\n"
"   In order to actaully view, or do anything useful, you must load a save\n"
"   file to edit. If your game is installed in a standard location, a listing\n"
"   of the available save games will be printed after this help, and in that\n"
"   case, you can simply call load with the index number eg. load 01\n\n"

" list\n"
"   List the character information (stats/skills) for the currently loaded\n"
"   save file. NOTE: the KEY column is needed for use with the set-<key> cmd."
"\n\n"

" set-<key> <value>   (See the list of properties above for the <key>s)\n"
"   Set the given property for the loaded save file. for example, to set the\n"
"   'Diabolic Iron Will' skill to 22, you would call: set-sdir 22.\n\n"

" cont(inue) <save file/index>\n"
"   With this command, you can actually continue after completing the story\n"
"   (sort of). Using this you will be able to start a new character, then\n"
"   move all stats, skills, and eqipment, to the said new character. Then\n"
"   re-play the entire story with all the gear you know and love. This\n"
"   includes all keys, and quest items. I've play tested it, and it works\n"
"   great. If you do it right, you can play through with all the relics.\n\n"

"   Here is the process to \"continue\" with your end-game character:\n"
"    1) Play through to some point in the story, ideally to the last sequence\n"
"       Just after killing the soul-eater, but BEFORE releaseing Gauldryn\n"
"       and save the game to the last save slot (10) (recommended).\n"
"    2) After saving, finish the game (release low-budget Gandalf), \n"
"       and enjoy the end sequence. It's at this point you are lied to,\n"
"       you're told you can continue with this character... But you can't.\n"
"    3) Rage quit.\n"
"    4) Start a new game/character, as soon as you load into the new\n"
"       game, save, to a DIFFERENT slot, slot 1 (recommended).\n"
"    5) Quit the game, and run this utility. Load the NEW game in (slot 1)\n"
"       To be clear, you are loading the new save, once loaded run: cont 10\n"
"       This will copy the stats/skills/items from the last slot (10) into\n"
"       the new game in slot 1. You may have to unequip/reequip your weapons."
"\n\n"
" NOTES: It would behoove you to make a backup of your game save before any\n"
" shenanigans with this utiltiy. Just saying, not my fault if you get things\n"
" backwards and overwrite your god-tier character with a level one putz.\n\n";

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

int n_offsets_v15 = 0;
int offset_count() {
	if(n_offsets_v15 == 0) {
		for(int i = 0; i < 1000; i++) {
			struct st_offset inst = offset_v15[i];

			if(inst.key == NULL && inst.offset == -1) {
				break;
			}

			n_offsets_v15++;
		}
	}

	return n_offsets_v15;
}

void print_usage() {
	printf("Usage dlse [OPTIONS] <save file path>\n");
	printf("See: dlse --help for more information.\n");
}


char * search_default_save_path(int target_idx) {
	#ifdef _WIN32
	const char *std_path = 
		"C:\\Program Files (x86)\\Dreamcatcher\\Dungeon Lords\\save";
	#elif __linux__
	char *home = getenv("HOME");
	char std_path[256];
	memset(std_path, '\0', 256);
	sprintf(std_path, "%s/%s",
		home,
		".wine/drive_c/Program Files (x86)/Dreamcatcher/Dungeon Lords/save");
	#else

	return NULL;
	#endif

	if(target_idx < 1) {
		printf(" Auto-detected Save Games:\n");
		printf(" -------------------------\n");
	}

	char buff[128];
	int fi = 0;
	for(int i = 0; i < 10; i++) {
		memset(buff, '\0', 128);
		sprintf(buff, "%s/game%02i.sav", std_path, i);
		FILE *fh = fopen(buff, "r");
		if(fh) {
			fclose(fh);
			int save_version = get_save_major_vers(buff);
			if(save_version != 150) {
				continue;
			}

			fi++;

			if(target_idx < 1) {
				char *save_name = get_save_name(buff);
				char *save_date = get_save_date(buff);
				char *bn = basename(buff);

				printf(" %02i: %-24s %s (%s)\n", i+1, save_name, save_date, bn);
				free(save_name);
				free(save_date);
			} else if (i == target_idx-1) {
				return strdup(buff);
			}
		}
	}

	if(target_idx < 1) {
		if(fi == 0) {
			printf(" No games found (in the default install location).\n");
			printf(" If you know your installtion path, you can still\n");
			printf(" specity the path of the save to load.\n");
		}
	}

	return NULL;
}

void interactive_mode() {
	printf(interactive_help);
	search_default_save_path(0);

	char buff[128];
	char cmd[5];
	char c;
	
	char *valid_savepath = NULL;
	while(true) {
		putchar('\n');
		memset(buff, '\0', 128);
		memset(cmd, '\0', 5);
		c = '\0';

		if(valid_savepath != NULL) {
			char *save_name = get_save_name(valid_savepath);
			printf("DLSE (%s): ", save_name);
			free(save_name);
		} else {
			printf("DLSE (No Save Loaded): ");
		}
		
		for(int i = 0; i < 128; i++) {
			fread(&buff[i], sizeof(char), 1, stdin);
			if(feof(stdin) || buff[i] == '\n' || buff[i] == '\r') {
				break;
			}
		}


		memcpy(cmd, buff, 4);

		// All commands are 4 characters
		if(strcmp(cmd, "help") == 0) {
			printf(interactive_help);
			
			search_default_save_path(0);
			continue;
		}


		if(strcmp(cmd, "show") == 0) {
			search_default_save_path(0);
			continue;
		}

		if(strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
			if(valid_savepath != NULL) {
				free(valid_savepath);
			}
			return;
		}

		// If someone just types a number clear the buffer and fake the load
		// cmd
		int moron_index = atoi(buff);
		if(moron_index > 0 && moron_index <= 10) {
			memset(buff, '\0', 128);
			sprintf(buff, "load %i", moron_index);
			sprintf(cmd, "load");
		}

		if(strcmp(cmd, "load") == 0) {
			// Get the path, provided to and test if the file exists.
			int path_start = 0;
			char load_path[128];
			memset(load_path, '\0', 128);
			for(int i = 4; i < 128; i++) {
				if(!path_start) {
					if(buff[i] == ' ') continue;
					path_start = i;
				}

				if(buff[i] != '\0' && buff[i] != '\n' && buff[i] != '\r') {
					load_path[i-path_start] = buff[i];
				} else {
					break;
				}
			}

			if(strlen(load_path) == 0) {
				printf("You must specify a filename after load.\n");
				printf("Nothing loaded!\n");
				continue;
			}

			int load_idx = atoi(load_path);
			if(load_idx > 0 && load_idx <= 10) {
				char *hf = search_default_save_path(load_idx);
				if(hf == NULL) {
					printf("Invalid index, nothing loaded.\n");
					free(hf);
					continue;
				}
				memset(load_path, '\0', 128);
				memcpy(load_path, hf, strlen(hf));
				free(hf);
			}

			int save_version = get_save_major_vers(load_path);
			if(save_version < 0) {
				printf("Nothing loaded!\n");
			} else if(save_version != 150) {
				printf("Unsupported game version %i\n", save_version);
				printf("Nothing loaded!\n");
			} else {
				char *save_name = get_save_name(load_path);
				char *save_date = get_save_date(load_path);
				printf("\n* Loaded savegame: %s (%s)**\n\n", 
					save_name, save_date);
				free(save_name);
				free(save_date);

				char *vsp = malloc(strlen(load_path)+1);
				memset(vsp, '\0', strlen(load_path)+1);
				sprintf(vsp, load_path, strlen(load_path));
				valid_savepath = strdup(vsp);
				free(vsp);

				printf("Hint: run list to see character stats.\n");
			}

			continue;
		}


		if(strcmp(cmd, "cont") == 0) {

			if(valid_savepath == NULL) {
				printf("You must load a save file first!\n");
				continue;
			}

			// Get the path, provided to and test if the file exists.
			int spath_start = 0;
			char sload_path[128];
			memset(sload_path, '\0', 128);
			bool found_space = false; // It's fucking late... I know, Iknow.
			for(int i = 4; i < 128; i++) {
				if(!spath_start) {
					if(buff[i] == ' ') found_space = true;
					if(buff[i] == ' ') continue;
					if(!found_space) continue;
					spath_start = i;
				}

				if(buff[i] != '\0' && buff[i] != '\n' && buff[i] != '\r') {
					sload_path[i-spath_start] = buff[i];
				} else {
					break;
				}
			}

			if(strlen(sload_path) == 0) {
				printf("You must specify a path or index to transfer from.\n");
				printf("Nothing transfered!\n");
				continue;
			}

			int sload_idx = atoi(sload_path);
			if(sload_idx > 0 && sload_idx <= 10) {
				char *hf = search_default_save_path(sload_idx);
				memset(sload_path, '\0', 128);
				memcpy(sload_path, hf, strlen(hf));
				free(hf);
			}

			if(strcmp(valid_savepath, sload_path) == 0) {
				printf("NOPE: The source of you character data is the same as " 
					"the destination.\nFigure it out!\n");
				continue;
			}

			int ssave_version = get_save_major_vers(sload_path);
			if(ssave_version < 0) {
				printf("Invalid / bad save file.\n");
			} else if(ssave_version != 150) {
				printf("Unsupported game version %i\n", ssave_version);
				printf("Nothing transfered!\n");
			} else {
				char *ssave_name = get_save_name(sload_path);
				char *ssave_date = get_save_date(sload_path);
				char *target_save_name = get_save_name(valid_savepath);
				char *target_save_date = get_save_date(valid_savepath);
				printf("\n** Transfer FROM: %-18s (%s)\n"
				         "** Transfer TO:   %-18s (%s)\n", 
					ssave_name, ssave_date, 
					target_save_name, target_save_date);
				free(ssave_name);
				free(ssave_date);
				free(target_save_name);
				free(target_save_date);

				printf("Are you sure (y/n): ");
				for(int i = 0; i < 128; i++) {
					// why keep reading, because we need to clear out stdin
					fread(&buff[i], sizeof(char), 1, stdin);
					if(feof(stdin) || buff[i] == '\n' || buff[i] == '\r') {
						break;
					}
				}
				if(buff[0] == 'y') {
					char *svsp = malloc(strlen(sload_path)+1);
					memset(svsp, '\0', strlen(sload_path)+1);
					sprintf(svsp, sload_path, strlen(sload_path));
					if(transfer_to(valid_savepath, svsp) == 0) {
						printf("** Transfer COMPLETE **\n");
						free(svsp);
						continue;
					}
					free(svsp);
				}

				printf("** Transfer ABORTED **\n");
				continue;
			}

			continue;
		}

		if(strcmp(cmd, "list") == 0) {
			if(valid_savepath != NULL) {
				list_character_data(valid_savepath);
				printf("---\n");
				continue;
			} else {
				printf("You must load a save file first!\n");
				continue;
			}
			
		}

		if(strcmp(cmd, "set-") == 0) {
			if(valid_savepath != NULL) {
				// Grab the remaining key (everything up to the space char)
				
				char key_buff[17];
				memset(key_buff, '\0', 17);
				for(int i = 4; i < 16; i++) {
					if(buff[i] == ' ' || buff[i] == '\n' || buff[i] == '\r') {
						break;
					}
					key_buff[i-4] = buff[i];
				}

				int val_start = 0;
				char val_buff[31];
				memset(val_buff, '\0', 31);

				// Now extract the value
				for(int i = 4+strlen(key_buff); i < 32; i++) {
					if(!val_start) {
						if(buff[i] == ' ') {
							continue;
						}
						val_start = i;
					}

					if(buff[i] == ' ' || buff[i] == '\n' || buff[i] == '\r') {
						break;
					}

					val_buff[i-val_start] = buff[i];
				}

				if(strlen(key_buff) == 0 || strlen(val_buff) == 0) {
					printf("Invalid set command: You must provide a property "
						"and a value. eg.: set-sdba 22\n");
					continue;
				}

				// All good
				set_property(valid_savepath, key_buff, val_buff);
				continue;
			} else {
				printf("You must load a save file first!\n");
				continue;
			}
		}


		buff[strlen(buff)-1] = '\0';

		printf("Unknown command: %s\n", buff);
		printf("Valid commands are: help, load <save/idx>, list, set-<key>, "
			"cont <save/idx>.\n");

		
	}
}

void print_help() {

	printf("%s\nSET CHARACTER PROPERTIES\n", help_text);

	// Iterate over the defined structure, and 
	for(int i = 0; i < n_offsets_v15; i++) {
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
	for(int i = 0; i < n_offsets_v15; i++) {
		struct st_offset inst = offset_v15[i];
		if(inst.key == NULL) continue;
		if(strcmp(inst.key, key) == 0) {
			return i;
		}
	}

	return -1;
}

int get_save_major_vers(const char *filename) {
	FILE *sfile_stream = fopen(filename, "r");
	if(!sfile_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return -1;
	}

	int mvers;

	int br = fread(&mvers, sizeof(int), 1, sfile_stream);

	fclose(sfile_stream);

	if(br > 0) return mvers;
	
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
	fclose(sfile_stream);
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
	fclose(sfile_stream);
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

	if(inst.type == DT_CH20) {
		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);
		fread(buff, sizeof(char), 20, sfile_stream);
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
	}

	free(buff);
	return NULL;
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

	if(inst.type == DT_CH20) {
		// printf("Unsupported property: %s (%s)\n", inst.key, inst.label);
		// fclose(sfile_stream);
		// return 1;
		if(strlen(str_val) > 20) {
			printf("FAILED setting name: too large > 20 characters\n");
			return 1;
		}

		fseek(sfile_stream, sfile_size-inst.offset, SEEK_SET);

		char buff[21];
		memset(buff, '\0', 21);
		sprintf(buff, "%s", str_val);

		int bw = fwrite(&buff, sizeof(char), 20, sfile_stream);
		if(bw > 0) {
			printf("Set %s to %s\n", inst.label, str_val);
			fclose(sfile_stream);
			return 0;
		} else {
			printf("FAILED setting %s to %s\n", inst.label, str_val);
			fclose(sfile_stream);
			return 1;
		}
	}


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

	printf("%-*s %-*s %s\n", 12, "KEY", 
			32, "LABEL", "VALUE");
	printf("--------------------------------------------------------------\n");
	for(int i = 0; i < n_offsets_v15; i++) {
		struct st_offset inst = offset_v15[i];

		if(inst.key == NULL) continue;

		char *str_val = get_property(filename, inst.key);

		printf("%-*s %-*s %s\n", 12, inst.key, 
			32, inst.label, str_val);
		free(str_val);
	}


	// Save the items to the dest file
	FILE *sfile_stream = fopen(filename, "r");
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
	fseek(sfile_stream, sfile_size-OFFSET_ITEMS_START, SEEK_SET);

	// 1275 items... potentially?
	// for(int i = 0; i < 5100/4; i++) {
	// 	uint16_t val;
	// 	fread(&val, sizeof(uint16_t), 1, sfile_stream);
	// 	if(val > 0) {
	// 		printf("%03i: %06u\n", i, val);
	// 	}
	// 	fseek(sfile_stream, sfile_size-OFFSET_ITEMS_START+(i*4), SEEK_SET);
	// }
	fclose(sfile_stream);



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
	char *src_file_dup = strdup(src_file);
	char *dst_file_dup = strdup(filename);

	printf("Copy/Replace items from %s --> %s (%li bytes)\n", 
		basename(src_file_dup), basename(dst_file_dup), items_len);
	free(src_file_dup);
	free(dst_file_dup);
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

	for(int i = 0; i < n_offsets_v15; i++) {
		struct st_offset inst = offset_v15[i];

		if(inst.key == NULL) continue;
		// Skip the character name...
		if(strcmp(inst.key, "name") == 0) continue;

		char *str_val = get_property(src_file, inst.key);
		if(str_val == NULL) {
			printf("ERROR getting property %s\n", inst.key);
			continue;
		}

		if(inst.type != DT_CH20) {
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


