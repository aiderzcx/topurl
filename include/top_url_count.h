#ifndef _TOP_URL_COUNT_H_
#define _TOP_URL_COUNT_H_

#include "url_info.h"
#include "hash_map.h"

// count url repeat numbers in a file.
int FileCount(const char *file_name, CHashMap &hash_map);

// use min heap count the top url
class CTopUrlCount {
	public:
		CTopUrlCount(unsigned int top_num);
		~CTopUrlCount();
		
		void Count(const char *url, unsigned int count);
		
		// get node by index, when index more than count, return not CODE_SUCC
		int GetNode(unsigned int index, CUrlInfo &url_info);
		
		// show the result
		void Show() const;
		int SaveToFile(const char *file_name);
		
	private:
		CUrlInfo **m_top_arry;
		unsigned int m_size;
		unsigned int m_len;
};






#endif //_TOP_URL_COUNT_H_
