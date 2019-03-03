#include "url_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "define.h"

CUrlInfo::CUrlInfo() {
	m_buf = NULL;
	m_size = 0;
	m_len = 0;
	m_count = 0;
}

CUrlInfo& CUrlInfo::operator=(const CUrlInfo &url){
	if (this == &url){
		return *this;
	}
	
	const char *str = url.GetUrl();	
	Update(str, strlen(str), url.GetCount());
	
	return *this;
}


CUrlInfo::~CUrlInfo() {
	clear();
}

void CUrlInfo::clear(){
	if (NULL != m_buf) {
		free(m_buf);
		m_buf = NULL;
	}
	
	m_size = 0;
	m_len = 0;
	m_count = 0;
}

int CUrlInfo::Update(const char *url, unsigned int len, unsigned int count) {
	assert(NULL != url && len > 0);
	
	if (m_size < len) {
		clear();
		
		m_buf = (char *)malloc(len+1);
		if (NULL == m_buf) {
			printf("CTopUrlInfo::Update malloc(%d) fail\n", len+1);
			return CODE_MALLOC_FAIL;
		}
		
		m_size = len+1;
	}
	
	memcpy(m_buf, url, len);
	m_buf[len] = '\0';
	
	m_len = len;
	m_count = count;
	
	return CODE_SUCC;
}

char* CUrlInfo::GetUrl() const{
	return m_buf;
}

unsigned int CUrlInfo::GetCount() const{
	return m_count;
}

void CUrlInfo::AddCount(){
	m_count += 1;
}
		
