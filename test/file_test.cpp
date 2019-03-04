#include "test_inc.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include "define.h"
#include "file_mgr.h"

using namespace std;

// Split big file test

int CSplitBigFileTest() {
	const char *big_file_name = "h:/github.com/myprj/bigfile/top_url_count.txt";
	const char *small_file_path = "h:/github.com/myprj/smallfiles/";
	unsigned int small_file_num = 4;
	
	CSpiltBigFile split_file(small_file_path, small_file_num, '\n');
	split_file.Split(big_file_name);
	
	vector<string> small_files;
	DirFiles(small_file_path, small_files);
	
	if (small_files.size() != small_file_num) {
		printf("CSplitBigFileTest small_files.size(%d) != %d\n", small_files.size(), small_file_num);
		return CODE_FAIL;
	}
	
	char small_file_name[128];
	sprintf(small_file_name, "%s", small_file_path);
	int path_len = strlen(small_file_path);
	for (unsigned int i = 0; i < small_file_num; i++) {
		sprintf(&small_file_name[path_len],"small_file_%04d", i);
		
		bool bfind = false;
		for (unsigned int v = 0; v < small_file_num; v++) {
			if (0 == strcmp(small_file_name, small_files[v].c_str())) {
				bfind = true;
				break;
			}
		}
		
		if (!bfind) {
			printf("CSplitBigFileTest can't find small_file: %s\n", small_file_name);
			
			for (unsigned int v = 0; v < small_file_num; v++) {
				printf("CSplitBigFileTest small_name: %s\n", small_files[v].c_str());
			}
			
			return CODE_FAIL;
		}
	}
		
	return CODE_SUCC;
}


// write date to file then read it
int CWriteAndReadFileTest() {
	const char *file_name = "read_test_date.txt";
	const char *file_data[] = {
		"h111111111111110\n",
		"h111111111111111\n",
		"h111111111111112\n",
		"h111111111111113\n",
		"h111111111111114\n",
		"h111111111111115\n",
		"h111111111111116\n",
		"h111111111111117\n",
		"h111111111111118\n",
		"h111111111111119\n",
		"h111111111111120\n",
		"h111111111111121\n",
		"h111111111111122\n",
	};
	
	int data_num = sizeof(file_data) / sizeof(file_data[0]);

	CWriteFile *wfile = new CWriteFile(file_name, 100);
	for (int i=0; i < data_num; i++) {
		wfile->Write(file_data[i], strlen(file_data[i]));
	}
	wfile->Flush();
	delete wfile;
	
	CReadFile read_file(file_name, 100, SPLIT_C);
	unsigned long file_size = read_file.FileSize();
	if (file_size < 100) {
		printf("WARNING: CReadFile.FileSize err, return: %d\n", file_size);
		return CODE_FAIL;
	}
	
	char read_buf[MAX_URL_LEN];
	int ret = 0;
	
	for (int i=0; i < data_num; i++) {
		ret = read_file.ReadLine(read_buf, MAX_URL_LEN);
		if (ret == 0) {
			printf("WARNING: CReadFile.ReadLine not date, line_num: %d\n", i);
			return CODE_FAIL;
		}
		
		//printf("debug: readline %d: %s\n", i, read_buf);	
		if (0 != strcmp(file_data[i], read_buf)){
			printf("WARNING: CReadFile.Readline(%s) not equal file data, line_num: %d\n", read_buf, i);
			return CODE_FAIL;
		}
	}
	
	ret = read_file.ReadLine(read_buf, MAX_URL_LEN);
	if (0 != ret) {
		printf("WARNING: CReadFile.ReadLine get more data(%s)\n", read_buf);
		return CODE_FAIL;
	}
	
	return CODE_SUCC;
}

