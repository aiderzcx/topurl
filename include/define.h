#ifndef _DEFINE_H_
#define _DEFINE_H_
	
	const unsigned int TOTAL_MEM_SIZE = 1024*1024*1024; // 1G memory
	
	// big file split to small files, small file size
	const unsigned int SPLIT_FILE_SIZE = TOTAL_MEM_SIZE / 4;
	
	const char SPLIT_C = '\n';

	// small file resplit size
	const unsigned int RESPLIT_FILE_SIZE = TOTAL_MEM_SIZE * 3 / 4;
	
	//once read data size
	const int READ_FILE_BUF_SIZE = 8*1024; 
	
	const int TOP_URL_NUM = 100;
	
	const int MAX_URL_LEN = 512;
	
	const int FILE_COUNT_HASH_SIZE = 20000;
	
	enum err_code{
		CODE_SUCC = 0, 
		CODE_INPUT_ERR,
		CODE_FAIL,
		CODE_OPEN_FILE_FAIL,
		CODE_DIR_NOT_EXIST,
		CODE_MALLOC_FAIL,
		CODE_EOF
	};
	

#endif // _DEFINE_H_
