/*
 * Author: Logan Thomas
 * Assignment Number: 4
 * Date of Submission: 12/11/2018
 * Name of this file: Assignment4.h
 * Description of the program: The macros for the assignment
 */

#ifndef ASSIGNMENT4_H
#define ASSIGNMENT4_H

// <editor-fold desc='Includes'>

#include <stdint.h>

// </editor-fold>

//<editor-fold Defines>

// Number of bits to store the offset
#define NUM_OFFSET_BITS 8
// The size of a data block
#define DATA_BLOCK_SIZE (1 << 8)

// The size of a frame in bytes (should be the same size as a page)
#define FRAME_SIZE (1 << NUM_OFFSET_BITS)
// The number of frames in memory
#define FRAME_COUNT (1 << 6)
// The number of pages
#define NUM_PAGES (1 << 8)

// Number of frames * number of bytes per frame gives total memory size
#define RAM_SIZE (FRAME_SIZE * FRAME_COUNT)

// Mask to extract offset from address (16 bit unsigned)
#define OFFSET_MASK ((uint16_t)((1 << NUM_OFFSET_BITS) - 1))
// Macro to extract offset using OFFSET_MASK (8 bit unsigned)
#define OFFSET(logical_address) ((uint8_t)(logical_address & OFFSET_MASK))
// Macro to extract page from address (8 bit unsigned)
#define PAGE(logical_address) ((uint8_t)(logical_address >> NUM_OFFSET_BITS))
// Macro to get the physical address (16 bit unsigned)
#define PHYSICAL_ADDRESS(frame, offset) ((uint16_t)((frame << NUM_OFFSET_BITS) | offset))
// Macro to get the address in the backing store without the offset
#define PAGE_ADDRESS(page) ((uint16_t) page << NUM_OFFSET_BITS)
// Macro to get the address of a frame in RAM
#define FRAME_ADDRESS(frame) ((uint16_t) frame << NUM_OFFSET_BITS)

//</editor-fold>

#endif	/* end of include guard: ASSIGNMENT4_H */
