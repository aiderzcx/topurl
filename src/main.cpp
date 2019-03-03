
#include <stdio.h>
#include <string.h>
#include "top_url_count.h"
#include "define.h"
#include "file_mgr.h"
#include "hash_map.h"



// exec:  topurl big_file_path small_file_path
// file_path end with  / , exp: /var/log/  h:/doc/  
//

extern const char *SMALL_FILE_PRE;

int main(int argc, char *argv[]){
	if (argc < 3) {
		printf("need input big file path and small file path!\n");
		printf("exp: topurl /var/big/file/path/ /var/small/file/paht/ \n");
		return -1;
	}
	
	printf("INFO: big file path: %s\n", argv[1]);
	printf("INFO: small file path: %s\n", argv[2]);
	
	// 1. read big file from the big file path, 
	// 2. calc the total file size, and calc the small file number
	// 3. split big file to small file one by one
	// 4. count small file's url and count top url, one by one
	// 5. show the top ur
	
	
	std::vector<std::string> big_files;
	int ret = DirFiles(argv[1], big_files);
	if (CODE_SUCC != ret) {
		printf("warning: DirFiles(%s) fail(%d)\n", argv[1], ret);
		return 0;
	}
	
	// calc big_file's total size, and small_file_num
	long long total_size = 0;
	long long file_size = 0;
	for (int i=0; i < big_files.size(); i++) {
		CReadFile big_file(big_files[i].c_str(), 8, SPLIT_C);	
		file_size = big_file.FileSize();
		total_size += file_size; 
		
		printf("INFO: big_file_name: %s, file_size: %d\n", big_files[i].c_str(), file_size);
	}
	
	unsigned int smal_file_num = (total_size+SPLIT_FILE_SIZE-1)/SPLIT_FILE_SIZE;
	printf("INFO: big file total size: %d, small_file_num: %d\n", total_size, smal_file_num);
	
	// split big file one by one
	CSpiltBigFile split_file(argv[2], smal_file_num, SPLIT_C);
	
	int result = 0;
	for (int i=0; i < big_files.size(); i++) {
		result = split_file.Split(big_files[i].c_str());
		if (CODE_SUCC != result) {
			printf("WARNING:  split_file(%s) fail, ret: %d\n", big_files[i].c_str(), result);
			// not return, do all one can
		}
	}
	
	// count small files one by one
	CTopUrlCount top_url(TOP_URL_NUM);
	CUrlInfo url_info;
	
	std::vector<std::string> small_files;
	DirFiles(argv[2], small_files);
	
	unsigned int small_path_len = strlen(argv[2]);
	for (int i=0; i < small_files.size(); i++) {
		const char *pfile_name = small_files[i].c_str();
		printf("debug: start count small_file: %s\n", pfile_name);
		
		// only process strart with  SMALL_FILE_PRE file
		if (0 == strncmp(SMALL_FILE_PRE, &pfile_name[small_path_len], strlen(SMALL_FILE_PRE))) {
			CHashMap hash_map(FILE_COUNT_HASH_SIZE);
			FileCount(small_files[i].c_str(), hash_map);
			
			// top url count
			for (;;){
				ret = hash_map.GetNext(&url_info);
				if (CODE_SUCC != ret) {
					break;
				}
				
				top_url.Count(url_info.GetUrl(), url_info.GetCount());
			}
		}
	}
	
	printf("===show the result====\n"); 
	top_url.Show();
	top_url.SaveToFile("top_url_count.txt");
	
	return 0;
}


