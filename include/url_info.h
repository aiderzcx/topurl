#ifndef _URL_INFO_H_
#define _URL_INFO_H_

class CUrlInfo {
	public:
		CUrlInfo();
		~CUrlInfo();
		
		CUrlInfo& operator=(const CUrlInfo &url);
		
		int Update(const char *url, unsigned int len, unsigned int count);
		char* GetUrl() const;
		unsigned int GetCount() const;
		void AddCount();
		
		

	private:
		char *m_buf;
		unsigned int m_size;
		unsigned int m_len;
		unsigned int m_count;
		
		void clear();
		
		CUrlInfo(const CUrlInfo &);
};

#endif // _URL_INFO_H_
