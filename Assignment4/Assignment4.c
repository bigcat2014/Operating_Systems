/*
 * Author: Logan Thomas
 * Assignment Number: 4
 * Date of Submission: 12/11/2018
 * Name of this file: Assignment4.c
 * Description of the program: The main function of the assignment
 */

//<editor-fold desc="Includes">

#include "Assignment4.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//</editor-fold>

//<editor-fold desc="Function declarations">

int8_t get_lowest_value_index(const int8_t lru[]);
int get_page(const int8_t page_table[], int8_t frame);

//</editor-fold>

int main(int argc, char **argv){
	// Pointer to hold the name of the address file
	char *address_filename = argv[1];
	// Array to hold the page table
	int8_t page_table[NUM_PAGES];
	// Initialize all page table values to -1
	memset(page_table, -1, NUM_PAGES * sizeof(int8_t));
	// Array to hold the "RAM"
	int8_t ram[RAM_SIZE];
	// The last time referenced
	uint8_t time_referenced = 0;
	// Create an empty array to store the lru frames
	int8_t lru[FRAME_COUNT];
	// The number of elements in the lru
	int8_t lru_count = 0;
	// The number of page faults
	float page_faults = 0;
	// The total number of accesses
	float total_accesses = 0;

	// File pointer to the file containing the addresses
	FILE *address_file_ptr;
	// File pointer to the backing store
	FILE *backing_store_file_ptr;
	// File pointer to the output file
	FILE *output_file_ptr;
	
	// Pointer to the line read from the addresses file
	char *line = NULL;
	// The length read from the file
	size_t len = 0;
	// The return value of getline
	ssize_t read;
	
	// The address read from the address file
	uint16_t address;
	// The page extracted from the address
	uint8_t page;
	// The frame from the page table or the lru
	int8_t frame;
	// The offset extracted from the address
	uint8_t offset;
	// The physical address composed of the frame and offset
	uint16_t physical_address;
	
	// Open the address file
	address_file_ptr = fopen(address_filename, "r");
	// Make sure the file is not NULL
	assert(address_file_ptr != NULL);
	
	// Read the file line by line
	while ((read = getline(&line, &len, address_file_ptr)) != -1) {
		
		// Get the address from the file
		address = (uint16_t) strtoul(line, NULL, 10);
		
		// Extract page and offset from address
		page = PAGE(address);
		offset = OFFSET(address);
		
		// Consult the page table to get the frame the data is stored in
		frame = page_table[page];
		
		// Page fault, consult BACK_STORE.bin
		if (frame == -1){
			
			// Buffer to store the data from BACKING_STORE.bin
			int8_t data_block[DATA_BLOCK_SIZE];
			
			// Open the file
			backing_store_file_ptr = fopen("../BACKING_STORE.bin", "rb");
			// Make sure the file pointer is not NULL
			assert(backing_store_file_ptr != NULL);

			// Seek to the proper address in backing store
			fseek(backing_store_file_ptr, PAGE_ADDRESS(page), SEEK_SET);
			// Read the data from the backing store into data_block
			fread(&data_block, DATA_BLOCK_SIZE, sizeof(int8_t), backing_store_file_ptr);
			// Close the backing store file
			fclose(backing_store_file_ptr);
			
			// If the lru is full, kick a block out of RAM to make room to the new room
			if (lru_count >= FRAME_COUNT){
				// Get the least recently used frame from the lru
				frame = get_lowest_value_index(lru);
				// Get the page associated with the least recently used frame
				int old_page = get_page(page_table, frame);
				// Reset the least recently used page in the page table
				page_table[old_page] = -1;
			} else {
				// Get an open frame (use lru count as an index)
				frame = lru_count++;
			}
			// Copy the data block into RAM
			memcpy(&ram[FRAME_ADDRESS(frame)], data_block, DATA_BLOCK_SIZE * sizeof(int8_t));
			// Increment the page faults
			page_faults++;
		}
		// Increment the total accesses
		total_accesses++;
		// Update the page table
		page_table[page] = frame;
		// Update the lru
		lru[frame] = time_referenced++;
		
		// Convert frame and offset to physical address
		physical_address = PHYSICAL_ADDRESS(frame, offset);
		
		// Open the file
		output_file_ptr = fopen("../output.txt", "a");
		// Make sure the file pointer is not NULL
		assert(output_file_ptr != NULL);
		
		// Print to the file
		fprintf(output_file_ptr, "Virtual address: %d,\t\tPhysical address: %d,\t\tValue: %d\n", address, physical_address, ram[physical_address]);
		// Close the file
		fclose(output_file_ptr);
	}
	
	// Close the address file
	fclose(address_file_ptr);
	// Free the line parsed from the addresses file
	if (line != NULL) {
		free(line);
	}
	
	// Print the page fault rate
	printf("Page fault rate: %.02f%%\n", 100*(page_faults/total_accesses));
	exit(EXIT_SUCCESS);
}

//<editor-fold desc="Function definitions">

/*
 * Name: get_lowest_value_index
 * Input: The lru
 * Output: The least recently used frame number
 * Description: Goes through the lru and finds the least recently used frame
 */
int8_t get_lowest_value_index(const int8_t lru[]){
	// The result frame
	int8_t result = 0;
	// Loop through the lru
	for (int8_t i = 1; i < FRAME_COUNT; i++){
		// If the lru at index i is less than the lru at the current index, store the new index
		if (lru[i] < lru[result]){
			result = i;
		}
	}
	// Return the frame
	return result;
}

/*
 * Name: get_page
 * Input: The page table and the frame
 * Output: The page associated with the frame
 * Description: Goes through the page table and finds the page that is stored in the given frame
 */
int get_page(const int8_t page_table[], int8_t frame){
	// The result page
	int result = 0;
	// Loop through the page table
	for (int i = 0; i < NUM_PAGES; i++){
		// If the page table at index i contains the frame, store the page and break
		if (page_table[i] == frame){
			result = i;
			break;
		}
	}
	
	// Return the page
	return result;
}

//</editor-fold>
