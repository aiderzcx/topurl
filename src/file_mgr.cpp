#include "file_mgr.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
#include "define.h"
#include "hash_map.h"


const int URL_MAX_SIZE = 512;
const int FILE_PATH_LEN = 512;

const char *SMALL_FILE_PRE = "small_file";

// list files of the dir
int DirFiles(const char *file_path, std::vector<std::string> &files) {
	assert(NULL != file_path);
	
	DIR *dirptr = NULL;
    struct dirent *entry = NULL;

    if ((dirptr = opendir(file_path)) == NULL) {
        printf("DirFiles opendir(%s) fail\n", file_path);
        return CODE_DIR_NOT_EXIST;
    }
    
    char file_name[FILE_PATH_LEN];
    
    for (; ;) {
    	entry = readdir(dirptr);
    	if (NULL == entry) {
    		break;
		}
		
		// ., .. not and to vector 
		if (entry->d_namlen > 3) {
			sprintf(file_name, "%s%s", file_path, entry->d_name);
   			files.push_back(file_name);
		}
	}

    closedir(dirptr);
    return CODE_SUCC;
}


void RemoveDirFiles(const char *file_path){
	std::vector<std::string> files(10);
	DirFiles(file_path, files);
	
	for (int i=0; i < files.size(); i++) {
		remove(files[i].c_str());
	}
}




/*****************************************************************************
								CSpiltBigFile
*****************************************************************************/
CSpiltBigFile::CSpiltBigFile(const char *small_file_path, unsigned int small_file_num, char split_c) {
	if (NULL == small_file_path || small_file_num == 0) {
		printf("CSpiltBigFile::CSpiltBigFile input err, small_file_path: %s, small_file_num: %d\n", small_file_path,small_file_num);
		return;
	}
	
	m_split_c = split_c;
	m_small_file_num = small_file_num;
	m_file_path_len = strlen(small_file_path);
	
	m_small_file_path = (char *)malloc(m_file_path_len+64); // 64 for small file name
	if (NULL == m_small_file_path) {
		printf("CSpiltBigFile::CSpiltBigFile m_small_file_path = malloc(%d) fail\n", m_file_path_len+64);
		return; 
	}
	strcpy(m_small_file_path, small_file_path);
	
	m_small_files = new CWriteFile[small_file_num];
	if (NULL == m_small_files) {
		free(m_small_file_path);
		m_small_file_path = NULL;
		printf("CSpiltBigFile::CSpiltBigFile new CWriteFile[%d] fail\n", small_file_num);
		return;
	}
	
	int result = 0;
	for (unsigned int i = 0; i < small_file_num; i++) {
		sprintf(&m_small_file_path[m_file_path_len], "%s_%04d", SMALL_FILE_PRE, i);
		
		result = m_small_files[i].init(m_small_file_path, READ_FILE_BUF_SIZE);
		if (CODE_SUCC != result) {
			printf("CSpiltBigFile::CSpiltBigFile CWriteFile::init(%s): %d fail\n", m_small_file_path, result);
		}
	}
	
	return;
}

CSpiltBigFile::~CSpiltBigFile(){
	if (NULL != m_small_file_path){
		free(m_small_file_path);
		m_small_file_path = NULL;
	}

	if (NULL != m_small_files) {
		delete[] m_small_files;
		m_small_files = NULL;
	}
}

int CSpiltBigFile::Split(const char *big_file_name) {
	if (NULL == big_file_name) {
		printf("CSpiltBigFile::Split big_file_name is null\n");
		return CODE_INPUT_ERR;
	}
	
	// read big file-> hash -> write to small file
	CReadFile big_file(big_file_name, READ_FILE_BUF_SIZE, m_split_c);

	// the url's length must less than URL_MAX_SIZE, or url will be splited.
	char url[URL_MAX_SIZE];
	unsigned int line_len = 0;
	unsigned int hash_val = 0;
	
	while (true) {
		line_len = big_file.ReadLine(url, URL_MAX_SIZE);	
		if (0 == line_len) {
			break;
		}
		
		hash_val = BkdrHash(url);	
		m_small_files[hash_val%m_small_file_num].Write(url, line_len);
	}
	
	// flush the CWriteFile buf
	for (int i=0; i < m_small_file_num; i++) {
		m_small_files[i].Flush();
	}
	
	return CODE_SUCC;
}


/*****************************************************************************
								CReadFile
*****************************************************************************/
long long CReadFile::FileSize() {
	int code = init();
	if (CODE_SUCC != code) {
		printf("WARNING: CReadFile::init fail, ret:%d\n", code);
		return 0;
	}
	
	assert(NULL != m_hfile);
	
	fpos_t pos = 0;
    fseeko64(m_hfile, 0, SEEK_END);
    fgetpos(m_hfile, &pos);
    fseeko64(m_hfile, 0, SEEK_SET);
    
    //printf("debug: CReadFile::FileSize file:%s, size: %d\n",m_file_name, pos);    
    return pos;
}


CReadFile::CReadFile(const char *file_name, unsigned int read_buf_size, char split_c) {
	m_hfile = NULL;
	m_split_c = split_c;
	m_read_buf_size = read_buf_size;
	m_inited = false;
	
	if (NULL == file_name) {
		return ;
	}
	
	int len = strlen(file_name);
	m_file_name = (char *)malloc(len+1);
	if (NULL == m_file_name) {
		printf("CReadFile::CReadFile m_file_name=malloc(%d) fail\n", len+1);
		return;
	}
	
	strcpy(m_file_name, file_name);
	
	m_buf = (char *)malloc(m_read_buf_size+1);
	if (NULL == m_buf) {
		clear();
		printf("CReadFile::CReadFile m_readbuf=malloc(%d) fail\n", m_read_buf_size);
		return;
	}
	m_buf[m_read_buf_size] = '\0';
	
	m_data = new CDataBuf(m_buf, 0, READ_FILE_BUF_SIZE);
	if (NULL == m_data) {
		clear();
		printf("CReadFile::CReadFile m_data = new CDataBuf fail\n");
		return;
	}
	
	return;
}

CReadFile::~CReadFile(){
	m_inited = false;
	clear();
}

// read 8k data from file, and return online, 
unsigned int CReadFile::ReadLine(char *buf, unsigned int len){
	assert(NULL != buf && len > 0);
	
	int code = init();
	if (CODE_SUCC != code) {
		printf("WARNING: CReadFile::ReadLine init fail, ret:%d\n", code);
		return 0;
	}
	
	unsigned int read_len = 0;
	unsigned int read_file_len = 0; 
	struct BufResult result ;

	do {
		result = m_data->GetDataWithSplitC(&buf[read_len], len-read_len, m_split_c);
		switch (result.code) {
			case BUF_SUCC:
				if (result.len+read_len < len) {
					buf[result.len+read_len] = '\0';
				}else{
					buf[len-1] = '\0';
				}
				
				//printf("cread file : %s, len: %d\n", buf, result.len+read_len);
				return result.len+read_len;
			
			case BUF_PART: // empty and part need read file, and update databuf
				read_len += result.len;
				
			case BUF_EMPTY:
				m_buf[0] = '\0';
				read_file_len = fread(m_buf, 1, m_read_buf_size, m_hfile);
				if (read_file_len == 0) {
					printf("INFO CReadFile::ReadLine file: %s, read finished\n", m_file_name);
					return read_len;
				}
				
				m_data->Reset(read_file_len);
				break;
			
			default:
				printf("CReadFile::ReadLine m_data.GetDataWithSplitC return.code(%d)\n", result.code);
				return 0;
		}
	}while (read_len < len);
}

int CReadFile::init(){
	if (m_inited) {
		return CODE_SUCC;
	}
	
	if (NULL == m_file_name) {
		printf("CReadFile::init m_file_name is null\n");
		return CODE_FAIL;
	}
		
	m_hfile = fopen(m_file_name, "r");
	if (NULL == m_hfile) {
		printf("CReadFile::init fopen(%s) fail\n", m_file_name);
		return CODE_OPEN_FILE_FAIL;
	}
	
	m_inited = true;
	return CODE_SUCC;	
}

void CReadFile::clear() {
	if (NULL != m_file_name) {
		free(m_file_name);
		m_file_name = NULL;
	}
	
	if (NULL != m_hfile) {
		fclose(m_hfile);
		m_hfile = NULL;
	}
	
	if (NULL != m_buf) {
		free(m_buf);
		m_buf = NULL;
	}
	
	if (NULL != m_data) {
		delete m_data;
		m_data = NULL;
	}
}


/*****************************************************************************
								CWriteFile
*****************************************************************************/
unsigned int  CWriteFile::Write(const char *buf, unsigned int len){
	assert(NULL != buf && len > 0);
		
	if (!m_inited){
		return 0;
	}
	
	unsigned int write_len = 0;
	struct BufResult result;
	
	//printf("write file len: %d, data: %s\n", len, buf);
	do {
		result = m_data->Append(&buf[write_len], len-write_len);
		//printf("write file append, write_len: %d, code: %d, len: %d\n", write_len, result.code, result.len);
		switch (result.code) {
			case BUF_SUCC:
				return write_len + result.len;
			
			case BUF_PART: // full and part need flush memory to file
				write_len += result.len;
				
			case BUF_FULL:
				Flush();
				m_data->Reset(0);
				break;
			
			default:
				printf("CWriteFile::Write m_data.Append().code(%d) unsupported\n", result.code);
				return 0;
		}
	} while (write_len < len);
}

int CWriteFile::Flush() {
	if (!m_inited) {
		return 0;
	}
	
	unsigned int data_len = m_data->DataLen();
	if (data_len > 0) {
		fwrite(m_buf, sizeof(char), data_len, m_hfile);
	}
	
	//printf("CWriteFile::Flush file: %s, data_len: %d\n", m_file_name, data_len);
	return data_len;	
}


CWriteFile::CWriteFile(){
	m_hfile = NULL;
	m_file_name = NULL;
	m_buf = NULL;
	m_write_buf_size = 0;
	m_data = NULL;
	m_inited = false;
}

CWriteFile::CWriteFile(const char *file_name,  unsigned int write_buf_size) {
	assert(NULL != file_name);
	m_inited = false;
	
	init(file_name, write_buf_size);
	return;
}

CWriteFile::~CWriteFile(){
	clear();
	m_inited = false;
}


int CWriteFile::init(const char *file_name, unsigned int write_buf) {
	if (m_inited) {
		clear();
	}
	
	if (NULL == file_name) {
		printf("CWriteFile::init file_name is zero\n");
		return CODE_INPUT_ERR;
	}
	
	int len = strlen(file_name);
	m_file_name = (char *)malloc(len+1);
	if (NULL == m_file_name) {
		printf("CWriteFile::CWriteFile m_file_name=malloc(%d) fail\n", len+1);
		return CODE_MALLOC_FAIL;
	}
	strcpy(m_file_name, file_name);
	
	m_write_buf_size = write_buf;
	m_buf = (char *)malloc(m_write_buf_size+1);
	if (NULL == m_buf) {
		clear();
		printf("CWriteFile::CWriteFile m_readbuf=malloc(%d) fail\n", m_write_buf_size);
		return CODE_MALLOC_FAIL;
	}
	m_buf[m_write_buf_size] = '\0';
	
	m_data = new CDataBuf(m_buf, 0, m_write_buf_size);
	if (NULL == m_data) {
		clear();
		printf("CWriteFile::CWriteFile m_data = new CDataBuf fail\n");
		return CODE_MALLOC_FAIL;
	}
		
	m_hfile = fopen(m_file_name, "w+");
	if (NULL == m_hfile) {
		printf("CWriteFile::init fopen(%s) fail\n", m_file_name);
		clear();	
		return CODE_OPEN_FILE_FAIL;
	}
	
	m_inited = true;
	return CODE_SUCC;
}

void CWriteFile::clear() {
	if (NULL != m_file_name) {
		free(m_file_name);
		m_file_name = NULL;
	}
	
	if (NULL != m_hfile) {
		fclose(m_hfile);
		m_hfile = NULL;
	}
	
	if (NULL != m_buf) {
		free(m_buf);
		m_buf = NULL;
	}
	
	if (NULL != m_data) {
		delete m_data;
		m_data = NULL;
	}
}

