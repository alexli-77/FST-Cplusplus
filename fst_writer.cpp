/*********************************************************************
 * 
 * File:	fst_writer.cpp 
 * Desc:	build fst array and write array to file.
 *
 *********************************************************************/

#include <fstream>
#include <cstring>

#include <indexlib/fst/fst_writer.h>
#include <indexlib/fst/fstlib.h>

INDEXLIB_BEGIN_NS(fst);
using namespace std;

LOG_SETUP(fst, FstWriter);
	
FstWriter::FstWriter(const std::string &fstMmapFile):m_fstMmapFile(fstMmapFile) {}

FstWriter::~FstWriter() {}

bool FstWriter::Init() {

    m_fout = ofstream(m_fstMmapFile, ios_base::binary);

    if (!m_fout) { 
		INDEXLIB_LOG(ERROR, "cannot init fst file: %s\n", m_fstMmapFile.c_str()); 
		return false; 
	}

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
    //sort(m_keys.begin(), m_keys.end(),
    //        [](const auto &a, const auto &b) { return strcmp(a.first, b.first);});
    //sort(m_keys.begin(), m_keys.end(),
    //        [](const auto &a, const auto &b) { return a.first < b.first;});

    
    size_t keyCnt = m_keys.size();
    m_fout.write((const char*)(&keyCnt), sizeof(size_t));
    INDEXLIB_LOG(INFO, "write fstfile: %s, total key: %lu.", m_fstMmapFile.c_str(), keyCnt); 
    if (0 == keyCnt) {
        return; 
    }
    
    auto byte_code = fst::build(m_keys);
    m_fout.write(byte_code.data(), byte_code.size());
}

void FstWriter::swapKey(){
        std::vector<std::pair<std::string, output_t> >().swap(m_keys);
}
INDEXLIB_END_NS(fst);
