#ifndef __GNILK_MEMFILE_H__
#define __GNILK_MEMFILE_H__

#include <stdint.h>
#include <string>

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

//
// This is a binary writer
//
class Memfile {
    // Note: Use these constants to avoid strange integer conversions when
    //       writing strings out
public:
    static const uint8_t eol; // end-of-line since '\n' converts to an integer
    static const uint8_t eos; // end-of-string since '\0' converts to an integer
public:
	Memfile();
	virtual ~Memfile();

	kMFError Open(kMFOpen flags = kMFOpen_RW);
	kMFError Close();

    // old types
	__inline int32_t Write(const float value) { return(Write(&value, sizeof(float))); }
	__inline int32_t Write(const double value) { return(Write(&value, sizeof(double))); }
    // stdint
	__inline int32_t Write(const char value) { return(Write(&value, sizeof(char))); }
	__inline int32_t Write(const int8_t value) { return(Write(&value, sizeof(int8_t))); }
	__inline int32_t Write(const uint8_t value) { return(Write(&value, sizeof(uint8_t))); }
	__inline int32_t Write(const int16_t value) { return(Write(&value, sizeof(int16_t))); }
	__inline int32_t Write(const uint16_t value) { return(Write(&value, sizeof(uint16_t))); }
	__inline int32_t Write(const int32_t value) { return(Write(&value, sizeof(int32_t))); }
	__inline int32_t Write(const uint32_t value) { return(Write(&value, sizeof(uint32_t))); }

	int32_t Write(const void *data, int32_t numbytes);
	int32_t Read(void *dst, int32_t numbytes);
    int32_t ReadString(char *dst, int32_t maxbytes);

    int32_t ReadSeek(int offset);
    int32_t WriteSeek(int offset);
	uint8_t *Buffer();
	int32_t Size();
	uint32_t Capacity();
	uint32_t Blocksize();

#ifdef __MEMFILE_STD__
	__inline int32_t Write(const std::string value) { return(Write(value.c_str(), value.length())); }
    inline Memfile& operator<<(const std::string &str) {
        Write(str);
        return *this;
    }
	__inline std::string String() {
		const uint8_t zero = 0;
		Write(zero);	// make sure to terminate
		return std::string((char *)buffer)
;	}
#endif
    #define MEMFILE_OP_WRITE(__T__) inline gnilk::Memfile& operator<<(const __T__ value) { Write(value); return *this; }
    MEMFILE_OP_WRITE(float);
    MEMFILE_OP_WRITE(double);
    MEMFILE_OP_WRITE(int8_t);
    MEMFILE_OP_WRITE(uint8_t);
    MEMFILE_OP_WRITE(int16_t);
    MEMFILE_OP_WRITE(uint16_t);
    MEMFILE_OP_WRITE(int32_t);
    MEMFILE_OP_WRITE(uint32_t);
    #undef MEMFILE_OP_WRITE

    inline gnilk::Memfile& operator<<(const char *cstr) {
        Write(cstr, strlen(cstr));
        return *this;
    }

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