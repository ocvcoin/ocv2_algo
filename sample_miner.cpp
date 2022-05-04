

#include <iostream>

#include <chrono>

#include "ocv2/functions.cpp"





using namespace std;
using namespace std::chrono;

int main(int argc, char** argv)
{
		
	






//test! important!!!	
if(ocv2_test_algo() != true)
		cerr << endl << "Error!! ocv2_test_algo() failed." << endl;
	
/*
It is necessary to check whether opencv is compiled correctly and verify that the cpu is able to correctly calculate our algorithm.
Some processors use different float point arithmetic and our algo doesn't work properly on them.
Also opencv compilation flags ( like avx sse eg..) may break this algorithm.
So we will test.
*/





	
	
		
		
	//mine this block header	
	char block_header[] = "\x00\x00\x00\x20\x61\x0b\xdd\xe6\xe3\x98\x80\x7a\xa5\x03\x33\xdc\xda\x8c\x17\xcc\x6d\xe6\x54\x41\x99\x09\x76\xda\xae\x11\x96\xe4\x02\x00\x00\x00\x4a\x04\xdb\xbc\x44\x6f\x9a\x7b\xc5\x70\xbe\x4e\x8e\xc3\x31\xc7\x54\xba\x99\xdd\xa9\x1b\xcb\xdf\xf9\xf4\xf1\x34\x1c\xf6\x96\xe6\x6b\xf7\x6b\x62\x51\xa9\x09\x1d\x00\x00\x00\x00";	
		
	
		
	

 


	

	
	
	
	auto start = high_resolution_clock::now();
	
	
	//nonce start value
	uint32_t nonce = 0;	
	
	
	char hash[32]; //output hash
	
	ocv2_init_image(block_header); //this should definitely be outside the loop. otherwise you will waste your cpu power!
	
	
	//start mining	
	while(true)	{	
		
	ocv2_calculate_hash(block_header,&nonce,hash);
	


/*
	
	//You can test that everything works correctly with the standalone function by removing the comment.	
	block_header[76]=*(( char*)&nonce+0);
	block_header[77]=*(( char*)&nonce+1);
	block_header[78]=*(( char*)&nonce+2);
	block_header[79]=*(( char*)&nonce+3);
	
	char tmp_hash_for_test[32];
	ocv2_hash(block_header,tmp_hash_for_test); //this is a standalone alternative function. It is not designed for mining and is slow.
	
	char tmp_reversed_hash_for_test[32]; //We will reverse the hash. because the function we optimized for mining gives the output reverse hash.
	
	
	
	for(int i=(32 - 1);i>=0;i--)
		tmp_reversed_hash_for_test[(32 - 1) - i] = tmp_hash_for_test[i];	
	
	
	
	if(memcmp ( tmp_reversed_hash_for_test ,hash, 32 ) != 0)	{
		
			cerr << endl << "wtf???" << endl;
			break;
		
		}
		
*/

	
	

	


	//this part is to show how many hashes per second.
	if(nonce % 1000 == 0 && nonce > 0){
		
		auto stop = high_resolution_clock::now();		
		
		auto duration = duration_cast<microseconds>(stop - start); 

		cout << (1000*1000000 / duration.count()) << " h/sn"  << endl;
		
		start = high_resolution_clock::now();
		
		
	}
	
	
	/*
	
		hash[0] == '\x00' and hash[1] == '\x00' ...
		here to check leading zeros bla bla bla...
	
	*/
	
	
	
	
		nonce++;
		
		
	}		
		
    return 0;
}
