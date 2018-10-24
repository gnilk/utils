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

typedef enum {
	kMFErr_NoError = 0,
	kMFErr_Malloc = -1,
	kMFErr_NotImplemented = -2,
	kMFErr_NotOpen = -3,
	kMFErr_Realloc = -4,
	kMFErr_NotAllowed = -5,
} kMFError;

typedef enum {
	kMFOpen_R = 1,
	kMFOpen_W = 2,
	kMFOpen_RW = 3,
} kMFOpen;

#define MF_DEFAULT_BLOCKSZ 64

class Memfile {
public:
	Memfile();
	virtual ~Memfile();

	kMFError Open(kMFOpen flags = kMFOpen_RW);
	kMFError Close();

	int32_t Write(const uint8_t *data, int32_t numbytes);
	int32_t Read(uint8_t *dst, int32_t numbytes);

	uint8_t *Buffer();
	int32_t Size();
	uint32_t Capacity();
	uint32_t Blocksize();

	bool CanRead() { return (mode & kMFOpen_R); }
	bool CanWrite() { return (mode & kMFOpen_W); }

protected:
	kMFError Extend();
private:
	uint8_t *buffer;
	uint32_t capacity;
	uint32_t length;
	uint32_t wptr;
	uint32_t rptr;

	kMFOpen mode;

	uint32_t blocksize;
};

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
int32_t Memfile::Write(const uint8_t *data, int32_t numbytes) {
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

//const char *dummy="hello world!";

int main(int argc, char **argv) {
	printf("Hello world!\n");
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		printf("Failed to open, err: %d\n", err);
		exit(1);
	}
	int32_t szData = 256;
	uint8_t *data = (uint8_t *)malloc(szData);
	for(int i=0;i<szData;i++) {
		data[i] = (i & 0x08);
	}

	int ierr = mf.Write(data, szData);
	if (ierr < 0) {
		printf("Write failed, err: %d\n", err);
	}
	mf.Close();

	uint8_t *ptrData = mf.Buffer();
	szData = mf.Size();

	FILE *f = fopen("dump.bin","w");
	if (f == NULL) {
		printf("fopen failed\n");
		exit(1);
	}	
	fwrite(ptrData, szData, 1, f);
	fclose(f);
	printf("Wrote data to file\n");
	printf("Ok\n");
}