#include "hash_map.h"
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "define.h"


// BKDR hash
unsigned int BkdrHash(const char *str) {
	assert(NULL != str);
	
	unsigned int seed = 131;
	unsigned int hash = 0;
	
	while(*str != '\0') {
		
		hash = hash*seed + (*str++);
	}
	
	return (hash &0x7FFFFFFF);
} 

CHashMap::CHashMap(unsigned int size) {
	assert(size > 0); 
	
	m_hash_map = new std::map<std::string, int>[size];
	if (NULL == m_hash_map) {
		printf("CHashMap::CHashMap new hash_map[%d] fail\n", size);
		return;
	}
	
	m_hash_size = size;
	m_next_idx = 0;
	m_bnext = false;
}

CHashMap::~CHashMap(){
	if (NULL != m_hash_map) {
		for (unsigned int i=0; i < m_hash_size; i++) {
			m_hash_map[i].clear();
		}
		
		delete[] m_hash_map;
		m_hash_map = NULL;
	}
}

void CHashMap::Count(const char *url, unsigned int len, unsigned int hash_val) {
	assert(NULL != url && len > 0);
	
	std::string key(url, len);
	std::map<std::string, int>::iterator iter;  
  	
  	unsigned int hash_idx = hash_val % m_hash_size;
  
    iter = m_hash_map[hash_idx].find(key);  
    
    if (iter != m_hash_map[hash_idx].end()) {
    	m_hash_map[hash_idx][key] = iter->second +1;
	}else {
		m_hash_map[hash_idx][key] = 1;
	}
  
	return;
}

int CHashMap::GetNext(CUrlInfo *url) {
	assert(NULL != url);
	
	if (!m_bnext) {
		m_iter = m_hash_map[0].begin();
		m_next_idx = 0;
		m_bnext = true;
	}
	
	if (m_next_idx >= m_hash_size) {
		return CODE_EOF;
	}
	
	do {
		if (m_iter != m_hash_map[m_next_idx].end()){
			url->Update(m_iter->first.c_str(), m_iter->first.length(), m_iter->second);
			m_iter++;
			
			return CODE_SUCC;
		}else{
			m_next_idx += 1;
			
			if (m_hash_size > m_next_idx) {
				m_iter =  m_hash_map[m_next_idx].begin();
				continue;
			}else{
				return CODE_EOF;
			}
		}
	}while(true);
}

void CHashMap::RestartNext(){
	m_bnext = false;
}


void CHashMap::DebugShow() const{
	printf("CHashMap::DebugShow m_bnext: %d\n", m_bnext);
	printf("CHashMap::DebugShow m_hash_size: %d\n", m_hash_size);
	printf("CHashMap::DebugShow m_next_idx: %d\n", m_next_idx);
	
	std::map<std::string, int>::iterator iter;
	for (int i=0; i<m_hash_size; i++) {
		for (iter=m_hash_map[i].begin(); iter != m_hash_map[i].end(); iter++) {
			printf("CHashMap::DebugShow hash_map[%d] key: %s, v: %d\n", i, iter->first.c_str(), iter->second);
		}
	}	
}





