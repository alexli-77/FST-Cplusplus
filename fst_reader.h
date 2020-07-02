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
#include <indexlib/index/seg_manager.h>


INDEXLIB_BEGIN_NS(fst);

typedef uint64_t output_t;

class FstReader {
public:
	FstReader();
	~FstReader();

public:
    bool Init(const std::string &fstMmapFile);
    bool SetSegManager(SegWriterPtr segmanager);
	uint32_t GetSlotNum();
    bool IsOpen(const std::string &fstMmapFile);
	bool ExactMatch(const char *key, output_t &pMatchUnits);
    output_t GetUnitPos(const char *key);
    bool RealtimeExactMatch(const char *key, output_t &pMatchPos);

private:
    size_t m_keyCnt;
    size_t m_size;
    size_t m_realtimesize;
    char * m_fstByteArr;
    SegWriterPtr m_currmanager;
    LOG_DECLARE();
};

typedef shared_ptr<FstReader> FstReaderPtr;

INDEXLIB_END_NS(fst);
#endif //INDEXLIB_FST_READER_H_
