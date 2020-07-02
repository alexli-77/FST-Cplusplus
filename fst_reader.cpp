/*********************************************************************
 * File:	fst_reader.cpp 
 * Desc:	
 *
 *********************************************************************/

#include <fstream>
#include <indexlib/fst/fst_reader.h>
#include <indexlib/fst/fstlib.h>
#include <sstream>
INDEXLIB_BEGIN_NS(fst);

using namespace std;

LOG_SETUP(fst, FstReader);
	
FstReader::FstReader(): m_keyCnt(0), m_size(0), m_fstByteArr(NULL) {}

FstReader::~FstReader() {
    if (m_fstByteArr) {
        delete m_fstByteArr;
        m_fstByteArr = NULL;
    }
    
    m_size = 0;
}
bool FstReader::IsOpen(const std::string &fstMmapFile) {
    ifstream File(fstMmapFile);
    if(!File)
    {
        cerr<<"File could not be opend";
        return false;
    }
    return true;
}
bool FstReader::Init(const std::string &fstMmapFile) {
    ifstream fin(fstMmapFile, ios_base::binary);
    if (!fin) { 
        INDEXLIB_LOG(ERROR, "cannot open: %s", fstMmapFile.c_str());
        return false;
	}
    
    fin.seekg(0, ios_base::end);
    auto size = fin.tellg();
    fin.seekg(0, ios_base::beg);

    m_keyCnt = 0;
    fin.read((char*)(&m_keyCnt), sizeof(size_t));
    INDEXLIB_LOG(INFO, "read fst file: %s, file size=%lu, key count=%lu", fstMmapFile.c_str(), m_keyCnt, size);

    if (0 == m_keyCnt) return true;

    size -= sizeof(size_t);
    m_size = size;
    m_fstByteArr = new char[m_size];
    fin.read(m_fstByteArr, m_size);

    fin.close();
    return true;
}
bool FstReader::SetSegManager(SegWriterPtr segmanager) {

    m_currmanager = segmanager;

    return true;
}
bool FstReader::ExactMatch(const char *key, output_t &pMatchPos) {
    if (0 == m_keyCnt) return false;

    istringstream  fin(m_fstByteArr, ios_base::binary);
    if (!fin) {
        INDEXLIB_LOG(ERROR, "cannot open");
        return false;
    }

    fin.seekg(0, ios_base::end);
    auto size = fin.tellg();
    fin.seekg(0, ios_base::beg);

    m_keyCnt = 0;
    fin.read((char*)(&m_keyCnt), sizeof(size_t));
//    INDEXLIB_LOG(INFO, "realtime read data: file size=%lu, key count=%lu", m_keyCnt, size);

    auto outputs = fst::exact_match_search<output_t>(m_fstByteArr, m_size, key);

    if(outputs.empty()) {
        pMatchPos = -1;
    } else {
        pMatchPos = (output_t)(outputs[0]);
    }

	return (pMatchPos != -1);
}
bool FstReader::RealtimeExactMatch(const char *key, output_t &pMatchPos) {

    m_currmanager->mtx.lock();
    if (nullptr == m_currmanager->GetInvertedBufferReader()){
        INDEXLIB_LOG(INFO, "read fst reader is null");
        return false;
    }

    m_size = m_currmanager->GetInvertedBufferReaderSize();

    char *buffTail = m_currmanager->GetInvertedBufferReader();
    auto outputs = fst::exact_match_search<output_t>(buffTail, m_size, key);
    m_currmanager->mtx.unlock();
    if(outputs.empty()) {
        pMatchPos = -1;
        INDEXLIB_LOG(INFO, "exact_match_search is null");
    } else {
        pMatchPos = (output_t)(outputs[0]);
    }

    return (pMatchPos != -1);
}
output_t FstReader::GetUnitPos(const char *key)
{
    output_t matchPos = -1;
    ExactMatch(key, matchPos);

    return matchPos;
}

uint32_t FstReader::GetSlotNum()
{
	return m_keyCnt;
}

INDEXLIB_END_NS(fst);
