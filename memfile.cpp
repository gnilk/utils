/*-------------------------------------------------------------------------
File    : memfile.cpp
Author  : FKling
Version : $Revision: 1 $
Orginal : 2018-10-24
Descr   : Implements a membuffer file-like API

Note: This is a binary writer, 'mf << (int)1' will write the hex '0x01' to the 
      current write position. 


Modified: $Date: $ by $Author: FKling $
---------------------------------------------------------------------------
TODO: [ -:Not done, +:In progress, !:Completed]
<pre>
	- Split out string specific things to own some kind of string builder
</pre>


\History
- 18.11.05, FKling, Added capability to create an object from an existing buffer
- 18.10.25, FKling, Open called in constructor - simplifies thing, you can still change mode in 'Open'
- 18.10.25, FKling, Operators (<<) for data types, Read/Write seeking, ReadString
- 18.10.24, FKling, Implementation

---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "memfile.h"

using namespace gnilk;

// This can be platform specific
// macosx '\n' = 0x0a
// windows '\n' = 0x0d, 0x0a
// linux   '\n' = 

// TODO: Remove from here
const uint8_t Memfile::eol = (uint8_t)'\n'; // end-of-line since '\n' converts to an integer
const uint8_t Memfile::eos = (uint8_t)'\0'; // end-of-string since '\0' converts to an integer


Memfile::Memfile() {
	buffer = NULL;
	blocksize = MF_DEFAULT_BLOCKSZ;
	capacity = 0;
	wptr = 0;
	rptr = 0;
	length = 0;
	Open(kMFOpen_RW);
}
#ifdef __MEMFILE_STD__
Memfile::Memfile(const std::string &data) {
	buffer = NULL;
	blocksize = MF_DEFAULT_BLOCKSZ;
	capacity = 0;
	while(capacity < data.length()) {
		Extend();
	}
	memcpy(buffer, data.c_str(), data.length());
	length = data.length();

	wptr = 0;
	rptr = 0;
	Open(kMFOpen_RW);
}
#endif

//
// Destroy the object
//
Memfile::~Memfile() {
	if (buffer != NULL) {
		free(buffer);
		buffer = NULL;
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
		//printf("Memfile::Open, calling extend\n");
		Extend();
	}
	return kMFErr_NoError;
}

//
// Close a memfile object
//
kMFError Memfile::Close() {
	if (buffer != NULL) {
		//printf("Memfile::Close, buffer=%p\n", buffer);
		free(buffer);
		buffer = NULL;
	}
	blocksize = MF_DEFAULT_BLOCKSZ;
	capacity = 0;
	wptr = 0;
	rptr = 0;
	length = 0;

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
int32_t Memfile::Read(void *dst, int32_t numbytes) {
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
	//printf("Memfile::Read, numbytes %d (rptr: %d, length: %d)\n", numbytes, rptr, length);

	memcpy(dst, buffer + rptr, numbytes);
	rptr += numbytes;
	return numbytes;
}

//
// ReadString reads a bunch of char's and stops on anything below ' '
//
int32_t Memfile::ReadString(char *dst, int32_t maxbytes) {
	int32_t i = 0;
	char v;
	int32_t res;

	while(true) {
		// Max reached - need space for '\0' as we are reading a string...
		if (i > (maxbytes-1)) {
			break;
		}
		res = (kMFError)Read(&v,1);
	//	printf("%c,%.2x\n", v,v);
		if (res < 0) {
			return res;
		}
		// Stop on any non-ascii below 32
		if (v < 32) {
			break;
		}
		dst[i++] = v;
	}
	dst[i]='\0';
	return i;
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
	//printf("Extend, current: %d bytes, adding: %d\n", capacity, blocksize);

	buffer = (uint8_t *)realloc(buffer, capacity + blocksize);
	if (buffer == NULL) {
		return kMFErr_Realloc;
	}
	capacity += blocksize;
	return kMFErr_NoError;
}

int32_t Memfile::ReadSeek(int offset) {
	if (offset > length) {
		offset = length;
	}
	if (offset < 0) {
		offset = 0;
	}
	rptr = offset;
	return rptr;
}

int32_t Memfile::WriteSeek(int offset) {
	if (offset > length) {
		offset = length;
	}
	if (offset < 0) {
		offset = 0;
	}
	wptr = offset;
	return wptr;
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

