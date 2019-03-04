#include "top_url_count.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstdlib>
#include <algorithm>
#include "define.h"
#include "file_mgr.h"
#include "hash_map.h"

// count file's url repeat number
// input: file_name
// output: CHashMap &hash_map
// return: succ: CODE_SUCC, fail: other..
int FileCount(const char *file_name, CHashMap &hash_map){
	CReadFile read_file(file_name, READ_FILE_BUF_SIZE, SPLIT_C);
	
	int ret = 0;
	char url[MAX_URL_LEN];
	
	while(true) {
		ret = read_file.ReadLine(url, MAX_URL_LEN);
		if (ret <= 3) {
			return CODE_SUCC;
		}
		
		ret = (ret < MAX_URL_LEN) ? ret : ret-1;
		url[ret] = '\0';
		
		hash_map.Count(url, ret, 1);
	}
}


bool cmp(CUrlInfo *a , CUrlInfo *b) {
	return a->GetCount() > b->GetCount();
}

CTopUrlCount::CTopUrlCount(unsigned int top_num){
	m_top_arry = (CUrlInfo **)malloc(sizeof(CUrlInfo *)*(top_num+1));
	if (NULL == m_top_arry) {
		printf("CTopUrlCount::CTopUrlCount new CUrlInfo[%d] fail\n", top_num);
		return;
	}
	
	for (unsigned int i = 0; i < top_num; i++) {
		m_top_arry[i] = new CUrlInfo;
	}
	
	m_size = top_num;
	m_len = 0;
}

CTopUrlCount::~CTopUrlCount(){
	if (NULL != m_top_arry) {
		for (unsigned int i=0; i < m_size; i++){
			delete m_top_arry[i];
		} 
		
		
		free(m_top_arry);
		m_top_arry = NULL;
	}
	
	m_size = 0;
	m_len = 0;
}

void CTopUrlCount::Count(const char *url, unsigned int count) {
	assert(NULL != url && count > 0);
	//printf("CTopUrlCount::Count url: %s, count: %d, size(%d), len(%d)\n", url, count,m_size, m_len);
	
	int len = strlen(url);
	
	// url_num < max_top_url_num, save to arry;
	if (m_size > m_len) {
		m_top_arry[m_len]->Update(url, len, count);
		
		m_len += 1;
		if (m_size == m_len) {
			std::push_heap(&m_top_arry[0],&m_top_arry[m_len], cmp);
		}
	}
	else { // check the first url count
		if (m_top_arry[0]->GetCount() < count) {
			std::pop_heap(&m_top_arry[0], &m_top_arry[m_len], cmp);
			m_top_arry[m_len-1]->Update(url, len, count);
			std::push_heap(&m_top_arry[0],&m_top_arry[m_len], cmp);
		}else{
			return;
		}
	}

	return;
}


int CTopUrlCount::GetNode(unsigned int index, CUrlInfo &url_info) {
	if (index >= m_len) {
		return CODE_EOF;
	}
	
	const char *url = m_top_arry[index]->GetUrl();
	
	url_info.Update(url, strlen(url), m_top_arry[index]->GetCount());
	return CODE_SUCC;
}

void CTopUrlCount::Show() const {
	for (unsigned int i=0; i < m_len; i++) {
		printf("count: %d  url: %s\n", m_top_arry[i]->GetCount(), m_top_arry[i]->GetUrl());
	}
}

int CTopUrlCount::SaveToFile(const char *file_name) {
	assert(NULL != file_name);
	
	FILE *hfile = fopen(file_name, "a+");
	if (NULL == hfile) {
		printf("CTopUrlCount::SaveToFile fopen(%s) fail\n", file_name);
		return CODE_OPEN_FILE_FAIL;
	}
	
	for (unsigned int i=0; i < m_len; i++) {
		fprintf(hfile, "count: %d  url: %s\n", m_top_arry[i]->GetCount(), m_top_arry[i]->GetUrl());
	}
	
	fclose(hfile);
	return CODE_SUCC;
}



