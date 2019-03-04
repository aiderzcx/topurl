#ifndef _BUF_MGR_H_
#define _BUF_MGR_H_

enum BUF_RESULT {
	BUF_SUCC = 0,
	BUF_EMPTY,
	BUF_FULL,
	BUF_PART
};

struct BufResult {
	BUF_RESULT code;
	unsigned int len; // return the length of read or append;
};

/*
	share buf 
	used for read and write buf
*/
class CDataBuf {
	public:
		CDataBuf(char *buf, unsigned int len, unsigned int buf_size);
		~CDataBuf();
	
		struct BufResult Append(const char *buf, unsigned int len);
		
		struct BufResult GetData(char *buf, unsigned int len);
		struct BufResult GetDataWithSplitC(char *buf, unsigned int len, char split_c);
				
		unsigned int DataLen() const;
		
		// m_buf is shared, when update m_buf data, need reset the inner data_len. 
		void Reset(unsigned int len);
		
	private:
		char *m_buf;
		unsigned int m_size;
		unsigned int m_len;
		unsigned int m_cur;
		
		CDataBuf(const CDataBuf &);
		void operator=(const CDataBuf&);
};



#endif //_BUF_MGR_H_
