/*********************************************************************
 * 
 * File:	fst_writer.cpp 
 * Desc:	build fst array and write array to file.
 *
 *********************************************************************/

#include <fstream>
#include <cstring>
#include <sstream>
#include <indexlib/fst/fst_writer.h>
#include <indexlib/fst/fstlib.h>

INDEXLIB_BEGIN_NS(fst);
using namespace std;

LOG_SETUP(fst, FstWriter);
	
FstWriter::FstWriter(const std::string &fstMmapFile):m_fstMmapFile(fstMmapFile),m_fileOffset(0),m_bufLen(0) {}

FstWriter::~FstWriter() {}

bool FstWriter::Init() {

    m_fout = ofstream(m_fstMmapFile, ios_base::binary);

    if (!m_fout) { 
		INDEXLIB_LOG(ERROR, "cannot init fst file: %s\n", m_fstMmapFile.c_str()); 
		return false; 
	}

    return true;
}

bool FstWriter::Init(SegWriterPtr segmanager) {

    m_currmanager = segmanager;

    return true;
}

void FstWriter::AddKey(const char *key, output_t offset) {
    size_t len = strlen(key);
    if (len < 1) return;

    m_keys.emplace_back(std::make_pair(std::string(key), offset));
}

size_t FstWriter::getSlotNum() {
	return m_keys.size();
}

void FstWriter::Write() {

    size_t keyCnt = m_keys.size();
    INDEXLIB_LOG(INFO, "index write total key: %lu.", keyCnt);
    m_fout.write((const char*)(&keyCnt), sizeof(size_t));
    if (0 == keyCnt) {
        return; 
    }
    
    auto byte_code = fst::build(m_keys);
    m_fout.write(byte_code.data(), byte_code.size());
    INDEXLIB_LOG(INFO, "index::byte_code size is : %lu.",  byte_code.size());
}

void FstWriter::Write(size_t tail) {
    size_t keyCnt = m_keys.size();
    INDEXLIB_LOG(INFO, "realtimeindex write total key: %lu.", keyCnt);
    if (0 == keyCnt) {
        return;
    }


    auto byte_code = fst::build(m_keys);

    size_t len = byte_code.size();
    size_t dataLen = len + tail;
    m_currmanager->SetInvertedBufferReaderSize(dataLen);
    char *buffTail = m_currmanager->GetInvertedBufferReader() + m_bufLen;
//    memcpy(buffTail, byte_code.data(), dataLen);
    m_currmanager->SetInvertedBufferReader(byte_code.data());
    INDEXLIB_LOG(INFO, "byte_code size is : %lu. datalen size is : %lu",  byte_code.size(), dataLen);
    m_bufLen = dataLen;
    //stringstream并不清除缓存，需要使用这个来释放。flush无效。clear也无效。
}

void FstWriter::swapKey(){
    std::vector<std::pair<std::string, output_t> >().swap(m_keys);
}

void FstWriter::clearBufferSize(){
    m_bufLen = 0;
}
INDEXLIB_END_NS(fst);
