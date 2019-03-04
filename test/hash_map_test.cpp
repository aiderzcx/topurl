#include "test_inc.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include <string>
#include "define.h"
#include "url_info.h"
#include "hash_map.h"

using namespace std;

const char *g_hash_map_test_data[] = {
	"aaaaaaaaaa","aaaaaaaaaa","aaaaaaaaaa","aaaaaaaaaa","aaaaaaaaaa", // 5
	"bbbbbbbbbb","bbbbbbbbbb","bbbbbbbbbb","bbbbbbbbbb","bbbbbbbbbb",
	"bbbbbbbbbb","bbbbbbbbbb","bbbbbbbbbb","bbbbbbbbbb","bbbbbbbbbb", // 10
	"cccccccccc","cccccccccc","cccccccccc","cccccccccc","cccccccccc",
	"cccccccccc","cccccccccc","cccccccccc","cccccccccc","cccccccccc",
	"cccccccccc","cccccccccc","cccccccccc","cccccccccc","cccccccccc", // 15
	"dddddddddd","dddddddddd","dddddddddd","dddddddddd","dddddddddd",
	"dddddddddd","dddddddddd","dddddddddd","dddddddddd","dddddddddd",
	"dddddddddd","dddddddddd","dddddddddd","dddddddddd","dddddddddd",
	"dddddddddd","dddddddddd","dddddddddd","dddddddddd","dddddddddd", // 20
	"eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee",
	"eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee",
	"eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee",
	"eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee",
	"eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee","eeeeeeeeee", // 25
	"ffffffffff","ffffffffff","ffffffffff","ffffffffff","ffffffffff",
	"ffffffffff","ffffffffff","ffffffffff","ffffffffff","ffffffffff", // 10
	"gggggggggg","gggggggggg","gggggggggg","gggggggggg","gggggggggg",
	"gggggggggg","gggggggggg","gggggggggg","gggggggggg","gggggggggg",
	"gggggggggg","gggggggggg","gggggggggg","gggggggggg","gggggggggg", // 15
	"hhhhhhhhhh","hhhhhhhhhh","hhhhhhhhhh","hhhhhhhhhh", 			  // 4
	"iiiiiiiiii","iiiiiiiiii","iiiiiiiiii","iiiiiiiiii","iiiiiiiiii",
	"iiiiiiiiii","iiiiiiiiii","iiiiiiiiii",                           // 8
	"jjjjjjjjjj","jjjjjjjjjj","jjjjjjjjjj","jjjjjjjjjj","jjjjjjjjjj",
	"jjjjjjjjjj","jjjjjjjjjj","jjjjjjjjjj","jjjjjjjjjj","jjjjjjjjjj", // 10
	"kkkkkkkkkk","kkkkkkkkkk","kkkkkkkkkk","kkkkkkkkkk","kkkkkkkkkk",
	"kkkkkkkkkk","kkkkkkkkkk",                                        // 7
	"llllllllll","llllllllll","llllllllll",                           // 3
	"mmmmmmmmmm","mmmmmmmmmm","mmmmmmmmmm","mmmmmmmmmm","mmmmmmmmmm",
	"mmmmmmmmmm","mmmmmmmmmm","mmmmmmmmmm","mmmmmmmmmm",              // 9
	"nnnnnnnnnn","nnnnnnnnnn","nnnnnnnnnn","nnnnnnnnnn","nnnnnnnnnn",
	"nnnnnnnnnn","nnnnnnnnnn","nnnnnnnnnn","nnnnnnnnnn","nnnnnnnnnn",
	"nnnnnnnnnn","nnnnnnnnnn",                                         // 12
};

map<string, int> g_hash_map_result;

void initHashMapResult() {
	g_hash_map_result.insert(pair<string, int>("aaaaaaaaaa", 5));
	g_hash_map_result.insert(pair<string, int>("bbbbbbbbbb", 10));
	g_hash_map_result.insert(pair<string, int>("cccccccccc", 15));
	g_hash_map_result.insert(pair<string, int>("dddddddddd", 20));
	g_hash_map_result.insert(pair<string, int>("eeeeeeeeee", 25));
	g_hash_map_result.insert(pair<string, int>("ffffffffff", 10));
	g_hash_map_result.insert(pair<string, int>("gggggggggg", 15));
	g_hash_map_result.insert(pair<string, int>("hhhhhhhhhh", 4));
	g_hash_map_result.insert(pair<string, int>("iiiiiiiiii", 8));
	g_hash_map_result.insert(pair<string, int>("jjjjjjjjjj", 10));
	g_hash_map_result.insert(pair<string, int>("kkkkkkkkkk", 7));
	g_hash_map_result.insert(pair<string, int>("llllllllll", 3));
	g_hash_map_result.insert(pair<string, int>("mmmmmmmmmm", 9));
	g_hash_map_result.insert(pair<string, int>("nnnnnnnnnn", 12));
}

int CHashMapTest() {
	initHashMapResult();
	int data_num = sizeof(g_hash_map_test_data) / sizeof(g_hash_map_test_data[0]);
	
	CHashMap hash_map(5);
	
	// count test data
	for (int i=0; i < data_num; i++) {
		hash_map.Count(g_hash_map_test_data[i], strlen(g_hash_map_test_data[i]),  BkdrHash(g_hash_map_test_data[i]));
	}
	//hash_map.DebugShow();
	
	
	// check count
	CUrlInfo info;
	int ret = 0;
	while(true) {
		ret = hash_map.GetNext(&info);
		if (CODE_SUCC != ret) {
			break;
		}
		
		//printf("CHashMapTest count_result{url: %s, count: %d} \n", info.GetUrl(), info.GetCount());
		if (g_hash_map_result[info.GetUrl()] != info.GetCount()) {
			printf("CHashMapTest key: %s, count: %d not equal\n", info.GetUrl(), info.GetCount());
			break;
		}
		
		g_hash_map_result.erase(info.GetUrl());
	}
	
	// check over, the g_hash_map_result must empty, or same key not counted. 
	if (g_hash_map_result.size() != 0) {
		printf("CHashMapTest the g_hash_map_result.size(%d) not zero\n", g_hash_map_result.size());
		
		map<string, int>::iterator iter;
		for (iter = g_hash_map_result.begin(); iter != g_hash_map_result.end(); iter++) {
			printf("CHashMapTest g_hash_map_result key: %s, v: %d\n", iter->first.c_str(), iter->second);
		}
		
		return CODE_FAIL;
	}

	return CODE_SUCC;
}
