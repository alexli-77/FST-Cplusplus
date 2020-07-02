/*********************************************************************
 * File:	fst_reader.h 
 * Desc:	load fst file and search
 *
 *********************************************************************/
 
 
#ifndef INDEXLIB_FST_READER_H_
#define INDEXLIB_FST_READER_H_

#include <vector>
#include <cstdio>
#include <stdint.h>

#include <indexlib/common.h>
#include <indexlib/util/log.h>

INDEXLIB_BEGIN_NS(fst);

typedef uint64_t output_t;

class FstReader {
public:
	FstReader();
	~FstReader();

public:
    bool Init(const std::string &fstMmapFile);
	uint32_t GetSlotNum();
	bool ExactMatch(const char *key, output_t &pMatchUnits);
    output_t GetUnitPos(const char *key);

private:
    size_t m_keyCnt;
    size_t m_size;
    char * m_fstByteArr;

    LOG_DECLARE();
};	

TYPEDEF_SHARED_PTR(FstReader);

INDEXLIB_END_NS(fst);
#endif //INDEXLIB_FST_READER_H_
