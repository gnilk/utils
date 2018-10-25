//
// Various test cases for read/write
//
#include <testinterface.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../memfile.h"
#include <string>

using namespace gnilk;

extern "C" {
int test_memfile_open(ITesting *t);
//int test_memfile_generic(ITesting *t);	
int test_memfile_openread(ITesting *t);
int test_memfile_opentryread(ITesting *t);
int test_memfile_opentrywrite(ITesting *t);
int test_memfile_writetypecheck(ITesting *t);
int test_memfile_openrw(ITesting *t);
int test_memfile_opout(ITesting *t);
int test_memfile_readstring(ITesting *t);
int test_memfile_seek(ITesting *t);
#ifdef __MEMFILE_STD__
int test_memfile_opout_stdstring(ITesting *t);
#endif
}

int test_memfile_open(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		return kTR_Fail;
	}
	mf.Close();	
    return kTR_Pass;
}

int test_memfile_openread(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		return kTR_Fail;
	}
	uint8_t buffer[128];
	int32_t numRead = mf.Read(buffer, 16);
	if (numRead > 0) {
		return kTR_Fail;
	}
	if (numRead < 0) {
		return kTR_Fail;
	}
    return kTR_Pass;
}

int test_memfile_opentryread(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open(kMFOpen_W)) != kMFErr_NoError) {
		return kTR_Fail;
	}
	uint8_t buffer[128];
	int32_t numRead = mf.Read(buffer, 16);
	if (numRead != (int32_t)kMFErr_NotAllowed) {
		return kTR_Fail;
	}
    return kTR_Pass;
}

int test_memfile_openrw(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open(kMFOpen_RW)) != kMFErr_NoError) {
		return kTR_Fail;
	}
	//
	// This works as we have separate pointers for read & write
	// As long as length is not affect (write) no locks are required
	// for interleaved read's while writing..
	//
	uint8_t buffer[128];
	int32_t numWritten = mf.Write(buffer, 16);
	if (numWritten < 16) {
		return kTR_Fail;
	}
	int32_t numRead = mf.Read(buffer, 16);
	if (numWritten != numRead) {
		return kTR_Fail;
	}

    return kTR_Pass;
}


int test_memfile_writetypecheck(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open(kMFOpen_W)) != kMFErr_NoError) {
		return kTR_Fail;
	}
	int32_t numWritten;
	// No decoration should be an int
	numWritten = mf.Write(1);
	if (numWritten != sizeof(int)) {
		return kTR_Fail;
	}
	numWritten = mf.Write(1.0f);
	if (numWritten != sizeof(float)) {
		return kTR_Fail;
	}

	#define WRITE_CHECK(_t_,_v_) if ((numWritten = mf.Write((const _t_)(_v_))) != sizeof(_t_)) { return kTR_Fail; }

	WRITE_CHECK(int8_t, 1);
	WRITE_CHECK(uint8_t, 1);
	WRITE_CHECK(int16_t, 1);
	WRITE_CHECK(uint16_t, 1);
	WRITE_CHECK(int32_t, 1);
	WRITE_CHECK(uint32_t, 1);

	#undef WRITE_CHECK

    return kTR_Pass;
}

int test_memfile_opout(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		t->Error(__LINE__, __FILE__, "mf.Open, err: %d", err);
		return kTR_Fail;
	}
	mf << "hello world " << "this is fun";
	char buffer[128];
	int32_t nRead = mf.Read(buffer, mf.Size());
	buffer[nRead+1]='\0';
	if (strcmp(buffer, "hello world this is fun")) {
		t->Error(__LINE__, __FILE__, "write/read content mismatch");
		return kTR_Fail;
	}

	mf.Close();
	return kTR_Pass;
}



int test_memfile_opentrywrite(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open(kMFOpen_R)) != kMFErr_NoError) {
		return kTR_Fail;
	}
	uint8_t buffer[128];
	int32_t numWritten = mf.Write(buffer, 16);
	if (numWritten != (int32_t)kMFErr_NotAllowed) {
		return kTR_Fail;
	}
    return kTR_Pass;
}

// NOT EXPORTED!
int test_memfile_generic(ITesting *t) {
//const char *dummy="hello world!";
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		printf("Failed to open, err: %d\n", err);
		return kTR_Fail;
	}
	int32_t szData = 256;
	uint8_t *data = (uint8_t *)malloc(szData);
	for(int i=0;i<szData;i++) {
		data[i] = (i & 0x08);
	}

	int ierr = mf.Write(data, szData);
	if (ierr < 0) {
		printf("Write failed, err: %d\n", err);
		return kTR_Fail;
	}
	mf.Close();

	uint8_t *ptrData = mf.Buffer();
	szData = mf.Size();

	FILE *f = fopen("dump.bin","w");
	if (f == NULL) {
		printf("fopen failed\n");
		return kTR_Fail;
	}	
	fwrite(ptrData, szData, 1, f);
	fclose(f);
	printf("Wrote data to file\n");
	return kTR_Pass;
}

#ifdef __MEMFILE_STD__
int test_memfile_opout_stdstring(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		t->Error(__LINE__, __FILE__, "mf.Open, err: %d", err);
		return kTR_Fail;
	}
	std::string content("hello world");
	mf << content;

	if (strncmp((const char *)mf.Buffer(), content.c_str(),content.length())) {
		t->Error(__LINE__, __FILE__, "write/read content mismatch");
		return kTR_Fail;
	}
	mf.Close();
	return kTR_Pass;
}
#endif

int test_memfile_readstring(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		t->Error(__LINE__, __FILE__, "mf.Open, err: %d", err);
		return kTR_Fail;
	}
	mf << "hello world";
	mf << Memfile::eol;
	mf << "not there";
	mf << Memfile::eos;


	int res;
	char buffer[128];
	res = (kMFError)mf.ReadString(buffer, 128);
	if (res < 0) {
		t->Error(__LINE__, __FILE__, "Memfile::ReadString failed, err=%d", err);
		return kTR_Fail;
	}
	if (strcmp(buffer, "hello world")) {
		t->Error(__LINE__, __FILE__, "content mismatch, got: %s (%s)", buffer, mf.Buffer());
		return kTR_Fail;
	}
	res = (kMFError)mf.ReadString(buffer, 128);
	if (strcmp(buffer, "not there")) {
		t->Error(__LINE__, __FILE__, "content mismatch, got: %s (%s)", buffer, mf.Buffer());
	}
	return kTR_Pass;
}

int test_memfile_seek(ITesting *t) {
	Memfile mf;
	kMFError err;
	if ((err = mf.Open()) != kMFErr_NoError) {
		t->Error(__LINE__, __FILE__, "mf.Open, err: %d", err);
		return kTR_Fail;
	}
	mf << "hello world" << Memfile::eol << "string two" << Memfile::eos;

	int res;
	char buffer[128];
	res = (kMFError)mf.ReadString(buffer, 128);

	// Reset to zero and check if we got 'hello world'
	mf.ReadSeek(0);
	res = (kMFError)mf.ReadString(buffer, 128);
	if (res < 0) {
		t->Error(__LINE__, __FILE__, "Memfile::ReadString failed, err=%d", err);
		return kTR_Fail;
	}
	if (strcmp(buffer, "hello world")) {
		t->Error(__LINE__, __FILE__, "content mismatch, got: %s (%s)", buffer, mf.Buffer());
		return kTR_Fail;
	}

	// Reset to '6' and read again
	mf.ReadSeek(6);
	res = (kMFError)mf.ReadString(buffer, 128);
	if (res < 0) {
		t->Error(__LINE__, __FILE__, "Memfile::ReadString failed, err=%d", err);
		return kTR_Fail;
	}
	if (strcmp(buffer, "world")) {
		t->Error(__LINE__, __FILE__, "content mismatch, got: %s (%s)", buffer, mf.Buffer());
		return kTR_Fail;
	}
	return kTR_Pass;
}

