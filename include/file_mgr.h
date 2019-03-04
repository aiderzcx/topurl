/*
	class CSpiltBigFile: split big file to small files;
		int Split(const char *big_file_name);
		
	class CReadFile: readline from file
		unsigned int ReadLine(char *buf, unsigned int len);
		long long FileSize();
		
	class CWriteFile  write line to file
		unsigned int Write(const char *buf, unsigned int len);
		unsigned int Flush();
*/

#ifndef _FILE_MGR_H_
#define _FILE_MGR_H_

#include <stdio.h>
#include <vector>
#include <string>
#include "buf_mgr.h"

// remove files under the path
void RemoveDirFiles(const char *file_path);

// list the file under the path
int DirFiles(const char *file_path, std::vector<std::string> &files);


class CWriteFile;

// split big file to small files.
class CSpiltBigFile {
	public:
		CSpiltBigFile(const char *small_file_path, unsigned int small_file_num, char split_c);
		~CSpiltBigFile();
		
		int Split(const char *big_file_name);
	
	private:
		char *m_small_file_path;
		unsigned int m_small_file_num;
		unsigned int m_file_path_len;
		CWriteFile *m_small_files;
		
		char m_split_c;
		
		DISALLOW_COPY_AND_ASSIGN(CSpiltBigFile);
};


// read file line and get file size
//  read 8k data from file, and get line from the buf 
class CReadFile {
	public:
		unsigned int ReadLine(char *buf, unsigned int len);
		long long FileSize();
		
		CReadFile(const char *file_name, unsigned int read_buf, char split_c);
		~CReadFile();
	
	private:
		FILE *m_hfile;
		char *m_file_name;
		
		
		char *m_buf;
		CDataBuf *m_data;
		unsigned int m_read_buf_size;
		
		char m_split_c;
		bool m_inited;
				
		int init();
		void clear();
		DISALLOW_COPY_AND_ASSIGN(CReadFile);
};

// write line to file
// first write into buf, when buf is full, save to file
// can use flush save buf data to file
class CWriteFile {
	public:
		unsigned int Write(const char *buf, unsigned int len);
		int Flush();
		int init(const char *file_name, unsigned int write_buf_size);
		
		CWriteFile();
		CWriteFile(const char *file_name, unsigned int write_buf_size);
		~CWriteFile();
			
	private:
		FILE *m_hfile;
		char *m_file_name;
		char *m_buf;
		unsigned int m_write_buf_size;
		CDataBuf *m_data;

		bool m_inited;
		void clear();
		DISALLOW_COPY_AND_ASSIGN(CWriteFile);
};





#endif // _FILE_MGR_H_
