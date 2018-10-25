/*-------------------------------------------------------------------------
File    : memfile.cpp
Author  : FKling
Version : $Revision: 1 $
Orginal : 2018-10-24
Descr   : Implements a membuffer file-like API

Modified: $Date: $ by $Author: FKling $
---------------------------------------------------------------------------
TODO: [ -:Not done, +:In progress, !:Completed]
<pre>
</pre>


\History
- 18.10.24, FKling, Implementation

---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "memfile.h"

using namespace gnilk;

Memfile::Memfile() {
	buffer = NULL;
	blocksize = MF_DEFAULT_BLOCKSZ;
	capacity = 0;
	wptr = 0;
	rptr = 0;
	length = 0;
}
Memfile::~Memfile() {
	if (buffer != NULL) {
		free(buffer);
	}
}

//
// Open a memfile object
//
kMFError Memfile::Open(kMFOpen flags /* = kMFOpen_RW */) {

	// Reset here, allows a file to be repoened
	wptr = 0;
	rptr = 0;
	mode = flags;

	if (buffer == NULL) {
		Extend();
	}
	return kMFErr_NoError;
}

//
// Cose a memfile object
//
kMFError Memfile::Close() {
	return kMFErr_NoError;
}

//
// Write numbytes to the memfile from data, extend memfile buffers as needed
//
int32_t Memfile::Write(const void *data, int32_t numbytes) {
	if (buffer == NULL) {
		return kMFErr_NotOpen;
	}
	if (!CanWrite()) {
		return kMFErr_NotAllowed;
	}


	while((wptr + numbytes) > capacity) {
		kMFError err = Extend();
		if (err != kMFErr_NoError) {
			return err;
		}
	}
	// copy data
	memcpy(buffer + wptr, data, numbytes);
	// Advance write pointer and length

	int32_t dl = (length - wptr);
	if (numbytes > dl) {
		length += numbytes - dl;		
	}
	wptr += numbytes;

	return numbytes;
}

//
// Read copies numbytes to destination and returns actually number of bytes copied
//
int32_t Memfile::Read(uint8_t *dst, int32_t numbytes) {
	if (buffer == NULL) {
		return kMFErr_NotOpen;
	}
	if (!CanRead()) {
		return kMFErr_NotAllowed;
	}
	// Make sure we don't read outside available data
	if ((numbytes + rptr) > length) {
		numbytes = length - rptr;
	}

	memcpy(dst, buffer + rptr, numbytes);
	return numbytes;
}


//
// Extend the internal buffer by blocksize number of bytes
//
kMFError Memfile::Extend() {
	if (buffer == NULL) {
		buffer = (uint8_t *)malloc(blocksize);
		if (buffer == NULL) {
			return kMFErr_Malloc;
		}
		capacity = blocksize;
		return kMFErr_NoError;
	}
	printf("Extend, current: %d bytes, adding: %d\n", capacity, blocksize);

	buffer = (uint8_t *)realloc(buffer, capacity + blocksize);
	if (buffer == NULL) {
		return kMFErr_Realloc;
	}
	capacity += blocksize;
	return kMFErr_NoError;
}

//
// Buffer returns a raw pointer to the internal buffer
//
uint8_t *Memfile::Buffer() {
	return buffer;
}

//
// Size returns the length of actual data in the internal buffer
//
int32_t Memfile::Size() {
	return length;
}

//
// Capacity return the current capacity of the memfile 
//
uint32_t Memfile::Capacity() {
	return capacity;
}

//
// Blocksize returns the number of bytes per expansion
//
uint32_t Memfile::Blocksize() {
	return blocksize;
}

