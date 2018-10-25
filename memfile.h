#ifndef __GNILK_MEMFILE_H__
#define __GNILK_MEMFILE_H__

#include <stdint.h>

namespace gnilk {

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


	__inline int32_t Write(const float value) { return(Write(&value, sizeof(float))); }
	__inline int32_t Write(const double value) { return(Write(&value, sizeof(double))); }
	__inline int32_t Write(const int8_t value) { return(Write(&value, sizeof(int8_t))); }
	__inline int32_t Write(const uint8_t value) { return(Write(&value, sizeof(uint8_t))); }
	__inline int32_t Write(const int16_t value) { return(Write(&value, sizeof(int16_t))); }
	__inline int32_t Write(const uint16_t value) { return(Write(&value, sizeof(uint16_t))); }
	__inline int32_t Write(const int32_t value) { return(Write(&value, sizeof(int32_t))); }
	__inline int32_t Write(const uint32_t value) { return(Write(&value, sizeof(uint32_t))); }

	int32_t Write(const void *data, int32_t numbytes);
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

}   // namespace

#endif