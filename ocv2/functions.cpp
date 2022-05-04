#include <crypto/sha512.cpp>
#include <crypto/sha256.cpp>

#include <opencv2/opencv.hpp>


#include "test_data.hpp"

using namespace cv;
using namespace std;

void ocv2_hash(char* block_header,char* output) {	
	
	
	
	unsigned int block_time;
	
	block_time = int(
	
			(unsigned char)(block_header[71]) << 24 |
            (unsigned char)(block_header[70]) << 16 |
            (unsigned char)(block_header[69]) << 8 |
            (unsigned char)(block_header[68])
			
			);


	Mat kernel = Mat::zeros(3, 3, CV_32F);
	
	kernel.at<float>(0,1)--;
	kernel.at<float>(1,0)--;
	kernel.at<float>(1,1) += 5;
	kernel.at<float>(1,2)--;	
	kernel.at<float>(2,1)--;
	
	uint8_t hash[CSHA256::OUTPUT_SIZE];	
	uint8_t hash512[CSHA512::OUTPUT_SIZE];

	

/*
            WE ARE REPLACING THE UNSTABLE OLD FUNCTION!!!
            block timestamp >= 1636416000
            Unix Timestamp 	1636416000
            GMT 	Tue Nov 09 2021 00:00:00 GMT+0000
*/

	if(block_time >= 1636416000){
		
		
		
		
		
	char init_image_bytes[1782] = "\x42\x4D\xF6\x06\x00\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x18\x00\x00\x00\x18\x00\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00\xC0\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

	
	
	
	int i;
	i = 0;
    while (i < 27){
		
		if(i==0)
			CSHA512().Write((const unsigned char*)&block_header[0], 76).Finalize(hash512);
		else
			CSHA512().Write((const unsigned char*)hash512, CSHA512::OUTPUT_SIZE).Finalize(hash512);		
		
        
		std::memcpy(&init_image_bytes[(54 + ( i * CSHA512::OUTPUT_SIZE ))], hash512, CSHA512::OUTPUT_SIZE);			
		
		
        i += 1;	
	}
	
	

		
	
	char nonce_bytes[4];
	nonce_bytes[0] = block_header[76];
	nonce_bytes[1] = block_header[77];
	nonce_bytes[2] = block_header[78];
	nonce_bytes[3] = block_header[79];

	int j;
	
    i = 54;
    j = 0;
    while (i < 1782){        
        init_image_bytes[i] = init_image_bytes[i] ^ nonce_bytes[j];
        j += 1;
        i += 1;        
        if (j == 4)
            j = 0;	
	
	
	}
	
	
	cv::Mat converted_buf(1, 1782, CV_8U, (void*)init_image_bytes);
	
	Mat initial_image;	
	initial_image = imdecode ( converted_buf, IMREAD_COLOR );	
	




	Mat bilateralFilter_output;	
	bilateralFilter ( initial_image, bilateralFilter_output, 15,75,75 );



	


	Mat filter2D_output;	
	filter2D(bilateralFilter_output, filter2D_output, -1 , kernel );
	

	
	
	Mat blur_output;
	blur( filter2D_output, blur_output, Size( 5, 5 ) );
	
	
	Mat GaussianBlur_output;
	GaussianBlur( blur_output, GaussianBlur_output, Size( 5, 5 ),BORDER_DEFAULT );	


	Mat final_image;
	medianBlur( GaussianBlur_output, final_image, 5 );	


	
	
	
	std::vector<uchar>output_buff;
	imencode(".bmp", final_image, output_buff);
	
	for(i=0;i<80;i++)
		output_buff.push_back (block_header[i]);
	
	
	
	CSHA256().Write((const unsigned char*)&output_buff[0], 1782+80).Finalize(hash);



	uint8_t reversed_hash[CSHA256::OUTPUT_SIZE];
	
	
	
	for(i=(CSHA256::OUTPUT_SIZE - 1);i>=0;i--)
		reversed_hash[(CSHA256::OUTPUT_SIZE - 1) - i] = hash[i];	
	

	

	std::memcpy(output, reversed_hash, CSHA256::OUTPUT_SIZE);
	
	
	}else {
		
		
		
		char init_image_bytes[] = "\x42\x4d\x36\x0c\x00\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x20\x00\x00\x00\x20\x00\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x37\xec\x6a\xf6\x1a\xd3\x9f\xa5\xc4\x0b\x0c\x36\x65\xff\x6e\x2c\x2e\x54\xd0\x7d\xa8\xae\xa1\x1a\xbd\x61\x75\x0a\x6f\x02\xfd\x4e\x3b\x2f\x6d\xf5\x28\x8c\x62\x44\x5c\x01\x70\x69\xce\xc2\xb8\x7b\x19\xcb\x31\xba\x1e\x85\xbc\x91\xfd\xab\xf9\x46\x73\x55\x2b\x53\x09\xfd\x79\x7f\x00\xd0\x21\x20\x31\x9a\xff\x4f\x8b\x93\x45\x27\xe1\xd0\x92\x29\x41\x7c\x1b\xd9\xe0\xe4\x0c\xc4\x75\xb5\x45\xdc\x22\x4d\x38\xef\xf3\x24\x6c\xa3\x5a\x8f\x82\xa6\x2e\x1a\x44\xe3\x76\xa4\xd3\x9d\xd3\x95\x11\x36\x7e\x9f\xb4\x09\x08\x1a\xe8\x43\x8a\x50\xbf\x5f\xa7\x48\xb0\x88\xed\xcb\x4e\xb6\x3d\x24\xf0\x07\xc0\xb7\x75\x84\x87\x8c\xe5\x9d\x82\x06\x3d\x78\x07\xa7\x65\x37\x62\x98\xb0\xb2\x6a\x24\xcf\x43\x75\x3f\xd4\xaa\xf4\x48\xf9\xe9\x71\x16\x81\xd2\x4a\xe9\x39\x90\xbe\x63\x3b\xb7\x23\x5d\x82\x5d\x1d\x44\x6a\xd0\x3d\xbd\x05\xb0\x37\x63\x9d\x42\x4a\xcf\x1c\xf2\x17\x31\xec\x21\xc8\x44\xcb\x1a\x6b\xd4\x9f\xa9\xfc\x16\x26\xce\x48\xd5\xbd\xe4\xaa\xef\x82\xf4\xea\x3b\xd1\x22\xa5\xa2\xc9\x95\x51\x3f\x24\xea\xc0\xfb\x13\x68\x77\x36\x16\x88\x96\xe0\x21\xe9\x85\x14\x96\x2c\x8c\x86\xa2\x12\xea\xea\xde\xa0\x97\x24\x32\xe5\xf8\x98\xd1\x9e\x1d\x1e\xe2\xff\x1d\xee\x52\x2d\x46\x04\x6b\x69\x56\x09\xe9\xcd\xb8\xa2\x43\x88\x09\xa3\x38\xc0\xbc\x41\x19\x52\x04\x3b\xe9\x7d\xe4\x9a\x55\xe7\x66\x51\xbb\x4e\x5e\xbc\x3f\x67\xfe\xa2\xb9\xda\xaf\x46\xa9\xc7\xdd\x9b\xc6\xa2\x14\xc8\xe7\x3a\x47\x99\x5a\x28\x4d\x58\x09\x30\xb3\x0d\xe7\x19\xa8\x33\x44\xef\x60\x1a\x3c\xb5\x27\x54\x56\xda\x3d\xec\x58\xfb\x68\x4e\xb4\x10\xde\x32\x66\x1a\x55\x65\x2b\xa9\xd7\x76\xa9\xf9\x9f\xd4\x7e\x85\xc9\xdb\x5d\xe6\x4f\xa9\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xf9\xba\xf0\x67\x0f\xc2\x40\x80\x47\xff\xff\xff\x44\xe8\x44\xc3\xff\x23\x0e\x90\x68\xee\xa5\x66\x4f\x36\x0d\xe1\x8e\xe9\x6c\xd0\xfc\x10\x50\xca\xdf\x30\xa3\x5f\x35\xbf\x16\xbc\xc0\xf1\x9d\x6b\xc9\xf7\x39\x30\x52\x39\xd3\xbe\xd3\x79\x59\x47\x99\xa3\x8d\x01\xb3\x68\x87\x2d\x98\x6c\x09\x64\x93\xac\xe1\x8a\x1a\xc1\x06\x8b\xcd\xb1\x8f\x64\x6a\x6f\x4b\x6d\x50\x54\x54\x49\xc1\x16\x08\x67\x42\x9f\x9b\x31\x0c\xef\x58\xb2\x88\x3a\x86\x4e\x24\x66\x42\x4e\x6f\x10\x04\x2a\xb8\xf3\x3f\xef\xd0\xe7\x84\x80\x1e\x02\x95\x0c\x6e\xd0\x69\x59\xbd\x36\xc5\x2c\x1a\xc2\x31\xbe\xad\xa7\xb1\x8b\x51\xfc\x82\x7e\x77\x44\x6a\x88\x7f\xe0\x05\x5e\x59\x2f\x3a\x95\x63\x80\xd0\x3a\x51\x5b\xad\x7a\xab\x65\x92\xcb\xcc\x8f\xca\x0e\x94\x6c\x4b\xa9\xae\x59\x40\xe3\x45\x7c\x1c\xeb\x9e\x2a\xe5\x85\x3d\xcf\xd7\x0a\x15\x2e\x97\xc9\xac\x18\xda\x3d\x34\x9d\xc8\x37\xde\xbf\x64\xc1\x2c\xba\xf0\x96\xd0\x0f\x87\xb7\xa7\x24\xe8\x60\xae\xa5\xb7\x44\xd3\x35\x99\x2a\xb6\x22\x3e\xd6\x2d\x05\xcc\xc2\xdd\x5d\x18\xc8\x45\xa2\x01\x83\x59\x3c\xf0\xcb\xbb\xf2\xf9\x19\x79\xb5\xd4\x06\xf6\x9f\x13\xfa\x2f\x35\x17\x2b\x51\x3a\xb6\x25\xa3\x06\x22\x01\xff\x2b\xde\x14\xcd\xca\x32\x16\xbc\xa4\x36\x71\xdf\x2f\xe0\x75\x10\x24\x1f\x03\x3a\x66\x78\xd1\x16\x81\xb5\xe2\xe1\x5d\x0e\x30\x05\x49\xd9\xdb\xcc\xde\xd7\x83\xe1\xb1\x48\xaf\x7d\x4a\x11\xbc\xb5\x9f\x71\x26\x8d\x47\x9d\x2c\xe7\x1b\x05\x0f\x5e\x3d\x17\x0f\x78\x2e\xd6\x03\x8e\x47\x62\x7e\x10\x56\xf2\xad\x95\x42\x6a\x9a\xab\xae\xc8\x71\x7f\xc0\xde\x94\xbf\x17\x0a\x27\xb3\x2b\x85\x11\x15\x47\x6b\xd5\xf8\x67\x38\xdf\xd6\xff\xff\xff\x6f\xd2\x0c\x1d\x3d\x2c\xde\xf2\xe7\xff\xff\xff\x44\xc2\x9d\xd3\x42\xe2\x3f\x27\x3e\xd8\x39\xbb\xa9\xcc\x86\x64\x7b\xc2\x36\x90\xcb\x43\x38\x2e\x2b\x7d\xd6\xc4\xab\x0d\x65\xdf\x16\x25\x53\x86\x67\x30\x2e\x9f\xe9\x4a\x17\xf8\xaf\x49\xc2\x3e\xb2\xa4\x6d\xf2\x01\xfb\x28\x3c\xf8\xf2\x3a\x22\xd9\x42\x85\xbc\x0b\xd6\x93\x04\x99\xd1\x6f\x98\x3a\x57\x96\x2e\x65\x3b\xc1\x25\x9d\x95\xa2\x9a\x9e\x89\x84\xa8\xf6\x32\x19\xa2\x80\xd0\x88\x47\xbb\xdb\x2c\x52\x4d\x75\x6b\x20\x65\x48\x37\x53\x9a\xe8\xfd\x03\xe0\x3c\x22\xac\x8c\xfe\x29\xa0\xd9\xda\x6d\xee\xbe\x45\x32\x42\xf6\x91\x0f\xdc\x54\x5e\xdd\x83\x93\x67\xdd\x2d\x44\xb2\x91\x3e\xdb\xcd\xeb\xd4\x5a\xb2\x81\xa7\x80\x53\xdd\xcf\x44\x46\x29\x18\x3c\x62\x5a\x30\x7b\x3f\xfa\x59\x15\x4d\xbc\x44\x46\x41\xa7\x28\xa5\xed\xc2\xa1\x55\xa6\xe9\x39\xc5\x2a\x79\x8f\xba\xa5\x21\x55\x6c\x53\x1b\x2e\x5d\x9b\x21\x2a\x96\xab\xa0\xf6\x64\x45\xfb\xcc\x46\xb6\x9a\xdc\x6e\xe3\x04\x46\x11\x80\xde\xde\xc3\x6a\xc8\xf1\xc0\x3b\x6a\x1f\xf9\x2b\xbb\xf6\x54\x04\xac\x59\x45\x1b\x32\x80\xdf\xbc\x32\x53\x00\xfb\x3a\x7a\x0c\xd2\xbc\xd1\x88\xe7\x72\x49\x62\x50\xa2\x67\x4d\xaa\xa9\x7b\xe7\xbc\xc6\xea\x70\xe8\x43\xa2\x7f\xdf\x22\x98\xce\x82\x7d\x09\x55\x58\x1e\x7c\xa7\x39\xee\xa3\xa8\x7d\x19\xec\x84\x7f\xbf\x84\xf1\x85\x83\x13\xb2\x6f\x70\x8d\xda\x25\xbf\x3b\xe5\xf7\x89\x03\x07\xcc\x06\x98\x0e\xeb\x3b\x1c\x84\xfe\x4d\x7b\xbb\x5d\x8e\x11\x1e\x05\x9c\x0a\xc0\x21\xd2\x6c\x58\x11\x0a\xb2\x44\xd7\x5c\xff\x06\x3c\x14\x7d\xa1\x9f\xee\x9a\x45\x04\xac\x9b\x40\x82\xb6\x1f\x57\xe3\x80\x64\xe2\x8f\x81\x8e\xee\xff\xff\xff\x2e\xe9\x2b\xe9\x0c\x65\x47\xf8\x6a\xff\xff\xff\x43\x64\x27\xfb\xf7\xe9\x8e\x7e\x9d\x8f\xed\x2c\xef\xb3\x0a\x83\x6c\x07\x17\x64\x7c\x7a\xd0\x93\xa8\x74\xb6\x13\x89\x3e\x74\xe3\xdc\xc8\xdb\x15\xf5\xf2\x7c\xdf\xc8\x36\xc8\x6f\x6a\xba\x65\x21\x0d\xad\xae\x62\xda\xc4\x67\x79\x8c\x54\x9f\x66\x8c\x84\xc3\xc7\x29\x0e\x13\x53\xc5\x89\x94\x25\x12\x6c\xac\x3c\x48\x90\x49\xb0\x43\xec\x42\xdd\x0b\x7b\xa6\x23\xdb\x00\x4e\xfc\x4e\x62\x85\x7e\x7a\xa7\x86\x25\x24\x3d\x8a\x2d\x4a\x01\x50\xeb\xb5\x84\x8d\x2f\x64\xe4\xe8\xcd\x66\x00\xa3\x67\x94\x9e\xcf\x27\x97\x4d\x83\x3f\x9b\xba\x3b\x84\x86\x45\xfe\x12\x0e\x14\x25\x46\x6e\x7a\x3c\x5a\x5e\x53\x2a\xd3\x4a\x9c\xf5\x67\xef\x7f\xdc\x54\x41\x30\x08\xf5\x66\xf1\x03\xcc\x75\xfc\x47\x2d\x1f\xfc\x42\xf9\x4f\x27\xd1\x1d\x0e\xb5\x99\x0f\x82\x08\xc6\xe9\x35\x9c\xce\xca\xc1\x65\xe6\xdb\x28\xdf\xcc\xbf\x49\x57\xa3\x10\x1f\x23\xa6\xce\xd5\x00\x51\xbe\xf1\x94\x69\xbc\xe7\xef\x34\xc7\x0e\xb2\x9b\x51\xec\x00\xf0\xb4\xa3\x7b\xdc\xac\x07\x9e\x7a\xc3\xa9\x0a\xd3\xfc\x51\xac\xb4\x03\x93\x72\x46\xdd\xd9\x02\x36\x4a\xbd\x8b\x79\x0a\xdc\x01\x4d\xa1\x83\x68\x57\x31\x21\x46\x10\x39\xe3\x39\x42\x5b\x77\xcd\x24\x69\x9c\x03\x22\xba\xf2\xeb\xb9\x1a\x04\x74\x4b\x64\x0b\x71\x8f\xe9\x96\x14\x11\x6e\xd7\xc0\x5b\x30\xb1\xc9\x78\xd1\x85\xd7\x51\xd3\xce\x54\x53\xab\xd2\x6d\xf6\xd2\x12\xed\xf0\x0c\x5a\x9a\xa1\x62\x3e\x75\xd6\x78\xbd\x5d\xbc\xa8\xa4\x21\x1c\xae\x6e\xdf\x41\x4e\x4e\xae\x24\x6b\xef\x9f\x64\xbc\x45\xf8\x92\xf0\xe0\x09\x7f\x41\x70\x59\x1f\x47\x8e\x00\xba\xcb\x4a\x98\xf2\xe9\x8f\xe2\x16\x64\x09\xff\xff\xff\x1d\xce\x98\x56\x70\x00\xf3\x13\xdf\xff\xff\xff\x9a\xf0\x3c\x39\x3c\xf0\x7b\x8f\xfc\xb4\xfd\x8a\xe4\x2a\x0b\x81\x72\xb2\xd6\xcf\xdb\x94\x6f\x45\xd9\xa2\xaa\xfb\xf5\x44\xab\x81\xfa\xd2\x28\xd9\x9e\x41\xa3\xec\x1c\x4c\xaf\xdc\x4f\x44\x25\x7a\xae\x59\x1c\x7a\xab\x5e\xb3\xb7\x38\xb5\xd7\xf1\x93\xad\xa0\x21\x2a\x98\x69\x74\xb6\x21\x9d\x52\x69\xbf\x0d\xfa\x7e\x0f\x02\x68\x95\xfe\xb5\xdf\x63\x84\x80\x24\xd9\x59\x23\xb9\xc7\x04\x1e\x12\xe0\xda\xc1\x83\x5f\x62\x77\x9c\xb0\x54\x99\x87\x54\x89\x69\xd2\x48\x82\x54\x8b\x5f\x8f\x1c\xd9\xa5\x5e\x08\x9a\x03\xa3\x6d\x96\xb3\x9a\x2a\x96\xa0\xd5\x59\xbd\xa7\xa5\xe8\x17\xac\xcb\x30\x51\xd8\xcf\xf3\x3a\x57\x59\x8a\x7e\x8d\xc2\x22\x2f\x0d\xff\x24\x32\xae\xf2\xbe\xa8\x23\x92\xb3\x3d\x81\x95\x93\x98\xdc\xd1\x85\xb6\xea\xbf\x91\x27\xb2\xf0\x62\x0e\x26\xe0\xf7\x6b\x4d\xcb\xac\xa2\xd0\x8e\x23\x41\x1d\xd8\x2f\xb1\x9b\x03\x76\x3f\x88\xac\x56\xa2\x91\x8d\xfb\x2d\x53\xa1\x5e\x8a\xb8\x12\x77\x43\xc9\x4f\xb7\xbd\x2a\x10\xee\x58\xdd\xc4\xe4\x1e\xa3\x09\x8f\x20\x14\xb0\x40\xd3\x44\x52\x07\x4d\x69\xb6\x20\x8e\xcb\x60\x51\xfd\x92\x5b\x39\x18\xec\x50\xc0\x40\x2e\x19\x7b\xd7\x05\x06\x7f\x90\x34\x92\xdd\x0c\x98\xd0\x77\xca\x3b\x47\x1d\x64\x9c\x97\x2c\x40\x2b\x02\x99\x28\x9c\x46\x78\xe6\x24\xa7\x00\x0a\x07\x43\xde\x21\xac\xe5\xbd\x70\x67\xc3\x39\x6e\xff\xa7\xcf\x64\xdd\xd2\x6e\xb8\xa6\xbd\xe4\xd7\xb8\x24\x48\x89\xb9\xc4\x42\x66\xd4\xb8\xd4\xa3\xab\xf3\x67\x28\x1e\xc2\x48\x90\x1a\xfe\xa8\xa9\x9f\x1f\x17\xda\x61\x61\x55\x70\x3a\x11\x87\x41\x01\xee\x9a\xe9\x74\x17\x81\x76\x4b\xf0\x17\x8a\x65\xff\xff\xff\xa3\x3d\xf9\x31\x6d\x35\x5f\xc8\x8c\xff\xff\xff\x1e\xa6\x67\xda\x5f\x9a\xdd\x9c\xe1\x34\x32\x96\xc2\xb3\x49\x4a\xa3\x55\x84\x56\xfe\x91\x28\xbf\x55\x24\xb0\x8e\x88\x49\x41\x0c\x27\x43\x96\xe7\xd1\xb2\x15\xa8\x3f\x50\x60\xa5\x7e\x26\x32\xfb\x01\xe5\xb4\xcd\x33\x00\xdf\x5f\xc9\xd1\x32\x0a\x83\xb4\xab\x42\x01\xa5\x37\x43\x61\x9e\x80\x52\x1f\x76\xc2\x51\xcf\x20\x93\xcd\x00\x52\x16\xb1\x11\xad\xac\x15\xda\xab\x6f\xdd\x32\x30\x42\x3c\x2f\x9a\xd3\x8d\x65\x9a\xb5\x11\xda\x6e\x18\x49\x52\x17\x8b\xc0\x9a\x37\xbc\x7d\x0e\xd7\xfc\xb1\xe0\xa9\xd1\xed\xf0\x4c\x8c\xd9\x02\xd0\x94\x63\x5a\x24\xa7\xe0\x4c\x53\x95\xea\xfe\x87\x7e\x4e\x62\xc2\xdd\x72\x75\x7e\xad\xd8\x63\xf8\xe6\x1a\x57\x62\x92\xfb\xb1\x4a\x9e\xaf\x05\x0c\x0e\x9f\x75\x61\xeb\xa4\xd9\xc6\x4b\x05\x54\x7e\x9c\xda\x4e\x09\xbb\x82\x43\x1f\x0c\x0b\x19\x70\xfa\x98\xce\x5d\xd8\x12\xa7\xb4\x1c\x2b\xc1\x6f\xf2\xd2\xdb\x49\xf9\x7d\x97\xd4\xe7\xbc\x1b\x65\x01\xbc\x22\xed\xfe\xf9\x6c\xa2\x61\x50\x99\x54\x98\x4c\xe5\x27\x98\x9e\x75\x91\x6f\x3e\x4f\xf9\x7e\xfd\xe4\x28\x0a\x85\x99\xf9\xe6\xa6\x8b\x36\x11\xca\x74\x32\xba\x0e\x99\x4e\x0e\x53\xcd\xce\xaf\x05\x78\x23\xc4\x3c\xf6\x77\x08\xb8\x3b\xc2\x53\x51\x92\x5d\x8b\xb1\x27\x63\xa7\xc7\xe3\x81\x85\x7c\x29\xb0\xe8\xeb\x43\x43\xfe\xc8\xef\x5a\xdd\x73\xdf\xed\x3b\x5a\x5c\x2e\x00\x35\xbe\xa8\x2c\x2b\x19\x1f\x08\xb1\x94\xb1\xae\xdb\x27\x9a\xe4\x69\x88\xcd\xd9\xb1\xc6\x9e\x9e\xe4\xa3\x58\xe8\xd1\xc8\x74\xd3\xba\x58\x67\x69\x79\xf7\x19\x4a\x49\xb5\xd3\x71\x4f\xe1\x68\x1c\x7c\x8a\xb1\x05\x31\x19\x32\xd1\xc7\x71\xc0\xe2\x12\xe0\xff\xff\xff\x81\x08\x7c\xb7\xb6\xc6\x3a\x38\xc3\xff\xff\xff\xea\x85\x88\x62\x3c\xf0\x6c\x99\xbd\xb1\xe5\x0f\xc2\x4c\x7b\x16\x5a\xc5\xc3\x0f\xa5\xcb\x3d\x02\xff\xb1\x09\x38\x00\xf8\xab\x2f\xbd\x0d\x75\xf7\x9a\xeb\x20\x60\xe0\xbe\xe2\xe2\x5c\x15\xfc\x30\x81\x27\xd9\x99\x91\x47\xe3\x3c\x1a\x06\xcd\xa7\xc9\x06\x38\xe6\x5e\xcb\xaa\x71\xdb\x93\xaf\x23\xe1\x8a\xfe\x77\xc8\xd8\x2f\xfa\x6f\xfb\x32\x7c\x83\x15\x0d\xe9\x5b\xea\xc7\xfc\x6e\x23\x9e\x51\xc5\xa8\x4a\x94\x5e\xf4\xbb\x41\x92\xda\x08\x77\x75\x93\xa2\x25\x58\x3c\xa9\xb4\x1b\xff\xe1\x56\xf8\xe2\x2c\x5b\x62\xf3\xd3\xc2\x76\xcd\x80\x30\xdc\x60\xd0\xf7\xc5\x58\x41\x90\xe0\xb4\xc0\x67\x47\xdc\x70\xff\xff\xff\xff\xff\xff\xff\xff\xff\xbc\x43\x6e\xad\x6f\xc5\x8b\x1f\x53\x47\x2d\x4b\x32\x04\x93\xb2\x20\x78\xd7\xf9\x0c\x5b\x24\x69\x91\x73\xcd\x24\x99\x0d\x1a\x7d\x63\xd7\x06\x9b\x0e\x1c\xf3\xdd\x84\x5c\x66\xd6\x3f\x8f\x32\xf7\x50\x42\x37\x1b\x3f\x8d\xd1\xc4\x56\x9d\x97\xbc\xe7\x46\xb5\x9b\xc3\x05\x0a\xc4\xbd\xdd\xca\xfe\xcf\xbf\x67\x14\xbb\xb0\x1d\xbc\x33\xd4\x6b\x9c\xf1\xac\xac\x60\x48\x02\xf1\xd1\xf9\xd8\xdb\x97\xae\x02\x41\x2d\x1c\xc3\x6d\x1b\xce\xf1\x33\x40\xbe\x0d\x0c\x55\x94\x8b\x8a\x83\xae\xbd\x12\x00\x13\xe0\xe0\xb5\x60\xaf\x5f\x3d\xc2\x14\x21\xbe\xc9\x99\x68\xbf\x5d\xdb\x0d\x2f\x5c\x03\x7d\xfd\x66\xff\x1f\x80\xc8\xa8\x53\x05\x95\x9d\x5d\x88\x9e\x10\xe5\x58\x39\x19\xbf\x12\x49\xe3\x75\x0d\x9b\x92\xc6\xf9\xe5\x6f\x84\x65\x5a\x44\xe7\x32\x05\xe5\xd0\xa6\xd4\xa9\x48\xdf\xbc\x32\x3a\x7a\xb1\x99\x61\x33\x16\xa3\xdd\x94\x26\x56\x5c\x35\xc8\x5a\x18\x03\x75\xe8\xc9\x55\x81\xff\xff\xff\x9a\x16\x99";
		
		
		
		
		int i=54;
		int j;
		
		
		
		
    j = 0;
    while (i < 3126 and j < 80){
        init_image_bytes[i] = block_header[j];
        i += 1;
        j += 1;

	}
    j = 0;
    while (i < 3126){
        if (init_image_bytes[i] != '\xff'){
            init_image_bytes[i] = init_image_bytes[i] ^ block_header[j];
            j += 1;
		}
        i += 1;
        
        if (j == 80)
            j = 0;		
		
	}


	
		
		
	cv::Mat converted_buf(1, 3126, CV_8U, (void*)init_image_bytes);
	
	Mat initial_image;	
	initial_image = imdecode ( converted_buf, IMREAD_COLOR );			
		
		
	int algo_selector = ((unsigned char)block_header[5] % 6);


	Mat final_image;	
	
	if(algo_selector == 0){		
		
		
		bilateralFilter ( initial_image, final_image, 15,75,75 );		
		
	}else if(algo_selector == 1){
		
		fastNlMeansDenoisingColored( initial_image,final_image );	
		
	}
	else if(algo_selector == 2){
		
		filter2D(initial_image, final_image, -1 , kernel );	
		
	}
	else if(algo_selector == 3){
		
		blur( initial_image, final_image, Size( 5, 5 ) );	
		
	}
	else if(algo_selector == 4){
		
		GaussianBlur( initial_image, final_image, Size( 5, 5 ),BORDER_DEFAULT );	
		
	}
	else if(algo_selector == 5){
		
		medianBlur( initial_image, final_image, 5 );	
		
	}
	
	


	std::vector<uchar>output_buff;
	imencode(".bmp", final_image, output_buff);
	
	for(i=0;i<80;i++)
		output_buff.push_back (block_header[i]);
	
	
	
	CSHA256().Write((const unsigned char*)&output_buff[0], 3126+80).Finalize(hash);
	
	
	


	std::memcpy(output, hash, CSHA256::OUTPUT_SIZE);
		
		
	}	

	
	
	
    
	
	
	
    
}













			
		
		
	char NEW_IMAGE_REFERANCE_BYTES[1782] = "\x42\x4D\xF6\x06\x00\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x18\x00\x00\x00\x18\x00\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00\xC0\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

	char final_init_img[1782] = "\x42\x4D\xF6\x06\x00\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x18\x00\x00\x00\x18\x00\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00\xC0\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	
	char last_nonce_bytes[4];
	Mat kernel;


	void ocv2_init_image(char* block_header){
	
		last_nonce_bytes[0] = '\xff';
		last_nonce_bytes[1] = '\xff';
		last_nonce_bytes[2] = '\xff';
		last_nonce_bytes[3] = '\xff';
		
		
		//Let's prepare a reference image:
		uint8_t hash512[CSHA512::OUTPUT_SIZE];
		int i;
		i = 0;
		while (i < 27){
		
			if(i==0)
				CSHA512().Write((const unsigned char*)&block_header[0], 76).Finalize(hash512);
			else
				CSHA512().Write((const unsigned char*)hash512, CSHA512::OUTPUT_SIZE).Finalize(hash512);		
		
        
			std::memcpy(&NEW_IMAGE_REFERANCE_BYTES[(54 + ( i * CSHA512::OUTPUT_SIZE ))], hash512, CSHA512::OUTPUT_SIZE);			
		
		
			i += 1;	
		}	

	kernel = Mat::zeros(3, 3, CV_32F);
	
	kernel.at<float>(0,1)--;
	kernel.at<float>(1,0)--;
	kernel.at<float>(1,1) += 5;
	kernel.at<float>(1,2)--;	
	kernel.at<float>(2,1)--;		
	
	
	}






	
	
		

void ocv2_calculate_hash(char* block_header,void *nonce_bytes,char* output){
	
	
	
	
        
        
        if (last_nonce_bytes[0] != *(( char*)nonce_bytes+0)){
            last_nonce_bytes[0] = *(( char*)nonce_bytes+0);
            final_init_img[54] = NEW_IMAGE_REFERANCE_BYTES[54] ^ *(( char*)nonce_bytes+0);
            final_init_img[58] = NEW_IMAGE_REFERANCE_BYTES[58] ^ *(( char*)nonce_bytes+0);
            final_init_img[62] = NEW_IMAGE_REFERANCE_BYTES[62] ^ *(( char*)nonce_bytes+0);
            final_init_img[66] = NEW_IMAGE_REFERANCE_BYTES[66] ^ *(( char*)nonce_bytes+0);
            final_init_img[70] = NEW_IMAGE_REFERANCE_BYTES[70] ^ *(( char*)nonce_bytes+0);
            final_init_img[74] = NEW_IMAGE_REFERANCE_BYTES[74] ^ *(( char*)nonce_bytes+0);
            final_init_img[78] = NEW_IMAGE_REFERANCE_BYTES[78] ^ *(( char*)nonce_bytes+0);
            final_init_img[82] = NEW_IMAGE_REFERANCE_BYTES[82] ^ *(( char*)nonce_bytes+0);
            final_init_img[86] = NEW_IMAGE_REFERANCE_BYTES[86] ^ *(( char*)nonce_bytes+0);
            final_init_img[90] = NEW_IMAGE_REFERANCE_BYTES[90] ^ *(( char*)nonce_bytes+0);
            final_init_img[94] = NEW_IMAGE_REFERANCE_BYTES[94] ^ *(( char*)nonce_bytes+0);
            final_init_img[98] = NEW_IMAGE_REFERANCE_BYTES[98] ^ *(( char*)nonce_bytes+0);
            final_init_img[102] = NEW_IMAGE_REFERANCE_BYTES[102] ^ *(( char*)nonce_bytes+0);
            final_init_img[106] = NEW_IMAGE_REFERANCE_BYTES[106] ^ *(( char*)nonce_bytes+0);
            final_init_img[110] = NEW_IMAGE_REFERANCE_BYTES[110] ^ *(( char*)nonce_bytes+0);
            final_init_img[114] = NEW_IMAGE_REFERANCE_BYTES[114] ^ *(( char*)nonce_bytes+0);
            final_init_img[118] = NEW_IMAGE_REFERANCE_BYTES[118] ^ *(( char*)nonce_bytes+0);
            final_init_img[122] = NEW_IMAGE_REFERANCE_BYTES[122] ^ *(( char*)nonce_bytes+0);
            final_init_img[126] = NEW_IMAGE_REFERANCE_BYTES[126] ^ *(( char*)nonce_bytes+0);
            final_init_img[130] = NEW_IMAGE_REFERANCE_BYTES[130] ^ *(( char*)nonce_bytes+0);
            final_init_img[134] = NEW_IMAGE_REFERANCE_BYTES[134] ^ *(( char*)nonce_bytes+0);
            final_init_img[138] = NEW_IMAGE_REFERANCE_BYTES[138] ^ *(( char*)nonce_bytes+0);
            final_init_img[142] = NEW_IMAGE_REFERANCE_BYTES[142] ^ *(( char*)nonce_bytes+0);
            final_init_img[146] = NEW_IMAGE_REFERANCE_BYTES[146] ^ *(( char*)nonce_bytes+0);
            final_init_img[150] = NEW_IMAGE_REFERANCE_BYTES[150] ^ *(( char*)nonce_bytes+0);
            final_init_img[154] = NEW_IMAGE_REFERANCE_BYTES[154] ^ *(( char*)nonce_bytes+0);
            final_init_img[158] = NEW_IMAGE_REFERANCE_BYTES[158] ^ *(( char*)nonce_bytes+0);
            final_init_img[162] = NEW_IMAGE_REFERANCE_BYTES[162] ^ *(( char*)nonce_bytes+0);
            final_init_img[166] = NEW_IMAGE_REFERANCE_BYTES[166] ^ *(( char*)nonce_bytes+0);
            final_init_img[170] = NEW_IMAGE_REFERANCE_BYTES[170] ^ *(( char*)nonce_bytes+0);
            final_init_img[174] = NEW_IMAGE_REFERANCE_BYTES[174] ^ *(( char*)nonce_bytes+0);
            final_init_img[178] = NEW_IMAGE_REFERANCE_BYTES[178] ^ *(( char*)nonce_bytes+0);
            final_init_img[182] = NEW_IMAGE_REFERANCE_BYTES[182] ^ *(( char*)nonce_bytes+0);
            final_init_img[186] = NEW_IMAGE_REFERANCE_BYTES[186] ^ *(( char*)nonce_bytes+0);
            final_init_img[190] = NEW_IMAGE_REFERANCE_BYTES[190] ^ *(( char*)nonce_bytes+0);
            final_init_img[194] = NEW_IMAGE_REFERANCE_BYTES[194] ^ *(( char*)nonce_bytes+0);
            final_init_img[198] = NEW_IMAGE_REFERANCE_BYTES[198] ^ *(( char*)nonce_bytes+0);
            final_init_img[202] = NEW_IMAGE_REFERANCE_BYTES[202] ^ *(( char*)nonce_bytes+0);
            final_init_img[206] = NEW_IMAGE_REFERANCE_BYTES[206] ^ *(( char*)nonce_bytes+0);
            final_init_img[210] = NEW_IMAGE_REFERANCE_BYTES[210] ^ *(( char*)nonce_bytes+0);
            final_init_img[214] = NEW_IMAGE_REFERANCE_BYTES[214] ^ *(( char*)nonce_bytes+0);
            final_init_img[218] = NEW_IMAGE_REFERANCE_BYTES[218] ^ *(( char*)nonce_bytes+0);
            final_init_img[222] = NEW_IMAGE_REFERANCE_BYTES[222] ^ *(( char*)nonce_bytes+0);
            final_init_img[226] = NEW_IMAGE_REFERANCE_BYTES[226] ^ *(( char*)nonce_bytes+0);
            final_init_img[230] = NEW_IMAGE_REFERANCE_BYTES[230] ^ *(( char*)nonce_bytes+0);
            final_init_img[234] = NEW_IMAGE_REFERANCE_BYTES[234] ^ *(( char*)nonce_bytes+0);
            final_init_img[238] = NEW_IMAGE_REFERANCE_BYTES[238] ^ *(( char*)nonce_bytes+0);
            final_init_img[242] = NEW_IMAGE_REFERANCE_BYTES[242] ^ *(( char*)nonce_bytes+0);
            final_init_img[246] = NEW_IMAGE_REFERANCE_BYTES[246] ^ *(( char*)nonce_bytes+0);
            final_init_img[250] = NEW_IMAGE_REFERANCE_BYTES[250] ^ *(( char*)nonce_bytes+0);
            final_init_img[254] = NEW_IMAGE_REFERANCE_BYTES[254] ^ *(( char*)nonce_bytes+0);
            final_init_img[258] = NEW_IMAGE_REFERANCE_BYTES[258] ^ *(( char*)nonce_bytes+0);
            final_init_img[262] = NEW_IMAGE_REFERANCE_BYTES[262] ^ *(( char*)nonce_bytes+0);
            final_init_img[266] = NEW_IMAGE_REFERANCE_BYTES[266] ^ *(( char*)nonce_bytes+0);
            final_init_img[270] = NEW_IMAGE_REFERANCE_BYTES[270] ^ *(( char*)nonce_bytes+0);
            final_init_img[274] = NEW_IMAGE_REFERANCE_BYTES[274] ^ *(( char*)nonce_bytes+0);
            final_init_img[278] = NEW_IMAGE_REFERANCE_BYTES[278] ^ *(( char*)nonce_bytes+0);
            final_init_img[282] = NEW_IMAGE_REFERANCE_BYTES[282] ^ *(( char*)nonce_bytes+0);
            final_init_img[286] = NEW_IMAGE_REFERANCE_BYTES[286] ^ *(( char*)nonce_bytes+0);
            final_init_img[290] = NEW_IMAGE_REFERANCE_BYTES[290] ^ *(( char*)nonce_bytes+0);
            final_init_img[294] = NEW_IMAGE_REFERANCE_BYTES[294] ^ *(( char*)nonce_bytes+0);
            final_init_img[298] = NEW_IMAGE_REFERANCE_BYTES[298] ^ *(( char*)nonce_bytes+0);
            final_init_img[302] = NEW_IMAGE_REFERANCE_BYTES[302] ^ *(( char*)nonce_bytes+0);
            final_init_img[306] = NEW_IMAGE_REFERANCE_BYTES[306] ^ *(( char*)nonce_bytes+0);
            final_init_img[310] = NEW_IMAGE_REFERANCE_BYTES[310] ^ *(( char*)nonce_bytes+0);
            final_init_img[314] = NEW_IMAGE_REFERANCE_BYTES[314] ^ *(( char*)nonce_bytes+0);
            final_init_img[318] = NEW_IMAGE_REFERANCE_BYTES[318] ^ *(( char*)nonce_bytes+0);
            final_init_img[322] = NEW_IMAGE_REFERANCE_BYTES[322] ^ *(( char*)nonce_bytes+0);
            final_init_img[326] = NEW_IMAGE_REFERANCE_BYTES[326] ^ *(( char*)nonce_bytes+0);
            final_init_img[330] = NEW_IMAGE_REFERANCE_BYTES[330] ^ *(( char*)nonce_bytes+0);
            final_init_img[334] = NEW_IMAGE_REFERANCE_BYTES[334] ^ *(( char*)nonce_bytes+0);
            final_init_img[338] = NEW_IMAGE_REFERANCE_BYTES[338] ^ *(( char*)nonce_bytes+0);
            final_init_img[342] = NEW_IMAGE_REFERANCE_BYTES[342] ^ *(( char*)nonce_bytes+0);
            final_init_img[346] = NEW_IMAGE_REFERANCE_BYTES[346] ^ *(( char*)nonce_bytes+0);
            final_init_img[350] = NEW_IMAGE_REFERANCE_BYTES[350] ^ *(( char*)nonce_bytes+0);
            final_init_img[354] = NEW_IMAGE_REFERANCE_BYTES[354] ^ *(( char*)nonce_bytes+0);
            final_init_img[358] = NEW_IMAGE_REFERANCE_BYTES[358] ^ *(( char*)nonce_bytes+0);
            final_init_img[362] = NEW_IMAGE_REFERANCE_BYTES[362] ^ *(( char*)nonce_bytes+0);
            final_init_img[366] = NEW_IMAGE_REFERANCE_BYTES[366] ^ *(( char*)nonce_bytes+0);
            final_init_img[370] = NEW_IMAGE_REFERANCE_BYTES[370] ^ *(( char*)nonce_bytes+0);
            final_init_img[374] = NEW_IMAGE_REFERANCE_BYTES[374] ^ *(( char*)nonce_bytes+0);
            final_init_img[378] = NEW_IMAGE_REFERANCE_BYTES[378] ^ *(( char*)nonce_bytes+0);
            final_init_img[382] = NEW_IMAGE_REFERANCE_BYTES[382] ^ *(( char*)nonce_bytes+0);
            final_init_img[386] = NEW_IMAGE_REFERANCE_BYTES[386] ^ *(( char*)nonce_bytes+0);
            final_init_img[390] = NEW_IMAGE_REFERANCE_BYTES[390] ^ *(( char*)nonce_bytes+0);
            final_init_img[394] = NEW_IMAGE_REFERANCE_BYTES[394] ^ *(( char*)nonce_bytes+0);
            final_init_img[398] = NEW_IMAGE_REFERANCE_BYTES[398] ^ *(( char*)nonce_bytes+0);
            final_init_img[402] = NEW_IMAGE_REFERANCE_BYTES[402] ^ *(( char*)nonce_bytes+0);
            final_init_img[406] = NEW_IMAGE_REFERANCE_BYTES[406] ^ *(( char*)nonce_bytes+0);
            final_init_img[410] = NEW_IMAGE_REFERANCE_BYTES[410] ^ *(( char*)nonce_bytes+0);
            final_init_img[414] = NEW_IMAGE_REFERANCE_BYTES[414] ^ *(( char*)nonce_bytes+0);
            final_init_img[418] = NEW_IMAGE_REFERANCE_BYTES[418] ^ *(( char*)nonce_bytes+0);
            final_init_img[422] = NEW_IMAGE_REFERANCE_BYTES[422] ^ *(( char*)nonce_bytes+0);
            final_init_img[426] = NEW_IMAGE_REFERANCE_BYTES[426] ^ *(( char*)nonce_bytes+0);
            final_init_img[430] = NEW_IMAGE_REFERANCE_BYTES[430] ^ *(( char*)nonce_bytes+0);
            final_init_img[434] = NEW_IMAGE_REFERANCE_BYTES[434] ^ *(( char*)nonce_bytes+0);
            final_init_img[438] = NEW_IMAGE_REFERANCE_BYTES[438] ^ *(( char*)nonce_bytes+0);
            final_init_img[442] = NEW_IMAGE_REFERANCE_BYTES[442] ^ *(( char*)nonce_bytes+0);
            final_init_img[446] = NEW_IMAGE_REFERANCE_BYTES[446] ^ *(( char*)nonce_bytes+0);
            final_init_img[450] = NEW_IMAGE_REFERANCE_BYTES[450] ^ *(( char*)nonce_bytes+0);
            final_init_img[454] = NEW_IMAGE_REFERANCE_BYTES[454] ^ *(( char*)nonce_bytes+0);
            final_init_img[458] = NEW_IMAGE_REFERANCE_BYTES[458] ^ *(( char*)nonce_bytes+0);
            final_init_img[462] = NEW_IMAGE_REFERANCE_BYTES[462] ^ *(( char*)nonce_bytes+0);
            final_init_img[466] = NEW_IMAGE_REFERANCE_BYTES[466] ^ *(( char*)nonce_bytes+0);
            final_init_img[470] = NEW_IMAGE_REFERANCE_BYTES[470] ^ *(( char*)nonce_bytes+0);
            final_init_img[474] = NEW_IMAGE_REFERANCE_BYTES[474] ^ *(( char*)nonce_bytes+0);
            final_init_img[478] = NEW_IMAGE_REFERANCE_BYTES[478] ^ *(( char*)nonce_bytes+0);
            final_init_img[482] = NEW_IMAGE_REFERANCE_BYTES[482] ^ *(( char*)nonce_bytes+0);
            final_init_img[486] = NEW_IMAGE_REFERANCE_BYTES[486] ^ *(( char*)nonce_bytes+0);
            final_init_img[490] = NEW_IMAGE_REFERANCE_BYTES[490] ^ *(( char*)nonce_bytes+0);
            final_init_img[494] = NEW_IMAGE_REFERANCE_BYTES[494] ^ *(( char*)nonce_bytes+0);
            final_init_img[498] = NEW_IMAGE_REFERANCE_BYTES[498] ^ *(( char*)nonce_bytes+0);
            final_init_img[502] = NEW_IMAGE_REFERANCE_BYTES[502] ^ *(( char*)nonce_bytes+0);
            final_init_img[506] = NEW_IMAGE_REFERANCE_BYTES[506] ^ *(( char*)nonce_bytes+0);
            final_init_img[510] = NEW_IMAGE_REFERANCE_BYTES[510] ^ *(( char*)nonce_bytes+0);
            final_init_img[514] = NEW_IMAGE_REFERANCE_BYTES[514] ^ *(( char*)nonce_bytes+0);
            final_init_img[518] = NEW_IMAGE_REFERANCE_BYTES[518] ^ *(( char*)nonce_bytes+0);
            final_init_img[522] = NEW_IMAGE_REFERANCE_BYTES[522] ^ *(( char*)nonce_bytes+0);
            final_init_img[526] = NEW_IMAGE_REFERANCE_BYTES[526] ^ *(( char*)nonce_bytes+0);
            final_init_img[530] = NEW_IMAGE_REFERANCE_BYTES[530] ^ *(( char*)nonce_bytes+0);
            final_init_img[534] = NEW_IMAGE_REFERANCE_BYTES[534] ^ *(( char*)nonce_bytes+0);
            final_init_img[538] = NEW_IMAGE_REFERANCE_BYTES[538] ^ *(( char*)nonce_bytes+0);
            final_init_img[542] = NEW_IMAGE_REFERANCE_BYTES[542] ^ *(( char*)nonce_bytes+0);
            final_init_img[546] = NEW_IMAGE_REFERANCE_BYTES[546] ^ *(( char*)nonce_bytes+0);
            final_init_img[550] = NEW_IMAGE_REFERANCE_BYTES[550] ^ *(( char*)nonce_bytes+0);
            final_init_img[554] = NEW_IMAGE_REFERANCE_BYTES[554] ^ *(( char*)nonce_bytes+0);
            final_init_img[558] = NEW_IMAGE_REFERANCE_BYTES[558] ^ *(( char*)nonce_bytes+0);
            final_init_img[562] = NEW_IMAGE_REFERANCE_BYTES[562] ^ *(( char*)nonce_bytes+0);
            final_init_img[566] = NEW_IMAGE_REFERANCE_BYTES[566] ^ *(( char*)nonce_bytes+0);
            final_init_img[570] = NEW_IMAGE_REFERANCE_BYTES[570] ^ *(( char*)nonce_bytes+0);
            final_init_img[574] = NEW_IMAGE_REFERANCE_BYTES[574] ^ *(( char*)nonce_bytes+0);
            final_init_img[578] = NEW_IMAGE_REFERANCE_BYTES[578] ^ *(( char*)nonce_bytes+0);
            final_init_img[582] = NEW_IMAGE_REFERANCE_BYTES[582] ^ *(( char*)nonce_bytes+0);
            final_init_img[586] = NEW_IMAGE_REFERANCE_BYTES[586] ^ *(( char*)nonce_bytes+0);
            final_init_img[590] = NEW_IMAGE_REFERANCE_BYTES[590] ^ *(( char*)nonce_bytes+0);
            final_init_img[594] = NEW_IMAGE_REFERANCE_BYTES[594] ^ *(( char*)nonce_bytes+0);
            final_init_img[598] = NEW_IMAGE_REFERANCE_BYTES[598] ^ *(( char*)nonce_bytes+0);
            final_init_img[602] = NEW_IMAGE_REFERANCE_BYTES[602] ^ *(( char*)nonce_bytes+0);
            final_init_img[606] = NEW_IMAGE_REFERANCE_BYTES[606] ^ *(( char*)nonce_bytes+0);
            final_init_img[610] = NEW_IMAGE_REFERANCE_BYTES[610] ^ *(( char*)nonce_bytes+0);
            final_init_img[614] = NEW_IMAGE_REFERANCE_BYTES[614] ^ *(( char*)nonce_bytes+0);
            final_init_img[618] = NEW_IMAGE_REFERANCE_BYTES[618] ^ *(( char*)nonce_bytes+0);
            final_init_img[622] = NEW_IMAGE_REFERANCE_BYTES[622] ^ *(( char*)nonce_bytes+0);
            final_init_img[626] = NEW_IMAGE_REFERANCE_BYTES[626] ^ *(( char*)nonce_bytes+0);
            final_init_img[630] = NEW_IMAGE_REFERANCE_BYTES[630] ^ *(( char*)nonce_bytes+0);
            final_init_img[634] = NEW_IMAGE_REFERANCE_BYTES[634] ^ *(( char*)nonce_bytes+0);
            final_init_img[638] = NEW_IMAGE_REFERANCE_BYTES[638] ^ *(( char*)nonce_bytes+0);
            final_init_img[642] = NEW_IMAGE_REFERANCE_BYTES[642] ^ *(( char*)nonce_bytes+0);
            final_init_img[646] = NEW_IMAGE_REFERANCE_BYTES[646] ^ *(( char*)nonce_bytes+0);
            final_init_img[650] = NEW_IMAGE_REFERANCE_BYTES[650] ^ *(( char*)nonce_bytes+0);
            final_init_img[654] = NEW_IMAGE_REFERANCE_BYTES[654] ^ *(( char*)nonce_bytes+0);
            final_init_img[658] = NEW_IMAGE_REFERANCE_BYTES[658] ^ *(( char*)nonce_bytes+0);
            final_init_img[662] = NEW_IMAGE_REFERANCE_BYTES[662] ^ *(( char*)nonce_bytes+0);
            final_init_img[666] = NEW_IMAGE_REFERANCE_BYTES[666] ^ *(( char*)nonce_bytes+0);
            final_init_img[670] = NEW_IMAGE_REFERANCE_BYTES[670] ^ *(( char*)nonce_bytes+0);
            final_init_img[674] = NEW_IMAGE_REFERANCE_BYTES[674] ^ *(( char*)nonce_bytes+0);
            final_init_img[678] = NEW_IMAGE_REFERANCE_BYTES[678] ^ *(( char*)nonce_bytes+0);
            final_init_img[682] = NEW_IMAGE_REFERANCE_BYTES[682] ^ *(( char*)nonce_bytes+0);
            final_init_img[686] = NEW_IMAGE_REFERANCE_BYTES[686] ^ *(( char*)nonce_bytes+0);
            final_init_img[690] = NEW_IMAGE_REFERANCE_BYTES[690] ^ *(( char*)nonce_bytes+0);
            final_init_img[694] = NEW_IMAGE_REFERANCE_BYTES[694] ^ *(( char*)nonce_bytes+0);
            final_init_img[698] = NEW_IMAGE_REFERANCE_BYTES[698] ^ *(( char*)nonce_bytes+0);
            final_init_img[702] = NEW_IMAGE_REFERANCE_BYTES[702] ^ *(( char*)nonce_bytes+0);
            final_init_img[706] = NEW_IMAGE_REFERANCE_BYTES[706] ^ *(( char*)nonce_bytes+0);
            final_init_img[710] = NEW_IMAGE_REFERANCE_BYTES[710] ^ *(( char*)nonce_bytes+0);
            final_init_img[714] = NEW_IMAGE_REFERANCE_BYTES[714] ^ *(( char*)nonce_bytes+0);
            final_init_img[718] = NEW_IMAGE_REFERANCE_BYTES[718] ^ *(( char*)nonce_bytes+0);
            final_init_img[722] = NEW_IMAGE_REFERANCE_BYTES[722] ^ *(( char*)nonce_bytes+0);
            final_init_img[726] = NEW_IMAGE_REFERANCE_BYTES[726] ^ *(( char*)nonce_bytes+0);
            final_init_img[730] = NEW_IMAGE_REFERANCE_BYTES[730] ^ *(( char*)nonce_bytes+0);
            final_init_img[734] = NEW_IMAGE_REFERANCE_BYTES[734] ^ *(( char*)nonce_bytes+0);
            final_init_img[738] = NEW_IMAGE_REFERANCE_BYTES[738] ^ *(( char*)nonce_bytes+0);
            final_init_img[742] = NEW_IMAGE_REFERANCE_BYTES[742] ^ *(( char*)nonce_bytes+0);
            final_init_img[746] = NEW_IMAGE_REFERANCE_BYTES[746] ^ *(( char*)nonce_bytes+0);
            final_init_img[750] = NEW_IMAGE_REFERANCE_BYTES[750] ^ *(( char*)nonce_bytes+0);
            final_init_img[754] = NEW_IMAGE_REFERANCE_BYTES[754] ^ *(( char*)nonce_bytes+0);
            final_init_img[758] = NEW_IMAGE_REFERANCE_BYTES[758] ^ *(( char*)nonce_bytes+0);
            final_init_img[762] = NEW_IMAGE_REFERANCE_BYTES[762] ^ *(( char*)nonce_bytes+0);
            final_init_img[766] = NEW_IMAGE_REFERANCE_BYTES[766] ^ *(( char*)nonce_bytes+0);
            final_init_img[770] = NEW_IMAGE_REFERANCE_BYTES[770] ^ *(( char*)nonce_bytes+0);
            final_init_img[774] = NEW_IMAGE_REFERANCE_BYTES[774] ^ *(( char*)nonce_bytes+0);
            final_init_img[778] = NEW_IMAGE_REFERANCE_BYTES[778] ^ *(( char*)nonce_bytes+0);
            final_init_img[782] = NEW_IMAGE_REFERANCE_BYTES[782] ^ *(( char*)nonce_bytes+0);
            final_init_img[786] = NEW_IMAGE_REFERANCE_BYTES[786] ^ *(( char*)nonce_bytes+0);
            final_init_img[790] = NEW_IMAGE_REFERANCE_BYTES[790] ^ *(( char*)nonce_bytes+0);
            final_init_img[794] = NEW_IMAGE_REFERANCE_BYTES[794] ^ *(( char*)nonce_bytes+0);
            final_init_img[798] = NEW_IMAGE_REFERANCE_BYTES[798] ^ *(( char*)nonce_bytes+0);
            final_init_img[802] = NEW_IMAGE_REFERANCE_BYTES[802] ^ *(( char*)nonce_bytes+0);
            final_init_img[806] = NEW_IMAGE_REFERANCE_BYTES[806] ^ *(( char*)nonce_bytes+0);
            final_init_img[810] = NEW_IMAGE_REFERANCE_BYTES[810] ^ *(( char*)nonce_bytes+0);
            final_init_img[814] = NEW_IMAGE_REFERANCE_BYTES[814] ^ *(( char*)nonce_bytes+0);
            final_init_img[818] = NEW_IMAGE_REFERANCE_BYTES[818] ^ *(( char*)nonce_bytes+0);
            final_init_img[822] = NEW_IMAGE_REFERANCE_BYTES[822] ^ *(( char*)nonce_bytes+0);
            final_init_img[826] = NEW_IMAGE_REFERANCE_BYTES[826] ^ *(( char*)nonce_bytes+0);
            final_init_img[830] = NEW_IMAGE_REFERANCE_BYTES[830] ^ *(( char*)nonce_bytes+0);
            final_init_img[834] = NEW_IMAGE_REFERANCE_BYTES[834] ^ *(( char*)nonce_bytes+0);
            final_init_img[838] = NEW_IMAGE_REFERANCE_BYTES[838] ^ *(( char*)nonce_bytes+0);
            final_init_img[842] = NEW_IMAGE_REFERANCE_BYTES[842] ^ *(( char*)nonce_bytes+0);
            final_init_img[846] = NEW_IMAGE_REFERANCE_BYTES[846] ^ *(( char*)nonce_bytes+0);
            final_init_img[850] = NEW_IMAGE_REFERANCE_BYTES[850] ^ *(( char*)nonce_bytes+0);
            final_init_img[854] = NEW_IMAGE_REFERANCE_BYTES[854] ^ *(( char*)nonce_bytes+0);
            final_init_img[858] = NEW_IMAGE_REFERANCE_BYTES[858] ^ *(( char*)nonce_bytes+0);
            final_init_img[862] = NEW_IMAGE_REFERANCE_BYTES[862] ^ *(( char*)nonce_bytes+0);
            final_init_img[866] = NEW_IMAGE_REFERANCE_BYTES[866] ^ *(( char*)nonce_bytes+0);
            final_init_img[870] = NEW_IMAGE_REFERANCE_BYTES[870] ^ *(( char*)nonce_bytes+0);
            final_init_img[874] = NEW_IMAGE_REFERANCE_BYTES[874] ^ *(( char*)nonce_bytes+0);
            final_init_img[878] = NEW_IMAGE_REFERANCE_BYTES[878] ^ *(( char*)nonce_bytes+0);
            final_init_img[882] = NEW_IMAGE_REFERANCE_BYTES[882] ^ *(( char*)nonce_bytes+0);
            final_init_img[886] = NEW_IMAGE_REFERANCE_BYTES[886] ^ *(( char*)nonce_bytes+0);
            final_init_img[890] = NEW_IMAGE_REFERANCE_BYTES[890] ^ *(( char*)nonce_bytes+0);
            final_init_img[894] = NEW_IMAGE_REFERANCE_BYTES[894] ^ *(( char*)nonce_bytes+0);
            final_init_img[898] = NEW_IMAGE_REFERANCE_BYTES[898] ^ *(( char*)nonce_bytes+0);
            final_init_img[902] = NEW_IMAGE_REFERANCE_BYTES[902] ^ *(( char*)nonce_bytes+0);
            final_init_img[906] = NEW_IMAGE_REFERANCE_BYTES[906] ^ *(( char*)nonce_bytes+0);
            final_init_img[910] = NEW_IMAGE_REFERANCE_BYTES[910] ^ *(( char*)nonce_bytes+0);
            final_init_img[914] = NEW_IMAGE_REFERANCE_BYTES[914] ^ *(( char*)nonce_bytes+0);
            final_init_img[918] = NEW_IMAGE_REFERANCE_BYTES[918] ^ *(( char*)nonce_bytes+0);
            final_init_img[922] = NEW_IMAGE_REFERANCE_BYTES[922] ^ *(( char*)nonce_bytes+0);
            final_init_img[926] = NEW_IMAGE_REFERANCE_BYTES[926] ^ *(( char*)nonce_bytes+0);
            final_init_img[930] = NEW_IMAGE_REFERANCE_BYTES[930] ^ *(( char*)nonce_bytes+0);
            final_init_img[934] = NEW_IMAGE_REFERANCE_BYTES[934] ^ *(( char*)nonce_bytes+0);
            final_init_img[938] = NEW_IMAGE_REFERANCE_BYTES[938] ^ *(( char*)nonce_bytes+0);
            final_init_img[942] = NEW_IMAGE_REFERANCE_BYTES[942] ^ *(( char*)nonce_bytes+0);
            final_init_img[946] = NEW_IMAGE_REFERANCE_BYTES[946] ^ *(( char*)nonce_bytes+0);
            final_init_img[950] = NEW_IMAGE_REFERANCE_BYTES[950] ^ *(( char*)nonce_bytes+0);
            final_init_img[954] = NEW_IMAGE_REFERANCE_BYTES[954] ^ *(( char*)nonce_bytes+0);
            final_init_img[958] = NEW_IMAGE_REFERANCE_BYTES[958] ^ *(( char*)nonce_bytes+0);
            final_init_img[962] = NEW_IMAGE_REFERANCE_BYTES[962] ^ *(( char*)nonce_bytes+0);
            final_init_img[966] = NEW_IMAGE_REFERANCE_BYTES[966] ^ *(( char*)nonce_bytes+0);
            final_init_img[970] = NEW_IMAGE_REFERANCE_BYTES[970] ^ *(( char*)nonce_bytes+0);
            final_init_img[974] = NEW_IMAGE_REFERANCE_BYTES[974] ^ *(( char*)nonce_bytes+0);
            final_init_img[978] = NEW_IMAGE_REFERANCE_BYTES[978] ^ *(( char*)nonce_bytes+0);
            final_init_img[982] = NEW_IMAGE_REFERANCE_BYTES[982] ^ *(( char*)nonce_bytes+0);
            final_init_img[986] = NEW_IMAGE_REFERANCE_BYTES[986] ^ *(( char*)nonce_bytes+0);
            final_init_img[990] = NEW_IMAGE_REFERANCE_BYTES[990] ^ *(( char*)nonce_bytes+0);
            final_init_img[994] = NEW_IMAGE_REFERANCE_BYTES[994] ^ *(( char*)nonce_bytes+0);
            final_init_img[998] = NEW_IMAGE_REFERANCE_BYTES[998] ^ *(( char*)nonce_bytes+0);
            final_init_img[1002] = NEW_IMAGE_REFERANCE_BYTES[1002] ^ *(( char*)nonce_bytes+0);
            final_init_img[1006] = NEW_IMAGE_REFERANCE_BYTES[1006] ^ *(( char*)nonce_bytes+0);
            final_init_img[1010] = NEW_IMAGE_REFERANCE_BYTES[1010] ^ *(( char*)nonce_bytes+0);
            final_init_img[1014] = NEW_IMAGE_REFERANCE_BYTES[1014] ^ *(( char*)nonce_bytes+0);
            final_init_img[1018] = NEW_IMAGE_REFERANCE_BYTES[1018] ^ *(( char*)nonce_bytes+0);
            final_init_img[1022] = NEW_IMAGE_REFERANCE_BYTES[1022] ^ *(( char*)nonce_bytes+0);
            final_init_img[1026] = NEW_IMAGE_REFERANCE_BYTES[1026] ^ *(( char*)nonce_bytes+0);
            final_init_img[1030] = NEW_IMAGE_REFERANCE_BYTES[1030] ^ *(( char*)nonce_bytes+0);
            final_init_img[1034] = NEW_IMAGE_REFERANCE_BYTES[1034] ^ *(( char*)nonce_bytes+0);
            final_init_img[1038] = NEW_IMAGE_REFERANCE_BYTES[1038] ^ *(( char*)nonce_bytes+0);
            final_init_img[1042] = NEW_IMAGE_REFERANCE_BYTES[1042] ^ *(( char*)nonce_bytes+0);
            final_init_img[1046] = NEW_IMAGE_REFERANCE_BYTES[1046] ^ *(( char*)nonce_bytes+0);
            final_init_img[1050] = NEW_IMAGE_REFERANCE_BYTES[1050] ^ *(( char*)nonce_bytes+0);
            final_init_img[1054] = NEW_IMAGE_REFERANCE_BYTES[1054] ^ *(( char*)nonce_bytes+0);
            final_init_img[1058] = NEW_IMAGE_REFERANCE_BYTES[1058] ^ *(( char*)nonce_bytes+0);
            final_init_img[1062] = NEW_IMAGE_REFERANCE_BYTES[1062] ^ *(( char*)nonce_bytes+0);
            final_init_img[1066] = NEW_IMAGE_REFERANCE_BYTES[1066] ^ *(( char*)nonce_bytes+0);
            final_init_img[1070] = NEW_IMAGE_REFERANCE_BYTES[1070] ^ *(( char*)nonce_bytes+0);
            final_init_img[1074] = NEW_IMAGE_REFERANCE_BYTES[1074] ^ *(( char*)nonce_bytes+0);
            final_init_img[1078] = NEW_IMAGE_REFERANCE_BYTES[1078] ^ *(( char*)nonce_bytes+0);
            final_init_img[1082] = NEW_IMAGE_REFERANCE_BYTES[1082] ^ *(( char*)nonce_bytes+0);
            final_init_img[1086] = NEW_IMAGE_REFERANCE_BYTES[1086] ^ *(( char*)nonce_bytes+0);
            final_init_img[1090] = NEW_IMAGE_REFERANCE_BYTES[1090] ^ *(( char*)nonce_bytes+0);
            final_init_img[1094] = NEW_IMAGE_REFERANCE_BYTES[1094] ^ *(( char*)nonce_bytes+0);
            final_init_img[1098] = NEW_IMAGE_REFERANCE_BYTES[1098] ^ *(( char*)nonce_bytes+0);
            final_init_img[1102] = NEW_IMAGE_REFERANCE_BYTES[1102] ^ *(( char*)nonce_bytes+0);
            final_init_img[1106] = NEW_IMAGE_REFERANCE_BYTES[1106] ^ *(( char*)nonce_bytes+0);
            final_init_img[1110] = NEW_IMAGE_REFERANCE_BYTES[1110] ^ *(( char*)nonce_bytes+0);
            final_init_img[1114] = NEW_IMAGE_REFERANCE_BYTES[1114] ^ *(( char*)nonce_bytes+0);
            final_init_img[1118] = NEW_IMAGE_REFERANCE_BYTES[1118] ^ *(( char*)nonce_bytes+0);
            final_init_img[1122] = NEW_IMAGE_REFERANCE_BYTES[1122] ^ *(( char*)nonce_bytes+0);
            final_init_img[1126] = NEW_IMAGE_REFERANCE_BYTES[1126] ^ *(( char*)nonce_bytes+0);
            final_init_img[1130] = NEW_IMAGE_REFERANCE_BYTES[1130] ^ *(( char*)nonce_bytes+0);
            final_init_img[1134] = NEW_IMAGE_REFERANCE_BYTES[1134] ^ *(( char*)nonce_bytes+0);
            final_init_img[1138] = NEW_IMAGE_REFERANCE_BYTES[1138] ^ *(( char*)nonce_bytes+0);
            final_init_img[1142] = NEW_IMAGE_REFERANCE_BYTES[1142] ^ *(( char*)nonce_bytes+0);
            final_init_img[1146] = NEW_IMAGE_REFERANCE_BYTES[1146] ^ *(( char*)nonce_bytes+0);
            final_init_img[1150] = NEW_IMAGE_REFERANCE_BYTES[1150] ^ *(( char*)nonce_bytes+0);
            final_init_img[1154] = NEW_IMAGE_REFERANCE_BYTES[1154] ^ *(( char*)nonce_bytes+0);
            final_init_img[1158] = NEW_IMAGE_REFERANCE_BYTES[1158] ^ *(( char*)nonce_bytes+0);
            final_init_img[1162] = NEW_IMAGE_REFERANCE_BYTES[1162] ^ *(( char*)nonce_bytes+0);
            final_init_img[1166] = NEW_IMAGE_REFERANCE_BYTES[1166] ^ *(( char*)nonce_bytes+0);
            final_init_img[1170] = NEW_IMAGE_REFERANCE_BYTES[1170] ^ *(( char*)nonce_bytes+0);
            final_init_img[1174] = NEW_IMAGE_REFERANCE_BYTES[1174] ^ *(( char*)nonce_bytes+0);
            final_init_img[1178] = NEW_IMAGE_REFERANCE_BYTES[1178] ^ *(( char*)nonce_bytes+0);
            final_init_img[1182] = NEW_IMAGE_REFERANCE_BYTES[1182] ^ *(( char*)nonce_bytes+0);
            final_init_img[1186] = NEW_IMAGE_REFERANCE_BYTES[1186] ^ *(( char*)nonce_bytes+0);
            final_init_img[1190] = NEW_IMAGE_REFERANCE_BYTES[1190] ^ *(( char*)nonce_bytes+0);
            final_init_img[1194] = NEW_IMAGE_REFERANCE_BYTES[1194] ^ *(( char*)nonce_bytes+0);
            final_init_img[1198] = NEW_IMAGE_REFERANCE_BYTES[1198] ^ *(( char*)nonce_bytes+0);
            final_init_img[1202] = NEW_IMAGE_REFERANCE_BYTES[1202] ^ *(( char*)nonce_bytes+0);
            final_init_img[1206] = NEW_IMAGE_REFERANCE_BYTES[1206] ^ *(( char*)nonce_bytes+0);
            final_init_img[1210] = NEW_IMAGE_REFERANCE_BYTES[1210] ^ *(( char*)nonce_bytes+0);
            final_init_img[1214] = NEW_IMAGE_REFERANCE_BYTES[1214] ^ *(( char*)nonce_bytes+0);
            final_init_img[1218] = NEW_IMAGE_REFERANCE_BYTES[1218] ^ *(( char*)nonce_bytes+0);
            final_init_img[1222] = NEW_IMAGE_REFERANCE_BYTES[1222] ^ *(( char*)nonce_bytes+0);
            final_init_img[1226] = NEW_IMAGE_REFERANCE_BYTES[1226] ^ *(( char*)nonce_bytes+0);
            final_init_img[1230] = NEW_IMAGE_REFERANCE_BYTES[1230] ^ *(( char*)nonce_bytes+0);
            final_init_img[1234] = NEW_IMAGE_REFERANCE_BYTES[1234] ^ *(( char*)nonce_bytes+0);
            final_init_img[1238] = NEW_IMAGE_REFERANCE_BYTES[1238] ^ *(( char*)nonce_bytes+0);
            final_init_img[1242] = NEW_IMAGE_REFERANCE_BYTES[1242] ^ *(( char*)nonce_bytes+0);
            final_init_img[1246] = NEW_IMAGE_REFERANCE_BYTES[1246] ^ *(( char*)nonce_bytes+0);
            final_init_img[1250] = NEW_IMAGE_REFERANCE_BYTES[1250] ^ *(( char*)nonce_bytes+0);
            final_init_img[1254] = NEW_IMAGE_REFERANCE_BYTES[1254] ^ *(( char*)nonce_bytes+0);
            final_init_img[1258] = NEW_IMAGE_REFERANCE_BYTES[1258] ^ *(( char*)nonce_bytes+0);
            final_init_img[1262] = NEW_IMAGE_REFERANCE_BYTES[1262] ^ *(( char*)nonce_bytes+0);
            final_init_img[1266] = NEW_IMAGE_REFERANCE_BYTES[1266] ^ *(( char*)nonce_bytes+0);
            final_init_img[1270] = NEW_IMAGE_REFERANCE_BYTES[1270] ^ *(( char*)nonce_bytes+0);
            final_init_img[1274] = NEW_IMAGE_REFERANCE_BYTES[1274] ^ *(( char*)nonce_bytes+0);
            final_init_img[1278] = NEW_IMAGE_REFERANCE_BYTES[1278] ^ *(( char*)nonce_bytes+0);
            final_init_img[1282] = NEW_IMAGE_REFERANCE_BYTES[1282] ^ *(( char*)nonce_bytes+0);
            final_init_img[1286] = NEW_IMAGE_REFERANCE_BYTES[1286] ^ *(( char*)nonce_bytes+0);
            final_init_img[1290] = NEW_IMAGE_REFERANCE_BYTES[1290] ^ *(( char*)nonce_bytes+0);
            final_init_img[1294] = NEW_IMAGE_REFERANCE_BYTES[1294] ^ *(( char*)nonce_bytes+0);
            final_init_img[1298] = NEW_IMAGE_REFERANCE_BYTES[1298] ^ *(( char*)nonce_bytes+0);
            final_init_img[1302] = NEW_IMAGE_REFERANCE_BYTES[1302] ^ *(( char*)nonce_bytes+0);
            final_init_img[1306] = NEW_IMAGE_REFERANCE_BYTES[1306] ^ *(( char*)nonce_bytes+0);
            final_init_img[1310] = NEW_IMAGE_REFERANCE_BYTES[1310] ^ *(( char*)nonce_bytes+0);
            final_init_img[1314] = NEW_IMAGE_REFERANCE_BYTES[1314] ^ *(( char*)nonce_bytes+0);
            final_init_img[1318] = NEW_IMAGE_REFERANCE_BYTES[1318] ^ *(( char*)nonce_bytes+0);
            final_init_img[1322] = NEW_IMAGE_REFERANCE_BYTES[1322] ^ *(( char*)nonce_bytes+0);
            final_init_img[1326] = NEW_IMAGE_REFERANCE_BYTES[1326] ^ *(( char*)nonce_bytes+0);
            final_init_img[1330] = NEW_IMAGE_REFERANCE_BYTES[1330] ^ *(( char*)nonce_bytes+0);
            final_init_img[1334] = NEW_IMAGE_REFERANCE_BYTES[1334] ^ *(( char*)nonce_bytes+0);
            final_init_img[1338] = NEW_IMAGE_REFERANCE_BYTES[1338] ^ *(( char*)nonce_bytes+0);
            final_init_img[1342] = NEW_IMAGE_REFERANCE_BYTES[1342] ^ *(( char*)nonce_bytes+0);
            final_init_img[1346] = NEW_IMAGE_REFERANCE_BYTES[1346] ^ *(( char*)nonce_bytes+0);
            final_init_img[1350] = NEW_IMAGE_REFERANCE_BYTES[1350] ^ *(( char*)nonce_bytes+0);
            final_init_img[1354] = NEW_IMAGE_REFERANCE_BYTES[1354] ^ *(( char*)nonce_bytes+0);
            final_init_img[1358] = NEW_IMAGE_REFERANCE_BYTES[1358] ^ *(( char*)nonce_bytes+0);
            final_init_img[1362] = NEW_IMAGE_REFERANCE_BYTES[1362] ^ *(( char*)nonce_bytes+0);
            final_init_img[1366] = NEW_IMAGE_REFERANCE_BYTES[1366] ^ *(( char*)nonce_bytes+0);
            final_init_img[1370] = NEW_IMAGE_REFERANCE_BYTES[1370] ^ *(( char*)nonce_bytes+0);
            final_init_img[1374] = NEW_IMAGE_REFERANCE_BYTES[1374] ^ *(( char*)nonce_bytes+0);
            final_init_img[1378] = NEW_IMAGE_REFERANCE_BYTES[1378] ^ *(( char*)nonce_bytes+0);
            final_init_img[1382] = NEW_IMAGE_REFERANCE_BYTES[1382] ^ *(( char*)nonce_bytes+0);
            final_init_img[1386] = NEW_IMAGE_REFERANCE_BYTES[1386] ^ *(( char*)nonce_bytes+0);
            final_init_img[1390] = NEW_IMAGE_REFERANCE_BYTES[1390] ^ *(( char*)nonce_bytes+0);
            final_init_img[1394] = NEW_IMAGE_REFERANCE_BYTES[1394] ^ *(( char*)nonce_bytes+0);
            final_init_img[1398] = NEW_IMAGE_REFERANCE_BYTES[1398] ^ *(( char*)nonce_bytes+0);
            final_init_img[1402] = NEW_IMAGE_REFERANCE_BYTES[1402] ^ *(( char*)nonce_bytes+0);
            final_init_img[1406] = NEW_IMAGE_REFERANCE_BYTES[1406] ^ *(( char*)nonce_bytes+0);
            final_init_img[1410] = NEW_IMAGE_REFERANCE_BYTES[1410] ^ *(( char*)nonce_bytes+0);
            final_init_img[1414] = NEW_IMAGE_REFERANCE_BYTES[1414] ^ *(( char*)nonce_bytes+0);
            final_init_img[1418] = NEW_IMAGE_REFERANCE_BYTES[1418] ^ *(( char*)nonce_bytes+0);
            final_init_img[1422] = NEW_IMAGE_REFERANCE_BYTES[1422] ^ *(( char*)nonce_bytes+0);
            final_init_img[1426] = NEW_IMAGE_REFERANCE_BYTES[1426] ^ *(( char*)nonce_bytes+0);
            final_init_img[1430] = NEW_IMAGE_REFERANCE_BYTES[1430] ^ *(( char*)nonce_bytes+0);
            final_init_img[1434] = NEW_IMAGE_REFERANCE_BYTES[1434] ^ *(( char*)nonce_bytes+0);
            final_init_img[1438] = NEW_IMAGE_REFERANCE_BYTES[1438] ^ *(( char*)nonce_bytes+0);
            final_init_img[1442] = NEW_IMAGE_REFERANCE_BYTES[1442] ^ *(( char*)nonce_bytes+0);
            final_init_img[1446] = NEW_IMAGE_REFERANCE_BYTES[1446] ^ *(( char*)nonce_bytes+0);
            final_init_img[1450] = NEW_IMAGE_REFERANCE_BYTES[1450] ^ *(( char*)nonce_bytes+0);
            final_init_img[1454] = NEW_IMAGE_REFERANCE_BYTES[1454] ^ *(( char*)nonce_bytes+0);
            final_init_img[1458] = NEW_IMAGE_REFERANCE_BYTES[1458] ^ *(( char*)nonce_bytes+0);
            final_init_img[1462] = NEW_IMAGE_REFERANCE_BYTES[1462] ^ *(( char*)nonce_bytes+0);
            final_init_img[1466] = NEW_IMAGE_REFERANCE_BYTES[1466] ^ *(( char*)nonce_bytes+0);
            final_init_img[1470] = NEW_IMAGE_REFERANCE_BYTES[1470] ^ *(( char*)nonce_bytes+0);
            final_init_img[1474] = NEW_IMAGE_REFERANCE_BYTES[1474] ^ *(( char*)nonce_bytes+0);
            final_init_img[1478] = NEW_IMAGE_REFERANCE_BYTES[1478] ^ *(( char*)nonce_bytes+0);
            final_init_img[1482] = NEW_IMAGE_REFERANCE_BYTES[1482] ^ *(( char*)nonce_bytes+0);
            final_init_img[1486] = NEW_IMAGE_REFERANCE_BYTES[1486] ^ *(( char*)nonce_bytes+0);
            final_init_img[1490] = NEW_IMAGE_REFERANCE_BYTES[1490] ^ *(( char*)nonce_bytes+0);
            final_init_img[1494] = NEW_IMAGE_REFERANCE_BYTES[1494] ^ *(( char*)nonce_bytes+0);
            final_init_img[1498] = NEW_IMAGE_REFERANCE_BYTES[1498] ^ *(( char*)nonce_bytes+0);
            final_init_img[1502] = NEW_IMAGE_REFERANCE_BYTES[1502] ^ *(( char*)nonce_bytes+0);
            final_init_img[1506] = NEW_IMAGE_REFERANCE_BYTES[1506] ^ *(( char*)nonce_bytes+0);
            final_init_img[1510] = NEW_IMAGE_REFERANCE_BYTES[1510] ^ *(( char*)nonce_bytes+0);
            final_init_img[1514] = NEW_IMAGE_REFERANCE_BYTES[1514] ^ *(( char*)nonce_bytes+0);
            final_init_img[1518] = NEW_IMAGE_REFERANCE_BYTES[1518] ^ *(( char*)nonce_bytes+0);
            final_init_img[1522] = NEW_IMAGE_REFERANCE_BYTES[1522] ^ *(( char*)nonce_bytes+0);
            final_init_img[1526] = NEW_IMAGE_REFERANCE_BYTES[1526] ^ *(( char*)nonce_bytes+0);
            final_init_img[1530] = NEW_IMAGE_REFERANCE_BYTES[1530] ^ *(( char*)nonce_bytes+0);
            final_init_img[1534] = NEW_IMAGE_REFERANCE_BYTES[1534] ^ *(( char*)nonce_bytes+0);
            final_init_img[1538] = NEW_IMAGE_REFERANCE_BYTES[1538] ^ *(( char*)nonce_bytes+0);
            final_init_img[1542] = NEW_IMAGE_REFERANCE_BYTES[1542] ^ *(( char*)nonce_bytes+0);
            final_init_img[1546] = NEW_IMAGE_REFERANCE_BYTES[1546] ^ *(( char*)nonce_bytes+0);
            final_init_img[1550] = NEW_IMAGE_REFERANCE_BYTES[1550] ^ *(( char*)nonce_bytes+0);
            final_init_img[1554] = NEW_IMAGE_REFERANCE_BYTES[1554] ^ *(( char*)nonce_bytes+0);
            final_init_img[1558] = NEW_IMAGE_REFERANCE_BYTES[1558] ^ *(( char*)nonce_bytes+0);
            final_init_img[1562] = NEW_IMAGE_REFERANCE_BYTES[1562] ^ *(( char*)nonce_bytes+0);
            final_init_img[1566] = NEW_IMAGE_REFERANCE_BYTES[1566] ^ *(( char*)nonce_bytes+0);
            final_init_img[1570] = NEW_IMAGE_REFERANCE_BYTES[1570] ^ *(( char*)nonce_bytes+0);
            final_init_img[1574] = NEW_IMAGE_REFERANCE_BYTES[1574] ^ *(( char*)nonce_bytes+0);
            final_init_img[1578] = NEW_IMAGE_REFERANCE_BYTES[1578] ^ *(( char*)nonce_bytes+0);
            final_init_img[1582] = NEW_IMAGE_REFERANCE_BYTES[1582] ^ *(( char*)nonce_bytes+0);
            final_init_img[1586] = NEW_IMAGE_REFERANCE_BYTES[1586] ^ *(( char*)nonce_bytes+0);
            final_init_img[1590] = NEW_IMAGE_REFERANCE_BYTES[1590] ^ *(( char*)nonce_bytes+0);
            final_init_img[1594] = NEW_IMAGE_REFERANCE_BYTES[1594] ^ *(( char*)nonce_bytes+0);
            final_init_img[1598] = NEW_IMAGE_REFERANCE_BYTES[1598] ^ *(( char*)nonce_bytes+0);
            final_init_img[1602] = NEW_IMAGE_REFERANCE_BYTES[1602] ^ *(( char*)nonce_bytes+0);
            final_init_img[1606] = NEW_IMAGE_REFERANCE_BYTES[1606] ^ *(( char*)nonce_bytes+0);
            final_init_img[1610] = NEW_IMAGE_REFERANCE_BYTES[1610] ^ *(( char*)nonce_bytes+0);
            final_init_img[1614] = NEW_IMAGE_REFERANCE_BYTES[1614] ^ *(( char*)nonce_bytes+0);
            final_init_img[1618] = NEW_IMAGE_REFERANCE_BYTES[1618] ^ *(( char*)nonce_bytes+0);
            final_init_img[1622] = NEW_IMAGE_REFERANCE_BYTES[1622] ^ *(( char*)nonce_bytes+0);
            final_init_img[1626] = NEW_IMAGE_REFERANCE_BYTES[1626] ^ *(( char*)nonce_bytes+0);
            final_init_img[1630] = NEW_IMAGE_REFERANCE_BYTES[1630] ^ *(( char*)nonce_bytes+0);
            final_init_img[1634] = NEW_IMAGE_REFERANCE_BYTES[1634] ^ *(( char*)nonce_bytes+0);
            final_init_img[1638] = NEW_IMAGE_REFERANCE_BYTES[1638] ^ *(( char*)nonce_bytes+0);
            final_init_img[1642] = NEW_IMAGE_REFERANCE_BYTES[1642] ^ *(( char*)nonce_bytes+0);
            final_init_img[1646] = NEW_IMAGE_REFERANCE_BYTES[1646] ^ *(( char*)nonce_bytes+0);
            final_init_img[1650] = NEW_IMAGE_REFERANCE_BYTES[1650] ^ *(( char*)nonce_bytes+0);
            final_init_img[1654] = NEW_IMAGE_REFERANCE_BYTES[1654] ^ *(( char*)nonce_bytes+0);
            final_init_img[1658] = NEW_IMAGE_REFERANCE_BYTES[1658] ^ *(( char*)nonce_bytes+0);
            final_init_img[1662] = NEW_IMAGE_REFERANCE_BYTES[1662] ^ *(( char*)nonce_bytes+0);
            final_init_img[1666] = NEW_IMAGE_REFERANCE_BYTES[1666] ^ *(( char*)nonce_bytes+0);
            final_init_img[1670] = NEW_IMAGE_REFERANCE_BYTES[1670] ^ *(( char*)nonce_bytes+0);
            final_init_img[1674] = NEW_IMAGE_REFERANCE_BYTES[1674] ^ *(( char*)nonce_bytes+0);
            final_init_img[1678] = NEW_IMAGE_REFERANCE_BYTES[1678] ^ *(( char*)nonce_bytes+0);
            final_init_img[1682] = NEW_IMAGE_REFERANCE_BYTES[1682] ^ *(( char*)nonce_bytes+0);
            final_init_img[1686] = NEW_IMAGE_REFERANCE_BYTES[1686] ^ *(( char*)nonce_bytes+0);
            final_init_img[1690] = NEW_IMAGE_REFERANCE_BYTES[1690] ^ *(( char*)nonce_bytes+0);
            final_init_img[1694] = NEW_IMAGE_REFERANCE_BYTES[1694] ^ *(( char*)nonce_bytes+0);
            final_init_img[1698] = NEW_IMAGE_REFERANCE_BYTES[1698] ^ *(( char*)nonce_bytes+0);
            final_init_img[1702] = NEW_IMAGE_REFERANCE_BYTES[1702] ^ *(( char*)nonce_bytes+0);
            final_init_img[1706] = NEW_IMAGE_REFERANCE_BYTES[1706] ^ *(( char*)nonce_bytes+0);
            final_init_img[1710] = NEW_IMAGE_REFERANCE_BYTES[1710] ^ *(( char*)nonce_bytes+0);
            final_init_img[1714] = NEW_IMAGE_REFERANCE_BYTES[1714] ^ *(( char*)nonce_bytes+0);
            final_init_img[1718] = NEW_IMAGE_REFERANCE_BYTES[1718] ^ *(( char*)nonce_bytes+0);
            final_init_img[1722] = NEW_IMAGE_REFERANCE_BYTES[1722] ^ *(( char*)nonce_bytes+0);
            final_init_img[1726] = NEW_IMAGE_REFERANCE_BYTES[1726] ^ *(( char*)nonce_bytes+0);
            final_init_img[1730] = NEW_IMAGE_REFERANCE_BYTES[1730] ^ *(( char*)nonce_bytes+0);
            final_init_img[1734] = NEW_IMAGE_REFERANCE_BYTES[1734] ^ *(( char*)nonce_bytes+0);
            final_init_img[1738] = NEW_IMAGE_REFERANCE_BYTES[1738] ^ *(( char*)nonce_bytes+0);
            final_init_img[1742] = NEW_IMAGE_REFERANCE_BYTES[1742] ^ *(( char*)nonce_bytes+0);
            final_init_img[1746] = NEW_IMAGE_REFERANCE_BYTES[1746] ^ *(( char*)nonce_bytes+0);
            final_init_img[1750] = NEW_IMAGE_REFERANCE_BYTES[1750] ^ *(( char*)nonce_bytes+0);
            final_init_img[1754] = NEW_IMAGE_REFERANCE_BYTES[1754] ^ *(( char*)nonce_bytes+0);
            final_init_img[1758] = NEW_IMAGE_REFERANCE_BYTES[1758] ^ *(( char*)nonce_bytes+0);
            final_init_img[1762] = NEW_IMAGE_REFERANCE_BYTES[1762] ^ *(( char*)nonce_bytes+0);
            final_init_img[1766] = NEW_IMAGE_REFERANCE_BYTES[1766] ^ *(( char*)nonce_bytes+0);
            final_init_img[1770] = NEW_IMAGE_REFERANCE_BYTES[1770] ^ *(( char*)nonce_bytes+0);
            final_init_img[1774] = NEW_IMAGE_REFERANCE_BYTES[1774] ^ *(( char*)nonce_bytes+0);
            final_init_img[1778] = NEW_IMAGE_REFERANCE_BYTES[1778] ^ *(( char*)nonce_bytes+0);
       
		}

        if (last_nonce_bytes[1] != *(( char*)nonce_bytes+1)){
            last_nonce_bytes[1] = *(( char*)nonce_bytes+1);
            final_init_img[55] = NEW_IMAGE_REFERANCE_BYTES[55] ^ *(( char*)nonce_bytes+1);
            final_init_img[59] = NEW_IMAGE_REFERANCE_BYTES[59] ^ *(( char*)nonce_bytes+1);
            final_init_img[63] = NEW_IMAGE_REFERANCE_BYTES[63] ^ *(( char*)nonce_bytes+1);
            final_init_img[67] = NEW_IMAGE_REFERANCE_BYTES[67] ^ *(( char*)nonce_bytes+1);
            final_init_img[71] = NEW_IMAGE_REFERANCE_BYTES[71] ^ *(( char*)nonce_bytes+1);
            final_init_img[75] = NEW_IMAGE_REFERANCE_BYTES[75] ^ *(( char*)nonce_bytes+1);
            final_init_img[79] = NEW_IMAGE_REFERANCE_BYTES[79] ^ *(( char*)nonce_bytes+1);
            final_init_img[83] = NEW_IMAGE_REFERANCE_BYTES[83] ^ *(( char*)nonce_bytes+1);
            final_init_img[87] = NEW_IMAGE_REFERANCE_BYTES[87] ^ *(( char*)nonce_bytes+1);
            final_init_img[91] = NEW_IMAGE_REFERANCE_BYTES[91] ^ *(( char*)nonce_bytes+1);
            final_init_img[95] = NEW_IMAGE_REFERANCE_BYTES[95] ^ *(( char*)nonce_bytes+1);
            final_init_img[99] = NEW_IMAGE_REFERANCE_BYTES[99] ^ *(( char*)nonce_bytes+1);
            final_init_img[103] = NEW_IMAGE_REFERANCE_BYTES[103] ^ *(( char*)nonce_bytes+1);
            final_init_img[107] = NEW_IMAGE_REFERANCE_BYTES[107] ^ *(( char*)nonce_bytes+1);
            final_init_img[111] = NEW_IMAGE_REFERANCE_BYTES[111] ^ *(( char*)nonce_bytes+1);
            final_init_img[115] = NEW_IMAGE_REFERANCE_BYTES[115] ^ *(( char*)nonce_bytes+1);
            final_init_img[119] = NEW_IMAGE_REFERANCE_BYTES[119] ^ *(( char*)nonce_bytes+1);
            final_init_img[123] = NEW_IMAGE_REFERANCE_BYTES[123] ^ *(( char*)nonce_bytes+1);
            final_init_img[127] = NEW_IMAGE_REFERANCE_BYTES[127] ^ *(( char*)nonce_bytes+1);
            final_init_img[131] = NEW_IMAGE_REFERANCE_BYTES[131] ^ *(( char*)nonce_bytes+1);
            final_init_img[135] = NEW_IMAGE_REFERANCE_BYTES[135] ^ *(( char*)nonce_bytes+1);
            final_init_img[139] = NEW_IMAGE_REFERANCE_BYTES[139] ^ *(( char*)nonce_bytes+1);
            final_init_img[143] = NEW_IMAGE_REFERANCE_BYTES[143] ^ *(( char*)nonce_bytes+1);
            final_init_img[147] = NEW_IMAGE_REFERANCE_BYTES[147] ^ *(( char*)nonce_bytes+1);
            final_init_img[151] = NEW_IMAGE_REFERANCE_BYTES[151] ^ *(( char*)nonce_bytes+1);
            final_init_img[155] = NEW_IMAGE_REFERANCE_BYTES[155] ^ *(( char*)nonce_bytes+1);
            final_init_img[159] = NEW_IMAGE_REFERANCE_BYTES[159] ^ *(( char*)nonce_bytes+1);
            final_init_img[163] = NEW_IMAGE_REFERANCE_BYTES[163] ^ *(( char*)nonce_bytes+1);
            final_init_img[167] = NEW_IMAGE_REFERANCE_BYTES[167] ^ *(( char*)nonce_bytes+1);
            final_init_img[171] = NEW_IMAGE_REFERANCE_BYTES[171] ^ *(( char*)nonce_bytes+1);
            final_init_img[175] = NEW_IMAGE_REFERANCE_BYTES[175] ^ *(( char*)nonce_bytes+1);
            final_init_img[179] = NEW_IMAGE_REFERANCE_BYTES[179] ^ *(( char*)nonce_bytes+1);
            final_init_img[183] = NEW_IMAGE_REFERANCE_BYTES[183] ^ *(( char*)nonce_bytes+1);
            final_init_img[187] = NEW_IMAGE_REFERANCE_BYTES[187] ^ *(( char*)nonce_bytes+1);
            final_init_img[191] = NEW_IMAGE_REFERANCE_BYTES[191] ^ *(( char*)nonce_bytes+1);
            final_init_img[195] = NEW_IMAGE_REFERANCE_BYTES[195] ^ *(( char*)nonce_bytes+1);
            final_init_img[199] = NEW_IMAGE_REFERANCE_BYTES[199] ^ *(( char*)nonce_bytes+1);
            final_init_img[203] = NEW_IMAGE_REFERANCE_BYTES[203] ^ *(( char*)nonce_bytes+1);
            final_init_img[207] = NEW_IMAGE_REFERANCE_BYTES[207] ^ *(( char*)nonce_bytes+1);
            final_init_img[211] = NEW_IMAGE_REFERANCE_BYTES[211] ^ *(( char*)nonce_bytes+1);
            final_init_img[215] = NEW_IMAGE_REFERANCE_BYTES[215] ^ *(( char*)nonce_bytes+1);
            final_init_img[219] = NEW_IMAGE_REFERANCE_BYTES[219] ^ *(( char*)nonce_bytes+1);
            final_init_img[223] = NEW_IMAGE_REFERANCE_BYTES[223] ^ *(( char*)nonce_bytes+1);
            final_init_img[227] = NEW_IMAGE_REFERANCE_BYTES[227] ^ *(( char*)nonce_bytes+1);
            final_init_img[231] = NEW_IMAGE_REFERANCE_BYTES[231] ^ *(( char*)nonce_bytes+1);
            final_init_img[235] = NEW_IMAGE_REFERANCE_BYTES[235] ^ *(( char*)nonce_bytes+1);
            final_init_img[239] = NEW_IMAGE_REFERANCE_BYTES[239] ^ *(( char*)nonce_bytes+1);
            final_init_img[243] = NEW_IMAGE_REFERANCE_BYTES[243] ^ *(( char*)nonce_bytes+1);
            final_init_img[247] = NEW_IMAGE_REFERANCE_BYTES[247] ^ *(( char*)nonce_bytes+1);
            final_init_img[251] = NEW_IMAGE_REFERANCE_BYTES[251] ^ *(( char*)nonce_bytes+1);
            final_init_img[255] = NEW_IMAGE_REFERANCE_BYTES[255] ^ *(( char*)nonce_bytes+1);
            final_init_img[259] = NEW_IMAGE_REFERANCE_BYTES[259] ^ *(( char*)nonce_bytes+1);
            final_init_img[263] = NEW_IMAGE_REFERANCE_BYTES[263] ^ *(( char*)nonce_bytes+1);
            final_init_img[267] = NEW_IMAGE_REFERANCE_BYTES[267] ^ *(( char*)nonce_bytes+1);
            final_init_img[271] = NEW_IMAGE_REFERANCE_BYTES[271] ^ *(( char*)nonce_bytes+1);
            final_init_img[275] = NEW_IMAGE_REFERANCE_BYTES[275] ^ *(( char*)nonce_bytes+1);
            final_init_img[279] = NEW_IMAGE_REFERANCE_BYTES[279] ^ *(( char*)nonce_bytes+1);
            final_init_img[283] = NEW_IMAGE_REFERANCE_BYTES[283] ^ *(( char*)nonce_bytes+1);
            final_init_img[287] = NEW_IMAGE_REFERANCE_BYTES[287] ^ *(( char*)nonce_bytes+1);
            final_init_img[291] = NEW_IMAGE_REFERANCE_BYTES[291] ^ *(( char*)nonce_bytes+1);
            final_init_img[295] = NEW_IMAGE_REFERANCE_BYTES[295] ^ *(( char*)nonce_bytes+1);
            final_init_img[299] = NEW_IMAGE_REFERANCE_BYTES[299] ^ *(( char*)nonce_bytes+1);
            final_init_img[303] = NEW_IMAGE_REFERANCE_BYTES[303] ^ *(( char*)nonce_bytes+1);
            final_init_img[307] = NEW_IMAGE_REFERANCE_BYTES[307] ^ *(( char*)nonce_bytes+1);
            final_init_img[311] = NEW_IMAGE_REFERANCE_BYTES[311] ^ *(( char*)nonce_bytes+1);
            final_init_img[315] = NEW_IMAGE_REFERANCE_BYTES[315] ^ *(( char*)nonce_bytes+1);
            final_init_img[319] = NEW_IMAGE_REFERANCE_BYTES[319] ^ *(( char*)nonce_bytes+1);
            final_init_img[323] = NEW_IMAGE_REFERANCE_BYTES[323] ^ *(( char*)nonce_bytes+1);
            final_init_img[327] = NEW_IMAGE_REFERANCE_BYTES[327] ^ *(( char*)nonce_bytes+1);
            final_init_img[331] = NEW_IMAGE_REFERANCE_BYTES[331] ^ *(( char*)nonce_bytes+1);
            final_init_img[335] = NEW_IMAGE_REFERANCE_BYTES[335] ^ *(( char*)nonce_bytes+1);
            final_init_img[339] = NEW_IMAGE_REFERANCE_BYTES[339] ^ *(( char*)nonce_bytes+1);
            final_init_img[343] = NEW_IMAGE_REFERANCE_BYTES[343] ^ *(( char*)nonce_bytes+1);
            final_init_img[347] = NEW_IMAGE_REFERANCE_BYTES[347] ^ *(( char*)nonce_bytes+1);
            final_init_img[351] = NEW_IMAGE_REFERANCE_BYTES[351] ^ *(( char*)nonce_bytes+1);
            final_init_img[355] = NEW_IMAGE_REFERANCE_BYTES[355] ^ *(( char*)nonce_bytes+1);
            final_init_img[359] = NEW_IMAGE_REFERANCE_BYTES[359] ^ *(( char*)nonce_bytes+1);
            final_init_img[363] = NEW_IMAGE_REFERANCE_BYTES[363] ^ *(( char*)nonce_bytes+1);
            final_init_img[367] = NEW_IMAGE_REFERANCE_BYTES[367] ^ *(( char*)nonce_bytes+1);
            final_init_img[371] = NEW_IMAGE_REFERANCE_BYTES[371] ^ *(( char*)nonce_bytes+1);
            final_init_img[375] = NEW_IMAGE_REFERANCE_BYTES[375] ^ *(( char*)nonce_bytes+1);
            final_init_img[379] = NEW_IMAGE_REFERANCE_BYTES[379] ^ *(( char*)nonce_bytes+1);
            final_init_img[383] = NEW_IMAGE_REFERANCE_BYTES[383] ^ *(( char*)nonce_bytes+1);
            final_init_img[387] = NEW_IMAGE_REFERANCE_BYTES[387] ^ *(( char*)nonce_bytes+1);
            final_init_img[391] = NEW_IMAGE_REFERANCE_BYTES[391] ^ *(( char*)nonce_bytes+1);
            final_init_img[395] = NEW_IMAGE_REFERANCE_BYTES[395] ^ *(( char*)nonce_bytes+1);
            final_init_img[399] = NEW_IMAGE_REFERANCE_BYTES[399] ^ *(( char*)nonce_bytes+1);
            final_init_img[403] = NEW_IMAGE_REFERANCE_BYTES[403] ^ *(( char*)nonce_bytes+1);
            final_init_img[407] = NEW_IMAGE_REFERANCE_BYTES[407] ^ *(( char*)nonce_bytes+1);
            final_init_img[411] = NEW_IMAGE_REFERANCE_BYTES[411] ^ *(( char*)nonce_bytes+1);
            final_init_img[415] = NEW_IMAGE_REFERANCE_BYTES[415] ^ *(( char*)nonce_bytes+1);
            final_init_img[419] = NEW_IMAGE_REFERANCE_BYTES[419] ^ *(( char*)nonce_bytes+1);
            final_init_img[423] = NEW_IMAGE_REFERANCE_BYTES[423] ^ *(( char*)nonce_bytes+1);
            final_init_img[427] = NEW_IMAGE_REFERANCE_BYTES[427] ^ *(( char*)nonce_bytes+1);
            final_init_img[431] = NEW_IMAGE_REFERANCE_BYTES[431] ^ *(( char*)nonce_bytes+1);
            final_init_img[435] = NEW_IMAGE_REFERANCE_BYTES[435] ^ *(( char*)nonce_bytes+1);
            final_init_img[439] = NEW_IMAGE_REFERANCE_BYTES[439] ^ *(( char*)nonce_bytes+1);
            final_init_img[443] = NEW_IMAGE_REFERANCE_BYTES[443] ^ *(( char*)nonce_bytes+1);
            final_init_img[447] = NEW_IMAGE_REFERANCE_BYTES[447] ^ *(( char*)nonce_bytes+1);
            final_init_img[451] = NEW_IMAGE_REFERANCE_BYTES[451] ^ *(( char*)nonce_bytes+1);
            final_init_img[455] = NEW_IMAGE_REFERANCE_BYTES[455] ^ *(( char*)nonce_bytes+1);
            final_init_img[459] = NEW_IMAGE_REFERANCE_BYTES[459] ^ *(( char*)nonce_bytes+1);
            final_init_img[463] = NEW_IMAGE_REFERANCE_BYTES[463] ^ *(( char*)nonce_bytes+1);
            final_init_img[467] = NEW_IMAGE_REFERANCE_BYTES[467] ^ *(( char*)nonce_bytes+1);
            final_init_img[471] = NEW_IMAGE_REFERANCE_BYTES[471] ^ *(( char*)nonce_bytes+1);
            final_init_img[475] = NEW_IMAGE_REFERANCE_BYTES[475] ^ *(( char*)nonce_bytes+1);
            final_init_img[479] = NEW_IMAGE_REFERANCE_BYTES[479] ^ *(( char*)nonce_bytes+1);
            final_init_img[483] = NEW_IMAGE_REFERANCE_BYTES[483] ^ *(( char*)nonce_bytes+1);
            final_init_img[487] = NEW_IMAGE_REFERANCE_BYTES[487] ^ *(( char*)nonce_bytes+1);
            final_init_img[491] = NEW_IMAGE_REFERANCE_BYTES[491] ^ *(( char*)nonce_bytes+1);
            final_init_img[495] = NEW_IMAGE_REFERANCE_BYTES[495] ^ *(( char*)nonce_bytes+1);
            final_init_img[499] = NEW_IMAGE_REFERANCE_BYTES[499] ^ *(( char*)nonce_bytes+1);
            final_init_img[503] = NEW_IMAGE_REFERANCE_BYTES[503] ^ *(( char*)nonce_bytes+1);
            final_init_img[507] = NEW_IMAGE_REFERANCE_BYTES[507] ^ *(( char*)nonce_bytes+1);
            final_init_img[511] = NEW_IMAGE_REFERANCE_BYTES[511] ^ *(( char*)nonce_bytes+1);
            final_init_img[515] = NEW_IMAGE_REFERANCE_BYTES[515] ^ *(( char*)nonce_bytes+1);
            final_init_img[519] = NEW_IMAGE_REFERANCE_BYTES[519] ^ *(( char*)nonce_bytes+1);
            final_init_img[523] = NEW_IMAGE_REFERANCE_BYTES[523] ^ *(( char*)nonce_bytes+1);
            final_init_img[527] = NEW_IMAGE_REFERANCE_BYTES[527] ^ *(( char*)nonce_bytes+1);
            final_init_img[531] = NEW_IMAGE_REFERANCE_BYTES[531] ^ *(( char*)nonce_bytes+1);
            final_init_img[535] = NEW_IMAGE_REFERANCE_BYTES[535] ^ *(( char*)nonce_bytes+1);
            final_init_img[539] = NEW_IMAGE_REFERANCE_BYTES[539] ^ *(( char*)nonce_bytes+1);
            final_init_img[543] = NEW_IMAGE_REFERANCE_BYTES[543] ^ *(( char*)nonce_bytes+1);
            final_init_img[547] = NEW_IMAGE_REFERANCE_BYTES[547] ^ *(( char*)nonce_bytes+1);
            final_init_img[551] = NEW_IMAGE_REFERANCE_BYTES[551] ^ *(( char*)nonce_bytes+1);
            final_init_img[555] = NEW_IMAGE_REFERANCE_BYTES[555] ^ *(( char*)nonce_bytes+1);
            final_init_img[559] = NEW_IMAGE_REFERANCE_BYTES[559] ^ *(( char*)nonce_bytes+1);
            final_init_img[563] = NEW_IMAGE_REFERANCE_BYTES[563] ^ *(( char*)nonce_bytes+1);
            final_init_img[567] = NEW_IMAGE_REFERANCE_BYTES[567] ^ *(( char*)nonce_bytes+1);
            final_init_img[571] = NEW_IMAGE_REFERANCE_BYTES[571] ^ *(( char*)nonce_bytes+1);
            final_init_img[575] = NEW_IMAGE_REFERANCE_BYTES[575] ^ *(( char*)nonce_bytes+1);
            final_init_img[579] = NEW_IMAGE_REFERANCE_BYTES[579] ^ *(( char*)nonce_bytes+1);
            final_init_img[583] = NEW_IMAGE_REFERANCE_BYTES[583] ^ *(( char*)nonce_bytes+1);
            final_init_img[587] = NEW_IMAGE_REFERANCE_BYTES[587] ^ *(( char*)nonce_bytes+1);
            final_init_img[591] = NEW_IMAGE_REFERANCE_BYTES[591] ^ *(( char*)nonce_bytes+1);
            final_init_img[595] = NEW_IMAGE_REFERANCE_BYTES[595] ^ *(( char*)nonce_bytes+1);
            final_init_img[599] = NEW_IMAGE_REFERANCE_BYTES[599] ^ *(( char*)nonce_bytes+1);
            final_init_img[603] = NEW_IMAGE_REFERANCE_BYTES[603] ^ *(( char*)nonce_bytes+1);
            final_init_img[607] = NEW_IMAGE_REFERANCE_BYTES[607] ^ *(( char*)nonce_bytes+1);
            final_init_img[611] = NEW_IMAGE_REFERANCE_BYTES[611] ^ *(( char*)nonce_bytes+1);
            final_init_img[615] = NEW_IMAGE_REFERANCE_BYTES[615] ^ *(( char*)nonce_bytes+1);
            final_init_img[619] = NEW_IMAGE_REFERANCE_BYTES[619] ^ *(( char*)nonce_bytes+1);
            final_init_img[623] = NEW_IMAGE_REFERANCE_BYTES[623] ^ *(( char*)nonce_bytes+1);
            final_init_img[627] = NEW_IMAGE_REFERANCE_BYTES[627] ^ *(( char*)nonce_bytes+1);
            final_init_img[631] = NEW_IMAGE_REFERANCE_BYTES[631] ^ *(( char*)nonce_bytes+1);
            final_init_img[635] = NEW_IMAGE_REFERANCE_BYTES[635] ^ *(( char*)nonce_bytes+1);
            final_init_img[639] = NEW_IMAGE_REFERANCE_BYTES[639] ^ *(( char*)nonce_bytes+1);
            final_init_img[643] = NEW_IMAGE_REFERANCE_BYTES[643] ^ *(( char*)nonce_bytes+1);
            final_init_img[647] = NEW_IMAGE_REFERANCE_BYTES[647] ^ *(( char*)nonce_bytes+1);
            final_init_img[651] = NEW_IMAGE_REFERANCE_BYTES[651] ^ *(( char*)nonce_bytes+1);
            final_init_img[655] = NEW_IMAGE_REFERANCE_BYTES[655] ^ *(( char*)nonce_bytes+1);
            final_init_img[659] = NEW_IMAGE_REFERANCE_BYTES[659] ^ *(( char*)nonce_bytes+1);
            final_init_img[663] = NEW_IMAGE_REFERANCE_BYTES[663] ^ *(( char*)nonce_bytes+1);
            final_init_img[667] = NEW_IMAGE_REFERANCE_BYTES[667] ^ *(( char*)nonce_bytes+1);
            final_init_img[671] = NEW_IMAGE_REFERANCE_BYTES[671] ^ *(( char*)nonce_bytes+1);
            final_init_img[675] = NEW_IMAGE_REFERANCE_BYTES[675] ^ *(( char*)nonce_bytes+1);
            final_init_img[679] = NEW_IMAGE_REFERANCE_BYTES[679] ^ *(( char*)nonce_bytes+1);
            final_init_img[683] = NEW_IMAGE_REFERANCE_BYTES[683] ^ *(( char*)nonce_bytes+1);
            final_init_img[687] = NEW_IMAGE_REFERANCE_BYTES[687] ^ *(( char*)nonce_bytes+1);
            final_init_img[691] = NEW_IMAGE_REFERANCE_BYTES[691] ^ *(( char*)nonce_bytes+1);
            final_init_img[695] = NEW_IMAGE_REFERANCE_BYTES[695] ^ *(( char*)nonce_bytes+1);
            final_init_img[699] = NEW_IMAGE_REFERANCE_BYTES[699] ^ *(( char*)nonce_bytes+1);
            final_init_img[703] = NEW_IMAGE_REFERANCE_BYTES[703] ^ *(( char*)nonce_bytes+1);
            final_init_img[707] = NEW_IMAGE_REFERANCE_BYTES[707] ^ *(( char*)nonce_bytes+1);
            final_init_img[711] = NEW_IMAGE_REFERANCE_BYTES[711] ^ *(( char*)nonce_bytes+1);
            final_init_img[715] = NEW_IMAGE_REFERANCE_BYTES[715] ^ *(( char*)nonce_bytes+1);
            final_init_img[719] = NEW_IMAGE_REFERANCE_BYTES[719] ^ *(( char*)nonce_bytes+1);
            final_init_img[723] = NEW_IMAGE_REFERANCE_BYTES[723] ^ *(( char*)nonce_bytes+1);
            final_init_img[727] = NEW_IMAGE_REFERANCE_BYTES[727] ^ *(( char*)nonce_bytes+1);
            final_init_img[731] = NEW_IMAGE_REFERANCE_BYTES[731] ^ *(( char*)nonce_bytes+1);
            final_init_img[735] = NEW_IMAGE_REFERANCE_BYTES[735] ^ *(( char*)nonce_bytes+1);
            final_init_img[739] = NEW_IMAGE_REFERANCE_BYTES[739] ^ *(( char*)nonce_bytes+1);
            final_init_img[743] = NEW_IMAGE_REFERANCE_BYTES[743] ^ *(( char*)nonce_bytes+1);
            final_init_img[747] = NEW_IMAGE_REFERANCE_BYTES[747] ^ *(( char*)nonce_bytes+1);
            final_init_img[751] = NEW_IMAGE_REFERANCE_BYTES[751] ^ *(( char*)nonce_bytes+1);
            final_init_img[755] = NEW_IMAGE_REFERANCE_BYTES[755] ^ *(( char*)nonce_bytes+1);
            final_init_img[759] = NEW_IMAGE_REFERANCE_BYTES[759] ^ *(( char*)nonce_bytes+1);
            final_init_img[763] = NEW_IMAGE_REFERANCE_BYTES[763] ^ *(( char*)nonce_bytes+1);
            final_init_img[767] = NEW_IMAGE_REFERANCE_BYTES[767] ^ *(( char*)nonce_bytes+1);
            final_init_img[771] = NEW_IMAGE_REFERANCE_BYTES[771] ^ *(( char*)nonce_bytes+1);
            final_init_img[775] = NEW_IMAGE_REFERANCE_BYTES[775] ^ *(( char*)nonce_bytes+1);
            final_init_img[779] = NEW_IMAGE_REFERANCE_BYTES[779] ^ *(( char*)nonce_bytes+1);
            final_init_img[783] = NEW_IMAGE_REFERANCE_BYTES[783] ^ *(( char*)nonce_bytes+1);
            final_init_img[787] = NEW_IMAGE_REFERANCE_BYTES[787] ^ *(( char*)nonce_bytes+1);
            final_init_img[791] = NEW_IMAGE_REFERANCE_BYTES[791] ^ *(( char*)nonce_bytes+1);
            final_init_img[795] = NEW_IMAGE_REFERANCE_BYTES[795] ^ *(( char*)nonce_bytes+1);
            final_init_img[799] = NEW_IMAGE_REFERANCE_BYTES[799] ^ *(( char*)nonce_bytes+1);
            final_init_img[803] = NEW_IMAGE_REFERANCE_BYTES[803] ^ *(( char*)nonce_bytes+1);
            final_init_img[807] = NEW_IMAGE_REFERANCE_BYTES[807] ^ *(( char*)nonce_bytes+1);
            final_init_img[811] = NEW_IMAGE_REFERANCE_BYTES[811] ^ *(( char*)nonce_bytes+1);
            final_init_img[815] = NEW_IMAGE_REFERANCE_BYTES[815] ^ *(( char*)nonce_bytes+1);
            final_init_img[819] = NEW_IMAGE_REFERANCE_BYTES[819] ^ *(( char*)nonce_bytes+1);
            final_init_img[823] = NEW_IMAGE_REFERANCE_BYTES[823] ^ *(( char*)nonce_bytes+1);
            final_init_img[827] = NEW_IMAGE_REFERANCE_BYTES[827] ^ *(( char*)nonce_bytes+1);
            final_init_img[831] = NEW_IMAGE_REFERANCE_BYTES[831] ^ *(( char*)nonce_bytes+1);
            final_init_img[835] = NEW_IMAGE_REFERANCE_BYTES[835] ^ *(( char*)nonce_bytes+1);
            final_init_img[839] = NEW_IMAGE_REFERANCE_BYTES[839] ^ *(( char*)nonce_bytes+1);
            final_init_img[843] = NEW_IMAGE_REFERANCE_BYTES[843] ^ *(( char*)nonce_bytes+1);
            final_init_img[847] = NEW_IMAGE_REFERANCE_BYTES[847] ^ *(( char*)nonce_bytes+1);
            final_init_img[851] = NEW_IMAGE_REFERANCE_BYTES[851] ^ *(( char*)nonce_bytes+1);
            final_init_img[855] = NEW_IMAGE_REFERANCE_BYTES[855] ^ *(( char*)nonce_bytes+1);
            final_init_img[859] = NEW_IMAGE_REFERANCE_BYTES[859] ^ *(( char*)nonce_bytes+1);
            final_init_img[863] = NEW_IMAGE_REFERANCE_BYTES[863] ^ *(( char*)nonce_bytes+1);
            final_init_img[867] = NEW_IMAGE_REFERANCE_BYTES[867] ^ *(( char*)nonce_bytes+1);
            final_init_img[871] = NEW_IMAGE_REFERANCE_BYTES[871] ^ *(( char*)nonce_bytes+1);
            final_init_img[875] = NEW_IMAGE_REFERANCE_BYTES[875] ^ *(( char*)nonce_bytes+1);
            final_init_img[879] = NEW_IMAGE_REFERANCE_BYTES[879] ^ *(( char*)nonce_bytes+1);
            final_init_img[883] = NEW_IMAGE_REFERANCE_BYTES[883] ^ *(( char*)nonce_bytes+1);
            final_init_img[887] = NEW_IMAGE_REFERANCE_BYTES[887] ^ *(( char*)nonce_bytes+1);
            final_init_img[891] = NEW_IMAGE_REFERANCE_BYTES[891] ^ *(( char*)nonce_bytes+1);
            final_init_img[895] = NEW_IMAGE_REFERANCE_BYTES[895] ^ *(( char*)nonce_bytes+1);
            final_init_img[899] = NEW_IMAGE_REFERANCE_BYTES[899] ^ *(( char*)nonce_bytes+1);
            final_init_img[903] = NEW_IMAGE_REFERANCE_BYTES[903] ^ *(( char*)nonce_bytes+1);
            final_init_img[907] = NEW_IMAGE_REFERANCE_BYTES[907] ^ *(( char*)nonce_bytes+1);
            final_init_img[911] = NEW_IMAGE_REFERANCE_BYTES[911] ^ *(( char*)nonce_bytes+1);
            final_init_img[915] = NEW_IMAGE_REFERANCE_BYTES[915] ^ *(( char*)nonce_bytes+1);
            final_init_img[919] = NEW_IMAGE_REFERANCE_BYTES[919] ^ *(( char*)nonce_bytes+1);
            final_init_img[923] = NEW_IMAGE_REFERANCE_BYTES[923] ^ *(( char*)nonce_bytes+1);
            final_init_img[927] = NEW_IMAGE_REFERANCE_BYTES[927] ^ *(( char*)nonce_bytes+1);
            final_init_img[931] = NEW_IMAGE_REFERANCE_BYTES[931] ^ *(( char*)nonce_bytes+1);
            final_init_img[935] = NEW_IMAGE_REFERANCE_BYTES[935] ^ *(( char*)nonce_bytes+1);
            final_init_img[939] = NEW_IMAGE_REFERANCE_BYTES[939] ^ *(( char*)nonce_bytes+1);
            final_init_img[943] = NEW_IMAGE_REFERANCE_BYTES[943] ^ *(( char*)nonce_bytes+1);
            final_init_img[947] = NEW_IMAGE_REFERANCE_BYTES[947] ^ *(( char*)nonce_bytes+1);
            final_init_img[951] = NEW_IMAGE_REFERANCE_BYTES[951] ^ *(( char*)nonce_bytes+1);
            final_init_img[955] = NEW_IMAGE_REFERANCE_BYTES[955] ^ *(( char*)nonce_bytes+1);
            final_init_img[959] = NEW_IMAGE_REFERANCE_BYTES[959] ^ *(( char*)nonce_bytes+1);
            final_init_img[963] = NEW_IMAGE_REFERANCE_BYTES[963] ^ *(( char*)nonce_bytes+1);
            final_init_img[967] = NEW_IMAGE_REFERANCE_BYTES[967] ^ *(( char*)nonce_bytes+1);
            final_init_img[971] = NEW_IMAGE_REFERANCE_BYTES[971] ^ *(( char*)nonce_bytes+1);
            final_init_img[975] = NEW_IMAGE_REFERANCE_BYTES[975] ^ *(( char*)nonce_bytes+1);
            final_init_img[979] = NEW_IMAGE_REFERANCE_BYTES[979] ^ *(( char*)nonce_bytes+1);
            final_init_img[983] = NEW_IMAGE_REFERANCE_BYTES[983] ^ *(( char*)nonce_bytes+1);
            final_init_img[987] = NEW_IMAGE_REFERANCE_BYTES[987] ^ *(( char*)nonce_bytes+1);
            final_init_img[991] = NEW_IMAGE_REFERANCE_BYTES[991] ^ *(( char*)nonce_bytes+1);
            final_init_img[995] = NEW_IMAGE_REFERANCE_BYTES[995] ^ *(( char*)nonce_bytes+1);
            final_init_img[999] = NEW_IMAGE_REFERANCE_BYTES[999] ^ *(( char*)nonce_bytes+1);
            final_init_img[1003] = NEW_IMAGE_REFERANCE_BYTES[1003] ^ *(( char*)nonce_bytes+1);
            final_init_img[1007] = NEW_IMAGE_REFERANCE_BYTES[1007] ^ *(( char*)nonce_bytes+1);
            final_init_img[1011] = NEW_IMAGE_REFERANCE_BYTES[1011] ^ *(( char*)nonce_bytes+1);
            final_init_img[1015] = NEW_IMAGE_REFERANCE_BYTES[1015] ^ *(( char*)nonce_bytes+1);
            final_init_img[1019] = NEW_IMAGE_REFERANCE_BYTES[1019] ^ *(( char*)nonce_bytes+1);
            final_init_img[1023] = NEW_IMAGE_REFERANCE_BYTES[1023] ^ *(( char*)nonce_bytes+1);
            final_init_img[1027] = NEW_IMAGE_REFERANCE_BYTES[1027] ^ *(( char*)nonce_bytes+1);
            final_init_img[1031] = NEW_IMAGE_REFERANCE_BYTES[1031] ^ *(( char*)nonce_bytes+1);
            final_init_img[1035] = NEW_IMAGE_REFERANCE_BYTES[1035] ^ *(( char*)nonce_bytes+1);
            final_init_img[1039] = NEW_IMAGE_REFERANCE_BYTES[1039] ^ *(( char*)nonce_bytes+1);
            final_init_img[1043] = NEW_IMAGE_REFERANCE_BYTES[1043] ^ *(( char*)nonce_bytes+1);
            final_init_img[1047] = NEW_IMAGE_REFERANCE_BYTES[1047] ^ *(( char*)nonce_bytes+1);
            final_init_img[1051] = NEW_IMAGE_REFERANCE_BYTES[1051] ^ *(( char*)nonce_bytes+1);
            final_init_img[1055] = NEW_IMAGE_REFERANCE_BYTES[1055] ^ *(( char*)nonce_bytes+1);
            final_init_img[1059] = NEW_IMAGE_REFERANCE_BYTES[1059] ^ *(( char*)nonce_bytes+1);
            final_init_img[1063] = NEW_IMAGE_REFERANCE_BYTES[1063] ^ *(( char*)nonce_bytes+1);
            final_init_img[1067] = NEW_IMAGE_REFERANCE_BYTES[1067] ^ *(( char*)nonce_bytes+1);
            final_init_img[1071] = NEW_IMAGE_REFERANCE_BYTES[1071] ^ *(( char*)nonce_bytes+1);
            final_init_img[1075] = NEW_IMAGE_REFERANCE_BYTES[1075] ^ *(( char*)nonce_bytes+1);
            final_init_img[1079] = NEW_IMAGE_REFERANCE_BYTES[1079] ^ *(( char*)nonce_bytes+1);
            final_init_img[1083] = NEW_IMAGE_REFERANCE_BYTES[1083] ^ *(( char*)nonce_bytes+1);
            final_init_img[1087] = NEW_IMAGE_REFERANCE_BYTES[1087] ^ *(( char*)nonce_bytes+1);
            final_init_img[1091] = NEW_IMAGE_REFERANCE_BYTES[1091] ^ *(( char*)nonce_bytes+1);
            final_init_img[1095] = NEW_IMAGE_REFERANCE_BYTES[1095] ^ *(( char*)nonce_bytes+1);
            final_init_img[1099] = NEW_IMAGE_REFERANCE_BYTES[1099] ^ *(( char*)nonce_bytes+1);
            final_init_img[1103] = NEW_IMAGE_REFERANCE_BYTES[1103] ^ *(( char*)nonce_bytes+1);
            final_init_img[1107] = NEW_IMAGE_REFERANCE_BYTES[1107] ^ *(( char*)nonce_bytes+1);
            final_init_img[1111] = NEW_IMAGE_REFERANCE_BYTES[1111] ^ *(( char*)nonce_bytes+1);
            final_init_img[1115] = NEW_IMAGE_REFERANCE_BYTES[1115] ^ *(( char*)nonce_bytes+1);
            final_init_img[1119] = NEW_IMAGE_REFERANCE_BYTES[1119] ^ *(( char*)nonce_bytes+1);
            final_init_img[1123] = NEW_IMAGE_REFERANCE_BYTES[1123] ^ *(( char*)nonce_bytes+1);
            final_init_img[1127] = NEW_IMAGE_REFERANCE_BYTES[1127] ^ *(( char*)nonce_bytes+1);
            final_init_img[1131] = NEW_IMAGE_REFERANCE_BYTES[1131] ^ *(( char*)nonce_bytes+1);
            final_init_img[1135] = NEW_IMAGE_REFERANCE_BYTES[1135] ^ *(( char*)nonce_bytes+1);
            final_init_img[1139] = NEW_IMAGE_REFERANCE_BYTES[1139] ^ *(( char*)nonce_bytes+1);
            final_init_img[1143] = NEW_IMAGE_REFERANCE_BYTES[1143] ^ *(( char*)nonce_bytes+1);
            final_init_img[1147] = NEW_IMAGE_REFERANCE_BYTES[1147] ^ *(( char*)nonce_bytes+1);
            final_init_img[1151] = NEW_IMAGE_REFERANCE_BYTES[1151] ^ *(( char*)nonce_bytes+1);
            final_init_img[1155] = NEW_IMAGE_REFERANCE_BYTES[1155] ^ *(( char*)nonce_bytes+1);
            final_init_img[1159] = NEW_IMAGE_REFERANCE_BYTES[1159] ^ *(( char*)nonce_bytes+1);
            final_init_img[1163] = NEW_IMAGE_REFERANCE_BYTES[1163] ^ *(( char*)nonce_bytes+1);
            final_init_img[1167] = NEW_IMAGE_REFERANCE_BYTES[1167] ^ *(( char*)nonce_bytes+1);
            final_init_img[1171] = NEW_IMAGE_REFERANCE_BYTES[1171] ^ *(( char*)nonce_bytes+1);
            final_init_img[1175] = NEW_IMAGE_REFERANCE_BYTES[1175] ^ *(( char*)nonce_bytes+1);
            final_init_img[1179] = NEW_IMAGE_REFERANCE_BYTES[1179] ^ *(( char*)nonce_bytes+1);
            final_init_img[1183] = NEW_IMAGE_REFERANCE_BYTES[1183] ^ *(( char*)nonce_bytes+1);
            final_init_img[1187] = NEW_IMAGE_REFERANCE_BYTES[1187] ^ *(( char*)nonce_bytes+1);
            final_init_img[1191] = NEW_IMAGE_REFERANCE_BYTES[1191] ^ *(( char*)nonce_bytes+1);
            final_init_img[1195] = NEW_IMAGE_REFERANCE_BYTES[1195] ^ *(( char*)nonce_bytes+1);
            final_init_img[1199] = NEW_IMAGE_REFERANCE_BYTES[1199] ^ *(( char*)nonce_bytes+1);
            final_init_img[1203] = NEW_IMAGE_REFERANCE_BYTES[1203] ^ *(( char*)nonce_bytes+1);
            final_init_img[1207] = NEW_IMAGE_REFERANCE_BYTES[1207] ^ *(( char*)nonce_bytes+1);
            final_init_img[1211] = NEW_IMAGE_REFERANCE_BYTES[1211] ^ *(( char*)nonce_bytes+1);
            final_init_img[1215] = NEW_IMAGE_REFERANCE_BYTES[1215] ^ *(( char*)nonce_bytes+1);
            final_init_img[1219] = NEW_IMAGE_REFERANCE_BYTES[1219] ^ *(( char*)nonce_bytes+1);
            final_init_img[1223] = NEW_IMAGE_REFERANCE_BYTES[1223] ^ *(( char*)nonce_bytes+1);
            final_init_img[1227] = NEW_IMAGE_REFERANCE_BYTES[1227] ^ *(( char*)nonce_bytes+1);
            final_init_img[1231] = NEW_IMAGE_REFERANCE_BYTES[1231] ^ *(( char*)nonce_bytes+1);
            final_init_img[1235] = NEW_IMAGE_REFERANCE_BYTES[1235] ^ *(( char*)nonce_bytes+1);
            final_init_img[1239] = NEW_IMAGE_REFERANCE_BYTES[1239] ^ *(( char*)nonce_bytes+1);
            final_init_img[1243] = NEW_IMAGE_REFERANCE_BYTES[1243] ^ *(( char*)nonce_bytes+1);
            final_init_img[1247] = NEW_IMAGE_REFERANCE_BYTES[1247] ^ *(( char*)nonce_bytes+1);
            final_init_img[1251] = NEW_IMAGE_REFERANCE_BYTES[1251] ^ *(( char*)nonce_bytes+1);
            final_init_img[1255] = NEW_IMAGE_REFERANCE_BYTES[1255] ^ *(( char*)nonce_bytes+1);
            final_init_img[1259] = NEW_IMAGE_REFERANCE_BYTES[1259] ^ *(( char*)nonce_bytes+1);
            final_init_img[1263] = NEW_IMAGE_REFERANCE_BYTES[1263] ^ *(( char*)nonce_bytes+1);
            final_init_img[1267] = NEW_IMAGE_REFERANCE_BYTES[1267] ^ *(( char*)nonce_bytes+1);
            final_init_img[1271] = NEW_IMAGE_REFERANCE_BYTES[1271] ^ *(( char*)nonce_bytes+1);
            final_init_img[1275] = NEW_IMAGE_REFERANCE_BYTES[1275] ^ *(( char*)nonce_bytes+1);
            final_init_img[1279] = NEW_IMAGE_REFERANCE_BYTES[1279] ^ *(( char*)nonce_bytes+1);
            final_init_img[1283] = NEW_IMAGE_REFERANCE_BYTES[1283] ^ *(( char*)nonce_bytes+1);
            final_init_img[1287] = NEW_IMAGE_REFERANCE_BYTES[1287] ^ *(( char*)nonce_bytes+1);
            final_init_img[1291] = NEW_IMAGE_REFERANCE_BYTES[1291] ^ *(( char*)nonce_bytes+1);
            final_init_img[1295] = NEW_IMAGE_REFERANCE_BYTES[1295] ^ *(( char*)nonce_bytes+1);
            final_init_img[1299] = NEW_IMAGE_REFERANCE_BYTES[1299] ^ *(( char*)nonce_bytes+1);
            final_init_img[1303] = NEW_IMAGE_REFERANCE_BYTES[1303] ^ *(( char*)nonce_bytes+1);
            final_init_img[1307] = NEW_IMAGE_REFERANCE_BYTES[1307] ^ *(( char*)nonce_bytes+1);
            final_init_img[1311] = NEW_IMAGE_REFERANCE_BYTES[1311] ^ *(( char*)nonce_bytes+1);
            final_init_img[1315] = NEW_IMAGE_REFERANCE_BYTES[1315] ^ *(( char*)nonce_bytes+1);
            final_init_img[1319] = NEW_IMAGE_REFERANCE_BYTES[1319] ^ *(( char*)nonce_bytes+1);
            final_init_img[1323] = NEW_IMAGE_REFERANCE_BYTES[1323] ^ *(( char*)nonce_bytes+1);
            final_init_img[1327] = NEW_IMAGE_REFERANCE_BYTES[1327] ^ *(( char*)nonce_bytes+1);
            final_init_img[1331] = NEW_IMAGE_REFERANCE_BYTES[1331] ^ *(( char*)nonce_bytes+1);
            final_init_img[1335] = NEW_IMAGE_REFERANCE_BYTES[1335] ^ *(( char*)nonce_bytes+1);
            final_init_img[1339] = NEW_IMAGE_REFERANCE_BYTES[1339] ^ *(( char*)nonce_bytes+1);
            final_init_img[1343] = NEW_IMAGE_REFERANCE_BYTES[1343] ^ *(( char*)nonce_bytes+1);
            final_init_img[1347] = NEW_IMAGE_REFERANCE_BYTES[1347] ^ *(( char*)nonce_bytes+1);
            final_init_img[1351] = NEW_IMAGE_REFERANCE_BYTES[1351] ^ *(( char*)nonce_bytes+1);
            final_init_img[1355] = NEW_IMAGE_REFERANCE_BYTES[1355] ^ *(( char*)nonce_bytes+1);
            final_init_img[1359] = NEW_IMAGE_REFERANCE_BYTES[1359] ^ *(( char*)nonce_bytes+1);
            final_init_img[1363] = NEW_IMAGE_REFERANCE_BYTES[1363] ^ *(( char*)nonce_bytes+1);
            final_init_img[1367] = NEW_IMAGE_REFERANCE_BYTES[1367] ^ *(( char*)nonce_bytes+1);
            final_init_img[1371] = NEW_IMAGE_REFERANCE_BYTES[1371] ^ *(( char*)nonce_bytes+1);
            final_init_img[1375] = NEW_IMAGE_REFERANCE_BYTES[1375] ^ *(( char*)nonce_bytes+1);
            final_init_img[1379] = NEW_IMAGE_REFERANCE_BYTES[1379] ^ *(( char*)nonce_bytes+1);
            final_init_img[1383] = NEW_IMAGE_REFERANCE_BYTES[1383] ^ *(( char*)nonce_bytes+1);
            final_init_img[1387] = NEW_IMAGE_REFERANCE_BYTES[1387] ^ *(( char*)nonce_bytes+1);
            final_init_img[1391] = NEW_IMAGE_REFERANCE_BYTES[1391] ^ *(( char*)nonce_bytes+1);
            final_init_img[1395] = NEW_IMAGE_REFERANCE_BYTES[1395] ^ *(( char*)nonce_bytes+1);
            final_init_img[1399] = NEW_IMAGE_REFERANCE_BYTES[1399] ^ *(( char*)nonce_bytes+1);
            final_init_img[1403] = NEW_IMAGE_REFERANCE_BYTES[1403] ^ *(( char*)nonce_bytes+1);
            final_init_img[1407] = NEW_IMAGE_REFERANCE_BYTES[1407] ^ *(( char*)nonce_bytes+1);
            final_init_img[1411] = NEW_IMAGE_REFERANCE_BYTES[1411] ^ *(( char*)nonce_bytes+1);
            final_init_img[1415] = NEW_IMAGE_REFERANCE_BYTES[1415] ^ *(( char*)nonce_bytes+1);
            final_init_img[1419] = NEW_IMAGE_REFERANCE_BYTES[1419] ^ *(( char*)nonce_bytes+1);
            final_init_img[1423] = NEW_IMAGE_REFERANCE_BYTES[1423] ^ *(( char*)nonce_bytes+1);
            final_init_img[1427] = NEW_IMAGE_REFERANCE_BYTES[1427] ^ *(( char*)nonce_bytes+1);
            final_init_img[1431] = NEW_IMAGE_REFERANCE_BYTES[1431] ^ *(( char*)nonce_bytes+1);
            final_init_img[1435] = NEW_IMAGE_REFERANCE_BYTES[1435] ^ *(( char*)nonce_bytes+1);
            final_init_img[1439] = NEW_IMAGE_REFERANCE_BYTES[1439] ^ *(( char*)nonce_bytes+1);
            final_init_img[1443] = NEW_IMAGE_REFERANCE_BYTES[1443] ^ *(( char*)nonce_bytes+1);
            final_init_img[1447] = NEW_IMAGE_REFERANCE_BYTES[1447] ^ *(( char*)nonce_bytes+1);
            final_init_img[1451] = NEW_IMAGE_REFERANCE_BYTES[1451] ^ *(( char*)nonce_bytes+1);
            final_init_img[1455] = NEW_IMAGE_REFERANCE_BYTES[1455] ^ *(( char*)nonce_bytes+1);
            final_init_img[1459] = NEW_IMAGE_REFERANCE_BYTES[1459] ^ *(( char*)nonce_bytes+1);
            final_init_img[1463] = NEW_IMAGE_REFERANCE_BYTES[1463] ^ *(( char*)nonce_bytes+1);
            final_init_img[1467] = NEW_IMAGE_REFERANCE_BYTES[1467] ^ *(( char*)nonce_bytes+1);
            final_init_img[1471] = NEW_IMAGE_REFERANCE_BYTES[1471] ^ *(( char*)nonce_bytes+1);
            final_init_img[1475] = NEW_IMAGE_REFERANCE_BYTES[1475] ^ *(( char*)nonce_bytes+1);
            final_init_img[1479] = NEW_IMAGE_REFERANCE_BYTES[1479] ^ *(( char*)nonce_bytes+1);
            final_init_img[1483] = NEW_IMAGE_REFERANCE_BYTES[1483] ^ *(( char*)nonce_bytes+1);
            final_init_img[1487] = NEW_IMAGE_REFERANCE_BYTES[1487] ^ *(( char*)nonce_bytes+1);
            final_init_img[1491] = NEW_IMAGE_REFERANCE_BYTES[1491] ^ *(( char*)nonce_bytes+1);
            final_init_img[1495] = NEW_IMAGE_REFERANCE_BYTES[1495] ^ *(( char*)nonce_bytes+1);
            final_init_img[1499] = NEW_IMAGE_REFERANCE_BYTES[1499] ^ *(( char*)nonce_bytes+1);
            final_init_img[1503] = NEW_IMAGE_REFERANCE_BYTES[1503] ^ *(( char*)nonce_bytes+1);
            final_init_img[1507] = NEW_IMAGE_REFERANCE_BYTES[1507] ^ *(( char*)nonce_bytes+1);
            final_init_img[1511] = NEW_IMAGE_REFERANCE_BYTES[1511] ^ *(( char*)nonce_bytes+1);
            final_init_img[1515] = NEW_IMAGE_REFERANCE_BYTES[1515] ^ *(( char*)nonce_bytes+1);
            final_init_img[1519] = NEW_IMAGE_REFERANCE_BYTES[1519] ^ *(( char*)nonce_bytes+1);
            final_init_img[1523] = NEW_IMAGE_REFERANCE_BYTES[1523] ^ *(( char*)nonce_bytes+1);
            final_init_img[1527] = NEW_IMAGE_REFERANCE_BYTES[1527] ^ *(( char*)nonce_bytes+1);
            final_init_img[1531] = NEW_IMAGE_REFERANCE_BYTES[1531] ^ *(( char*)nonce_bytes+1);
            final_init_img[1535] = NEW_IMAGE_REFERANCE_BYTES[1535] ^ *(( char*)nonce_bytes+1);
            final_init_img[1539] = NEW_IMAGE_REFERANCE_BYTES[1539] ^ *(( char*)nonce_bytes+1);
            final_init_img[1543] = NEW_IMAGE_REFERANCE_BYTES[1543] ^ *(( char*)nonce_bytes+1);
            final_init_img[1547] = NEW_IMAGE_REFERANCE_BYTES[1547] ^ *(( char*)nonce_bytes+1);
            final_init_img[1551] = NEW_IMAGE_REFERANCE_BYTES[1551] ^ *(( char*)nonce_bytes+1);
            final_init_img[1555] = NEW_IMAGE_REFERANCE_BYTES[1555] ^ *(( char*)nonce_bytes+1);
            final_init_img[1559] = NEW_IMAGE_REFERANCE_BYTES[1559] ^ *(( char*)nonce_bytes+1);
            final_init_img[1563] = NEW_IMAGE_REFERANCE_BYTES[1563] ^ *(( char*)nonce_bytes+1);
            final_init_img[1567] = NEW_IMAGE_REFERANCE_BYTES[1567] ^ *(( char*)nonce_bytes+1);
            final_init_img[1571] = NEW_IMAGE_REFERANCE_BYTES[1571] ^ *(( char*)nonce_bytes+1);
            final_init_img[1575] = NEW_IMAGE_REFERANCE_BYTES[1575] ^ *(( char*)nonce_bytes+1);
            final_init_img[1579] = NEW_IMAGE_REFERANCE_BYTES[1579] ^ *(( char*)nonce_bytes+1);
            final_init_img[1583] = NEW_IMAGE_REFERANCE_BYTES[1583] ^ *(( char*)nonce_bytes+1);
            final_init_img[1587] = NEW_IMAGE_REFERANCE_BYTES[1587] ^ *(( char*)nonce_bytes+1);
            final_init_img[1591] = NEW_IMAGE_REFERANCE_BYTES[1591] ^ *(( char*)nonce_bytes+1);
            final_init_img[1595] = NEW_IMAGE_REFERANCE_BYTES[1595] ^ *(( char*)nonce_bytes+1);
            final_init_img[1599] = NEW_IMAGE_REFERANCE_BYTES[1599] ^ *(( char*)nonce_bytes+1);
            final_init_img[1603] = NEW_IMAGE_REFERANCE_BYTES[1603] ^ *(( char*)nonce_bytes+1);
            final_init_img[1607] = NEW_IMAGE_REFERANCE_BYTES[1607] ^ *(( char*)nonce_bytes+1);
            final_init_img[1611] = NEW_IMAGE_REFERANCE_BYTES[1611] ^ *(( char*)nonce_bytes+1);
            final_init_img[1615] = NEW_IMAGE_REFERANCE_BYTES[1615] ^ *(( char*)nonce_bytes+1);
            final_init_img[1619] = NEW_IMAGE_REFERANCE_BYTES[1619] ^ *(( char*)nonce_bytes+1);
            final_init_img[1623] = NEW_IMAGE_REFERANCE_BYTES[1623] ^ *(( char*)nonce_bytes+1);
            final_init_img[1627] = NEW_IMAGE_REFERANCE_BYTES[1627] ^ *(( char*)nonce_bytes+1);
            final_init_img[1631] = NEW_IMAGE_REFERANCE_BYTES[1631] ^ *(( char*)nonce_bytes+1);
            final_init_img[1635] = NEW_IMAGE_REFERANCE_BYTES[1635] ^ *(( char*)nonce_bytes+1);
            final_init_img[1639] = NEW_IMAGE_REFERANCE_BYTES[1639] ^ *(( char*)nonce_bytes+1);
            final_init_img[1643] = NEW_IMAGE_REFERANCE_BYTES[1643] ^ *(( char*)nonce_bytes+1);
            final_init_img[1647] = NEW_IMAGE_REFERANCE_BYTES[1647] ^ *(( char*)nonce_bytes+1);
            final_init_img[1651] = NEW_IMAGE_REFERANCE_BYTES[1651] ^ *(( char*)nonce_bytes+1);
            final_init_img[1655] = NEW_IMAGE_REFERANCE_BYTES[1655] ^ *(( char*)nonce_bytes+1);
            final_init_img[1659] = NEW_IMAGE_REFERANCE_BYTES[1659] ^ *(( char*)nonce_bytes+1);
            final_init_img[1663] = NEW_IMAGE_REFERANCE_BYTES[1663] ^ *(( char*)nonce_bytes+1);
            final_init_img[1667] = NEW_IMAGE_REFERANCE_BYTES[1667] ^ *(( char*)nonce_bytes+1);
            final_init_img[1671] = NEW_IMAGE_REFERANCE_BYTES[1671] ^ *(( char*)nonce_bytes+1);
            final_init_img[1675] = NEW_IMAGE_REFERANCE_BYTES[1675] ^ *(( char*)nonce_bytes+1);
            final_init_img[1679] = NEW_IMAGE_REFERANCE_BYTES[1679] ^ *(( char*)nonce_bytes+1);
            final_init_img[1683] = NEW_IMAGE_REFERANCE_BYTES[1683] ^ *(( char*)nonce_bytes+1);
            final_init_img[1687] = NEW_IMAGE_REFERANCE_BYTES[1687] ^ *(( char*)nonce_bytes+1);
            final_init_img[1691] = NEW_IMAGE_REFERANCE_BYTES[1691] ^ *(( char*)nonce_bytes+1);
            final_init_img[1695] = NEW_IMAGE_REFERANCE_BYTES[1695] ^ *(( char*)nonce_bytes+1);
            final_init_img[1699] = NEW_IMAGE_REFERANCE_BYTES[1699] ^ *(( char*)nonce_bytes+1);
            final_init_img[1703] = NEW_IMAGE_REFERANCE_BYTES[1703] ^ *(( char*)nonce_bytes+1);
            final_init_img[1707] = NEW_IMAGE_REFERANCE_BYTES[1707] ^ *(( char*)nonce_bytes+1);
            final_init_img[1711] = NEW_IMAGE_REFERANCE_BYTES[1711] ^ *(( char*)nonce_bytes+1);
            final_init_img[1715] = NEW_IMAGE_REFERANCE_BYTES[1715] ^ *(( char*)nonce_bytes+1);
            final_init_img[1719] = NEW_IMAGE_REFERANCE_BYTES[1719] ^ *(( char*)nonce_bytes+1);
            final_init_img[1723] = NEW_IMAGE_REFERANCE_BYTES[1723] ^ *(( char*)nonce_bytes+1);
            final_init_img[1727] = NEW_IMAGE_REFERANCE_BYTES[1727] ^ *(( char*)nonce_bytes+1);
            final_init_img[1731] = NEW_IMAGE_REFERANCE_BYTES[1731] ^ *(( char*)nonce_bytes+1);
            final_init_img[1735] = NEW_IMAGE_REFERANCE_BYTES[1735] ^ *(( char*)nonce_bytes+1);
            final_init_img[1739] = NEW_IMAGE_REFERANCE_BYTES[1739] ^ *(( char*)nonce_bytes+1);
            final_init_img[1743] = NEW_IMAGE_REFERANCE_BYTES[1743] ^ *(( char*)nonce_bytes+1);
            final_init_img[1747] = NEW_IMAGE_REFERANCE_BYTES[1747] ^ *(( char*)nonce_bytes+1);
            final_init_img[1751] = NEW_IMAGE_REFERANCE_BYTES[1751] ^ *(( char*)nonce_bytes+1);
            final_init_img[1755] = NEW_IMAGE_REFERANCE_BYTES[1755] ^ *(( char*)nonce_bytes+1);
            final_init_img[1759] = NEW_IMAGE_REFERANCE_BYTES[1759] ^ *(( char*)nonce_bytes+1);
            final_init_img[1763] = NEW_IMAGE_REFERANCE_BYTES[1763] ^ *(( char*)nonce_bytes+1);
            final_init_img[1767] = NEW_IMAGE_REFERANCE_BYTES[1767] ^ *(( char*)nonce_bytes+1);
            final_init_img[1771] = NEW_IMAGE_REFERANCE_BYTES[1771] ^ *(( char*)nonce_bytes+1);
            final_init_img[1775] = NEW_IMAGE_REFERANCE_BYTES[1775] ^ *(( char*)nonce_bytes+1);
            final_init_img[1779] = NEW_IMAGE_REFERANCE_BYTES[1779] ^ *(( char*)nonce_bytes+1);
		}


        if (last_nonce_bytes[2] != *(( char*)nonce_bytes+2)){
            last_nonce_bytes[2] = *(( char*)nonce_bytes+2);
            final_init_img[56] = NEW_IMAGE_REFERANCE_BYTES[56] ^ *(( char*)nonce_bytes+2);
            final_init_img[60] = NEW_IMAGE_REFERANCE_BYTES[60] ^ *(( char*)nonce_bytes+2);
            final_init_img[64] = NEW_IMAGE_REFERANCE_BYTES[64] ^ *(( char*)nonce_bytes+2);
            final_init_img[68] = NEW_IMAGE_REFERANCE_BYTES[68] ^ *(( char*)nonce_bytes+2);
            final_init_img[72] = NEW_IMAGE_REFERANCE_BYTES[72] ^ *(( char*)nonce_bytes+2);
            final_init_img[76] = NEW_IMAGE_REFERANCE_BYTES[76] ^ *(( char*)nonce_bytes+2);
            final_init_img[80] = NEW_IMAGE_REFERANCE_BYTES[80] ^ *(( char*)nonce_bytes+2);
            final_init_img[84] = NEW_IMAGE_REFERANCE_BYTES[84] ^ *(( char*)nonce_bytes+2);
            final_init_img[88] = NEW_IMAGE_REFERANCE_BYTES[88] ^ *(( char*)nonce_bytes+2);
            final_init_img[92] = NEW_IMAGE_REFERANCE_BYTES[92] ^ *(( char*)nonce_bytes+2);
            final_init_img[96] = NEW_IMAGE_REFERANCE_BYTES[96] ^ *(( char*)nonce_bytes+2);
            final_init_img[100] = NEW_IMAGE_REFERANCE_BYTES[100] ^ *(( char*)nonce_bytes+2);
            final_init_img[104] = NEW_IMAGE_REFERANCE_BYTES[104] ^ *(( char*)nonce_bytes+2);
            final_init_img[108] = NEW_IMAGE_REFERANCE_BYTES[108] ^ *(( char*)nonce_bytes+2);
            final_init_img[112] = NEW_IMAGE_REFERANCE_BYTES[112] ^ *(( char*)nonce_bytes+2);
            final_init_img[116] = NEW_IMAGE_REFERANCE_BYTES[116] ^ *(( char*)nonce_bytes+2);
            final_init_img[120] = NEW_IMAGE_REFERANCE_BYTES[120] ^ *(( char*)nonce_bytes+2);
            final_init_img[124] = NEW_IMAGE_REFERANCE_BYTES[124] ^ *(( char*)nonce_bytes+2);
            final_init_img[128] = NEW_IMAGE_REFERANCE_BYTES[128] ^ *(( char*)nonce_bytes+2);
            final_init_img[132] = NEW_IMAGE_REFERANCE_BYTES[132] ^ *(( char*)nonce_bytes+2);
            final_init_img[136] = NEW_IMAGE_REFERANCE_BYTES[136] ^ *(( char*)nonce_bytes+2);
            final_init_img[140] = NEW_IMAGE_REFERANCE_BYTES[140] ^ *(( char*)nonce_bytes+2);
            final_init_img[144] = NEW_IMAGE_REFERANCE_BYTES[144] ^ *(( char*)nonce_bytes+2);
            final_init_img[148] = NEW_IMAGE_REFERANCE_BYTES[148] ^ *(( char*)nonce_bytes+2);
            final_init_img[152] = NEW_IMAGE_REFERANCE_BYTES[152] ^ *(( char*)nonce_bytes+2);
            final_init_img[156] = NEW_IMAGE_REFERANCE_BYTES[156] ^ *(( char*)nonce_bytes+2);
            final_init_img[160] = NEW_IMAGE_REFERANCE_BYTES[160] ^ *(( char*)nonce_bytes+2);
            final_init_img[164] = NEW_IMAGE_REFERANCE_BYTES[164] ^ *(( char*)nonce_bytes+2);
            final_init_img[168] = NEW_IMAGE_REFERANCE_BYTES[168] ^ *(( char*)nonce_bytes+2);
            final_init_img[172] = NEW_IMAGE_REFERANCE_BYTES[172] ^ *(( char*)nonce_bytes+2);
            final_init_img[176] = NEW_IMAGE_REFERANCE_BYTES[176] ^ *(( char*)nonce_bytes+2);
            final_init_img[180] = NEW_IMAGE_REFERANCE_BYTES[180] ^ *(( char*)nonce_bytes+2);
            final_init_img[184] = NEW_IMAGE_REFERANCE_BYTES[184] ^ *(( char*)nonce_bytes+2);
            final_init_img[188] = NEW_IMAGE_REFERANCE_BYTES[188] ^ *(( char*)nonce_bytes+2);
            final_init_img[192] = NEW_IMAGE_REFERANCE_BYTES[192] ^ *(( char*)nonce_bytes+2);
            final_init_img[196] = NEW_IMAGE_REFERANCE_BYTES[196] ^ *(( char*)nonce_bytes+2);
            final_init_img[200] = NEW_IMAGE_REFERANCE_BYTES[200] ^ *(( char*)nonce_bytes+2);
            final_init_img[204] = NEW_IMAGE_REFERANCE_BYTES[204] ^ *(( char*)nonce_bytes+2);
            final_init_img[208] = NEW_IMAGE_REFERANCE_BYTES[208] ^ *(( char*)nonce_bytes+2);
            final_init_img[212] = NEW_IMAGE_REFERANCE_BYTES[212] ^ *(( char*)nonce_bytes+2);
            final_init_img[216] = NEW_IMAGE_REFERANCE_BYTES[216] ^ *(( char*)nonce_bytes+2);
            final_init_img[220] = NEW_IMAGE_REFERANCE_BYTES[220] ^ *(( char*)nonce_bytes+2);
            final_init_img[224] = NEW_IMAGE_REFERANCE_BYTES[224] ^ *(( char*)nonce_bytes+2);
            final_init_img[228] = NEW_IMAGE_REFERANCE_BYTES[228] ^ *(( char*)nonce_bytes+2);
            final_init_img[232] = NEW_IMAGE_REFERANCE_BYTES[232] ^ *(( char*)nonce_bytes+2);
            final_init_img[236] = NEW_IMAGE_REFERANCE_BYTES[236] ^ *(( char*)nonce_bytes+2);
            final_init_img[240] = NEW_IMAGE_REFERANCE_BYTES[240] ^ *(( char*)nonce_bytes+2);
            final_init_img[244] = NEW_IMAGE_REFERANCE_BYTES[244] ^ *(( char*)nonce_bytes+2);
            final_init_img[248] = NEW_IMAGE_REFERANCE_BYTES[248] ^ *(( char*)nonce_bytes+2);
            final_init_img[252] = NEW_IMAGE_REFERANCE_BYTES[252] ^ *(( char*)nonce_bytes+2);
            final_init_img[256] = NEW_IMAGE_REFERANCE_BYTES[256] ^ *(( char*)nonce_bytes+2);
            final_init_img[260] = NEW_IMAGE_REFERANCE_BYTES[260] ^ *(( char*)nonce_bytes+2);
            final_init_img[264] = NEW_IMAGE_REFERANCE_BYTES[264] ^ *(( char*)nonce_bytes+2);
            final_init_img[268] = NEW_IMAGE_REFERANCE_BYTES[268] ^ *(( char*)nonce_bytes+2);
            final_init_img[272] = NEW_IMAGE_REFERANCE_BYTES[272] ^ *(( char*)nonce_bytes+2);
            final_init_img[276] = NEW_IMAGE_REFERANCE_BYTES[276] ^ *(( char*)nonce_bytes+2);
            final_init_img[280] = NEW_IMAGE_REFERANCE_BYTES[280] ^ *(( char*)nonce_bytes+2);
            final_init_img[284] = NEW_IMAGE_REFERANCE_BYTES[284] ^ *(( char*)nonce_bytes+2);
            final_init_img[288] = NEW_IMAGE_REFERANCE_BYTES[288] ^ *(( char*)nonce_bytes+2);
            final_init_img[292] = NEW_IMAGE_REFERANCE_BYTES[292] ^ *(( char*)nonce_bytes+2);
            final_init_img[296] = NEW_IMAGE_REFERANCE_BYTES[296] ^ *(( char*)nonce_bytes+2);
            final_init_img[300] = NEW_IMAGE_REFERANCE_BYTES[300] ^ *(( char*)nonce_bytes+2);
            final_init_img[304] = NEW_IMAGE_REFERANCE_BYTES[304] ^ *(( char*)nonce_bytes+2);
            final_init_img[308] = NEW_IMAGE_REFERANCE_BYTES[308] ^ *(( char*)nonce_bytes+2);
            final_init_img[312] = NEW_IMAGE_REFERANCE_BYTES[312] ^ *(( char*)nonce_bytes+2);
            final_init_img[316] = NEW_IMAGE_REFERANCE_BYTES[316] ^ *(( char*)nonce_bytes+2);
            final_init_img[320] = NEW_IMAGE_REFERANCE_BYTES[320] ^ *(( char*)nonce_bytes+2);
            final_init_img[324] = NEW_IMAGE_REFERANCE_BYTES[324] ^ *(( char*)nonce_bytes+2);
            final_init_img[328] = NEW_IMAGE_REFERANCE_BYTES[328] ^ *(( char*)nonce_bytes+2);
            final_init_img[332] = NEW_IMAGE_REFERANCE_BYTES[332] ^ *(( char*)nonce_bytes+2);
            final_init_img[336] = NEW_IMAGE_REFERANCE_BYTES[336] ^ *(( char*)nonce_bytes+2);
            final_init_img[340] = NEW_IMAGE_REFERANCE_BYTES[340] ^ *(( char*)nonce_bytes+2);
            final_init_img[344] = NEW_IMAGE_REFERANCE_BYTES[344] ^ *(( char*)nonce_bytes+2);
            final_init_img[348] = NEW_IMAGE_REFERANCE_BYTES[348] ^ *(( char*)nonce_bytes+2);
            final_init_img[352] = NEW_IMAGE_REFERANCE_BYTES[352] ^ *(( char*)nonce_bytes+2);
            final_init_img[356] = NEW_IMAGE_REFERANCE_BYTES[356] ^ *(( char*)nonce_bytes+2);
            final_init_img[360] = NEW_IMAGE_REFERANCE_BYTES[360] ^ *(( char*)nonce_bytes+2);
            final_init_img[364] = NEW_IMAGE_REFERANCE_BYTES[364] ^ *(( char*)nonce_bytes+2);
            final_init_img[368] = NEW_IMAGE_REFERANCE_BYTES[368] ^ *(( char*)nonce_bytes+2);
            final_init_img[372] = NEW_IMAGE_REFERANCE_BYTES[372] ^ *(( char*)nonce_bytes+2);
            final_init_img[376] = NEW_IMAGE_REFERANCE_BYTES[376] ^ *(( char*)nonce_bytes+2);
            final_init_img[380] = NEW_IMAGE_REFERANCE_BYTES[380] ^ *(( char*)nonce_bytes+2);
            final_init_img[384] = NEW_IMAGE_REFERANCE_BYTES[384] ^ *(( char*)nonce_bytes+2);
            final_init_img[388] = NEW_IMAGE_REFERANCE_BYTES[388] ^ *(( char*)nonce_bytes+2);
            final_init_img[392] = NEW_IMAGE_REFERANCE_BYTES[392] ^ *(( char*)nonce_bytes+2);
            final_init_img[396] = NEW_IMAGE_REFERANCE_BYTES[396] ^ *(( char*)nonce_bytes+2);
            final_init_img[400] = NEW_IMAGE_REFERANCE_BYTES[400] ^ *(( char*)nonce_bytes+2);
            final_init_img[404] = NEW_IMAGE_REFERANCE_BYTES[404] ^ *(( char*)nonce_bytes+2);
            final_init_img[408] = NEW_IMAGE_REFERANCE_BYTES[408] ^ *(( char*)nonce_bytes+2);
            final_init_img[412] = NEW_IMAGE_REFERANCE_BYTES[412] ^ *(( char*)nonce_bytes+2);
            final_init_img[416] = NEW_IMAGE_REFERANCE_BYTES[416] ^ *(( char*)nonce_bytes+2);
            final_init_img[420] = NEW_IMAGE_REFERANCE_BYTES[420] ^ *(( char*)nonce_bytes+2);
            final_init_img[424] = NEW_IMAGE_REFERANCE_BYTES[424] ^ *(( char*)nonce_bytes+2);
            final_init_img[428] = NEW_IMAGE_REFERANCE_BYTES[428] ^ *(( char*)nonce_bytes+2);
            final_init_img[432] = NEW_IMAGE_REFERANCE_BYTES[432] ^ *(( char*)nonce_bytes+2);
            final_init_img[436] = NEW_IMAGE_REFERANCE_BYTES[436] ^ *(( char*)nonce_bytes+2);
            final_init_img[440] = NEW_IMAGE_REFERANCE_BYTES[440] ^ *(( char*)nonce_bytes+2);
            final_init_img[444] = NEW_IMAGE_REFERANCE_BYTES[444] ^ *(( char*)nonce_bytes+2);
            final_init_img[448] = NEW_IMAGE_REFERANCE_BYTES[448] ^ *(( char*)nonce_bytes+2);
            final_init_img[452] = NEW_IMAGE_REFERANCE_BYTES[452] ^ *(( char*)nonce_bytes+2);
            final_init_img[456] = NEW_IMAGE_REFERANCE_BYTES[456] ^ *(( char*)nonce_bytes+2);
            final_init_img[460] = NEW_IMAGE_REFERANCE_BYTES[460] ^ *(( char*)nonce_bytes+2);
            final_init_img[464] = NEW_IMAGE_REFERANCE_BYTES[464] ^ *(( char*)nonce_bytes+2);
            final_init_img[468] = NEW_IMAGE_REFERANCE_BYTES[468] ^ *(( char*)nonce_bytes+2);
            final_init_img[472] = NEW_IMAGE_REFERANCE_BYTES[472] ^ *(( char*)nonce_bytes+2);
            final_init_img[476] = NEW_IMAGE_REFERANCE_BYTES[476] ^ *(( char*)nonce_bytes+2);
            final_init_img[480] = NEW_IMAGE_REFERANCE_BYTES[480] ^ *(( char*)nonce_bytes+2);
            final_init_img[484] = NEW_IMAGE_REFERANCE_BYTES[484] ^ *(( char*)nonce_bytes+2);
            final_init_img[488] = NEW_IMAGE_REFERANCE_BYTES[488] ^ *(( char*)nonce_bytes+2);
            final_init_img[492] = NEW_IMAGE_REFERANCE_BYTES[492] ^ *(( char*)nonce_bytes+2);
            final_init_img[496] = NEW_IMAGE_REFERANCE_BYTES[496] ^ *(( char*)nonce_bytes+2);
            final_init_img[500] = NEW_IMAGE_REFERANCE_BYTES[500] ^ *(( char*)nonce_bytes+2);
            final_init_img[504] = NEW_IMAGE_REFERANCE_BYTES[504] ^ *(( char*)nonce_bytes+2);
            final_init_img[508] = NEW_IMAGE_REFERANCE_BYTES[508] ^ *(( char*)nonce_bytes+2);
            final_init_img[512] = NEW_IMAGE_REFERANCE_BYTES[512] ^ *(( char*)nonce_bytes+2);
            final_init_img[516] = NEW_IMAGE_REFERANCE_BYTES[516] ^ *(( char*)nonce_bytes+2);
            final_init_img[520] = NEW_IMAGE_REFERANCE_BYTES[520] ^ *(( char*)nonce_bytes+2);
            final_init_img[524] = NEW_IMAGE_REFERANCE_BYTES[524] ^ *(( char*)nonce_bytes+2);
            final_init_img[528] = NEW_IMAGE_REFERANCE_BYTES[528] ^ *(( char*)nonce_bytes+2);
            final_init_img[532] = NEW_IMAGE_REFERANCE_BYTES[532] ^ *(( char*)nonce_bytes+2);
            final_init_img[536] = NEW_IMAGE_REFERANCE_BYTES[536] ^ *(( char*)nonce_bytes+2);
            final_init_img[540] = NEW_IMAGE_REFERANCE_BYTES[540] ^ *(( char*)nonce_bytes+2);
            final_init_img[544] = NEW_IMAGE_REFERANCE_BYTES[544] ^ *(( char*)nonce_bytes+2);
            final_init_img[548] = NEW_IMAGE_REFERANCE_BYTES[548] ^ *(( char*)nonce_bytes+2);
            final_init_img[552] = NEW_IMAGE_REFERANCE_BYTES[552] ^ *(( char*)nonce_bytes+2);
            final_init_img[556] = NEW_IMAGE_REFERANCE_BYTES[556] ^ *(( char*)nonce_bytes+2);
            final_init_img[560] = NEW_IMAGE_REFERANCE_BYTES[560] ^ *(( char*)nonce_bytes+2);
            final_init_img[564] = NEW_IMAGE_REFERANCE_BYTES[564] ^ *(( char*)nonce_bytes+2);
            final_init_img[568] = NEW_IMAGE_REFERANCE_BYTES[568] ^ *(( char*)nonce_bytes+2);
            final_init_img[572] = NEW_IMAGE_REFERANCE_BYTES[572] ^ *(( char*)nonce_bytes+2);
            final_init_img[576] = NEW_IMAGE_REFERANCE_BYTES[576] ^ *(( char*)nonce_bytes+2);
            final_init_img[580] = NEW_IMAGE_REFERANCE_BYTES[580] ^ *(( char*)nonce_bytes+2);
            final_init_img[584] = NEW_IMAGE_REFERANCE_BYTES[584] ^ *(( char*)nonce_bytes+2);
            final_init_img[588] = NEW_IMAGE_REFERANCE_BYTES[588] ^ *(( char*)nonce_bytes+2);
            final_init_img[592] = NEW_IMAGE_REFERANCE_BYTES[592] ^ *(( char*)nonce_bytes+2);
            final_init_img[596] = NEW_IMAGE_REFERANCE_BYTES[596] ^ *(( char*)nonce_bytes+2);
            final_init_img[600] = NEW_IMAGE_REFERANCE_BYTES[600] ^ *(( char*)nonce_bytes+2);
            final_init_img[604] = NEW_IMAGE_REFERANCE_BYTES[604] ^ *(( char*)nonce_bytes+2);
            final_init_img[608] = NEW_IMAGE_REFERANCE_BYTES[608] ^ *(( char*)nonce_bytes+2);
            final_init_img[612] = NEW_IMAGE_REFERANCE_BYTES[612] ^ *(( char*)nonce_bytes+2);
            final_init_img[616] = NEW_IMAGE_REFERANCE_BYTES[616] ^ *(( char*)nonce_bytes+2);
            final_init_img[620] = NEW_IMAGE_REFERANCE_BYTES[620] ^ *(( char*)nonce_bytes+2);
            final_init_img[624] = NEW_IMAGE_REFERANCE_BYTES[624] ^ *(( char*)nonce_bytes+2);
            final_init_img[628] = NEW_IMAGE_REFERANCE_BYTES[628] ^ *(( char*)nonce_bytes+2);
            final_init_img[632] = NEW_IMAGE_REFERANCE_BYTES[632] ^ *(( char*)nonce_bytes+2);
            final_init_img[636] = NEW_IMAGE_REFERANCE_BYTES[636] ^ *(( char*)nonce_bytes+2);
            final_init_img[640] = NEW_IMAGE_REFERANCE_BYTES[640] ^ *(( char*)nonce_bytes+2);
            final_init_img[644] = NEW_IMAGE_REFERANCE_BYTES[644] ^ *(( char*)nonce_bytes+2);
            final_init_img[648] = NEW_IMAGE_REFERANCE_BYTES[648] ^ *(( char*)nonce_bytes+2);
            final_init_img[652] = NEW_IMAGE_REFERANCE_BYTES[652] ^ *(( char*)nonce_bytes+2);
            final_init_img[656] = NEW_IMAGE_REFERANCE_BYTES[656] ^ *(( char*)nonce_bytes+2);
            final_init_img[660] = NEW_IMAGE_REFERANCE_BYTES[660] ^ *(( char*)nonce_bytes+2);
            final_init_img[664] = NEW_IMAGE_REFERANCE_BYTES[664] ^ *(( char*)nonce_bytes+2);
            final_init_img[668] = NEW_IMAGE_REFERANCE_BYTES[668] ^ *(( char*)nonce_bytes+2);
            final_init_img[672] = NEW_IMAGE_REFERANCE_BYTES[672] ^ *(( char*)nonce_bytes+2);
            final_init_img[676] = NEW_IMAGE_REFERANCE_BYTES[676] ^ *(( char*)nonce_bytes+2);
            final_init_img[680] = NEW_IMAGE_REFERANCE_BYTES[680] ^ *(( char*)nonce_bytes+2);
            final_init_img[684] = NEW_IMAGE_REFERANCE_BYTES[684] ^ *(( char*)nonce_bytes+2);
            final_init_img[688] = NEW_IMAGE_REFERANCE_BYTES[688] ^ *(( char*)nonce_bytes+2);
            final_init_img[692] = NEW_IMAGE_REFERANCE_BYTES[692] ^ *(( char*)nonce_bytes+2);
            final_init_img[696] = NEW_IMAGE_REFERANCE_BYTES[696] ^ *(( char*)nonce_bytes+2);
            final_init_img[700] = NEW_IMAGE_REFERANCE_BYTES[700] ^ *(( char*)nonce_bytes+2);
            final_init_img[704] = NEW_IMAGE_REFERANCE_BYTES[704] ^ *(( char*)nonce_bytes+2);
            final_init_img[708] = NEW_IMAGE_REFERANCE_BYTES[708] ^ *(( char*)nonce_bytes+2);
            final_init_img[712] = NEW_IMAGE_REFERANCE_BYTES[712] ^ *(( char*)nonce_bytes+2);
            final_init_img[716] = NEW_IMAGE_REFERANCE_BYTES[716] ^ *(( char*)nonce_bytes+2);
            final_init_img[720] = NEW_IMAGE_REFERANCE_BYTES[720] ^ *(( char*)nonce_bytes+2);
            final_init_img[724] = NEW_IMAGE_REFERANCE_BYTES[724] ^ *(( char*)nonce_bytes+2);
            final_init_img[728] = NEW_IMAGE_REFERANCE_BYTES[728] ^ *(( char*)nonce_bytes+2);
            final_init_img[732] = NEW_IMAGE_REFERANCE_BYTES[732] ^ *(( char*)nonce_bytes+2);
            final_init_img[736] = NEW_IMAGE_REFERANCE_BYTES[736] ^ *(( char*)nonce_bytes+2);
            final_init_img[740] = NEW_IMAGE_REFERANCE_BYTES[740] ^ *(( char*)nonce_bytes+2);
            final_init_img[744] = NEW_IMAGE_REFERANCE_BYTES[744] ^ *(( char*)nonce_bytes+2);
            final_init_img[748] = NEW_IMAGE_REFERANCE_BYTES[748] ^ *(( char*)nonce_bytes+2);
            final_init_img[752] = NEW_IMAGE_REFERANCE_BYTES[752] ^ *(( char*)nonce_bytes+2);
            final_init_img[756] = NEW_IMAGE_REFERANCE_BYTES[756] ^ *(( char*)nonce_bytes+2);
            final_init_img[760] = NEW_IMAGE_REFERANCE_BYTES[760] ^ *(( char*)nonce_bytes+2);
            final_init_img[764] = NEW_IMAGE_REFERANCE_BYTES[764] ^ *(( char*)nonce_bytes+2);
            final_init_img[768] = NEW_IMAGE_REFERANCE_BYTES[768] ^ *(( char*)nonce_bytes+2);
            final_init_img[772] = NEW_IMAGE_REFERANCE_BYTES[772] ^ *(( char*)nonce_bytes+2);
            final_init_img[776] = NEW_IMAGE_REFERANCE_BYTES[776] ^ *(( char*)nonce_bytes+2);
            final_init_img[780] = NEW_IMAGE_REFERANCE_BYTES[780] ^ *(( char*)nonce_bytes+2);
            final_init_img[784] = NEW_IMAGE_REFERANCE_BYTES[784] ^ *(( char*)nonce_bytes+2);
            final_init_img[788] = NEW_IMAGE_REFERANCE_BYTES[788] ^ *(( char*)nonce_bytes+2);
            final_init_img[792] = NEW_IMAGE_REFERANCE_BYTES[792] ^ *(( char*)nonce_bytes+2);
            final_init_img[796] = NEW_IMAGE_REFERANCE_BYTES[796] ^ *(( char*)nonce_bytes+2);
            final_init_img[800] = NEW_IMAGE_REFERANCE_BYTES[800] ^ *(( char*)nonce_bytes+2);
            final_init_img[804] = NEW_IMAGE_REFERANCE_BYTES[804] ^ *(( char*)nonce_bytes+2);
            final_init_img[808] = NEW_IMAGE_REFERANCE_BYTES[808] ^ *(( char*)nonce_bytes+2);
            final_init_img[812] = NEW_IMAGE_REFERANCE_BYTES[812] ^ *(( char*)nonce_bytes+2);
            final_init_img[816] = NEW_IMAGE_REFERANCE_BYTES[816] ^ *(( char*)nonce_bytes+2);
            final_init_img[820] = NEW_IMAGE_REFERANCE_BYTES[820] ^ *(( char*)nonce_bytes+2);
            final_init_img[824] = NEW_IMAGE_REFERANCE_BYTES[824] ^ *(( char*)nonce_bytes+2);
            final_init_img[828] = NEW_IMAGE_REFERANCE_BYTES[828] ^ *(( char*)nonce_bytes+2);
            final_init_img[832] = NEW_IMAGE_REFERANCE_BYTES[832] ^ *(( char*)nonce_bytes+2);
            final_init_img[836] = NEW_IMAGE_REFERANCE_BYTES[836] ^ *(( char*)nonce_bytes+2);
            final_init_img[840] = NEW_IMAGE_REFERANCE_BYTES[840] ^ *(( char*)nonce_bytes+2);
            final_init_img[844] = NEW_IMAGE_REFERANCE_BYTES[844] ^ *(( char*)nonce_bytes+2);
            final_init_img[848] = NEW_IMAGE_REFERANCE_BYTES[848] ^ *(( char*)nonce_bytes+2);
            final_init_img[852] = NEW_IMAGE_REFERANCE_BYTES[852] ^ *(( char*)nonce_bytes+2);
            final_init_img[856] = NEW_IMAGE_REFERANCE_BYTES[856] ^ *(( char*)nonce_bytes+2);
            final_init_img[860] = NEW_IMAGE_REFERANCE_BYTES[860] ^ *(( char*)nonce_bytes+2);
            final_init_img[864] = NEW_IMAGE_REFERANCE_BYTES[864] ^ *(( char*)nonce_bytes+2);
            final_init_img[868] = NEW_IMAGE_REFERANCE_BYTES[868] ^ *(( char*)nonce_bytes+2);
            final_init_img[872] = NEW_IMAGE_REFERANCE_BYTES[872] ^ *(( char*)nonce_bytes+2);
            final_init_img[876] = NEW_IMAGE_REFERANCE_BYTES[876] ^ *(( char*)nonce_bytes+2);
            final_init_img[880] = NEW_IMAGE_REFERANCE_BYTES[880] ^ *(( char*)nonce_bytes+2);
            final_init_img[884] = NEW_IMAGE_REFERANCE_BYTES[884] ^ *(( char*)nonce_bytes+2);
            final_init_img[888] = NEW_IMAGE_REFERANCE_BYTES[888] ^ *(( char*)nonce_bytes+2);
            final_init_img[892] = NEW_IMAGE_REFERANCE_BYTES[892] ^ *(( char*)nonce_bytes+2);
            final_init_img[896] = NEW_IMAGE_REFERANCE_BYTES[896] ^ *(( char*)nonce_bytes+2);
            final_init_img[900] = NEW_IMAGE_REFERANCE_BYTES[900] ^ *(( char*)nonce_bytes+2);
            final_init_img[904] = NEW_IMAGE_REFERANCE_BYTES[904] ^ *(( char*)nonce_bytes+2);
            final_init_img[908] = NEW_IMAGE_REFERANCE_BYTES[908] ^ *(( char*)nonce_bytes+2);
            final_init_img[912] = NEW_IMAGE_REFERANCE_BYTES[912] ^ *(( char*)nonce_bytes+2);
            final_init_img[916] = NEW_IMAGE_REFERANCE_BYTES[916] ^ *(( char*)nonce_bytes+2);
            final_init_img[920] = NEW_IMAGE_REFERANCE_BYTES[920] ^ *(( char*)nonce_bytes+2);
            final_init_img[924] = NEW_IMAGE_REFERANCE_BYTES[924] ^ *(( char*)nonce_bytes+2);
            final_init_img[928] = NEW_IMAGE_REFERANCE_BYTES[928] ^ *(( char*)nonce_bytes+2);
            final_init_img[932] = NEW_IMAGE_REFERANCE_BYTES[932] ^ *(( char*)nonce_bytes+2);
            final_init_img[936] = NEW_IMAGE_REFERANCE_BYTES[936] ^ *(( char*)nonce_bytes+2);
            final_init_img[940] = NEW_IMAGE_REFERANCE_BYTES[940] ^ *(( char*)nonce_bytes+2);
            final_init_img[944] = NEW_IMAGE_REFERANCE_BYTES[944] ^ *(( char*)nonce_bytes+2);
            final_init_img[948] = NEW_IMAGE_REFERANCE_BYTES[948] ^ *(( char*)nonce_bytes+2);
            final_init_img[952] = NEW_IMAGE_REFERANCE_BYTES[952] ^ *(( char*)nonce_bytes+2);
            final_init_img[956] = NEW_IMAGE_REFERANCE_BYTES[956] ^ *(( char*)nonce_bytes+2);
            final_init_img[960] = NEW_IMAGE_REFERANCE_BYTES[960] ^ *(( char*)nonce_bytes+2);
            final_init_img[964] = NEW_IMAGE_REFERANCE_BYTES[964] ^ *(( char*)nonce_bytes+2);
            final_init_img[968] = NEW_IMAGE_REFERANCE_BYTES[968] ^ *(( char*)nonce_bytes+2);
            final_init_img[972] = NEW_IMAGE_REFERANCE_BYTES[972] ^ *(( char*)nonce_bytes+2);
            final_init_img[976] = NEW_IMAGE_REFERANCE_BYTES[976] ^ *(( char*)nonce_bytes+2);
            final_init_img[980] = NEW_IMAGE_REFERANCE_BYTES[980] ^ *(( char*)nonce_bytes+2);
            final_init_img[984] = NEW_IMAGE_REFERANCE_BYTES[984] ^ *(( char*)nonce_bytes+2);
            final_init_img[988] = NEW_IMAGE_REFERANCE_BYTES[988] ^ *(( char*)nonce_bytes+2);
            final_init_img[992] = NEW_IMAGE_REFERANCE_BYTES[992] ^ *(( char*)nonce_bytes+2);
            final_init_img[996] = NEW_IMAGE_REFERANCE_BYTES[996] ^ *(( char*)nonce_bytes+2);
            final_init_img[1000] = NEW_IMAGE_REFERANCE_BYTES[1000] ^ *(( char*)nonce_bytes+2);
            final_init_img[1004] = NEW_IMAGE_REFERANCE_BYTES[1004] ^ *(( char*)nonce_bytes+2);
            final_init_img[1008] = NEW_IMAGE_REFERANCE_BYTES[1008] ^ *(( char*)nonce_bytes+2);
            final_init_img[1012] = NEW_IMAGE_REFERANCE_BYTES[1012] ^ *(( char*)nonce_bytes+2);
            final_init_img[1016] = NEW_IMAGE_REFERANCE_BYTES[1016] ^ *(( char*)nonce_bytes+2);
            final_init_img[1020] = NEW_IMAGE_REFERANCE_BYTES[1020] ^ *(( char*)nonce_bytes+2);
            final_init_img[1024] = NEW_IMAGE_REFERANCE_BYTES[1024] ^ *(( char*)nonce_bytes+2);
            final_init_img[1028] = NEW_IMAGE_REFERANCE_BYTES[1028] ^ *(( char*)nonce_bytes+2);
            final_init_img[1032] = NEW_IMAGE_REFERANCE_BYTES[1032] ^ *(( char*)nonce_bytes+2);
            final_init_img[1036] = NEW_IMAGE_REFERANCE_BYTES[1036] ^ *(( char*)nonce_bytes+2);
            final_init_img[1040] = NEW_IMAGE_REFERANCE_BYTES[1040] ^ *(( char*)nonce_bytes+2);
            final_init_img[1044] = NEW_IMAGE_REFERANCE_BYTES[1044] ^ *(( char*)nonce_bytes+2);
            final_init_img[1048] = NEW_IMAGE_REFERANCE_BYTES[1048] ^ *(( char*)nonce_bytes+2);
            final_init_img[1052] = NEW_IMAGE_REFERANCE_BYTES[1052] ^ *(( char*)nonce_bytes+2);
            final_init_img[1056] = NEW_IMAGE_REFERANCE_BYTES[1056] ^ *(( char*)nonce_bytes+2);
            final_init_img[1060] = NEW_IMAGE_REFERANCE_BYTES[1060] ^ *(( char*)nonce_bytes+2);
            final_init_img[1064] = NEW_IMAGE_REFERANCE_BYTES[1064] ^ *(( char*)nonce_bytes+2);
            final_init_img[1068] = NEW_IMAGE_REFERANCE_BYTES[1068] ^ *(( char*)nonce_bytes+2);
            final_init_img[1072] = NEW_IMAGE_REFERANCE_BYTES[1072] ^ *(( char*)nonce_bytes+2);
            final_init_img[1076] = NEW_IMAGE_REFERANCE_BYTES[1076] ^ *(( char*)nonce_bytes+2);
            final_init_img[1080] = NEW_IMAGE_REFERANCE_BYTES[1080] ^ *(( char*)nonce_bytes+2);
            final_init_img[1084] = NEW_IMAGE_REFERANCE_BYTES[1084] ^ *(( char*)nonce_bytes+2);
            final_init_img[1088] = NEW_IMAGE_REFERANCE_BYTES[1088] ^ *(( char*)nonce_bytes+2);
            final_init_img[1092] = NEW_IMAGE_REFERANCE_BYTES[1092] ^ *(( char*)nonce_bytes+2);
            final_init_img[1096] = NEW_IMAGE_REFERANCE_BYTES[1096] ^ *(( char*)nonce_bytes+2);
            final_init_img[1100] = NEW_IMAGE_REFERANCE_BYTES[1100] ^ *(( char*)nonce_bytes+2);
            final_init_img[1104] = NEW_IMAGE_REFERANCE_BYTES[1104] ^ *(( char*)nonce_bytes+2);
            final_init_img[1108] = NEW_IMAGE_REFERANCE_BYTES[1108] ^ *(( char*)nonce_bytes+2);
            final_init_img[1112] = NEW_IMAGE_REFERANCE_BYTES[1112] ^ *(( char*)nonce_bytes+2);
            final_init_img[1116] = NEW_IMAGE_REFERANCE_BYTES[1116] ^ *(( char*)nonce_bytes+2);
            final_init_img[1120] = NEW_IMAGE_REFERANCE_BYTES[1120] ^ *(( char*)nonce_bytes+2);
            final_init_img[1124] = NEW_IMAGE_REFERANCE_BYTES[1124] ^ *(( char*)nonce_bytes+2);
            final_init_img[1128] = NEW_IMAGE_REFERANCE_BYTES[1128] ^ *(( char*)nonce_bytes+2);
            final_init_img[1132] = NEW_IMAGE_REFERANCE_BYTES[1132] ^ *(( char*)nonce_bytes+2);
            final_init_img[1136] = NEW_IMAGE_REFERANCE_BYTES[1136] ^ *(( char*)nonce_bytes+2);
            final_init_img[1140] = NEW_IMAGE_REFERANCE_BYTES[1140] ^ *(( char*)nonce_bytes+2);
            final_init_img[1144] = NEW_IMAGE_REFERANCE_BYTES[1144] ^ *(( char*)nonce_bytes+2);
            final_init_img[1148] = NEW_IMAGE_REFERANCE_BYTES[1148] ^ *(( char*)nonce_bytes+2);
            final_init_img[1152] = NEW_IMAGE_REFERANCE_BYTES[1152] ^ *(( char*)nonce_bytes+2);
            final_init_img[1156] = NEW_IMAGE_REFERANCE_BYTES[1156] ^ *(( char*)nonce_bytes+2);
            final_init_img[1160] = NEW_IMAGE_REFERANCE_BYTES[1160] ^ *(( char*)nonce_bytes+2);
            final_init_img[1164] = NEW_IMAGE_REFERANCE_BYTES[1164] ^ *(( char*)nonce_bytes+2);
            final_init_img[1168] = NEW_IMAGE_REFERANCE_BYTES[1168] ^ *(( char*)nonce_bytes+2);
            final_init_img[1172] = NEW_IMAGE_REFERANCE_BYTES[1172] ^ *(( char*)nonce_bytes+2);
            final_init_img[1176] = NEW_IMAGE_REFERANCE_BYTES[1176] ^ *(( char*)nonce_bytes+2);
            final_init_img[1180] = NEW_IMAGE_REFERANCE_BYTES[1180] ^ *(( char*)nonce_bytes+2);
            final_init_img[1184] = NEW_IMAGE_REFERANCE_BYTES[1184] ^ *(( char*)nonce_bytes+2);
            final_init_img[1188] = NEW_IMAGE_REFERANCE_BYTES[1188] ^ *(( char*)nonce_bytes+2);
            final_init_img[1192] = NEW_IMAGE_REFERANCE_BYTES[1192] ^ *(( char*)nonce_bytes+2);
            final_init_img[1196] = NEW_IMAGE_REFERANCE_BYTES[1196] ^ *(( char*)nonce_bytes+2);
            final_init_img[1200] = NEW_IMAGE_REFERANCE_BYTES[1200] ^ *(( char*)nonce_bytes+2);
            final_init_img[1204] = NEW_IMAGE_REFERANCE_BYTES[1204] ^ *(( char*)nonce_bytes+2);
            final_init_img[1208] = NEW_IMAGE_REFERANCE_BYTES[1208] ^ *(( char*)nonce_bytes+2);
            final_init_img[1212] = NEW_IMAGE_REFERANCE_BYTES[1212] ^ *(( char*)nonce_bytes+2);
            final_init_img[1216] = NEW_IMAGE_REFERANCE_BYTES[1216] ^ *(( char*)nonce_bytes+2);
            final_init_img[1220] = NEW_IMAGE_REFERANCE_BYTES[1220] ^ *(( char*)nonce_bytes+2);
            final_init_img[1224] = NEW_IMAGE_REFERANCE_BYTES[1224] ^ *(( char*)nonce_bytes+2);
            final_init_img[1228] = NEW_IMAGE_REFERANCE_BYTES[1228] ^ *(( char*)nonce_bytes+2);
            final_init_img[1232] = NEW_IMAGE_REFERANCE_BYTES[1232] ^ *(( char*)nonce_bytes+2);
            final_init_img[1236] = NEW_IMAGE_REFERANCE_BYTES[1236] ^ *(( char*)nonce_bytes+2);
            final_init_img[1240] = NEW_IMAGE_REFERANCE_BYTES[1240] ^ *(( char*)nonce_bytes+2);
            final_init_img[1244] = NEW_IMAGE_REFERANCE_BYTES[1244] ^ *(( char*)nonce_bytes+2);
            final_init_img[1248] = NEW_IMAGE_REFERANCE_BYTES[1248] ^ *(( char*)nonce_bytes+2);
            final_init_img[1252] = NEW_IMAGE_REFERANCE_BYTES[1252] ^ *(( char*)nonce_bytes+2);
            final_init_img[1256] = NEW_IMAGE_REFERANCE_BYTES[1256] ^ *(( char*)nonce_bytes+2);
            final_init_img[1260] = NEW_IMAGE_REFERANCE_BYTES[1260] ^ *(( char*)nonce_bytes+2);
            final_init_img[1264] = NEW_IMAGE_REFERANCE_BYTES[1264] ^ *(( char*)nonce_bytes+2);
            final_init_img[1268] = NEW_IMAGE_REFERANCE_BYTES[1268] ^ *(( char*)nonce_bytes+2);
            final_init_img[1272] = NEW_IMAGE_REFERANCE_BYTES[1272] ^ *(( char*)nonce_bytes+2);
            final_init_img[1276] = NEW_IMAGE_REFERANCE_BYTES[1276] ^ *(( char*)nonce_bytes+2);
            final_init_img[1280] = NEW_IMAGE_REFERANCE_BYTES[1280] ^ *(( char*)nonce_bytes+2);
            final_init_img[1284] = NEW_IMAGE_REFERANCE_BYTES[1284] ^ *(( char*)nonce_bytes+2);
            final_init_img[1288] = NEW_IMAGE_REFERANCE_BYTES[1288] ^ *(( char*)nonce_bytes+2);
            final_init_img[1292] = NEW_IMAGE_REFERANCE_BYTES[1292] ^ *(( char*)nonce_bytes+2);
            final_init_img[1296] = NEW_IMAGE_REFERANCE_BYTES[1296] ^ *(( char*)nonce_bytes+2);
            final_init_img[1300] = NEW_IMAGE_REFERANCE_BYTES[1300] ^ *(( char*)nonce_bytes+2);
            final_init_img[1304] = NEW_IMAGE_REFERANCE_BYTES[1304] ^ *(( char*)nonce_bytes+2);
            final_init_img[1308] = NEW_IMAGE_REFERANCE_BYTES[1308] ^ *(( char*)nonce_bytes+2);
            final_init_img[1312] = NEW_IMAGE_REFERANCE_BYTES[1312] ^ *(( char*)nonce_bytes+2);
            final_init_img[1316] = NEW_IMAGE_REFERANCE_BYTES[1316] ^ *(( char*)nonce_bytes+2);
            final_init_img[1320] = NEW_IMAGE_REFERANCE_BYTES[1320] ^ *(( char*)nonce_bytes+2);
            final_init_img[1324] = NEW_IMAGE_REFERANCE_BYTES[1324] ^ *(( char*)nonce_bytes+2);
            final_init_img[1328] = NEW_IMAGE_REFERANCE_BYTES[1328] ^ *(( char*)nonce_bytes+2);
            final_init_img[1332] = NEW_IMAGE_REFERANCE_BYTES[1332] ^ *(( char*)nonce_bytes+2);
            final_init_img[1336] = NEW_IMAGE_REFERANCE_BYTES[1336] ^ *(( char*)nonce_bytes+2);
            final_init_img[1340] = NEW_IMAGE_REFERANCE_BYTES[1340] ^ *(( char*)nonce_bytes+2);
            final_init_img[1344] = NEW_IMAGE_REFERANCE_BYTES[1344] ^ *(( char*)nonce_bytes+2);
            final_init_img[1348] = NEW_IMAGE_REFERANCE_BYTES[1348] ^ *(( char*)nonce_bytes+2);
            final_init_img[1352] = NEW_IMAGE_REFERANCE_BYTES[1352] ^ *(( char*)nonce_bytes+2);
            final_init_img[1356] = NEW_IMAGE_REFERANCE_BYTES[1356] ^ *(( char*)nonce_bytes+2);
            final_init_img[1360] = NEW_IMAGE_REFERANCE_BYTES[1360] ^ *(( char*)nonce_bytes+2);
            final_init_img[1364] = NEW_IMAGE_REFERANCE_BYTES[1364] ^ *(( char*)nonce_bytes+2);
            final_init_img[1368] = NEW_IMAGE_REFERANCE_BYTES[1368] ^ *(( char*)nonce_bytes+2);
            final_init_img[1372] = NEW_IMAGE_REFERANCE_BYTES[1372] ^ *(( char*)nonce_bytes+2);
            final_init_img[1376] = NEW_IMAGE_REFERANCE_BYTES[1376] ^ *(( char*)nonce_bytes+2);
            final_init_img[1380] = NEW_IMAGE_REFERANCE_BYTES[1380] ^ *(( char*)nonce_bytes+2);
            final_init_img[1384] = NEW_IMAGE_REFERANCE_BYTES[1384] ^ *(( char*)nonce_bytes+2);
            final_init_img[1388] = NEW_IMAGE_REFERANCE_BYTES[1388] ^ *(( char*)nonce_bytes+2);
            final_init_img[1392] = NEW_IMAGE_REFERANCE_BYTES[1392] ^ *(( char*)nonce_bytes+2);
            final_init_img[1396] = NEW_IMAGE_REFERANCE_BYTES[1396] ^ *(( char*)nonce_bytes+2);
            final_init_img[1400] = NEW_IMAGE_REFERANCE_BYTES[1400] ^ *(( char*)nonce_bytes+2);
            final_init_img[1404] = NEW_IMAGE_REFERANCE_BYTES[1404] ^ *(( char*)nonce_bytes+2);
            final_init_img[1408] = NEW_IMAGE_REFERANCE_BYTES[1408] ^ *(( char*)nonce_bytes+2);
            final_init_img[1412] = NEW_IMAGE_REFERANCE_BYTES[1412] ^ *(( char*)nonce_bytes+2);
            final_init_img[1416] = NEW_IMAGE_REFERANCE_BYTES[1416] ^ *(( char*)nonce_bytes+2);
            final_init_img[1420] = NEW_IMAGE_REFERANCE_BYTES[1420] ^ *(( char*)nonce_bytes+2);
            final_init_img[1424] = NEW_IMAGE_REFERANCE_BYTES[1424] ^ *(( char*)nonce_bytes+2);
            final_init_img[1428] = NEW_IMAGE_REFERANCE_BYTES[1428] ^ *(( char*)nonce_bytes+2);
            final_init_img[1432] = NEW_IMAGE_REFERANCE_BYTES[1432] ^ *(( char*)nonce_bytes+2);
            final_init_img[1436] = NEW_IMAGE_REFERANCE_BYTES[1436] ^ *(( char*)nonce_bytes+2);
            final_init_img[1440] = NEW_IMAGE_REFERANCE_BYTES[1440] ^ *(( char*)nonce_bytes+2);
            final_init_img[1444] = NEW_IMAGE_REFERANCE_BYTES[1444] ^ *(( char*)nonce_bytes+2);
            final_init_img[1448] = NEW_IMAGE_REFERANCE_BYTES[1448] ^ *(( char*)nonce_bytes+2);
            final_init_img[1452] = NEW_IMAGE_REFERANCE_BYTES[1452] ^ *(( char*)nonce_bytes+2);
            final_init_img[1456] = NEW_IMAGE_REFERANCE_BYTES[1456] ^ *(( char*)nonce_bytes+2);
            final_init_img[1460] = NEW_IMAGE_REFERANCE_BYTES[1460] ^ *(( char*)nonce_bytes+2);
            final_init_img[1464] = NEW_IMAGE_REFERANCE_BYTES[1464] ^ *(( char*)nonce_bytes+2);
            final_init_img[1468] = NEW_IMAGE_REFERANCE_BYTES[1468] ^ *(( char*)nonce_bytes+2);
            final_init_img[1472] = NEW_IMAGE_REFERANCE_BYTES[1472] ^ *(( char*)nonce_bytes+2);
            final_init_img[1476] = NEW_IMAGE_REFERANCE_BYTES[1476] ^ *(( char*)nonce_bytes+2);
            final_init_img[1480] = NEW_IMAGE_REFERANCE_BYTES[1480] ^ *(( char*)nonce_bytes+2);
            final_init_img[1484] = NEW_IMAGE_REFERANCE_BYTES[1484] ^ *(( char*)nonce_bytes+2);
            final_init_img[1488] = NEW_IMAGE_REFERANCE_BYTES[1488] ^ *(( char*)nonce_bytes+2);
            final_init_img[1492] = NEW_IMAGE_REFERANCE_BYTES[1492] ^ *(( char*)nonce_bytes+2);
            final_init_img[1496] = NEW_IMAGE_REFERANCE_BYTES[1496] ^ *(( char*)nonce_bytes+2);
            final_init_img[1500] = NEW_IMAGE_REFERANCE_BYTES[1500] ^ *(( char*)nonce_bytes+2);
            final_init_img[1504] = NEW_IMAGE_REFERANCE_BYTES[1504] ^ *(( char*)nonce_bytes+2);
            final_init_img[1508] = NEW_IMAGE_REFERANCE_BYTES[1508] ^ *(( char*)nonce_bytes+2);
            final_init_img[1512] = NEW_IMAGE_REFERANCE_BYTES[1512] ^ *(( char*)nonce_bytes+2);
            final_init_img[1516] = NEW_IMAGE_REFERANCE_BYTES[1516] ^ *(( char*)nonce_bytes+2);
            final_init_img[1520] = NEW_IMAGE_REFERANCE_BYTES[1520] ^ *(( char*)nonce_bytes+2);
            final_init_img[1524] = NEW_IMAGE_REFERANCE_BYTES[1524] ^ *(( char*)nonce_bytes+2);
            final_init_img[1528] = NEW_IMAGE_REFERANCE_BYTES[1528] ^ *(( char*)nonce_bytes+2);
            final_init_img[1532] = NEW_IMAGE_REFERANCE_BYTES[1532] ^ *(( char*)nonce_bytes+2);
            final_init_img[1536] = NEW_IMAGE_REFERANCE_BYTES[1536] ^ *(( char*)nonce_bytes+2);
            final_init_img[1540] = NEW_IMAGE_REFERANCE_BYTES[1540] ^ *(( char*)nonce_bytes+2);
            final_init_img[1544] = NEW_IMAGE_REFERANCE_BYTES[1544] ^ *(( char*)nonce_bytes+2);
            final_init_img[1548] = NEW_IMAGE_REFERANCE_BYTES[1548] ^ *(( char*)nonce_bytes+2);
            final_init_img[1552] = NEW_IMAGE_REFERANCE_BYTES[1552] ^ *(( char*)nonce_bytes+2);
            final_init_img[1556] = NEW_IMAGE_REFERANCE_BYTES[1556] ^ *(( char*)nonce_bytes+2);
            final_init_img[1560] = NEW_IMAGE_REFERANCE_BYTES[1560] ^ *(( char*)nonce_bytes+2);
            final_init_img[1564] = NEW_IMAGE_REFERANCE_BYTES[1564] ^ *(( char*)nonce_bytes+2);
            final_init_img[1568] = NEW_IMAGE_REFERANCE_BYTES[1568] ^ *(( char*)nonce_bytes+2);
            final_init_img[1572] = NEW_IMAGE_REFERANCE_BYTES[1572] ^ *(( char*)nonce_bytes+2);
            final_init_img[1576] = NEW_IMAGE_REFERANCE_BYTES[1576] ^ *(( char*)nonce_bytes+2);
            final_init_img[1580] = NEW_IMAGE_REFERANCE_BYTES[1580] ^ *(( char*)nonce_bytes+2);
            final_init_img[1584] = NEW_IMAGE_REFERANCE_BYTES[1584] ^ *(( char*)nonce_bytes+2);
            final_init_img[1588] = NEW_IMAGE_REFERANCE_BYTES[1588] ^ *(( char*)nonce_bytes+2);
            final_init_img[1592] = NEW_IMAGE_REFERANCE_BYTES[1592] ^ *(( char*)nonce_bytes+2);
            final_init_img[1596] = NEW_IMAGE_REFERANCE_BYTES[1596] ^ *(( char*)nonce_bytes+2);
            final_init_img[1600] = NEW_IMAGE_REFERANCE_BYTES[1600] ^ *(( char*)nonce_bytes+2);
            final_init_img[1604] = NEW_IMAGE_REFERANCE_BYTES[1604] ^ *(( char*)nonce_bytes+2);
            final_init_img[1608] = NEW_IMAGE_REFERANCE_BYTES[1608] ^ *(( char*)nonce_bytes+2);
            final_init_img[1612] = NEW_IMAGE_REFERANCE_BYTES[1612] ^ *(( char*)nonce_bytes+2);
            final_init_img[1616] = NEW_IMAGE_REFERANCE_BYTES[1616] ^ *(( char*)nonce_bytes+2);
            final_init_img[1620] = NEW_IMAGE_REFERANCE_BYTES[1620] ^ *(( char*)nonce_bytes+2);
            final_init_img[1624] = NEW_IMAGE_REFERANCE_BYTES[1624] ^ *(( char*)nonce_bytes+2);
            final_init_img[1628] = NEW_IMAGE_REFERANCE_BYTES[1628] ^ *(( char*)nonce_bytes+2);
            final_init_img[1632] = NEW_IMAGE_REFERANCE_BYTES[1632] ^ *(( char*)nonce_bytes+2);
            final_init_img[1636] = NEW_IMAGE_REFERANCE_BYTES[1636] ^ *(( char*)nonce_bytes+2);
            final_init_img[1640] = NEW_IMAGE_REFERANCE_BYTES[1640] ^ *(( char*)nonce_bytes+2);
            final_init_img[1644] = NEW_IMAGE_REFERANCE_BYTES[1644] ^ *(( char*)nonce_bytes+2);
            final_init_img[1648] = NEW_IMAGE_REFERANCE_BYTES[1648] ^ *(( char*)nonce_bytes+2);
            final_init_img[1652] = NEW_IMAGE_REFERANCE_BYTES[1652] ^ *(( char*)nonce_bytes+2);
            final_init_img[1656] = NEW_IMAGE_REFERANCE_BYTES[1656] ^ *(( char*)nonce_bytes+2);
            final_init_img[1660] = NEW_IMAGE_REFERANCE_BYTES[1660] ^ *(( char*)nonce_bytes+2);
            final_init_img[1664] = NEW_IMAGE_REFERANCE_BYTES[1664] ^ *(( char*)nonce_bytes+2);
            final_init_img[1668] = NEW_IMAGE_REFERANCE_BYTES[1668] ^ *(( char*)nonce_bytes+2);
            final_init_img[1672] = NEW_IMAGE_REFERANCE_BYTES[1672] ^ *(( char*)nonce_bytes+2);
            final_init_img[1676] = NEW_IMAGE_REFERANCE_BYTES[1676] ^ *(( char*)nonce_bytes+2);
            final_init_img[1680] = NEW_IMAGE_REFERANCE_BYTES[1680] ^ *(( char*)nonce_bytes+2);
            final_init_img[1684] = NEW_IMAGE_REFERANCE_BYTES[1684] ^ *(( char*)nonce_bytes+2);
            final_init_img[1688] = NEW_IMAGE_REFERANCE_BYTES[1688] ^ *(( char*)nonce_bytes+2);
            final_init_img[1692] = NEW_IMAGE_REFERANCE_BYTES[1692] ^ *(( char*)nonce_bytes+2);
            final_init_img[1696] = NEW_IMAGE_REFERANCE_BYTES[1696] ^ *(( char*)nonce_bytes+2);
            final_init_img[1700] = NEW_IMAGE_REFERANCE_BYTES[1700] ^ *(( char*)nonce_bytes+2);
            final_init_img[1704] = NEW_IMAGE_REFERANCE_BYTES[1704] ^ *(( char*)nonce_bytes+2);
            final_init_img[1708] = NEW_IMAGE_REFERANCE_BYTES[1708] ^ *(( char*)nonce_bytes+2);
            final_init_img[1712] = NEW_IMAGE_REFERANCE_BYTES[1712] ^ *(( char*)nonce_bytes+2);
            final_init_img[1716] = NEW_IMAGE_REFERANCE_BYTES[1716] ^ *(( char*)nonce_bytes+2);
            final_init_img[1720] = NEW_IMAGE_REFERANCE_BYTES[1720] ^ *(( char*)nonce_bytes+2);
            final_init_img[1724] = NEW_IMAGE_REFERANCE_BYTES[1724] ^ *(( char*)nonce_bytes+2);
            final_init_img[1728] = NEW_IMAGE_REFERANCE_BYTES[1728] ^ *(( char*)nonce_bytes+2);
            final_init_img[1732] = NEW_IMAGE_REFERANCE_BYTES[1732] ^ *(( char*)nonce_bytes+2);
            final_init_img[1736] = NEW_IMAGE_REFERANCE_BYTES[1736] ^ *(( char*)nonce_bytes+2);
            final_init_img[1740] = NEW_IMAGE_REFERANCE_BYTES[1740] ^ *(( char*)nonce_bytes+2);
            final_init_img[1744] = NEW_IMAGE_REFERANCE_BYTES[1744] ^ *(( char*)nonce_bytes+2);
            final_init_img[1748] = NEW_IMAGE_REFERANCE_BYTES[1748] ^ *(( char*)nonce_bytes+2);
            final_init_img[1752] = NEW_IMAGE_REFERANCE_BYTES[1752] ^ *(( char*)nonce_bytes+2);
            final_init_img[1756] = NEW_IMAGE_REFERANCE_BYTES[1756] ^ *(( char*)nonce_bytes+2);
            final_init_img[1760] = NEW_IMAGE_REFERANCE_BYTES[1760] ^ *(( char*)nonce_bytes+2);
            final_init_img[1764] = NEW_IMAGE_REFERANCE_BYTES[1764] ^ *(( char*)nonce_bytes+2);
            final_init_img[1768] = NEW_IMAGE_REFERANCE_BYTES[1768] ^ *(( char*)nonce_bytes+2);
            final_init_img[1772] = NEW_IMAGE_REFERANCE_BYTES[1772] ^ *(( char*)nonce_bytes+2);
            final_init_img[1776] = NEW_IMAGE_REFERANCE_BYTES[1776] ^ *(( char*)nonce_bytes+2);
            final_init_img[1780] = NEW_IMAGE_REFERANCE_BYTES[1780] ^ *(( char*)nonce_bytes+2);
		}


        if (last_nonce_bytes[3] != *(( char*)nonce_bytes+3)){
            last_nonce_bytes[3] = *(( char*)nonce_bytes+3);
            final_init_img[57] = NEW_IMAGE_REFERANCE_BYTES[57] ^ *(( char*)nonce_bytes+3);
            final_init_img[61] = NEW_IMAGE_REFERANCE_BYTES[61] ^ *(( char*)nonce_bytes+3);
            final_init_img[65] = NEW_IMAGE_REFERANCE_BYTES[65] ^ *(( char*)nonce_bytes+3);
            final_init_img[69] = NEW_IMAGE_REFERANCE_BYTES[69] ^ *(( char*)nonce_bytes+3);
            final_init_img[73] = NEW_IMAGE_REFERANCE_BYTES[73] ^ *(( char*)nonce_bytes+3);
            final_init_img[77] = NEW_IMAGE_REFERANCE_BYTES[77] ^ *(( char*)nonce_bytes+3);
            final_init_img[81] = NEW_IMAGE_REFERANCE_BYTES[81] ^ *(( char*)nonce_bytes+3);
            final_init_img[85] = NEW_IMAGE_REFERANCE_BYTES[85] ^ *(( char*)nonce_bytes+3);
            final_init_img[89] = NEW_IMAGE_REFERANCE_BYTES[89] ^ *(( char*)nonce_bytes+3);
            final_init_img[93] = NEW_IMAGE_REFERANCE_BYTES[93] ^ *(( char*)nonce_bytes+3);
            final_init_img[97] = NEW_IMAGE_REFERANCE_BYTES[97] ^ *(( char*)nonce_bytes+3);
            final_init_img[101] = NEW_IMAGE_REFERANCE_BYTES[101] ^ *(( char*)nonce_bytes+3);
            final_init_img[105] = NEW_IMAGE_REFERANCE_BYTES[105] ^ *(( char*)nonce_bytes+3);
            final_init_img[109] = NEW_IMAGE_REFERANCE_BYTES[109] ^ *(( char*)nonce_bytes+3);
            final_init_img[113] = NEW_IMAGE_REFERANCE_BYTES[113] ^ *(( char*)nonce_bytes+3);
            final_init_img[117] = NEW_IMAGE_REFERANCE_BYTES[117] ^ *(( char*)nonce_bytes+3);
            final_init_img[121] = NEW_IMAGE_REFERANCE_BYTES[121] ^ *(( char*)nonce_bytes+3);
            final_init_img[125] = NEW_IMAGE_REFERANCE_BYTES[125] ^ *(( char*)nonce_bytes+3);
            final_init_img[129] = NEW_IMAGE_REFERANCE_BYTES[129] ^ *(( char*)nonce_bytes+3);
            final_init_img[133] = NEW_IMAGE_REFERANCE_BYTES[133] ^ *(( char*)nonce_bytes+3);
            final_init_img[137] = NEW_IMAGE_REFERANCE_BYTES[137] ^ *(( char*)nonce_bytes+3);
            final_init_img[141] = NEW_IMAGE_REFERANCE_BYTES[141] ^ *(( char*)nonce_bytes+3);
            final_init_img[145] = NEW_IMAGE_REFERANCE_BYTES[145] ^ *(( char*)nonce_bytes+3);
            final_init_img[149] = NEW_IMAGE_REFERANCE_BYTES[149] ^ *(( char*)nonce_bytes+3);
            final_init_img[153] = NEW_IMAGE_REFERANCE_BYTES[153] ^ *(( char*)nonce_bytes+3);
            final_init_img[157] = NEW_IMAGE_REFERANCE_BYTES[157] ^ *(( char*)nonce_bytes+3);
            final_init_img[161] = NEW_IMAGE_REFERANCE_BYTES[161] ^ *(( char*)nonce_bytes+3);
            final_init_img[165] = NEW_IMAGE_REFERANCE_BYTES[165] ^ *(( char*)nonce_bytes+3);
            final_init_img[169] = NEW_IMAGE_REFERANCE_BYTES[169] ^ *(( char*)nonce_bytes+3);
            final_init_img[173] = NEW_IMAGE_REFERANCE_BYTES[173] ^ *(( char*)nonce_bytes+3);
            final_init_img[177] = NEW_IMAGE_REFERANCE_BYTES[177] ^ *(( char*)nonce_bytes+3);
            final_init_img[181] = NEW_IMAGE_REFERANCE_BYTES[181] ^ *(( char*)nonce_bytes+3);
            final_init_img[185] = NEW_IMAGE_REFERANCE_BYTES[185] ^ *(( char*)nonce_bytes+3);
            final_init_img[189] = NEW_IMAGE_REFERANCE_BYTES[189] ^ *(( char*)nonce_bytes+3);
            final_init_img[193] = NEW_IMAGE_REFERANCE_BYTES[193] ^ *(( char*)nonce_bytes+3);
            final_init_img[197] = NEW_IMAGE_REFERANCE_BYTES[197] ^ *(( char*)nonce_bytes+3);
            final_init_img[201] = NEW_IMAGE_REFERANCE_BYTES[201] ^ *(( char*)nonce_bytes+3);
            final_init_img[205] = NEW_IMAGE_REFERANCE_BYTES[205] ^ *(( char*)nonce_bytes+3);
            final_init_img[209] = NEW_IMAGE_REFERANCE_BYTES[209] ^ *(( char*)nonce_bytes+3);
            final_init_img[213] = NEW_IMAGE_REFERANCE_BYTES[213] ^ *(( char*)nonce_bytes+3);
            final_init_img[217] = NEW_IMAGE_REFERANCE_BYTES[217] ^ *(( char*)nonce_bytes+3);
            final_init_img[221] = NEW_IMAGE_REFERANCE_BYTES[221] ^ *(( char*)nonce_bytes+3);
            final_init_img[225] = NEW_IMAGE_REFERANCE_BYTES[225] ^ *(( char*)nonce_bytes+3);
            final_init_img[229] = NEW_IMAGE_REFERANCE_BYTES[229] ^ *(( char*)nonce_bytes+3);
            final_init_img[233] = NEW_IMAGE_REFERANCE_BYTES[233] ^ *(( char*)nonce_bytes+3);
            final_init_img[237] = NEW_IMAGE_REFERANCE_BYTES[237] ^ *(( char*)nonce_bytes+3);
            final_init_img[241] = NEW_IMAGE_REFERANCE_BYTES[241] ^ *(( char*)nonce_bytes+3);
            final_init_img[245] = NEW_IMAGE_REFERANCE_BYTES[245] ^ *(( char*)nonce_bytes+3);
            final_init_img[249] = NEW_IMAGE_REFERANCE_BYTES[249] ^ *(( char*)nonce_bytes+3);
            final_init_img[253] = NEW_IMAGE_REFERANCE_BYTES[253] ^ *(( char*)nonce_bytes+3);
            final_init_img[257] = NEW_IMAGE_REFERANCE_BYTES[257] ^ *(( char*)nonce_bytes+3);
            final_init_img[261] = NEW_IMAGE_REFERANCE_BYTES[261] ^ *(( char*)nonce_bytes+3);
            final_init_img[265] = NEW_IMAGE_REFERANCE_BYTES[265] ^ *(( char*)nonce_bytes+3);
            final_init_img[269] = NEW_IMAGE_REFERANCE_BYTES[269] ^ *(( char*)nonce_bytes+3);
            final_init_img[273] = NEW_IMAGE_REFERANCE_BYTES[273] ^ *(( char*)nonce_bytes+3);
            final_init_img[277] = NEW_IMAGE_REFERANCE_BYTES[277] ^ *(( char*)nonce_bytes+3);
            final_init_img[281] = NEW_IMAGE_REFERANCE_BYTES[281] ^ *(( char*)nonce_bytes+3);
            final_init_img[285] = NEW_IMAGE_REFERANCE_BYTES[285] ^ *(( char*)nonce_bytes+3);
            final_init_img[289] = NEW_IMAGE_REFERANCE_BYTES[289] ^ *(( char*)nonce_bytes+3);
            final_init_img[293] = NEW_IMAGE_REFERANCE_BYTES[293] ^ *(( char*)nonce_bytes+3);
            final_init_img[297] = NEW_IMAGE_REFERANCE_BYTES[297] ^ *(( char*)nonce_bytes+3);
            final_init_img[301] = NEW_IMAGE_REFERANCE_BYTES[301] ^ *(( char*)nonce_bytes+3);
            final_init_img[305] = NEW_IMAGE_REFERANCE_BYTES[305] ^ *(( char*)nonce_bytes+3);
            final_init_img[309] = NEW_IMAGE_REFERANCE_BYTES[309] ^ *(( char*)nonce_bytes+3);
            final_init_img[313] = NEW_IMAGE_REFERANCE_BYTES[313] ^ *(( char*)nonce_bytes+3);
            final_init_img[317] = NEW_IMAGE_REFERANCE_BYTES[317] ^ *(( char*)nonce_bytes+3);
            final_init_img[321] = NEW_IMAGE_REFERANCE_BYTES[321] ^ *(( char*)nonce_bytes+3);
            final_init_img[325] = NEW_IMAGE_REFERANCE_BYTES[325] ^ *(( char*)nonce_bytes+3);
            final_init_img[329] = NEW_IMAGE_REFERANCE_BYTES[329] ^ *(( char*)nonce_bytes+3);
            final_init_img[333] = NEW_IMAGE_REFERANCE_BYTES[333] ^ *(( char*)nonce_bytes+3);
            final_init_img[337] = NEW_IMAGE_REFERANCE_BYTES[337] ^ *(( char*)nonce_bytes+3);
            final_init_img[341] = NEW_IMAGE_REFERANCE_BYTES[341] ^ *(( char*)nonce_bytes+3);
            final_init_img[345] = NEW_IMAGE_REFERANCE_BYTES[345] ^ *(( char*)nonce_bytes+3);
            final_init_img[349] = NEW_IMAGE_REFERANCE_BYTES[349] ^ *(( char*)nonce_bytes+3);
            final_init_img[353] = NEW_IMAGE_REFERANCE_BYTES[353] ^ *(( char*)nonce_bytes+3);
            final_init_img[357] = NEW_IMAGE_REFERANCE_BYTES[357] ^ *(( char*)nonce_bytes+3);
            final_init_img[361] = NEW_IMAGE_REFERANCE_BYTES[361] ^ *(( char*)nonce_bytes+3);
            final_init_img[365] = NEW_IMAGE_REFERANCE_BYTES[365] ^ *(( char*)nonce_bytes+3);
            final_init_img[369] = NEW_IMAGE_REFERANCE_BYTES[369] ^ *(( char*)nonce_bytes+3);
            final_init_img[373] = NEW_IMAGE_REFERANCE_BYTES[373] ^ *(( char*)nonce_bytes+3);
            final_init_img[377] = NEW_IMAGE_REFERANCE_BYTES[377] ^ *(( char*)nonce_bytes+3);
            final_init_img[381] = NEW_IMAGE_REFERANCE_BYTES[381] ^ *(( char*)nonce_bytes+3);
            final_init_img[385] = NEW_IMAGE_REFERANCE_BYTES[385] ^ *(( char*)nonce_bytes+3);
            final_init_img[389] = NEW_IMAGE_REFERANCE_BYTES[389] ^ *(( char*)nonce_bytes+3);
            final_init_img[393] = NEW_IMAGE_REFERANCE_BYTES[393] ^ *(( char*)nonce_bytes+3);
            final_init_img[397] = NEW_IMAGE_REFERANCE_BYTES[397] ^ *(( char*)nonce_bytes+3);
            final_init_img[401] = NEW_IMAGE_REFERANCE_BYTES[401] ^ *(( char*)nonce_bytes+3);
            final_init_img[405] = NEW_IMAGE_REFERANCE_BYTES[405] ^ *(( char*)nonce_bytes+3);
            final_init_img[409] = NEW_IMAGE_REFERANCE_BYTES[409] ^ *(( char*)nonce_bytes+3);
            final_init_img[413] = NEW_IMAGE_REFERANCE_BYTES[413] ^ *(( char*)nonce_bytes+3);
            final_init_img[417] = NEW_IMAGE_REFERANCE_BYTES[417] ^ *(( char*)nonce_bytes+3);
            final_init_img[421] = NEW_IMAGE_REFERANCE_BYTES[421] ^ *(( char*)nonce_bytes+3);
            final_init_img[425] = NEW_IMAGE_REFERANCE_BYTES[425] ^ *(( char*)nonce_bytes+3);
            final_init_img[429] = NEW_IMAGE_REFERANCE_BYTES[429] ^ *(( char*)nonce_bytes+3);
            final_init_img[433] = NEW_IMAGE_REFERANCE_BYTES[433] ^ *(( char*)nonce_bytes+3);
            final_init_img[437] = NEW_IMAGE_REFERANCE_BYTES[437] ^ *(( char*)nonce_bytes+3);
            final_init_img[441] = NEW_IMAGE_REFERANCE_BYTES[441] ^ *(( char*)nonce_bytes+3);
            final_init_img[445] = NEW_IMAGE_REFERANCE_BYTES[445] ^ *(( char*)nonce_bytes+3);
            final_init_img[449] = NEW_IMAGE_REFERANCE_BYTES[449] ^ *(( char*)nonce_bytes+3);
            final_init_img[453] = NEW_IMAGE_REFERANCE_BYTES[453] ^ *(( char*)nonce_bytes+3);
            final_init_img[457] = NEW_IMAGE_REFERANCE_BYTES[457] ^ *(( char*)nonce_bytes+3);
            final_init_img[461] = NEW_IMAGE_REFERANCE_BYTES[461] ^ *(( char*)nonce_bytes+3);
            final_init_img[465] = NEW_IMAGE_REFERANCE_BYTES[465] ^ *(( char*)nonce_bytes+3);
            final_init_img[469] = NEW_IMAGE_REFERANCE_BYTES[469] ^ *(( char*)nonce_bytes+3);
            final_init_img[473] = NEW_IMAGE_REFERANCE_BYTES[473] ^ *(( char*)nonce_bytes+3);
            final_init_img[477] = NEW_IMAGE_REFERANCE_BYTES[477] ^ *(( char*)nonce_bytes+3);
            final_init_img[481] = NEW_IMAGE_REFERANCE_BYTES[481] ^ *(( char*)nonce_bytes+3);
            final_init_img[485] = NEW_IMAGE_REFERANCE_BYTES[485] ^ *(( char*)nonce_bytes+3);
            final_init_img[489] = NEW_IMAGE_REFERANCE_BYTES[489] ^ *(( char*)nonce_bytes+3);
            final_init_img[493] = NEW_IMAGE_REFERANCE_BYTES[493] ^ *(( char*)nonce_bytes+3);
            final_init_img[497] = NEW_IMAGE_REFERANCE_BYTES[497] ^ *(( char*)nonce_bytes+3);
            final_init_img[501] = NEW_IMAGE_REFERANCE_BYTES[501] ^ *(( char*)nonce_bytes+3);
            final_init_img[505] = NEW_IMAGE_REFERANCE_BYTES[505] ^ *(( char*)nonce_bytes+3);
            final_init_img[509] = NEW_IMAGE_REFERANCE_BYTES[509] ^ *(( char*)nonce_bytes+3);
            final_init_img[513] = NEW_IMAGE_REFERANCE_BYTES[513] ^ *(( char*)nonce_bytes+3);
            final_init_img[517] = NEW_IMAGE_REFERANCE_BYTES[517] ^ *(( char*)nonce_bytes+3);
            final_init_img[521] = NEW_IMAGE_REFERANCE_BYTES[521] ^ *(( char*)nonce_bytes+3);
            final_init_img[525] = NEW_IMAGE_REFERANCE_BYTES[525] ^ *(( char*)nonce_bytes+3);
            final_init_img[529] = NEW_IMAGE_REFERANCE_BYTES[529] ^ *(( char*)nonce_bytes+3);
            final_init_img[533] = NEW_IMAGE_REFERANCE_BYTES[533] ^ *(( char*)nonce_bytes+3);
            final_init_img[537] = NEW_IMAGE_REFERANCE_BYTES[537] ^ *(( char*)nonce_bytes+3);
            final_init_img[541] = NEW_IMAGE_REFERANCE_BYTES[541] ^ *(( char*)nonce_bytes+3);
            final_init_img[545] = NEW_IMAGE_REFERANCE_BYTES[545] ^ *(( char*)nonce_bytes+3);
            final_init_img[549] = NEW_IMAGE_REFERANCE_BYTES[549] ^ *(( char*)nonce_bytes+3);
            final_init_img[553] = NEW_IMAGE_REFERANCE_BYTES[553] ^ *(( char*)nonce_bytes+3);
            final_init_img[557] = NEW_IMAGE_REFERANCE_BYTES[557] ^ *(( char*)nonce_bytes+3);
            final_init_img[561] = NEW_IMAGE_REFERANCE_BYTES[561] ^ *(( char*)nonce_bytes+3);
            final_init_img[565] = NEW_IMAGE_REFERANCE_BYTES[565] ^ *(( char*)nonce_bytes+3);
            final_init_img[569] = NEW_IMAGE_REFERANCE_BYTES[569] ^ *(( char*)nonce_bytes+3);
            final_init_img[573] = NEW_IMAGE_REFERANCE_BYTES[573] ^ *(( char*)nonce_bytes+3);
            final_init_img[577] = NEW_IMAGE_REFERANCE_BYTES[577] ^ *(( char*)nonce_bytes+3);
            final_init_img[581] = NEW_IMAGE_REFERANCE_BYTES[581] ^ *(( char*)nonce_bytes+3);
            final_init_img[585] = NEW_IMAGE_REFERANCE_BYTES[585] ^ *(( char*)nonce_bytes+3);
            final_init_img[589] = NEW_IMAGE_REFERANCE_BYTES[589] ^ *(( char*)nonce_bytes+3);
            final_init_img[593] = NEW_IMAGE_REFERANCE_BYTES[593] ^ *(( char*)nonce_bytes+3);
            final_init_img[597] = NEW_IMAGE_REFERANCE_BYTES[597] ^ *(( char*)nonce_bytes+3);
            final_init_img[601] = NEW_IMAGE_REFERANCE_BYTES[601] ^ *(( char*)nonce_bytes+3);
            final_init_img[605] = NEW_IMAGE_REFERANCE_BYTES[605] ^ *(( char*)nonce_bytes+3);
            final_init_img[609] = NEW_IMAGE_REFERANCE_BYTES[609] ^ *(( char*)nonce_bytes+3);
            final_init_img[613] = NEW_IMAGE_REFERANCE_BYTES[613] ^ *(( char*)nonce_bytes+3);
            final_init_img[617] = NEW_IMAGE_REFERANCE_BYTES[617] ^ *(( char*)nonce_bytes+3);
            final_init_img[621] = NEW_IMAGE_REFERANCE_BYTES[621] ^ *(( char*)nonce_bytes+3);
            final_init_img[625] = NEW_IMAGE_REFERANCE_BYTES[625] ^ *(( char*)nonce_bytes+3);
            final_init_img[629] = NEW_IMAGE_REFERANCE_BYTES[629] ^ *(( char*)nonce_bytes+3);
            final_init_img[633] = NEW_IMAGE_REFERANCE_BYTES[633] ^ *(( char*)nonce_bytes+3);
            final_init_img[637] = NEW_IMAGE_REFERANCE_BYTES[637] ^ *(( char*)nonce_bytes+3);
            final_init_img[641] = NEW_IMAGE_REFERANCE_BYTES[641] ^ *(( char*)nonce_bytes+3);
            final_init_img[645] = NEW_IMAGE_REFERANCE_BYTES[645] ^ *(( char*)nonce_bytes+3);
            final_init_img[649] = NEW_IMAGE_REFERANCE_BYTES[649] ^ *(( char*)nonce_bytes+3);
            final_init_img[653] = NEW_IMAGE_REFERANCE_BYTES[653] ^ *(( char*)nonce_bytes+3);
            final_init_img[657] = NEW_IMAGE_REFERANCE_BYTES[657] ^ *(( char*)nonce_bytes+3);
            final_init_img[661] = NEW_IMAGE_REFERANCE_BYTES[661] ^ *(( char*)nonce_bytes+3);
            final_init_img[665] = NEW_IMAGE_REFERANCE_BYTES[665] ^ *(( char*)nonce_bytes+3);
            final_init_img[669] = NEW_IMAGE_REFERANCE_BYTES[669] ^ *(( char*)nonce_bytes+3);
            final_init_img[673] = NEW_IMAGE_REFERANCE_BYTES[673] ^ *(( char*)nonce_bytes+3);
            final_init_img[677] = NEW_IMAGE_REFERANCE_BYTES[677] ^ *(( char*)nonce_bytes+3);
            final_init_img[681] = NEW_IMAGE_REFERANCE_BYTES[681] ^ *(( char*)nonce_bytes+3);
            final_init_img[685] = NEW_IMAGE_REFERANCE_BYTES[685] ^ *(( char*)nonce_bytes+3);
            final_init_img[689] = NEW_IMAGE_REFERANCE_BYTES[689] ^ *(( char*)nonce_bytes+3);
            final_init_img[693] = NEW_IMAGE_REFERANCE_BYTES[693] ^ *(( char*)nonce_bytes+3);
            final_init_img[697] = NEW_IMAGE_REFERANCE_BYTES[697] ^ *(( char*)nonce_bytes+3);
            final_init_img[701] = NEW_IMAGE_REFERANCE_BYTES[701] ^ *(( char*)nonce_bytes+3);
            final_init_img[705] = NEW_IMAGE_REFERANCE_BYTES[705] ^ *(( char*)nonce_bytes+3);
            final_init_img[709] = NEW_IMAGE_REFERANCE_BYTES[709] ^ *(( char*)nonce_bytes+3);
            final_init_img[713] = NEW_IMAGE_REFERANCE_BYTES[713] ^ *(( char*)nonce_bytes+3);
            final_init_img[717] = NEW_IMAGE_REFERANCE_BYTES[717] ^ *(( char*)nonce_bytes+3);
            final_init_img[721] = NEW_IMAGE_REFERANCE_BYTES[721] ^ *(( char*)nonce_bytes+3);
            final_init_img[725] = NEW_IMAGE_REFERANCE_BYTES[725] ^ *(( char*)nonce_bytes+3);
            final_init_img[729] = NEW_IMAGE_REFERANCE_BYTES[729] ^ *(( char*)nonce_bytes+3);
            final_init_img[733] = NEW_IMAGE_REFERANCE_BYTES[733] ^ *(( char*)nonce_bytes+3);
            final_init_img[737] = NEW_IMAGE_REFERANCE_BYTES[737] ^ *(( char*)nonce_bytes+3);
            final_init_img[741] = NEW_IMAGE_REFERANCE_BYTES[741] ^ *(( char*)nonce_bytes+3);
            final_init_img[745] = NEW_IMAGE_REFERANCE_BYTES[745] ^ *(( char*)nonce_bytes+3);
            final_init_img[749] = NEW_IMAGE_REFERANCE_BYTES[749] ^ *(( char*)nonce_bytes+3);
            final_init_img[753] = NEW_IMAGE_REFERANCE_BYTES[753] ^ *(( char*)nonce_bytes+3);
            final_init_img[757] = NEW_IMAGE_REFERANCE_BYTES[757] ^ *(( char*)nonce_bytes+3);
            final_init_img[761] = NEW_IMAGE_REFERANCE_BYTES[761] ^ *(( char*)nonce_bytes+3);
            final_init_img[765] = NEW_IMAGE_REFERANCE_BYTES[765] ^ *(( char*)nonce_bytes+3);
            final_init_img[769] = NEW_IMAGE_REFERANCE_BYTES[769] ^ *(( char*)nonce_bytes+3);
            final_init_img[773] = NEW_IMAGE_REFERANCE_BYTES[773] ^ *(( char*)nonce_bytes+3);
            final_init_img[777] = NEW_IMAGE_REFERANCE_BYTES[777] ^ *(( char*)nonce_bytes+3);
            final_init_img[781] = NEW_IMAGE_REFERANCE_BYTES[781] ^ *(( char*)nonce_bytes+3);
            final_init_img[785] = NEW_IMAGE_REFERANCE_BYTES[785] ^ *(( char*)nonce_bytes+3);
            final_init_img[789] = NEW_IMAGE_REFERANCE_BYTES[789] ^ *(( char*)nonce_bytes+3);
            final_init_img[793] = NEW_IMAGE_REFERANCE_BYTES[793] ^ *(( char*)nonce_bytes+3);
            final_init_img[797] = NEW_IMAGE_REFERANCE_BYTES[797] ^ *(( char*)nonce_bytes+3);
            final_init_img[801] = NEW_IMAGE_REFERANCE_BYTES[801] ^ *(( char*)nonce_bytes+3);
            final_init_img[805] = NEW_IMAGE_REFERANCE_BYTES[805] ^ *(( char*)nonce_bytes+3);
            final_init_img[809] = NEW_IMAGE_REFERANCE_BYTES[809] ^ *(( char*)nonce_bytes+3);
            final_init_img[813] = NEW_IMAGE_REFERANCE_BYTES[813] ^ *(( char*)nonce_bytes+3);
            final_init_img[817] = NEW_IMAGE_REFERANCE_BYTES[817] ^ *(( char*)nonce_bytes+3);
            final_init_img[821] = NEW_IMAGE_REFERANCE_BYTES[821] ^ *(( char*)nonce_bytes+3);
            final_init_img[825] = NEW_IMAGE_REFERANCE_BYTES[825] ^ *(( char*)nonce_bytes+3);
            final_init_img[829] = NEW_IMAGE_REFERANCE_BYTES[829] ^ *(( char*)nonce_bytes+3);
            final_init_img[833] = NEW_IMAGE_REFERANCE_BYTES[833] ^ *(( char*)nonce_bytes+3);
            final_init_img[837] = NEW_IMAGE_REFERANCE_BYTES[837] ^ *(( char*)nonce_bytes+3);
            final_init_img[841] = NEW_IMAGE_REFERANCE_BYTES[841] ^ *(( char*)nonce_bytes+3);
            final_init_img[845] = NEW_IMAGE_REFERANCE_BYTES[845] ^ *(( char*)nonce_bytes+3);
            final_init_img[849] = NEW_IMAGE_REFERANCE_BYTES[849] ^ *(( char*)nonce_bytes+3);
            final_init_img[853] = NEW_IMAGE_REFERANCE_BYTES[853] ^ *(( char*)nonce_bytes+3);
            final_init_img[857] = NEW_IMAGE_REFERANCE_BYTES[857] ^ *(( char*)nonce_bytes+3);
            final_init_img[861] = NEW_IMAGE_REFERANCE_BYTES[861] ^ *(( char*)nonce_bytes+3);
            final_init_img[865] = NEW_IMAGE_REFERANCE_BYTES[865] ^ *(( char*)nonce_bytes+3);
            final_init_img[869] = NEW_IMAGE_REFERANCE_BYTES[869] ^ *(( char*)nonce_bytes+3);
            final_init_img[873] = NEW_IMAGE_REFERANCE_BYTES[873] ^ *(( char*)nonce_bytes+3);
            final_init_img[877] = NEW_IMAGE_REFERANCE_BYTES[877] ^ *(( char*)nonce_bytes+3);
            final_init_img[881] = NEW_IMAGE_REFERANCE_BYTES[881] ^ *(( char*)nonce_bytes+3);
            final_init_img[885] = NEW_IMAGE_REFERANCE_BYTES[885] ^ *(( char*)nonce_bytes+3);
            final_init_img[889] = NEW_IMAGE_REFERANCE_BYTES[889] ^ *(( char*)nonce_bytes+3);
            final_init_img[893] = NEW_IMAGE_REFERANCE_BYTES[893] ^ *(( char*)nonce_bytes+3);
            final_init_img[897] = NEW_IMAGE_REFERANCE_BYTES[897] ^ *(( char*)nonce_bytes+3);
            final_init_img[901] = NEW_IMAGE_REFERANCE_BYTES[901] ^ *(( char*)nonce_bytes+3);
            final_init_img[905] = NEW_IMAGE_REFERANCE_BYTES[905] ^ *(( char*)nonce_bytes+3);
            final_init_img[909] = NEW_IMAGE_REFERANCE_BYTES[909] ^ *(( char*)nonce_bytes+3);
            final_init_img[913] = NEW_IMAGE_REFERANCE_BYTES[913] ^ *(( char*)nonce_bytes+3);
            final_init_img[917] = NEW_IMAGE_REFERANCE_BYTES[917] ^ *(( char*)nonce_bytes+3);
            final_init_img[921] = NEW_IMAGE_REFERANCE_BYTES[921] ^ *(( char*)nonce_bytes+3);
            final_init_img[925] = NEW_IMAGE_REFERANCE_BYTES[925] ^ *(( char*)nonce_bytes+3);
            final_init_img[929] = NEW_IMAGE_REFERANCE_BYTES[929] ^ *(( char*)nonce_bytes+3);
            final_init_img[933] = NEW_IMAGE_REFERANCE_BYTES[933] ^ *(( char*)nonce_bytes+3);
            final_init_img[937] = NEW_IMAGE_REFERANCE_BYTES[937] ^ *(( char*)nonce_bytes+3);
            final_init_img[941] = NEW_IMAGE_REFERANCE_BYTES[941] ^ *(( char*)nonce_bytes+3);
            final_init_img[945] = NEW_IMAGE_REFERANCE_BYTES[945] ^ *(( char*)nonce_bytes+3);
            final_init_img[949] = NEW_IMAGE_REFERANCE_BYTES[949] ^ *(( char*)nonce_bytes+3);
            final_init_img[953] = NEW_IMAGE_REFERANCE_BYTES[953] ^ *(( char*)nonce_bytes+3);
            final_init_img[957] = NEW_IMAGE_REFERANCE_BYTES[957] ^ *(( char*)nonce_bytes+3);
            final_init_img[961] = NEW_IMAGE_REFERANCE_BYTES[961] ^ *(( char*)nonce_bytes+3);
            final_init_img[965] = NEW_IMAGE_REFERANCE_BYTES[965] ^ *(( char*)nonce_bytes+3);
            final_init_img[969] = NEW_IMAGE_REFERANCE_BYTES[969] ^ *(( char*)nonce_bytes+3);
            final_init_img[973] = NEW_IMAGE_REFERANCE_BYTES[973] ^ *(( char*)nonce_bytes+3);
            final_init_img[977] = NEW_IMAGE_REFERANCE_BYTES[977] ^ *(( char*)nonce_bytes+3);
            final_init_img[981] = NEW_IMAGE_REFERANCE_BYTES[981] ^ *(( char*)nonce_bytes+3);
            final_init_img[985] = NEW_IMAGE_REFERANCE_BYTES[985] ^ *(( char*)nonce_bytes+3);
            final_init_img[989] = NEW_IMAGE_REFERANCE_BYTES[989] ^ *(( char*)nonce_bytes+3);
            final_init_img[993] = NEW_IMAGE_REFERANCE_BYTES[993] ^ *(( char*)nonce_bytes+3);
            final_init_img[997] = NEW_IMAGE_REFERANCE_BYTES[997] ^ *(( char*)nonce_bytes+3);
            final_init_img[1001] = NEW_IMAGE_REFERANCE_BYTES[1001] ^ *(( char*)nonce_bytes+3);
            final_init_img[1005] = NEW_IMAGE_REFERANCE_BYTES[1005] ^ *(( char*)nonce_bytes+3);
            final_init_img[1009] = NEW_IMAGE_REFERANCE_BYTES[1009] ^ *(( char*)nonce_bytes+3);
            final_init_img[1013] = NEW_IMAGE_REFERANCE_BYTES[1013] ^ *(( char*)nonce_bytes+3);
            final_init_img[1017] = NEW_IMAGE_REFERANCE_BYTES[1017] ^ *(( char*)nonce_bytes+3);
            final_init_img[1021] = NEW_IMAGE_REFERANCE_BYTES[1021] ^ *(( char*)nonce_bytes+3);
            final_init_img[1025] = NEW_IMAGE_REFERANCE_BYTES[1025] ^ *(( char*)nonce_bytes+3);
            final_init_img[1029] = NEW_IMAGE_REFERANCE_BYTES[1029] ^ *(( char*)nonce_bytes+3);
            final_init_img[1033] = NEW_IMAGE_REFERANCE_BYTES[1033] ^ *(( char*)nonce_bytes+3);
            final_init_img[1037] = NEW_IMAGE_REFERANCE_BYTES[1037] ^ *(( char*)nonce_bytes+3);
            final_init_img[1041] = NEW_IMAGE_REFERANCE_BYTES[1041] ^ *(( char*)nonce_bytes+3);
            final_init_img[1045] = NEW_IMAGE_REFERANCE_BYTES[1045] ^ *(( char*)nonce_bytes+3);
            final_init_img[1049] = NEW_IMAGE_REFERANCE_BYTES[1049] ^ *(( char*)nonce_bytes+3);
            final_init_img[1053] = NEW_IMAGE_REFERANCE_BYTES[1053] ^ *(( char*)nonce_bytes+3);
            final_init_img[1057] = NEW_IMAGE_REFERANCE_BYTES[1057] ^ *(( char*)nonce_bytes+3);
            final_init_img[1061] = NEW_IMAGE_REFERANCE_BYTES[1061] ^ *(( char*)nonce_bytes+3);
            final_init_img[1065] = NEW_IMAGE_REFERANCE_BYTES[1065] ^ *(( char*)nonce_bytes+3);
            final_init_img[1069] = NEW_IMAGE_REFERANCE_BYTES[1069] ^ *(( char*)nonce_bytes+3);
            final_init_img[1073] = NEW_IMAGE_REFERANCE_BYTES[1073] ^ *(( char*)nonce_bytes+3);
            final_init_img[1077] = NEW_IMAGE_REFERANCE_BYTES[1077] ^ *(( char*)nonce_bytes+3);
            final_init_img[1081] = NEW_IMAGE_REFERANCE_BYTES[1081] ^ *(( char*)nonce_bytes+3);
            final_init_img[1085] = NEW_IMAGE_REFERANCE_BYTES[1085] ^ *(( char*)nonce_bytes+3);
            final_init_img[1089] = NEW_IMAGE_REFERANCE_BYTES[1089] ^ *(( char*)nonce_bytes+3);
            final_init_img[1093] = NEW_IMAGE_REFERANCE_BYTES[1093] ^ *(( char*)nonce_bytes+3);
            final_init_img[1097] = NEW_IMAGE_REFERANCE_BYTES[1097] ^ *(( char*)nonce_bytes+3);
            final_init_img[1101] = NEW_IMAGE_REFERANCE_BYTES[1101] ^ *(( char*)nonce_bytes+3);
            final_init_img[1105] = NEW_IMAGE_REFERANCE_BYTES[1105] ^ *(( char*)nonce_bytes+3);
            final_init_img[1109] = NEW_IMAGE_REFERANCE_BYTES[1109] ^ *(( char*)nonce_bytes+3);
            final_init_img[1113] = NEW_IMAGE_REFERANCE_BYTES[1113] ^ *(( char*)nonce_bytes+3);
            final_init_img[1117] = NEW_IMAGE_REFERANCE_BYTES[1117] ^ *(( char*)nonce_bytes+3);
            final_init_img[1121] = NEW_IMAGE_REFERANCE_BYTES[1121] ^ *(( char*)nonce_bytes+3);
            final_init_img[1125] = NEW_IMAGE_REFERANCE_BYTES[1125] ^ *(( char*)nonce_bytes+3);
            final_init_img[1129] = NEW_IMAGE_REFERANCE_BYTES[1129] ^ *(( char*)nonce_bytes+3);
            final_init_img[1133] = NEW_IMAGE_REFERANCE_BYTES[1133] ^ *(( char*)nonce_bytes+3);
            final_init_img[1137] = NEW_IMAGE_REFERANCE_BYTES[1137] ^ *(( char*)nonce_bytes+3);
            final_init_img[1141] = NEW_IMAGE_REFERANCE_BYTES[1141] ^ *(( char*)nonce_bytes+3);
            final_init_img[1145] = NEW_IMAGE_REFERANCE_BYTES[1145] ^ *(( char*)nonce_bytes+3);
            final_init_img[1149] = NEW_IMAGE_REFERANCE_BYTES[1149] ^ *(( char*)nonce_bytes+3);
            final_init_img[1153] = NEW_IMAGE_REFERANCE_BYTES[1153] ^ *(( char*)nonce_bytes+3);
            final_init_img[1157] = NEW_IMAGE_REFERANCE_BYTES[1157] ^ *(( char*)nonce_bytes+3);
            final_init_img[1161] = NEW_IMAGE_REFERANCE_BYTES[1161] ^ *(( char*)nonce_bytes+3);
            final_init_img[1165] = NEW_IMAGE_REFERANCE_BYTES[1165] ^ *(( char*)nonce_bytes+3);
            final_init_img[1169] = NEW_IMAGE_REFERANCE_BYTES[1169] ^ *(( char*)nonce_bytes+3);
            final_init_img[1173] = NEW_IMAGE_REFERANCE_BYTES[1173] ^ *(( char*)nonce_bytes+3);
            final_init_img[1177] = NEW_IMAGE_REFERANCE_BYTES[1177] ^ *(( char*)nonce_bytes+3);
            final_init_img[1181] = NEW_IMAGE_REFERANCE_BYTES[1181] ^ *(( char*)nonce_bytes+3);
            final_init_img[1185] = NEW_IMAGE_REFERANCE_BYTES[1185] ^ *(( char*)nonce_bytes+3);
            final_init_img[1189] = NEW_IMAGE_REFERANCE_BYTES[1189] ^ *(( char*)nonce_bytes+3);
            final_init_img[1193] = NEW_IMAGE_REFERANCE_BYTES[1193] ^ *(( char*)nonce_bytes+3);
            final_init_img[1197] = NEW_IMAGE_REFERANCE_BYTES[1197] ^ *(( char*)nonce_bytes+3);
            final_init_img[1201] = NEW_IMAGE_REFERANCE_BYTES[1201] ^ *(( char*)nonce_bytes+3);
            final_init_img[1205] = NEW_IMAGE_REFERANCE_BYTES[1205] ^ *(( char*)nonce_bytes+3);
            final_init_img[1209] = NEW_IMAGE_REFERANCE_BYTES[1209] ^ *(( char*)nonce_bytes+3);
            final_init_img[1213] = NEW_IMAGE_REFERANCE_BYTES[1213] ^ *(( char*)nonce_bytes+3);
            final_init_img[1217] = NEW_IMAGE_REFERANCE_BYTES[1217] ^ *(( char*)nonce_bytes+3);
            final_init_img[1221] = NEW_IMAGE_REFERANCE_BYTES[1221] ^ *(( char*)nonce_bytes+3);
            final_init_img[1225] = NEW_IMAGE_REFERANCE_BYTES[1225] ^ *(( char*)nonce_bytes+3);
            final_init_img[1229] = NEW_IMAGE_REFERANCE_BYTES[1229] ^ *(( char*)nonce_bytes+3);
            final_init_img[1233] = NEW_IMAGE_REFERANCE_BYTES[1233] ^ *(( char*)nonce_bytes+3);
            final_init_img[1237] = NEW_IMAGE_REFERANCE_BYTES[1237] ^ *(( char*)nonce_bytes+3);
            final_init_img[1241] = NEW_IMAGE_REFERANCE_BYTES[1241] ^ *(( char*)nonce_bytes+3);
            final_init_img[1245] = NEW_IMAGE_REFERANCE_BYTES[1245] ^ *(( char*)nonce_bytes+3);
            final_init_img[1249] = NEW_IMAGE_REFERANCE_BYTES[1249] ^ *(( char*)nonce_bytes+3);
            final_init_img[1253] = NEW_IMAGE_REFERANCE_BYTES[1253] ^ *(( char*)nonce_bytes+3);
            final_init_img[1257] = NEW_IMAGE_REFERANCE_BYTES[1257] ^ *(( char*)nonce_bytes+3);
            final_init_img[1261] = NEW_IMAGE_REFERANCE_BYTES[1261] ^ *(( char*)nonce_bytes+3);
            final_init_img[1265] = NEW_IMAGE_REFERANCE_BYTES[1265] ^ *(( char*)nonce_bytes+3);
            final_init_img[1269] = NEW_IMAGE_REFERANCE_BYTES[1269] ^ *(( char*)nonce_bytes+3);
            final_init_img[1273] = NEW_IMAGE_REFERANCE_BYTES[1273] ^ *(( char*)nonce_bytes+3);
            final_init_img[1277] = NEW_IMAGE_REFERANCE_BYTES[1277] ^ *(( char*)nonce_bytes+3);
            final_init_img[1281] = NEW_IMAGE_REFERANCE_BYTES[1281] ^ *(( char*)nonce_bytes+3);
            final_init_img[1285] = NEW_IMAGE_REFERANCE_BYTES[1285] ^ *(( char*)nonce_bytes+3);
            final_init_img[1289] = NEW_IMAGE_REFERANCE_BYTES[1289] ^ *(( char*)nonce_bytes+3);
            final_init_img[1293] = NEW_IMAGE_REFERANCE_BYTES[1293] ^ *(( char*)nonce_bytes+3);
            final_init_img[1297] = NEW_IMAGE_REFERANCE_BYTES[1297] ^ *(( char*)nonce_bytes+3);
            final_init_img[1301] = NEW_IMAGE_REFERANCE_BYTES[1301] ^ *(( char*)nonce_bytes+3);
            final_init_img[1305] = NEW_IMAGE_REFERANCE_BYTES[1305] ^ *(( char*)nonce_bytes+3);
            final_init_img[1309] = NEW_IMAGE_REFERANCE_BYTES[1309] ^ *(( char*)nonce_bytes+3);
            final_init_img[1313] = NEW_IMAGE_REFERANCE_BYTES[1313] ^ *(( char*)nonce_bytes+3);
            final_init_img[1317] = NEW_IMAGE_REFERANCE_BYTES[1317] ^ *(( char*)nonce_bytes+3);
            final_init_img[1321] = NEW_IMAGE_REFERANCE_BYTES[1321] ^ *(( char*)nonce_bytes+3);
            final_init_img[1325] = NEW_IMAGE_REFERANCE_BYTES[1325] ^ *(( char*)nonce_bytes+3);
            final_init_img[1329] = NEW_IMAGE_REFERANCE_BYTES[1329] ^ *(( char*)nonce_bytes+3);
            final_init_img[1333] = NEW_IMAGE_REFERANCE_BYTES[1333] ^ *(( char*)nonce_bytes+3);
            final_init_img[1337] = NEW_IMAGE_REFERANCE_BYTES[1337] ^ *(( char*)nonce_bytes+3);
            final_init_img[1341] = NEW_IMAGE_REFERANCE_BYTES[1341] ^ *(( char*)nonce_bytes+3);
            final_init_img[1345] = NEW_IMAGE_REFERANCE_BYTES[1345] ^ *(( char*)nonce_bytes+3);
            final_init_img[1349] = NEW_IMAGE_REFERANCE_BYTES[1349] ^ *(( char*)nonce_bytes+3);
            final_init_img[1353] = NEW_IMAGE_REFERANCE_BYTES[1353] ^ *(( char*)nonce_bytes+3);
            final_init_img[1357] = NEW_IMAGE_REFERANCE_BYTES[1357] ^ *(( char*)nonce_bytes+3);
            final_init_img[1361] = NEW_IMAGE_REFERANCE_BYTES[1361] ^ *(( char*)nonce_bytes+3);
            final_init_img[1365] = NEW_IMAGE_REFERANCE_BYTES[1365] ^ *(( char*)nonce_bytes+3);
            final_init_img[1369] = NEW_IMAGE_REFERANCE_BYTES[1369] ^ *(( char*)nonce_bytes+3);
            final_init_img[1373] = NEW_IMAGE_REFERANCE_BYTES[1373] ^ *(( char*)nonce_bytes+3);
            final_init_img[1377] = NEW_IMAGE_REFERANCE_BYTES[1377] ^ *(( char*)nonce_bytes+3);
            final_init_img[1381] = NEW_IMAGE_REFERANCE_BYTES[1381] ^ *(( char*)nonce_bytes+3);
            final_init_img[1385] = NEW_IMAGE_REFERANCE_BYTES[1385] ^ *(( char*)nonce_bytes+3);
            final_init_img[1389] = NEW_IMAGE_REFERANCE_BYTES[1389] ^ *(( char*)nonce_bytes+3);
            final_init_img[1393] = NEW_IMAGE_REFERANCE_BYTES[1393] ^ *(( char*)nonce_bytes+3);
            final_init_img[1397] = NEW_IMAGE_REFERANCE_BYTES[1397] ^ *(( char*)nonce_bytes+3);
            final_init_img[1401] = NEW_IMAGE_REFERANCE_BYTES[1401] ^ *(( char*)nonce_bytes+3);
            final_init_img[1405] = NEW_IMAGE_REFERANCE_BYTES[1405] ^ *(( char*)nonce_bytes+3);
            final_init_img[1409] = NEW_IMAGE_REFERANCE_BYTES[1409] ^ *(( char*)nonce_bytes+3);
            final_init_img[1413] = NEW_IMAGE_REFERANCE_BYTES[1413] ^ *(( char*)nonce_bytes+3);
            final_init_img[1417] = NEW_IMAGE_REFERANCE_BYTES[1417] ^ *(( char*)nonce_bytes+3);
            final_init_img[1421] = NEW_IMAGE_REFERANCE_BYTES[1421] ^ *(( char*)nonce_bytes+3);
            final_init_img[1425] = NEW_IMAGE_REFERANCE_BYTES[1425] ^ *(( char*)nonce_bytes+3);
            final_init_img[1429] = NEW_IMAGE_REFERANCE_BYTES[1429] ^ *(( char*)nonce_bytes+3);
            final_init_img[1433] = NEW_IMAGE_REFERANCE_BYTES[1433] ^ *(( char*)nonce_bytes+3);
            final_init_img[1437] = NEW_IMAGE_REFERANCE_BYTES[1437] ^ *(( char*)nonce_bytes+3);
            final_init_img[1441] = NEW_IMAGE_REFERANCE_BYTES[1441] ^ *(( char*)nonce_bytes+3);
            final_init_img[1445] = NEW_IMAGE_REFERANCE_BYTES[1445] ^ *(( char*)nonce_bytes+3);
            final_init_img[1449] = NEW_IMAGE_REFERANCE_BYTES[1449] ^ *(( char*)nonce_bytes+3);
            final_init_img[1453] = NEW_IMAGE_REFERANCE_BYTES[1453] ^ *(( char*)nonce_bytes+3);
            final_init_img[1457] = NEW_IMAGE_REFERANCE_BYTES[1457] ^ *(( char*)nonce_bytes+3);
            final_init_img[1461] = NEW_IMAGE_REFERANCE_BYTES[1461] ^ *(( char*)nonce_bytes+3);
            final_init_img[1465] = NEW_IMAGE_REFERANCE_BYTES[1465] ^ *(( char*)nonce_bytes+3);
            final_init_img[1469] = NEW_IMAGE_REFERANCE_BYTES[1469] ^ *(( char*)nonce_bytes+3);
            final_init_img[1473] = NEW_IMAGE_REFERANCE_BYTES[1473] ^ *(( char*)nonce_bytes+3);
            final_init_img[1477] = NEW_IMAGE_REFERANCE_BYTES[1477] ^ *(( char*)nonce_bytes+3);
            final_init_img[1481] = NEW_IMAGE_REFERANCE_BYTES[1481] ^ *(( char*)nonce_bytes+3);
            final_init_img[1485] = NEW_IMAGE_REFERANCE_BYTES[1485] ^ *(( char*)nonce_bytes+3);
            final_init_img[1489] = NEW_IMAGE_REFERANCE_BYTES[1489] ^ *(( char*)nonce_bytes+3);
            final_init_img[1493] = NEW_IMAGE_REFERANCE_BYTES[1493] ^ *(( char*)nonce_bytes+3);
            final_init_img[1497] = NEW_IMAGE_REFERANCE_BYTES[1497] ^ *(( char*)nonce_bytes+3);
            final_init_img[1501] = NEW_IMAGE_REFERANCE_BYTES[1501] ^ *(( char*)nonce_bytes+3);
            final_init_img[1505] = NEW_IMAGE_REFERANCE_BYTES[1505] ^ *(( char*)nonce_bytes+3);
            final_init_img[1509] = NEW_IMAGE_REFERANCE_BYTES[1509] ^ *(( char*)nonce_bytes+3);
            final_init_img[1513] = NEW_IMAGE_REFERANCE_BYTES[1513] ^ *(( char*)nonce_bytes+3);
            final_init_img[1517] = NEW_IMAGE_REFERANCE_BYTES[1517] ^ *(( char*)nonce_bytes+3);
            final_init_img[1521] = NEW_IMAGE_REFERANCE_BYTES[1521] ^ *(( char*)nonce_bytes+3);
            final_init_img[1525] = NEW_IMAGE_REFERANCE_BYTES[1525] ^ *(( char*)nonce_bytes+3);
            final_init_img[1529] = NEW_IMAGE_REFERANCE_BYTES[1529] ^ *(( char*)nonce_bytes+3);
            final_init_img[1533] = NEW_IMAGE_REFERANCE_BYTES[1533] ^ *(( char*)nonce_bytes+3);
            final_init_img[1537] = NEW_IMAGE_REFERANCE_BYTES[1537] ^ *(( char*)nonce_bytes+3);
            final_init_img[1541] = NEW_IMAGE_REFERANCE_BYTES[1541] ^ *(( char*)nonce_bytes+3);
            final_init_img[1545] = NEW_IMAGE_REFERANCE_BYTES[1545] ^ *(( char*)nonce_bytes+3);
            final_init_img[1549] = NEW_IMAGE_REFERANCE_BYTES[1549] ^ *(( char*)nonce_bytes+3);
            final_init_img[1553] = NEW_IMAGE_REFERANCE_BYTES[1553] ^ *(( char*)nonce_bytes+3);
            final_init_img[1557] = NEW_IMAGE_REFERANCE_BYTES[1557] ^ *(( char*)nonce_bytes+3);
            final_init_img[1561] = NEW_IMAGE_REFERANCE_BYTES[1561] ^ *(( char*)nonce_bytes+3);
            final_init_img[1565] = NEW_IMAGE_REFERANCE_BYTES[1565] ^ *(( char*)nonce_bytes+3);
            final_init_img[1569] = NEW_IMAGE_REFERANCE_BYTES[1569] ^ *(( char*)nonce_bytes+3);
            final_init_img[1573] = NEW_IMAGE_REFERANCE_BYTES[1573] ^ *(( char*)nonce_bytes+3);
            final_init_img[1577] = NEW_IMAGE_REFERANCE_BYTES[1577] ^ *(( char*)nonce_bytes+3);
            final_init_img[1581] = NEW_IMAGE_REFERANCE_BYTES[1581] ^ *(( char*)nonce_bytes+3);
            final_init_img[1585] = NEW_IMAGE_REFERANCE_BYTES[1585] ^ *(( char*)nonce_bytes+3);
            final_init_img[1589] = NEW_IMAGE_REFERANCE_BYTES[1589] ^ *(( char*)nonce_bytes+3);
            final_init_img[1593] = NEW_IMAGE_REFERANCE_BYTES[1593] ^ *(( char*)nonce_bytes+3);
            final_init_img[1597] = NEW_IMAGE_REFERANCE_BYTES[1597] ^ *(( char*)nonce_bytes+3);
            final_init_img[1601] = NEW_IMAGE_REFERANCE_BYTES[1601] ^ *(( char*)nonce_bytes+3);
            final_init_img[1605] = NEW_IMAGE_REFERANCE_BYTES[1605] ^ *(( char*)nonce_bytes+3);
            final_init_img[1609] = NEW_IMAGE_REFERANCE_BYTES[1609] ^ *(( char*)nonce_bytes+3);
            final_init_img[1613] = NEW_IMAGE_REFERANCE_BYTES[1613] ^ *(( char*)nonce_bytes+3);
            final_init_img[1617] = NEW_IMAGE_REFERANCE_BYTES[1617] ^ *(( char*)nonce_bytes+3);
            final_init_img[1621] = NEW_IMAGE_REFERANCE_BYTES[1621] ^ *(( char*)nonce_bytes+3);
            final_init_img[1625] = NEW_IMAGE_REFERANCE_BYTES[1625] ^ *(( char*)nonce_bytes+3);
            final_init_img[1629] = NEW_IMAGE_REFERANCE_BYTES[1629] ^ *(( char*)nonce_bytes+3);
            final_init_img[1633] = NEW_IMAGE_REFERANCE_BYTES[1633] ^ *(( char*)nonce_bytes+3);
            final_init_img[1637] = NEW_IMAGE_REFERANCE_BYTES[1637] ^ *(( char*)nonce_bytes+3);
            final_init_img[1641] = NEW_IMAGE_REFERANCE_BYTES[1641] ^ *(( char*)nonce_bytes+3);
            final_init_img[1645] = NEW_IMAGE_REFERANCE_BYTES[1645] ^ *(( char*)nonce_bytes+3);
            final_init_img[1649] = NEW_IMAGE_REFERANCE_BYTES[1649] ^ *(( char*)nonce_bytes+3);
            final_init_img[1653] = NEW_IMAGE_REFERANCE_BYTES[1653] ^ *(( char*)nonce_bytes+3);
            final_init_img[1657] = NEW_IMAGE_REFERANCE_BYTES[1657] ^ *(( char*)nonce_bytes+3);
            final_init_img[1661] = NEW_IMAGE_REFERANCE_BYTES[1661] ^ *(( char*)nonce_bytes+3);
            final_init_img[1665] = NEW_IMAGE_REFERANCE_BYTES[1665] ^ *(( char*)nonce_bytes+3);
            final_init_img[1669] = NEW_IMAGE_REFERANCE_BYTES[1669] ^ *(( char*)nonce_bytes+3);
            final_init_img[1673] = NEW_IMAGE_REFERANCE_BYTES[1673] ^ *(( char*)nonce_bytes+3);
            final_init_img[1677] = NEW_IMAGE_REFERANCE_BYTES[1677] ^ *(( char*)nonce_bytes+3);
            final_init_img[1681] = NEW_IMAGE_REFERANCE_BYTES[1681] ^ *(( char*)nonce_bytes+3);
            final_init_img[1685] = NEW_IMAGE_REFERANCE_BYTES[1685] ^ *(( char*)nonce_bytes+3);
            final_init_img[1689] = NEW_IMAGE_REFERANCE_BYTES[1689] ^ *(( char*)nonce_bytes+3);
            final_init_img[1693] = NEW_IMAGE_REFERANCE_BYTES[1693] ^ *(( char*)nonce_bytes+3);
            final_init_img[1697] = NEW_IMAGE_REFERANCE_BYTES[1697] ^ *(( char*)nonce_bytes+3);
            final_init_img[1701] = NEW_IMAGE_REFERANCE_BYTES[1701] ^ *(( char*)nonce_bytes+3);
            final_init_img[1705] = NEW_IMAGE_REFERANCE_BYTES[1705] ^ *(( char*)nonce_bytes+3);
            final_init_img[1709] = NEW_IMAGE_REFERANCE_BYTES[1709] ^ *(( char*)nonce_bytes+3);
            final_init_img[1713] = NEW_IMAGE_REFERANCE_BYTES[1713] ^ *(( char*)nonce_bytes+3);
            final_init_img[1717] = NEW_IMAGE_REFERANCE_BYTES[1717] ^ *(( char*)nonce_bytes+3);
            final_init_img[1721] = NEW_IMAGE_REFERANCE_BYTES[1721] ^ *(( char*)nonce_bytes+3);
            final_init_img[1725] = NEW_IMAGE_REFERANCE_BYTES[1725] ^ *(( char*)nonce_bytes+3);
            final_init_img[1729] = NEW_IMAGE_REFERANCE_BYTES[1729] ^ *(( char*)nonce_bytes+3);
            final_init_img[1733] = NEW_IMAGE_REFERANCE_BYTES[1733] ^ *(( char*)nonce_bytes+3);
            final_init_img[1737] = NEW_IMAGE_REFERANCE_BYTES[1737] ^ *(( char*)nonce_bytes+3);
            final_init_img[1741] = NEW_IMAGE_REFERANCE_BYTES[1741] ^ *(( char*)nonce_bytes+3);
            final_init_img[1745] = NEW_IMAGE_REFERANCE_BYTES[1745] ^ *(( char*)nonce_bytes+3);
            final_init_img[1749] = NEW_IMAGE_REFERANCE_BYTES[1749] ^ *(( char*)nonce_bytes+3);
            final_init_img[1753] = NEW_IMAGE_REFERANCE_BYTES[1753] ^ *(( char*)nonce_bytes+3);
            final_init_img[1757] = NEW_IMAGE_REFERANCE_BYTES[1757] ^ *(( char*)nonce_bytes+3);
            final_init_img[1761] = NEW_IMAGE_REFERANCE_BYTES[1761] ^ *(( char*)nonce_bytes+3);
            final_init_img[1765] = NEW_IMAGE_REFERANCE_BYTES[1765] ^ *(( char*)nonce_bytes+3);
            final_init_img[1769] = NEW_IMAGE_REFERANCE_BYTES[1769] ^ *(( char*)nonce_bytes+3);
            final_init_img[1773] = NEW_IMAGE_REFERANCE_BYTES[1773] ^ *(( char*)nonce_bytes+3);
            final_init_img[1777] = NEW_IMAGE_REFERANCE_BYTES[1777] ^ *(( char*)nonce_bytes+3);
            final_init_img[1781] = NEW_IMAGE_REFERANCE_BYTES[1781] ^ *(( char*)nonce_bytes+3);
       

		}		
		
		
		
		
	cv::Mat converted_buf(1, 1782, CV_8U, (void*)final_init_img);
	
	Mat initial_image;	
	initial_image = imdecode ( converted_buf, IMREAD_COLOR );	


	Mat bilateralFilter_output;	
	bilateralFilter ( initial_image, bilateralFilter_output, 15,75,75 );
	


	Mat filter2D_output;	
	filter2D(bilateralFilter_output, filter2D_output, -1 , kernel );
	
	
	Mat blur_output;
	blur( filter2D_output, blur_output, Size( 5, 5 ) );
	
	
	Mat GaussianBlur_output;
	GaussianBlur( blur_output, GaussianBlur_output, Size( 5, 5 ),BORDER_DEFAULT );	


	Mat final_image;
	medianBlur( GaussianBlur_output, final_image, 5 );		
	
	
	std::vector<uchar>output_buff;
	imencode(".bmp", final_image, output_buff);
	int i;
	for(i=0;i<76;i++)
		output_buff.push_back (block_header[i]);
	
	for(i=0;i<4;i++)
		output_buff.push_back (last_nonce_bytes[i]);
	
	CSHA256().Write((const unsigned char*)&output_buff[0], 1782+80).Finalize((uint8_t *)output);	
	
	
}




bool ocv2_test_algo(){
	
/*
	It is necessary to check whether opencv is compiled correctly and verify that the cpu is able to correctly calculate our algorithm.
	Some processors use different float point arithmetic and our algo doesn't work properly on them.
	Also opencv compilation flags ( like avx sse eg..) may break this algorithm.
	So we will test.
*/
	
	
		
		



	char block_headers[] = OCV2_TEST_BLOCKHEADERS;
	char hashes[] = OCV2_TEST_HASHES;	
	
	
	int i;
	char tmp_test_arr[32];
	
	for(i=0;i<100;i++){		
		
		
		ocv2_hash(&block_headers[i*80],tmp_test_arr);	
	
		if(	memcmp ( &hashes[i*32] ,tmp_test_arr, 32 ) != 0 )
			return false;			
		
		
		
	}	

		
		
		
	
	
	return true;
	
	
	
	
	
	
}