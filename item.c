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
#include <errno.h>
#include <string.h>
#include <stdint.h>

#define OFFSET_ITEMS_START 31355
#define OFFSET_ITEMS_END 26255

struct item_data {
	char name[22];
	char cat[22];
	char data[240];
};

static void printx(unsigned char *data, unsigned int len) {
	int n_items = 0;
	for(int i = 0; i < len; i += 32) {
		for(int ii = 0; ii < 32 && i+ii < len; ii++) {
			printf("%02x ", data[i+ii]);
		}
	}
}



/**
 * What is this you say.  Just trying to reverse engineer the item IDs to
 * labels...
 * 
 * You can ignore this file, it's not part of dlse.
 */
int main(int argc, char *argv[]) {

	const char *savefile_path = "dev/test.sav";
	const char *dlords_exe_path = "dlords/dlords.exe";

	FILE *dlexe_stream = fopen(dlords_exe_path, "r+");
	if(!dlexe_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", dlords_exe_path, errstr);
		return 1;
	}

	fseek(dlexe_stream, 0, SEEK_END);
	size_t fsize = ftell(dlexe_stream);
	// fseek(dlexe_stream, 2467380, SEEK_SET);

	// int nl = 0;
	// int total = 0;
	// for(int i = 0; i < 9297; i++) {
	// 	char c;
	// 	fread(&c, 1, 1, dlexe_stream);
	// 	if( c != 0 ) {
	// 		putchar(c);
	// 		nl = 0;
	// 	} else if (nl == 0){
	// 		putchar('\n');
	// 		printf("%03i: ", total);
	// 		total++;
	// 		nl = 1;
	// 	}
	// }
	// printf("\n\n%i total items\n", total);

	printf("Searching for POSSIBLE resultion(s)...\n");
	int n_found = 0;
	int found_idx[30];
	int vsize = sizeof(int);
	for(size_t i = 0; i < fsize; i++) {
		
		fseek(dlexe_stream, i, SEEK_SET);
		int xres, yres;
		fread(&xres, vsize, 1, dlexe_stream);
		fseek(dlexe_stream, i+vsize, SEEK_SET);
		fread(&yres, vsize, 1, dlexe_stream);
		if(xres == 1600 && yres == 1200) {
			printf("%02i: 1600x1200 : %li\n", n_found+1, i);
			found_idx[n_found++] = i;
		}
	}

	// if(n_found > 0) {
	// 	// Try the first one...

	// 	for(int idx = 0; idx < n_found; idx++) {
	// 		printf("Attempting 1920x1080 write to #%02i: ", idx+1);
	// 		fseek(dlexe_stream, found_idx[idx], SEEK_SET);
	// 		int new_xres = 1920, new_yres = 1080;
	// 		int bwx = fwrite(&new_xres, vsize, 1, dlexe_stream);
	// 		fseek(dlexe_stream, found_idx[idx]+vsize, SEEK_SET);
	// 		int bwy = fwrite(&new_yres, vsize, 1, dlexe_stream);
	// 		if(bwx > 0 && bwy > 0) {
	// 			printf("DONE\n");	
	// 		} else {
	// 			printf("FAILED\n");
	// 		}		
	// 	}
	// }

	fclose(dlexe_stream);


	// -----------------------------------------------------------
	// // Now open the savefile and scan the item block

	// FILE *sfile_stream = fopen(savefile_path, "r");
	// if(!sfile_stream) {
	// 	char *errstr = strerror(errno);
	// 	printf("%s: %s\n", savefile_path, errstr);
	// 	return 1;
	// }

	// fseek(sfile_stream, 0, SEEK_END);

	// size_t sfile_size = ftell(sfile_stream);

	// fseek(sfile_stream, sfile_size-OFFSET_ITEMS_START, SEEK_SET);

	// // FOR D6ITEM.DAT
	// int n_elements = sfile_size/sizeof(struct item_data);
	// int n_items = 0;
	

	// for(int i = 0; i < n_elements; i++) {
	// 	struct item_data item; 
	// 	int br = fread(&item, sizeof(item), 1, sfile_stream);

	// 	if(item.cat[0] != '?' && item.cat[strlen(item.cat)-1] != '?') {
	// 		continue;
	// 	}
	// 	int cat_len = strlen(item.cat);
	// 	char cat[cat_len-1];
	// 	memcpy(cat, &item.cat[1], strlen(item.cat)-2);

	// 	//first-value
	// 	short fv = *(short *)&item.data[0];

	// 	n_items++;
	// 	printf("%-22s (%s) | %i\n", item.name, cat, fv);

	// 	// printx(item.data, 240);
	// 	// putchar('\n');

	// }

	// printf("%i total records\n", n_elements);
	// printf("%i total items\n", n_items);
	// fclose(sfile_stream);


	// -----------------------------------------------------------
	// int tot = OFFSET_ITEMS_START-OFFSET_ITEMS_END;
	// for(int i = 1; i < tot; i++) {
	// 	if(tot%i == 0) {
	// 		printf("%i is a factor of %i giving %i elements total\n", 
	// 			i, tot, tot/i);
	// 	}
	// }


	
	return 0;

}