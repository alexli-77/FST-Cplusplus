/*********************************************************************
 * 
 * File:	fst_writer.h 
 * Desc:	build fst array and write fst array to file.
 *
 *********************************************************************/
 
#ifndef INDEXLIB_FST_WRITER_H_
#define INDEXLIB_FST_WRITER_H_

#include <vector>
#include <string>
#include <cstdio>
#include <stdint.h>
#include <fstream>
#include <indexlib/index/seg_writer.h>
#include <indexlib/util/log.h>
#include <indexlib/common.h>

INDEXLIB_BEGIN_NS(fst);

typedef size_t output_t;

class FstWriter
{
public:
	FstWriter(const std::string &fstMmapFile);
	~FstWriter();
public:
    bool Init();
    // add key into key array
	void AddKey(const char *key, output_t offset);

	size_t getSlotNum();

    // build fst array and write to file.
	void Write();

    bool Init(SegWriterPtr segmanager);

    void Write(size_t tail);

    void swapKey();

    void clearBufferSize();
    
private:
	uint32_t m_keyIdx;
    std::ofstream m_fout;
    std::vector<std::pair<std::string, output_t> > m_keys;
    std::string m_fstMmapFile;
    SegWriterPtr m_currmanager;
    size_t m_bufLen;
    size_t m_bufferSize = 10 * 1024 * 1024;
    size_t m_fileOffset;
    LOG_DECLARE();
};	

TYPEDEF_SHARED_PTR(FstWriter);

INDEXLIB_END_NS(fst);

#endif //INDEXLIB_FST_WRITER_H_
