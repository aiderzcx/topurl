#include "test_inc.h"
#include <stdio.h>
#include <string.h>
#include "define.h"
#include "url_info.h"

int CUrlInfoTest() {
	CUrlInfo url1;
	CUrlInfo url2;
	CUrlInfo url_tmp;
	
	const char *str1 = "aaaaa";
	const char *str2 = "bbbbbbb";
	
	// test update, the url and count must right
	url1.Update(str1, 5, 1);
	if (0 != strcmp(str1,url1.GetUrl()) || 1 != url1.GetCount()) {
		printf("CUrlInfoTest 1 geturl: %s, getcount: %d error\n", url1.GetUrl(), url1.GetCount());
		return CODE_FAIL;
	}

	url1.Update(str2, 7, 2);
	if (0 != strcmp(str2,url1.GetUrl()) || 2 != url1.GetCount()) {
		printf("CUrlInfoTest 2 geturl: %s, getcount: %d error\n", url1.GetUrl(), url1.GetCount());
		return CODE_FAIL;
	}
	
	url1.Update(str1, 5, 1);
	if (0 != strcmp(str1,url1.GetUrl()) || 1 != url1.GetCount()) {
		printf("CUrlInfoTest 3 geturl: %s, getcount: %d error\n", url1.GetUrl(), url1.GetCount());
		return CODE_FAIL;
	}
	
	url2.Update(str2, 7, 2);
	
	// test opertor = , url1 and url2 can swap.
	url_tmp = url1;
	url1 = url2;
	url2 = url_tmp;
	
	if (0 != strcmp(str2,url1.GetUrl()) || 2 != url1.GetCount()) {
		printf("CUrlInfoTest 4 geturl: %s, getcount: %d error\n", url1.GetUrl(), url1.GetCount());
		return CODE_FAIL;
	}
	
	if (0 != strcmp(str1,url2.GetUrl()) || 2 != url1.GetCount()) {
		printf("CUrlInfoTest 5 geturl: %s, getcount: %d error\n", url1.GetUrl(), url1.GetCount());
		return CODE_FAIL;
	}
	
	return CODE_SUCC;
} 
