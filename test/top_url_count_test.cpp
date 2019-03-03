#include "test_inc.h"
#include <map>
#include <string>
#include "top_url_count.h"
#include "define.h"

using namespace std;

map<string, int> g_top_url_test_data;
map<string, int> g_top_url_result;

void initTopUrlTestData() {
	// init test data
	g_top_url_test_data.insert(pair<string, int>("aaaaaaaaaa", 5));
	g_top_url_test_data.insert(pair<string, int>("bbbbbbbbbb", 10));
	g_top_url_test_data.insert(pair<string, int>("cccccccccc", 15));
	g_top_url_test_data.insert(pair<string, int>("dddddddddd", 20));
	g_top_url_test_data.insert(pair<string, int>("eeeeeeeeee", 25));
	g_top_url_test_data.insert(pair<string, int>("ffffffffff", 10));
	g_top_url_test_data.insert(pair<string, int>("gggggggggg", 15));
	g_top_url_test_data.insert(pair<string, int>("hhhhhhhhhh", 4));
	g_top_url_test_data.insert(pair<string, int>("iiiiiiiiii", 8));
	g_top_url_test_data.insert(pair<string, int>("jjjjjjjjjj", 10));
	g_top_url_test_data.insert(pair<string, int>("kkkkkkkkkk", 7));
	g_top_url_test_data.insert(pair<string, int>("llllllllll", 3));
	g_top_url_test_data.insert(pair<string, int>("mmmmmmmmmm", 9));
	g_top_url_test_data.insert(pair<string, int>("nnnnnnnnnn", 12));
	
	// init the top url
	g_top_url_result.insert(pair<string, int>("eeeeeeeeee", 25));
	g_top_url_result.insert(pair<string, int>("dddddddddd", 20));
	g_top_url_result.insert(pair<string, int>("cccccccccc", 15));
	g_top_url_result.insert(pair<string, int>("gggggggggg", 15));
	g_top_url_result.insert(pair<string, int>("nnnnnnnnnn", 12));
}

int CTopUrlCountTest() {
	initTopUrlTestData();
	
	 printf("CTopUrlCountTest data init succ\n");
	 
	CTopUrlCount top_url(5);
	printf("CTopUrlCountTest top_url create\n");
	map<string, int>::iterator iter;
	for (iter = g_top_url_test_data.begin(); iter != g_top_url_test_data.end(); iter++) {
		top_url.Count(iter->first.c_str(), iter->second);
	}
	
	printf("CTopUrlCountTest count over..\n");
	
	int ret = 0;
	CUrlInfo url_info;
	for(int i = 0; i < 10; i++) {
		ret = top_url.GetNode(i, url_info);
		if (CODE_SUCC != ret) {
			break;
		}
		
		printf("CTopUrlCountTest count[%s]=%d\n", url_info.GetUrl(), url_info.GetCount());
		if (g_top_url_result[url_info.GetUrl()] != url_info.GetCount()){
			printf("CTopUrlCountTest count[%s] = %d error\n", url_info.GetUrl(), url_info.GetCount());
		}else {
			g_top_url_result.erase(url_info.GetUrl());
		}		
	}
	
	if (g_top_url_result.size() != 0) {
		printf("CTopUrlCountTest check over g_top_url_result.size != 0\n");
		
		for (iter = g_top_url_result.begin(); iter != g_top_url_result.end(); iter++) {
			printf("CTopUrlCountTest top_url_result leave key: %s\n", iter->first.c_str());
		}
		
		return CODE_FAIL;
	}

	return CODE_SUCC;
}
