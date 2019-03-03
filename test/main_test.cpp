#include <stdio.h>
#include "test_inc.h" 
#include "define.h"

typedef int (*TestFunc)();

struct TestCase {
	const char *name;
	TestFunc fp;
};



TestCase g_test_func[] = {
	{ "read_and_write_file", CWriteAndReadFileTest },
	{ "split_big_file", CSplitBigFileTest },
	{ "hash_map", CHashMapTest },
	{ "top_url_count", CTopUrlCountTest }, 
	{ "url_info", CUrlInfoTest }, 
};

int main() {

	int test_func_num = sizeof(g_test_func) / sizeof(g_test_func[0]);
	
	int code = 0;
	for (int i=0; i < test_func_num; i++) {
		code = g_test_func[i].fp();
		if (CODE_SUCC == code) {
			printf("succ ====%5d test case < %s > test succ..\n", i, g_test_func[i].name);
		}else {
			printf("fail ====%5d test case %s fail, ret code: %d\n", i, g_test_func[i].name, code);
		}
	}
}
