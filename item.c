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

	const char *filename = "dlords/dlords.exe";

	FILE *item_stream = fopen(filename, "r");
	if(!item_stream) {
		char *errstr = strerror(errno);
		printf("%s: %s\n", filename, errstr);
		return 1;
	}

	fseek(item_stream, 0, SEEK_END);
	size_t fsize = ftell(item_stream);
	fseek(item_stream, 2467380, SEEK_SET);
// 2476680

	int nl = 0;
	int total = 0;
	for(int i = 0; i < 9300; i++) {
		char c;
	
		fread(&c, 1, 1, item_stream);
		if( c != 0 ) {
			putchar(c);
			nl = 0;
		} else if (nl == 0){
			putchar('\n');
			printf("%03i: ", total);
			total++;
			nl = 1;
		}
	}

	printf("\n\n%i total items\n", total);

	// FOR D6ITEM.DAT
	// int n_elements = fsize/sizeof(struct item_data);
	// int n_items = 0;
	

	// for(int i = 0; i < n_elements; i++) {
	// 	struct item_data item; 
	// 	int br = fread(&item, sizeof(item), 1, item_stream);

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
	// printf("%u\n", sizeof(uint16_t));

	fclose(item_stream);

	return 0;

}