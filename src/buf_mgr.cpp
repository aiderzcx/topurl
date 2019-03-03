#include "buf_mgr.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


CDataBuf::CDataBuf(char *buf, unsigned int len, unsigned int buf_size) {
	assert(NULL != buf && buf_size > 0);
	
	m_buf = buf;
	m_size = buf_size;
	m_len = len;
	m_cur = 0;
}

CDataBuf::~CDataBuf() {
	m_size = 0;
	m_len = 0;
	m_cur = 0;
	m_buf = NULL;
}

unsigned int CDataBuf::DataLen() const {
	return m_len-m_cur;
}

void CDataBuf::Reset(unsigned int len) {
	m_len = len;
	m_cur = 0;
}

struct BufResult CDataBuf::Append(const char *buf, unsigned int len) {
	assert(NULL != buf && len > 0);
	
	struct BufResult result;
	
	if (m_size <= m_len) {
		result.code = BUF_FULL;
		result.len = 0;
		
		return result;
	}
	

	if (m_size - m_len >= len) {
		result.len = len;
		result.code = BUF_SUCC;
	} 
	else { // (m_size > m_len) 
		result.len = m_size - m_len;
		result.code = BUF_PART;
	}
	
	memcpy(&m_buf[m_len], buf, result.len);
	
	m_len += result.len;
	
	return result;
}

struct BufResult CDataBuf::GetData(char *buf, unsigned int len){
	assert(NULL != buf &&  len > 0);

	struct BufResult result;
	
	unsigned int data_len = m_len - m_cur;
	if (0 == data_len) {
		result.code = BUF_EMPTY;
		result.len = 0;
		return result;
	}
	
	if (data_len > len) {
		result.len = len;
		result.code = BUF_SUCC;
	}else {
		result.len = m_len - m_cur;
		result.code = BUF_PART;
	}
	
	memcpy(buf, &m_buf[m_cur], result.len);
	
	m_cur += result.len;
	
	return result;
}

struct BufResult CDataBuf::GetDataWithSplitC(char *buf, unsigned int len, char split_c){
	assert(NULL != buf &&  len > 0);
	
	struct BufResult result;
	
	// have no data to return
	if (m_cur >= m_len) {
		result.code = BUF_EMPTY;
		result.len = 0;
		return result;
	}
	
	unsigned int end_pos = m_cur;
	bool bend = false;
	while ((end_pos < m_len) && (end_pos-m_cur < len)) {
		if (m_buf[end_pos] == split_c) {
			end_pos++; // the url include the '\n'
			bend = true;
			break;
		}
		
		end_pos++;
	}

	result.len = end_pos - m_cur;
	result.code = bend ? BUF_SUCC : BUF_PART;

	if (result.len > 0) {
		memcpy(buf, &m_buf[m_cur], result.len);
	}
	
	m_cur = end_pos;
	return result;
}
	

