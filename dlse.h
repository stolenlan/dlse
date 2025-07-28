

#define N_STATIC_OPS 7
#define NUM_OFFSETS_V15 78

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
	DT_CH58
};

struct st_offset{
	uint32_t offset;
	int type;
	const char *label;
	const char *key;
	long int max;
};

struct st_offset offset_v15[NUM_OFFSETS_V15] = {
	{33463, DT_CH58, "Character Name", 				"name", 	-1},
	{33404, DT_SINT, "Stat Strength", 				"str", 		-1},
	{33402, DT_SINT, "Stat Strength2", 				"str2", 	-1},
	{33400, DT_SINT, "Stat Intelligence", 			"int", 		-1},
	{33398, DT_SINT, "Stat Intelligence2", 			"int2", 	-1},
	{33396, DT_SINT, "Stat Dexterity", 				"dex", 		-1},
	{33394, DT_SINT, "Stat Dexterity2", 			"dex2", 	-1},
	{33392, DT_SINT, "Stat Agility", 				"agl", 		-1},
	{33390, DT_SINT, "Stat Agility2", 				"agl2", 	-1},
	{33388, DT_SINT, "Stat Vitality", 				"vit", 		-1},
	{33386, DT_SINT, "Stat Vitality2", 				"vit2", 	-1},
	{33384, DT_SINT, "Stat Honor", 					"hnr", 		-1},
	{33382, DT_SINT, "Stat Honor2", 				"hnr2", 	-1},
	{33380, DT_SINT, "Life", 						"life", 	-1},
	{33378, DT_SINT, "Life2", 						"life2", 	-1},
	{33376, DT_SINT, "Strike", 						"strike", 	-1},
	{33372, DT_SINT, "Perry", 						"perry", 	-1},
	{33368, DT_SINT, "Influence", 					"infl", 	-1},
	{33326, DT_SINT, "Prot Magic Natural", 			"npmagic", 	-1},
	{33324, DT_SINT, "Prot Magic Enhanced", 		"epmagic", 	-1},
	{33322, DT_SINT, "Prot Fire Natural", 			"npfire", 	-1},
	{33320, DT_SINT, "Prot Fire Enhanced", 			"epfire", 	-1},
	{33318, DT_SINT, "Prot Petrif. Natural", 		"nppetrif", -1},
	{33316, DT_SINT, "Prot Petrif. Enhanced", 		"eppetrif", -1},
	{33314, DT_SINT, "Prot Poison Natural", 		"nppoison", -1},
	{33312, DT_SINT, "Prot Poison Enhanced", 		"eppoison", -1},
	{33310, DT_SINT, "Prot Ice Natural", 			"npice", 	-1},
	{33308, DT_SINT, "Prot Ice Enhanced", 			"epice", 	-1},
	{33306, DT_SINT, "Prot Gas Natural", 			"npgas", 	-1},
	{33304, DT_SINT, "Prot Gas Enhanced", 			"epgas", 	-1},
	{33302, DT_NINT, "Gold", 						"gold", 	-1},
	{33298, DT_NINT, "Experience Points", 			"exp", 		-1},
	{33294, DT_UINT, "Advancement Points", 			"adv", 		-1},
	{33248, DT_NINT, "Kills", 						"kills", 	-1},
	{33244, DT_NINT, "Deaths", 						"deaths", 	-1},
	{33216, DT_SINT, "Weaponry Light", 				"swlw", 	-1},
    {33208, DT_SINT, "Weaponry Medium", 			"swmw", 	-1},
    {33200, DT_SINT, "Weaponry Heavy", 				"swhw", 	-1},
    {33192, DT_SINT, "Weaponry Light Pole", 		"swlp", 	-1},
    {33184, DT_SINT, "Weaponry Heavy Pole", 		"swhp", 	-1},
    {33176, DT_SINT, "Weaponry Light Dual Weild", 	"swld", 	-1},
    {33168, DT_SINT, "Weaponry Medium Dual Wield", 	"swmd", 	-1},
    {33160, DT_SINT, "Weaponry Thrown", 			"swtw", 	-1},
    {33152, DT_SINT, "Weaponry Archery", 			"swar", 	-1},
    {33144, DT_SINT, "Defence Parry", 				"sdpa", 	-1},
    {33136, DT_SINT, "Defence Light Armor", 		"sdla", 	-1},
    {33128, DT_SINT, "Defence Medium Armor", 		"sdma", 	-1},
    {33120, DT_SINT, "Defence Heavy Armor", 		"sdha", 	-1},
    {33112, DT_SINT, "Defence Light Shield", 		"sdls", 	-1},
    {33104, DT_SINT, "Defence Medium Shield", 		"sdms", 	-1},
    {33096, DT_SINT, "Defence Heavy Shield", 		"sdhs", 	-1},
    {33088, DT_SINT, "General Athletics", 			"sgat", 	-1},
    {33080, DT_SINT, "General Scout", 				"sgsc", 	-1},
    {33072, DT_SINT, "General Bargain", 			"sgba", 	-1},
    {33064, DT_SINT, "General Repair", 				"sgre", 	-1},
    {32992, DT_SINT, "Magic Arcane Magic", 			"smam", 	-1},
    {32984, DT_SINT, "Magic Celestial Magic", 		"smcm", 	-1},
    {32976, DT_SINT, "Magic Nether Magic", 			"smnm", 	-1},
    {32968, DT_SINT, "Magic Rune Matic", 			"smrm", 	-1},
    {32960, DT_SINT, "Magic Magic Weapons", 		"smwm", 	-1},
    {33008, DT_SINT, "Magic Identify Items", 		"smid", 	-1},
    {32952, DT_SINT, "Magic Magic Scribe", 			"smsc", 	-1},
    {32944, DT_SINT, "Magic Magic Channel", 		"smch", 	-1},
    {32936, DT_SINT, "Magic Alchemy", 				"smal", 	-1},
    {33048, DT_SINT, "Theif Pick Locks", 			"stpl", 	-1},
    {33040, DT_SINT, "Theif Inspect", 				"stin", 	-1},
    {33032, DT_SINT, "Theif Disarm Trap", 			"stdt", 	-1},
    {33024, DT_SINT, "Theif Sneak", 				"stsn", 	-1},
    {33016, DT_SINT, "Theif Steal", 				"stst", 	-1},
    {32928, DT_SINT, "Diabolic Inflict Wounds", 	"sdiw", 	-1},
    {32920, DT_SINT, "Diabolic Crushing Blow", 		"sdcb", 	-1},
    {32912, DT_SINT, "Diabolic Critical Strike", 	"sdcs", 	-1},
    {32904, DT_SINT, "Diabolic Hawkeye", 			"sdhe", 	-1},
    {32896, DT_SINT, "Diabolic Spellfire", 			"sdsp", 	-1},
    {32888, DT_SINT, "Diabolic Drain Life",			"sddl", 	-1},
    {33224, DT_SINT, "Diabolic Ninjutsu", 			"sdni", 	-1},
    {33000, DT_SINT, "Diabolic Backstab", 			"sdba", 	-1},
    {33056, DT_SINT, "Diabolic Iron Will", 			"sdir", 	-1}

};	

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

void print_usage();
void print_help();
int get_offset_index(const char *key);
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

const char *help_text = "\n"
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
" Bitcoin: bc1qmgv9thga0ff0le4hg6kaf6y272g5j7gm3jw6h (may help with the GUI :)"
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