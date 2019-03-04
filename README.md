# topurl
    big file save url， count top 100 url 

## use
    cd src:
    	make clean
		make	
    	./topurl ../test/bigfile/ ../test/smallfiles/
    
    testcase:
    	cd test
		make clean
		make
		./topurl_test	

## execution flow
	1. get big-file path and small-file path from the args
	2. list the big files
	3. calc total size of the big files
	4. calc small file number
	5. split big-file one by one
		read big-file's url
		hash url
		write url to small file with hash_value % small_file_num
	6. list the small files
	7. count small-file one by one
		count small-file url with hash map
		count top url with min heap
	8 show the result

## class
	CSpiltBigFile : create small files, and split big file
	CReadFile: read file context, 
	CWriteFile: write context to small file
	CHashMap: used for count small-file's url
	CTopUrlCount: used for count top url
	CUrlInfo: one url info
	CDataBuf: share buf for read and write
	
		

