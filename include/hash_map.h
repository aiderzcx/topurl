/*
hashmap
*/

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include <map>
#include <string> 
#include "url_info.h"


unsigned int BkdrHash(const char *str);

// used for count url,
// list the count result
class CHashMap{
	public:
		CHashMap(unsigned int size);
		~CHashMap();
		
		// count the url repeat number
		void Count(const char *url, unsigned int len, unsigned int hash_val);
		
		// list the url and count
		int GetNext(CUrlInfo *url);
		// restart list, when you want list again
		void RestartNext();
		
		// for debug. show inner data 
		void DebugShow() const;
			
	private:
		std::map<std::string, int> *m_hash_map;
		unsigned int m_hash_size;
		unsigned int m_next_idx;
		std::map<std::string, int>::iterator m_iter;
		bool m_bnext;
		
		CHashMap(const CHashMap &);
		void operator=(const CHashMap&);
};

#endif //_HASH_MAP_H_


