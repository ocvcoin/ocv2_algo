#include <crypto/sha512.cpp>
#include <crypto/sha256.cpp>

#include <opencv2/opencv.hpp>

#include "ocv2.hpp"


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













			
		
		

	


	void ocv2_init_image(char* block_header,char* NEW_IMAGE_REFERANCE_BYTES,char* final_init_img,char* last_nonce_bytes){
		
		
		char bmp_header[] = "\x42\x4D\xF6\x06\x00\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x18\x00\x00\x00\x18\x00\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00\xC0\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
		
		memcpy(NEW_IMAGE_REFERANCE_BYTES, bmp_header, 54);
		memcpy(final_init_img, bmp_header, 54);
		int i;
		
		for(i=0;i<4;i++){			
			
			if(block_header[76+i] == '\xff')
				last_nonce_bytes[i] = '\xee';
			else
				last_nonce_bytes[i] = '\xff';	

		}
		
		//Let's prepare a reference image:
		uint8_t hash512[CSHA512::OUTPUT_SIZE];
		
		i = 0;
		while (i < 27){
		
			if(i==0)
				CSHA512().Write((const unsigned char*)&block_header[0], 76).Finalize(hash512);
			else
				CSHA512().Write((const unsigned char*)hash512, CSHA512::OUTPUT_SIZE).Finalize(hash512);		
		
        
			std::memcpy(&NEW_IMAGE_REFERANCE_BYTES[(54 + ( i * CSHA512::OUTPUT_SIZE ))], hash512, CSHA512::OUTPUT_SIZE);			
		
		
			i += 1;	
		}	

		
	
	
	}






	
	
		

void ocv2_calculate_hash(char* block_header,char* NEW_IMAGE_REFERANCE_BYTES,char* final_init_img,char* last_nonce_bytes,char* output){
	
	
	Mat kernel = Mat::zeros(3, 3, CV_32F);
	
	kernel.at<float>(0,1)--;
	kernel.at<float>(1,0)--;
	kernel.at<float>(1,1) += 5;
	kernel.at<float>(1,2)--;	
	kernel.at<float>(2,1)--;	
	
        
        
        if (last_nonce_bytes[0] != block_header[76]){
            last_nonce_bytes[0] = block_header[76];
            final_init_img[54] = NEW_IMAGE_REFERANCE_BYTES[54] ^ block_header[76];
            final_init_img[58] = NEW_IMAGE_REFERANCE_BYTES[58] ^ block_header[76];
            final_init_img[62] = NEW_IMAGE_REFERANCE_BYTES[62] ^ block_header[76];
            final_init_img[66] = NEW_IMAGE_REFERANCE_BYTES[66] ^ block_header[76];
            final_init_img[70] = NEW_IMAGE_REFERANCE_BYTES[70] ^ block_header[76];
            final_init_img[74] = NEW_IMAGE_REFERANCE_BYTES[74] ^ block_header[76];
            final_init_img[78] = NEW_IMAGE_REFERANCE_BYTES[78] ^ block_header[76];
            final_init_img[82] = NEW_IMAGE_REFERANCE_BYTES[82] ^ block_header[76];
            final_init_img[86] = NEW_IMAGE_REFERANCE_BYTES[86] ^ block_header[76];
            final_init_img[90] = NEW_IMAGE_REFERANCE_BYTES[90] ^ block_header[76];
            final_init_img[94] = NEW_IMAGE_REFERANCE_BYTES[94] ^ block_header[76];
            final_init_img[98] = NEW_IMAGE_REFERANCE_BYTES[98] ^ block_header[76];
            final_init_img[102] = NEW_IMAGE_REFERANCE_BYTES[102] ^ block_header[76];
            final_init_img[106] = NEW_IMAGE_REFERANCE_BYTES[106] ^ block_header[76];
            final_init_img[110] = NEW_IMAGE_REFERANCE_BYTES[110] ^ block_header[76];
            final_init_img[114] = NEW_IMAGE_REFERANCE_BYTES[114] ^ block_header[76];
            final_init_img[118] = NEW_IMAGE_REFERANCE_BYTES[118] ^ block_header[76];
            final_init_img[122] = NEW_IMAGE_REFERANCE_BYTES[122] ^ block_header[76];
            final_init_img[126] = NEW_IMAGE_REFERANCE_BYTES[126] ^ block_header[76];
            final_init_img[130] = NEW_IMAGE_REFERANCE_BYTES[130] ^ block_header[76];
            final_init_img[134] = NEW_IMAGE_REFERANCE_BYTES[134] ^ block_header[76];
            final_init_img[138] = NEW_IMAGE_REFERANCE_BYTES[138] ^ block_header[76];
            final_init_img[142] = NEW_IMAGE_REFERANCE_BYTES[142] ^ block_header[76];
            final_init_img[146] = NEW_IMAGE_REFERANCE_BYTES[146] ^ block_header[76];
            final_init_img[150] = NEW_IMAGE_REFERANCE_BYTES[150] ^ block_header[76];
            final_init_img[154] = NEW_IMAGE_REFERANCE_BYTES[154] ^ block_header[76];
            final_init_img[158] = NEW_IMAGE_REFERANCE_BYTES[158] ^ block_header[76];
            final_init_img[162] = NEW_IMAGE_REFERANCE_BYTES[162] ^ block_header[76];
            final_init_img[166] = NEW_IMAGE_REFERANCE_BYTES[166] ^ block_header[76];
            final_init_img[170] = NEW_IMAGE_REFERANCE_BYTES[170] ^ block_header[76];
            final_init_img[174] = NEW_IMAGE_REFERANCE_BYTES[174] ^ block_header[76];
            final_init_img[178] = NEW_IMAGE_REFERANCE_BYTES[178] ^ block_header[76];
            final_init_img[182] = NEW_IMAGE_REFERANCE_BYTES[182] ^ block_header[76];
            final_init_img[186] = NEW_IMAGE_REFERANCE_BYTES[186] ^ block_header[76];
            final_init_img[190] = NEW_IMAGE_REFERANCE_BYTES[190] ^ block_header[76];
            final_init_img[194] = NEW_IMAGE_REFERANCE_BYTES[194] ^ block_header[76];
            final_init_img[198] = NEW_IMAGE_REFERANCE_BYTES[198] ^ block_header[76];
            final_init_img[202] = NEW_IMAGE_REFERANCE_BYTES[202] ^ block_header[76];
            final_init_img[206] = NEW_IMAGE_REFERANCE_BYTES[206] ^ block_header[76];
            final_init_img[210] = NEW_IMAGE_REFERANCE_BYTES[210] ^ block_header[76];
            final_init_img[214] = NEW_IMAGE_REFERANCE_BYTES[214] ^ block_header[76];
            final_init_img[218] = NEW_IMAGE_REFERANCE_BYTES[218] ^ block_header[76];
            final_init_img[222] = NEW_IMAGE_REFERANCE_BYTES[222] ^ block_header[76];
            final_init_img[226] = NEW_IMAGE_REFERANCE_BYTES[226] ^ block_header[76];
            final_init_img[230] = NEW_IMAGE_REFERANCE_BYTES[230] ^ block_header[76];
            final_init_img[234] = NEW_IMAGE_REFERANCE_BYTES[234] ^ block_header[76];
            final_init_img[238] = NEW_IMAGE_REFERANCE_BYTES[238] ^ block_header[76];
            final_init_img[242] = NEW_IMAGE_REFERANCE_BYTES[242] ^ block_header[76];
            final_init_img[246] = NEW_IMAGE_REFERANCE_BYTES[246] ^ block_header[76];
            final_init_img[250] = NEW_IMAGE_REFERANCE_BYTES[250] ^ block_header[76];
            final_init_img[254] = NEW_IMAGE_REFERANCE_BYTES[254] ^ block_header[76];
            final_init_img[258] = NEW_IMAGE_REFERANCE_BYTES[258] ^ block_header[76];
            final_init_img[262] = NEW_IMAGE_REFERANCE_BYTES[262] ^ block_header[76];
            final_init_img[266] = NEW_IMAGE_REFERANCE_BYTES[266] ^ block_header[76];
            final_init_img[270] = NEW_IMAGE_REFERANCE_BYTES[270] ^ block_header[76];
            final_init_img[274] = NEW_IMAGE_REFERANCE_BYTES[274] ^ block_header[76];
            final_init_img[278] = NEW_IMAGE_REFERANCE_BYTES[278] ^ block_header[76];
            final_init_img[282] = NEW_IMAGE_REFERANCE_BYTES[282] ^ block_header[76];
            final_init_img[286] = NEW_IMAGE_REFERANCE_BYTES[286] ^ block_header[76];
            final_init_img[290] = NEW_IMAGE_REFERANCE_BYTES[290] ^ block_header[76];
            final_init_img[294] = NEW_IMAGE_REFERANCE_BYTES[294] ^ block_header[76];
            final_init_img[298] = NEW_IMAGE_REFERANCE_BYTES[298] ^ block_header[76];
            final_init_img[302] = NEW_IMAGE_REFERANCE_BYTES[302] ^ block_header[76];
            final_init_img[306] = NEW_IMAGE_REFERANCE_BYTES[306] ^ block_header[76];
            final_init_img[310] = NEW_IMAGE_REFERANCE_BYTES[310] ^ block_header[76];
            final_init_img[314] = NEW_IMAGE_REFERANCE_BYTES[314] ^ block_header[76];
            final_init_img[318] = NEW_IMAGE_REFERANCE_BYTES[318] ^ block_header[76];
            final_init_img[322] = NEW_IMAGE_REFERANCE_BYTES[322] ^ block_header[76];
            final_init_img[326] = NEW_IMAGE_REFERANCE_BYTES[326] ^ block_header[76];
            final_init_img[330] = NEW_IMAGE_REFERANCE_BYTES[330] ^ block_header[76];
            final_init_img[334] = NEW_IMAGE_REFERANCE_BYTES[334] ^ block_header[76];
            final_init_img[338] = NEW_IMAGE_REFERANCE_BYTES[338] ^ block_header[76];
            final_init_img[342] = NEW_IMAGE_REFERANCE_BYTES[342] ^ block_header[76];
            final_init_img[346] = NEW_IMAGE_REFERANCE_BYTES[346] ^ block_header[76];
            final_init_img[350] = NEW_IMAGE_REFERANCE_BYTES[350] ^ block_header[76];
            final_init_img[354] = NEW_IMAGE_REFERANCE_BYTES[354] ^ block_header[76];
            final_init_img[358] = NEW_IMAGE_REFERANCE_BYTES[358] ^ block_header[76];
            final_init_img[362] = NEW_IMAGE_REFERANCE_BYTES[362] ^ block_header[76];
            final_init_img[366] = NEW_IMAGE_REFERANCE_BYTES[366] ^ block_header[76];
            final_init_img[370] = NEW_IMAGE_REFERANCE_BYTES[370] ^ block_header[76];
            final_init_img[374] = NEW_IMAGE_REFERANCE_BYTES[374] ^ block_header[76];
            final_init_img[378] = NEW_IMAGE_REFERANCE_BYTES[378] ^ block_header[76];
            final_init_img[382] = NEW_IMAGE_REFERANCE_BYTES[382] ^ block_header[76];
            final_init_img[386] = NEW_IMAGE_REFERANCE_BYTES[386] ^ block_header[76];
            final_init_img[390] = NEW_IMAGE_REFERANCE_BYTES[390] ^ block_header[76];
            final_init_img[394] = NEW_IMAGE_REFERANCE_BYTES[394] ^ block_header[76];
            final_init_img[398] = NEW_IMAGE_REFERANCE_BYTES[398] ^ block_header[76];
            final_init_img[402] = NEW_IMAGE_REFERANCE_BYTES[402] ^ block_header[76];
            final_init_img[406] = NEW_IMAGE_REFERANCE_BYTES[406] ^ block_header[76];
            final_init_img[410] = NEW_IMAGE_REFERANCE_BYTES[410] ^ block_header[76];
            final_init_img[414] = NEW_IMAGE_REFERANCE_BYTES[414] ^ block_header[76];
            final_init_img[418] = NEW_IMAGE_REFERANCE_BYTES[418] ^ block_header[76];
            final_init_img[422] = NEW_IMAGE_REFERANCE_BYTES[422] ^ block_header[76];
            final_init_img[426] = NEW_IMAGE_REFERANCE_BYTES[426] ^ block_header[76];
            final_init_img[430] = NEW_IMAGE_REFERANCE_BYTES[430] ^ block_header[76];
            final_init_img[434] = NEW_IMAGE_REFERANCE_BYTES[434] ^ block_header[76];
            final_init_img[438] = NEW_IMAGE_REFERANCE_BYTES[438] ^ block_header[76];
            final_init_img[442] = NEW_IMAGE_REFERANCE_BYTES[442] ^ block_header[76];
            final_init_img[446] = NEW_IMAGE_REFERANCE_BYTES[446] ^ block_header[76];
            final_init_img[450] = NEW_IMAGE_REFERANCE_BYTES[450] ^ block_header[76];
            final_init_img[454] = NEW_IMAGE_REFERANCE_BYTES[454] ^ block_header[76];
            final_init_img[458] = NEW_IMAGE_REFERANCE_BYTES[458] ^ block_header[76];
            final_init_img[462] = NEW_IMAGE_REFERANCE_BYTES[462] ^ block_header[76];
            final_init_img[466] = NEW_IMAGE_REFERANCE_BYTES[466] ^ block_header[76];
            final_init_img[470] = NEW_IMAGE_REFERANCE_BYTES[470] ^ block_header[76];
            final_init_img[474] = NEW_IMAGE_REFERANCE_BYTES[474] ^ block_header[76];
            final_init_img[478] = NEW_IMAGE_REFERANCE_BYTES[478] ^ block_header[76];
            final_init_img[482] = NEW_IMAGE_REFERANCE_BYTES[482] ^ block_header[76];
            final_init_img[486] = NEW_IMAGE_REFERANCE_BYTES[486] ^ block_header[76];
            final_init_img[490] = NEW_IMAGE_REFERANCE_BYTES[490] ^ block_header[76];
            final_init_img[494] = NEW_IMAGE_REFERANCE_BYTES[494] ^ block_header[76];
            final_init_img[498] = NEW_IMAGE_REFERANCE_BYTES[498] ^ block_header[76];
            final_init_img[502] = NEW_IMAGE_REFERANCE_BYTES[502] ^ block_header[76];
            final_init_img[506] = NEW_IMAGE_REFERANCE_BYTES[506] ^ block_header[76];
            final_init_img[510] = NEW_IMAGE_REFERANCE_BYTES[510] ^ block_header[76];
            final_init_img[514] = NEW_IMAGE_REFERANCE_BYTES[514] ^ block_header[76];
            final_init_img[518] = NEW_IMAGE_REFERANCE_BYTES[518] ^ block_header[76];
            final_init_img[522] = NEW_IMAGE_REFERANCE_BYTES[522] ^ block_header[76];
            final_init_img[526] = NEW_IMAGE_REFERANCE_BYTES[526] ^ block_header[76];
            final_init_img[530] = NEW_IMAGE_REFERANCE_BYTES[530] ^ block_header[76];
            final_init_img[534] = NEW_IMAGE_REFERANCE_BYTES[534] ^ block_header[76];
            final_init_img[538] = NEW_IMAGE_REFERANCE_BYTES[538] ^ block_header[76];
            final_init_img[542] = NEW_IMAGE_REFERANCE_BYTES[542] ^ block_header[76];
            final_init_img[546] = NEW_IMAGE_REFERANCE_BYTES[546] ^ block_header[76];
            final_init_img[550] = NEW_IMAGE_REFERANCE_BYTES[550] ^ block_header[76];
            final_init_img[554] = NEW_IMAGE_REFERANCE_BYTES[554] ^ block_header[76];
            final_init_img[558] = NEW_IMAGE_REFERANCE_BYTES[558] ^ block_header[76];
            final_init_img[562] = NEW_IMAGE_REFERANCE_BYTES[562] ^ block_header[76];
            final_init_img[566] = NEW_IMAGE_REFERANCE_BYTES[566] ^ block_header[76];
            final_init_img[570] = NEW_IMAGE_REFERANCE_BYTES[570] ^ block_header[76];
            final_init_img[574] = NEW_IMAGE_REFERANCE_BYTES[574] ^ block_header[76];
            final_init_img[578] = NEW_IMAGE_REFERANCE_BYTES[578] ^ block_header[76];
            final_init_img[582] = NEW_IMAGE_REFERANCE_BYTES[582] ^ block_header[76];
            final_init_img[586] = NEW_IMAGE_REFERANCE_BYTES[586] ^ block_header[76];
            final_init_img[590] = NEW_IMAGE_REFERANCE_BYTES[590] ^ block_header[76];
            final_init_img[594] = NEW_IMAGE_REFERANCE_BYTES[594] ^ block_header[76];
            final_init_img[598] = NEW_IMAGE_REFERANCE_BYTES[598] ^ block_header[76];
            final_init_img[602] = NEW_IMAGE_REFERANCE_BYTES[602] ^ block_header[76];
            final_init_img[606] = NEW_IMAGE_REFERANCE_BYTES[606] ^ block_header[76];
            final_init_img[610] = NEW_IMAGE_REFERANCE_BYTES[610] ^ block_header[76];
            final_init_img[614] = NEW_IMAGE_REFERANCE_BYTES[614] ^ block_header[76];
            final_init_img[618] = NEW_IMAGE_REFERANCE_BYTES[618] ^ block_header[76];
            final_init_img[622] = NEW_IMAGE_REFERANCE_BYTES[622] ^ block_header[76];
            final_init_img[626] = NEW_IMAGE_REFERANCE_BYTES[626] ^ block_header[76];
            final_init_img[630] = NEW_IMAGE_REFERANCE_BYTES[630] ^ block_header[76];
            final_init_img[634] = NEW_IMAGE_REFERANCE_BYTES[634] ^ block_header[76];
            final_init_img[638] = NEW_IMAGE_REFERANCE_BYTES[638] ^ block_header[76];
            final_init_img[642] = NEW_IMAGE_REFERANCE_BYTES[642] ^ block_header[76];
            final_init_img[646] = NEW_IMAGE_REFERANCE_BYTES[646] ^ block_header[76];
            final_init_img[650] = NEW_IMAGE_REFERANCE_BYTES[650] ^ block_header[76];
            final_init_img[654] = NEW_IMAGE_REFERANCE_BYTES[654] ^ block_header[76];
            final_init_img[658] = NEW_IMAGE_REFERANCE_BYTES[658] ^ block_header[76];
            final_init_img[662] = NEW_IMAGE_REFERANCE_BYTES[662] ^ block_header[76];
            final_init_img[666] = NEW_IMAGE_REFERANCE_BYTES[666] ^ block_header[76];
            final_init_img[670] = NEW_IMAGE_REFERANCE_BYTES[670] ^ block_header[76];
            final_init_img[674] = NEW_IMAGE_REFERANCE_BYTES[674] ^ block_header[76];
            final_init_img[678] = NEW_IMAGE_REFERANCE_BYTES[678] ^ block_header[76];
            final_init_img[682] = NEW_IMAGE_REFERANCE_BYTES[682] ^ block_header[76];
            final_init_img[686] = NEW_IMAGE_REFERANCE_BYTES[686] ^ block_header[76];
            final_init_img[690] = NEW_IMAGE_REFERANCE_BYTES[690] ^ block_header[76];
            final_init_img[694] = NEW_IMAGE_REFERANCE_BYTES[694] ^ block_header[76];
            final_init_img[698] = NEW_IMAGE_REFERANCE_BYTES[698] ^ block_header[76];
            final_init_img[702] = NEW_IMAGE_REFERANCE_BYTES[702] ^ block_header[76];
            final_init_img[706] = NEW_IMAGE_REFERANCE_BYTES[706] ^ block_header[76];
            final_init_img[710] = NEW_IMAGE_REFERANCE_BYTES[710] ^ block_header[76];
            final_init_img[714] = NEW_IMAGE_REFERANCE_BYTES[714] ^ block_header[76];
            final_init_img[718] = NEW_IMAGE_REFERANCE_BYTES[718] ^ block_header[76];
            final_init_img[722] = NEW_IMAGE_REFERANCE_BYTES[722] ^ block_header[76];
            final_init_img[726] = NEW_IMAGE_REFERANCE_BYTES[726] ^ block_header[76];
            final_init_img[730] = NEW_IMAGE_REFERANCE_BYTES[730] ^ block_header[76];
            final_init_img[734] = NEW_IMAGE_REFERANCE_BYTES[734] ^ block_header[76];
            final_init_img[738] = NEW_IMAGE_REFERANCE_BYTES[738] ^ block_header[76];
            final_init_img[742] = NEW_IMAGE_REFERANCE_BYTES[742] ^ block_header[76];
            final_init_img[746] = NEW_IMAGE_REFERANCE_BYTES[746] ^ block_header[76];
            final_init_img[750] = NEW_IMAGE_REFERANCE_BYTES[750] ^ block_header[76];
            final_init_img[754] = NEW_IMAGE_REFERANCE_BYTES[754] ^ block_header[76];
            final_init_img[758] = NEW_IMAGE_REFERANCE_BYTES[758] ^ block_header[76];
            final_init_img[762] = NEW_IMAGE_REFERANCE_BYTES[762] ^ block_header[76];
            final_init_img[766] = NEW_IMAGE_REFERANCE_BYTES[766] ^ block_header[76];
            final_init_img[770] = NEW_IMAGE_REFERANCE_BYTES[770] ^ block_header[76];
            final_init_img[774] = NEW_IMAGE_REFERANCE_BYTES[774] ^ block_header[76];
            final_init_img[778] = NEW_IMAGE_REFERANCE_BYTES[778] ^ block_header[76];
            final_init_img[782] = NEW_IMAGE_REFERANCE_BYTES[782] ^ block_header[76];
            final_init_img[786] = NEW_IMAGE_REFERANCE_BYTES[786] ^ block_header[76];
            final_init_img[790] = NEW_IMAGE_REFERANCE_BYTES[790] ^ block_header[76];
            final_init_img[794] = NEW_IMAGE_REFERANCE_BYTES[794] ^ block_header[76];
            final_init_img[798] = NEW_IMAGE_REFERANCE_BYTES[798] ^ block_header[76];
            final_init_img[802] = NEW_IMAGE_REFERANCE_BYTES[802] ^ block_header[76];
            final_init_img[806] = NEW_IMAGE_REFERANCE_BYTES[806] ^ block_header[76];
            final_init_img[810] = NEW_IMAGE_REFERANCE_BYTES[810] ^ block_header[76];
            final_init_img[814] = NEW_IMAGE_REFERANCE_BYTES[814] ^ block_header[76];
            final_init_img[818] = NEW_IMAGE_REFERANCE_BYTES[818] ^ block_header[76];
            final_init_img[822] = NEW_IMAGE_REFERANCE_BYTES[822] ^ block_header[76];
            final_init_img[826] = NEW_IMAGE_REFERANCE_BYTES[826] ^ block_header[76];
            final_init_img[830] = NEW_IMAGE_REFERANCE_BYTES[830] ^ block_header[76];
            final_init_img[834] = NEW_IMAGE_REFERANCE_BYTES[834] ^ block_header[76];
            final_init_img[838] = NEW_IMAGE_REFERANCE_BYTES[838] ^ block_header[76];
            final_init_img[842] = NEW_IMAGE_REFERANCE_BYTES[842] ^ block_header[76];
            final_init_img[846] = NEW_IMAGE_REFERANCE_BYTES[846] ^ block_header[76];
            final_init_img[850] = NEW_IMAGE_REFERANCE_BYTES[850] ^ block_header[76];
            final_init_img[854] = NEW_IMAGE_REFERANCE_BYTES[854] ^ block_header[76];
            final_init_img[858] = NEW_IMAGE_REFERANCE_BYTES[858] ^ block_header[76];
            final_init_img[862] = NEW_IMAGE_REFERANCE_BYTES[862] ^ block_header[76];
            final_init_img[866] = NEW_IMAGE_REFERANCE_BYTES[866] ^ block_header[76];
            final_init_img[870] = NEW_IMAGE_REFERANCE_BYTES[870] ^ block_header[76];
            final_init_img[874] = NEW_IMAGE_REFERANCE_BYTES[874] ^ block_header[76];
            final_init_img[878] = NEW_IMAGE_REFERANCE_BYTES[878] ^ block_header[76];
            final_init_img[882] = NEW_IMAGE_REFERANCE_BYTES[882] ^ block_header[76];
            final_init_img[886] = NEW_IMAGE_REFERANCE_BYTES[886] ^ block_header[76];
            final_init_img[890] = NEW_IMAGE_REFERANCE_BYTES[890] ^ block_header[76];
            final_init_img[894] = NEW_IMAGE_REFERANCE_BYTES[894] ^ block_header[76];
            final_init_img[898] = NEW_IMAGE_REFERANCE_BYTES[898] ^ block_header[76];
            final_init_img[902] = NEW_IMAGE_REFERANCE_BYTES[902] ^ block_header[76];
            final_init_img[906] = NEW_IMAGE_REFERANCE_BYTES[906] ^ block_header[76];
            final_init_img[910] = NEW_IMAGE_REFERANCE_BYTES[910] ^ block_header[76];
            final_init_img[914] = NEW_IMAGE_REFERANCE_BYTES[914] ^ block_header[76];
            final_init_img[918] = NEW_IMAGE_REFERANCE_BYTES[918] ^ block_header[76];
            final_init_img[922] = NEW_IMAGE_REFERANCE_BYTES[922] ^ block_header[76];
            final_init_img[926] = NEW_IMAGE_REFERANCE_BYTES[926] ^ block_header[76];
            final_init_img[930] = NEW_IMAGE_REFERANCE_BYTES[930] ^ block_header[76];
            final_init_img[934] = NEW_IMAGE_REFERANCE_BYTES[934] ^ block_header[76];
            final_init_img[938] = NEW_IMAGE_REFERANCE_BYTES[938] ^ block_header[76];
            final_init_img[942] = NEW_IMAGE_REFERANCE_BYTES[942] ^ block_header[76];
            final_init_img[946] = NEW_IMAGE_REFERANCE_BYTES[946] ^ block_header[76];
            final_init_img[950] = NEW_IMAGE_REFERANCE_BYTES[950] ^ block_header[76];
            final_init_img[954] = NEW_IMAGE_REFERANCE_BYTES[954] ^ block_header[76];
            final_init_img[958] = NEW_IMAGE_REFERANCE_BYTES[958] ^ block_header[76];
            final_init_img[962] = NEW_IMAGE_REFERANCE_BYTES[962] ^ block_header[76];
            final_init_img[966] = NEW_IMAGE_REFERANCE_BYTES[966] ^ block_header[76];
            final_init_img[970] = NEW_IMAGE_REFERANCE_BYTES[970] ^ block_header[76];
            final_init_img[974] = NEW_IMAGE_REFERANCE_BYTES[974] ^ block_header[76];
            final_init_img[978] = NEW_IMAGE_REFERANCE_BYTES[978] ^ block_header[76];
            final_init_img[982] = NEW_IMAGE_REFERANCE_BYTES[982] ^ block_header[76];
            final_init_img[986] = NEW_IMAGE_REFERANCE_BYTES[986] ^ block_header[76];
            final_init_img[990] = NEW_IMAGE_REFERANCE_BYTES[990] ^ block_header[76];
            final_init_img[994] = NEW_IMAGE_REFERANCE_BYTES[994] ^ block_header[76];
            final_init_img[998] = NEW_IMAGE_REFERANCE_BYTES[998] ^ block_header[76];
            final_init_img[1002] = NEW_IMAGE_REFERANCE_BYTES[1002] ^ block_header[76];
            final_init_img[1006] = NEW_IMAGE_REFERANCE_BYTES[1006] ^ block_header[76];
            final_init_img[1010] = NEW_IMAGE_REFERANCE_BYTES[1010] ^ block_header[76];
            final_init_img[1014] = NEW_IMAGE_REFERANCE_BYTES[1014] ^ block_header[76];
            final_init_img[1018] = NEW_IMAGE_REFERANCE_BYTES[1018] ^ block_header[76];
            final_init_img[1022] = NEW_IMAGE_REFERANCE_BYTES[1022] ^ block_header[76];
            final_init_img[1026] = NEW_IMAGE_REFERANCE_BYTES[1026] ^ block_header[76];
            final_init_img[1030] = NEW_IMAGE_REFERANCE_BYTES[1030] ^ block_header[76];
            final_init_img[1034] = NEW_IMAGE_REFERANCE_BYTES[1034] ^ block_header[76];
            final_init_img[1038] = NEW_IMAGE_REFERANCE_BYTES[1038] ^ block_header[76];
            final_init_img[1042] = NEW_IMAGE_REFERANCE_BYTES[1042] ^ block_header[76];
            final_init_img[1046] = NEW_IMAGE_REFERANCE_BYTES[1046] ^ block_header[76];
            final_init_img[1050] = NEW_IMAGE_REFERANCE_BYTES[1050] ^ block_header[76];
            final_init_img[1054] = NEW_IMAGE_REFERANCE_BYTES[1054] ^ block_header[76];
            final_init_img[1058] = NEW_IMAGE_REFERANCE_BYTES[1058] ^ block_header[76];
            final_init_img[1062] = NEW_IMAGE_REFERANCE_BYTES[1062] ^ block_header[76];
            final_init_img[1066] = NEW_IMAGE_REFERANCE_BYTES[1066] ^ block_header[76];
            final_init_img[1070] = NEW_IMAGE_REFERANCE_BYTES[1070] ^ block_header[76];
            final_init_img[1074] = NEW_IMAGE_REFERANCE_BYTES[1074] ^ block_header[76];
            final_init_img[1078] = NEW_IMAGE_REFERANCE_BYTES[1078] ^ block_header[76];
            final_init_img[1082] = NEW_IMAGE_REFERANCE_BYTES[1082] ^ block_header[76];
            final_init_img[1086] = NEW_IMAGE_REFERANCE_BYTES[1086] ^ block_header[76];
            final_init_img[1090] = NEW_IMAGE_REFERANCE_BYTES[1090] ^ block_header[76];
            final_init_img[1094] = NEW_IMAGE_REFERANCE_BYTES[1094] ^ block_header[76];
            final_init_img[1098] = NEW_IMAGE_REFERANCE_BYTES[1098] ^ block_header[76];
            final_init_img[1102] = NEW_IMAGE_REFERANCE_BYTES[1102] ^ block_header[76];
            final_init_img[1106] = NEW_IMAGE_REFERANCE_BYTES[1106] ^ block_header[76];
            final_init_img[1110] = NEW_IMAGE_REFERANCE_BYTES[1110] ^ block_header[76];
            final_init_img[1114] = NEW_IMAGE_REFERANCE_BYTES[1114] ^ block_header[76];
            final_init_img[1118] = NEW_IMAGE_REFERANCE_BYTES[1118] ^ block_header[76];
            final_init_img[1122] = NEW_IMAGE_REFERANCE_BYTES[1122] ^ block_header[76];
            final_init_img[1126] = NEW_IMAGE_REFERANCE_BYTES[1126] ^ block_header[76];
            final_init_img[1130] = NEW_IMAGE_REFERANCE_BYTES[1130] ^ block_header[76];
            final_init_img[1134] = NEW_IMAGE_REFERANCE_BYTES[1134] ^ block_header[76];
            final_init_img[1138] = NEW_IMAGE_REFERANCE_BYTES[1138] ^ block_header[76];
            final_init_img[1142] = NEW_IMAGE_REFERANCE_BYTES[1142] ^ block_header[76];
            final_init_img[1146] = NEW_IMAGE_REFERANCE_BYTES[1146] ^ block_header[76];
            final_init_img[1150] = NEW_IMAGE_REFERANCE_BYTES[1150] ^ block_header[76];
            final_init_img[1154] = NEW_IMAGE_REFERANCE_BYTES[1154] ^ block_header[76];
            final_init_img[1158] = NEW_IMAGE_REFERANCE_BYTES[1158] ^ block_header[76];
            final_init_img[1162] = NEW_IMAGE_REFERANCE_BYTES[1162] ^ block_header[76];
            final_init_img[1166] = NEW_IMAGE_REFERANCE_BYTES[1166] ^ block_header[76];
            final_init_img[1170] = NEW_IMAGE_REFERANCE_BYTES[1170] ^ block_header[76];
            final_init_img[1174] = NEW_IMAGE_REFERANCE_BYTES[1174] ^ block_header[76];
            final_init_img[1178] = NEW_IMAGE_REFERANCE_BYTES[1178] ^ block_header[76];
            final_init_img[1182] = NEW_IMAGE_REFERANCE_BYTES[1182] ^ block_header[76];
            final_init_img[1186] = NEW_IMAGE_REFERANCE_BYTES[1186] ^ block_header[76];
            final_init_img[1190] = NEW_IMAGE_REFERANCE_BYTES[1190] ^ block_header[76];
            final_init_img[1194] = NEW_IMAGE_REFERANCE_BYTES[1194] ^ block_header[76];
            final_init_img[1198] = NEW_IMAGE_REFERANCE_BYTES[1198] ^ block_header[76];
            final_init_img[1202] = NEW_IMAGE_REFERANCE_BYTES[1202] ^ block_header[76];
            final_init_img[1206] = NEW_IMAGE_REFERANCE_BYTES[1206] ^ block_header[76];
            final_init_img[1210] = NEW_IMAGE_REFERANCE_BYTES[1210] ^ block_header[76];
            final_init_img[1214] = NEW_IMAGE_REFERANCE_BYTES[1214] ^ block_header[76];
            final_init_img[1218] = NEW_IMAGE_REFERANCE_BYTES[1218] ^ block_header[76];
            final_init_img[1222] = NEW_IMAGE_REFERANCE_BYTES[1222] ^ block_header[76];
            final_init_img[1226] = NEW_IMAGE_REFERANCE_BYTES[1226] ^ block_header[76];
            final_init_img[1230] = NEW_IMAGE_REFERANCE_BYTES[1230] ^ block_header[76];
            final_init_img[1234] = NEW_IMAGE_REFERANCE_BYTES[1234] ^ block_header[76];
            final_init_img[1238] = NEW_IMAGE_REFERANCE_BYTES[1238] ^ block_header[76];
            final_init_img[1242] = NEW_IMAGE_REFERANCE_BYTES[1242] ^ block_header[76];
            final_init_img[1246] = NEW_IMAGE_REFERANCE_BYTES[1246] ^ block_header[76];
            final_init_img[1250] = NEW_IMAGE_REFERANCE_BYTES[1250] ^ block_header[76];
            final_init_img[1254] = NEW_IMAGE_REFERANCE_BYTES[1254] ^ block_header[76];
            final_init_img[1258] = NEW_IMAGE_REFERANCE_BYTES[1258] ^ block_header[76];
            final_init_img[1262] = NEW_IMAGE_REFERANCE_BYTES[1262] ^ block_header[76];
            final_init_img[1266] = NEW_IMAGE_REFERANCE_BYTES[1266] ^ block_header[76];
            final_init_img[1270] = NEW_IMAGE_REFERANCE_BYTES[1270] ^ block_header[76];
            final_init_img[1274] = NEW_IMAGE_REFERANCE_BYTES[1274] ^ block_header[76];
            final_init_img[1278] = NEW_IMAGE_REFERANCE_BYTES[1278] ^ block_header[76];
            final_init_img[1282] = NEW_IMAGE_REFERANCE_BYTES[1282] ^ block_header[76];
            final_init_img[1286] = NEW_IMAGE_REFERANCE_BYTES[1286] ^ block_header[76];
            final_init_img[1290] = NEW_IMAGE_REFERANCE_BYTES[1290] ^ block_header[76];
            final_init_img[1294] = NEW_IMAGE_REFERANCE_BYTES[1294] ^ block_header[76];
            final_init_img[1298] = NEW_IMAGE_REFERANCE_BYTES[1298] ^ block_header[76];
            final_init_img[1302] = NEW_IMAGE_REFERANCE_BYTES[1302] ^ block_header[76];
            final_init_img[1306] = NEW_IMAGE_REFERANCE_BYTES[1306] ^ block_header[76];
            final_init_img[1310] = NEW_IMAGE_REFERANCE_BYTES[1310] ^ block_header[76];
            final_init_img[1314] = NEW_IMAGE_REFERANCE_BYTES[1314] ^ block_header[76];
            final_init_img[1318] = NEW_IMAGE_REFERANCE_BYTES[1318] ^ block_header[76];
            final_init_img[1322] = NEW_IMAGE_REFERANCE_BYTES[1322] ^ block_header[76];
            final_init_img[1326] = NEW_IMAGE_REFERANCE_BYTES[1326] ^ block_header[76];
            final_init_img[1330] = NEW_IMAGE_REFERANCE_BYTES[1330] ^ block_header[76];
            final_init_img[1334] = NEW_IMAGE_REFERANCE_BYTES[1334] ^ block_header[76];
            final_init_img[1338] = NEW_IMAGE_REFERANCE_BYTES[1338] ^ block_header[76];
            final_init_img[1342] = NEW_IMAGE_REFERANCE_BYTES[1342] ^ block_header[76];
            final_init_img[1346] = NEW_IMAGE_REFERANCE_BYTES[1346] ^ block_header[76];
            final_init_img[1350] = NEW_IMAGE_REFERANCE_BYTES[1350] ^ block_header[76];
            final_init_img[1354] = NEW_IMAGE_REFERANCE_BYTES[1354] ^ block_header[76];
            final_init_img[1358] = NEW_IMAGE_REFERANCE_BYTES[1358] ^ block_header[76];
            final_init_img[1362] = NEW_IMAGE_REFERANCE_BYTES[1362] ^ block_header[76];
            final_init_img[1366] = NEW_IMAGE_REFERANCE_BYTES[1366] ^ block_header[76];
            final_init_img[1370] = NEW_IMAGE_REFERANCE_BYTES[1370] ^ block_header[76];
            final_init_img[1374] = NEW_IMAGE_REFERANCE_BYTES[1374] ^ block_header[76];
            final_init_img[1378] = NEW_IMAGE_REFERANCE_BYTES[1378] ^ block_header[76];
            final_init_img[1382] = NEW_IMAGE_REFERANCE_BYTES[1382] ^ block_header[76];
            final_init_img[1386] = NEW_IMAGE_REFERANCE_BYTES[1386] ^ block_header[76];
            final_init_img[1390] = NEW_IMAGE_REFERANCE_BYTES[1390] ^ block_header[76];
            final_init_img[1394] = NEW_IMAGE_REFERANCE_BYTES[1394] ^ block_header[76];
            final_init_img[1398] = NEW_IMAGE_REFERANCE_BYTES[1398] ^ block_header[76];
            final_init_img[1402] = NEW_IMAGE_REFERANCE_BYTES[1402] ^ block_header[76];
            final_init_img[1406] = NEW_IMAGE_REFERANCE_BYTES[1406] ^ block_header[76];
            final_init_img[1410] = NEW_IMAGE_REFERANCE_BYTES[1410] ^ block_header[76];
            final_init_img[1414] = NEW_IMAGE_REFERANCE_BYTES[1414] ^ block_header[76];
            final_init_img[1418] = NEW_IMAGE_REFERANCE_BYTES[1418] ^ block_header[76];
            final_init_img[1422] = NEW_IMAGE_REFERANCE_BYTES[1422] ^ block_header[76];
            final_init_img[1426] = NEW_IMAGE_REFERANCE_BYTES[1426] ^ block_header[76];
            final_init_img[1430] = NEW_IMAGE_REFERANCE_BYTES[1430] ^ block_header[76];
            final_init_img[1434] = NEW_IMAGE_REFERANCE_BYTES[1434] ^ block_header[76];
            final_init_img[1438] = NEW_IMAGE_REFERANCE_BYTES[1438] ^ block_header[76];
            final_init_img[1442] = NEW_IMAGE_REFERANCE_BYTES[1442] ^ block_header[76];
            final_init_img[1446] = NEW_IMAGE_REFERANCE_BYTES[1446] ^ block_header[76];
            final_init_img[1450] = NEW_IMAGE_REFERANCE_BYTES[1450] ^ block_header[76];
            final_init_img[1454] = NEW_IMAGE_REFERANCE_BYTES[1454] ^ block_header[76];
            final_init_img[1458] = NEW_IMAGE_REFERANCE_BYTES[1458] ^ block_header[76];
            final_init_img[1462] = NEW_IMAGE_REFERANCE_BYTES[1462] ^ block_header[76];
            final_init_img[1466] = NEW_IMAGE_REFERANCE_BYTES[1466] ^ block_header[76];
            final_init_img[1470] = NEW_IMAGE_REFERANCE_BYTES[1470] ^ block_header[76];
            final_init_img[1474] = NEW_IMAGE_REFERANCE_BYTES[1474] ^ block_header[76];
            final_init_img[1478] = NEW_IMAGE_REFERANCE_BYTES[1478] ^ block_header[76];
            final_init_img[1482] = NEW_IMAGE_REFERANCE_BYTES[1482] ^ block_header[76];
            final_init_img[1486] = NEW_IMAGE_REFERANCE_BYTES[1486] ^ block_header[76];
            final_init_img[1490] = NEW_IMAGE_REFERANCE_BYTES[1490] ^ block_header[76];
            final_init_img[1494] = NEW_IMAGE_REFERANCE_BYTES[1494] ^ block_header[76];
            final_init_img[1498] = NEW_IMAGE_REFERANCE_BYTES[1498] ^ block_header[76];
            final_init_img[1502] = NEW_IMAGE_REFERANCE_BYTES[1502] ^ block_header[76];
            final_init_img[1506] = NEW_IMAGE_REFERANCE_BYTES[1506] ^ block_header[76];
            final_init_img[1510] = NEW_IMAGE_REFERANCE_BYTES[1510] ^ block_header[76];
            final_init_img[1514] = NEW_IMAGE_REFERANCE_BYTES[1514] ^ block_header[76];
            final_init_img[1518] = NEW_IMAGE_REFERANCE_BYTES[1518] ^ block_header[76];
            final_init_img[1522] = NEW_IMAGE_REFERANCE_BYTES[1522] ^ block_header[76];
            final_init_img[1526] = NEW_IMAGE_REFERANCE_BYTES[1526] ^ block_header[76];
            final_init_img[1530] = NEW_IMAGE_REFERANCE_BYTES[1530] ^ block_header[76];
            final_init_img[1534] = NEW_IMAGE_REFERANCE_BYTES[1534] ^ block_header[76];
            final_init_img[1538] = NEW_IMAGE_REFERANCE_BYTES[1538] ^ block_header[76];
            final_init_img[1542] = NEW_IMAGE_REFERANCE_BYTES[1542] ^ block_header[76];
            final_init_img[1546] = NEW_IMAGE_REFERANCE_BYTES[1546] ^ block_header[76];
            final_init_img[1550] = NEW_IMAGE_REFERANCE_BYTES[1550] ^ block_header[76];
            final_init_img[1554] = NEW_IMAGE_REFERANCE_BYTES[1554] ^ block_header[76];
            final_init_img[1558] = NEW_IMAGE_REFERANCE_BYTES[1558] ^ block_header[76];
            final_init_img[1562] = NEW_IMAGE_REFERANCE_BYTES[1562] ^ block_header[76];
            final_init_img[1566] = NEW_IMAGE_REFERANCE_BYTES[1566] ^ block_header[76];
            final_init_img[1570] = NEW_IMAGE_REFERANCE_BYTES[1570] ^ block_header[76];
            final_init_img[1574] = NEW_IMAGE_REFERANCE_BYTES[1574] ^ block_header[76];
            final_init_img[1578] = NEW_IMAGE_REFERANCE_BYTES[1578] ^ block_header[76];
            final_init_img[1582] = NEW_IMAGE_REFERANCE_BYTES[1582] ^ block_header[76];
            final_init_img[1586] = NEW_IMAGE_REFERANCE_BYTES[1586] ^ block_header[76];
            final_init_img[1590] = NEW_IMAGE_REFERANCE_BYTES[1590] ^ block_header[76];
            final_init_img[1594] = NEW_IMAGE_REFERANCE_BYTES[1594] ^ block_header[76];
            final_init_img[1598] = NEW_IMAGE_REFERANCE_BYTES[1598] ^ block_header[76];
            final_init_img[1602] = NEW_IMAGE_REFERANCE_BYTES[1602] ^ block_header[76];
            final_init_img[1606] = NEW_IMAGE_REFERANCE_BYTES[1606] ^ block_header[76];
            final_init_img[1610] = NEW_IMAGE_REFERANCE_BYTES[1610] ^ block_header[76];
            final_init_img[1614] = NEW_IMAGE_REFERANCE_BYTES[1614] ^ block_header[76];
            final_init_img[1618] = NEW_IMAGE_REFERANCE_BYTES[1618] ^ block_header[76];
            final_init_img[1622] = NEW_IMAGE_REFERANCE_BYTES[1622] ^ block_header[76];
            final_init_img[1626] = NEW_IMAGE_REFERANCE_BYTES[1626] ^ block_header[76];
            final_init_img[1630] = NEW_IMAGE_REFERANCE_BYTES[1630] ^ block_header[76];
            final_init_img[1634] = NEW_IMAGE_REFERANCE_BYTES[1634] ^ block_header[76];
            final_init_img[1638] = NEW_IMAGE_REFERANCE_BYTES[1638] ^ block_header[76];
            final_init_img[1642] = NEW_IMAGE_REFERANCE_BYTES[1642] ^ block_header[76];
            final_init_img[1646] = NEW_IMAGE_REFERANCE_BYTES[1646] ^ block_header[76];
            final_init_img[1650] = NEW_IMAGE_REFERANCE_BYTES[1650] ^ block_header[76];
            final_init_img[1654] = NEW_IMAGE_REFERANCE_BYTES[1654] ^ block_header[76];
            final_init_img[1658] = NEW_IMAGE_REFERANCE_BYTES[1658] ^ block_header[76];
            final_init_img[1662] = NEW_IMAGE_REFERANCE_BYTES[1662] ^ block_header[76];
            final_init_img[1666] = NEW_IMAGE_REFERANCE_BYTES[1666] ^ block_header[76];
            final_init_img[1670] = NEW_IMAGE_REFERANCE_BYTES[1670] ^ block_header[76];
            final_init_img[1674] = NEW_IMAGE_REFERANCE_BYTES[1674] ^ block_header[76];
            final_init_img[1678] = NEW_IMAGE_REFERANCE_BYTES[1678] ^ block_header[76];
            final_init_img[1682] = NEW_IMAGE_REFERANCE_BYTES[1682] ^ block_header[76];
            final_init_img[1686] = NEW_IMAGE_REFERANCE_BYTES[1686] ^ block_header[76];
            final_init_img[1690] = NEW_IMAGE_REFERANCE_BYTES[1690] ^ block_header[76];
            final_init_img[1694] = NEW_IMAGE_REFERANCE_BYTES[1694] ^ block_header[76];
            final_init_img[1698] = NEW_IMAGE_REFERANCE_BYTES[1698] ^ block_header[76];
            final_init_img[1702] = NEW_IMAGE_REFERANCE_BYTES[1702] ^ block_header[76];
            final_init_img[1706] = NEW_IMAGE_REFERANCE_BYTES[1706] ^ block_header[76];
            final_init_img[1710] = NEW_IMAGE_REFERANCE_BYTES[1710] ^ block_header[76];
            final_init_img[1714] = NEW_IMAGE_REFERANCE_BYTES[1714] ^ block_header[76];
            final_init_img[1718] = NEW_IMAGE_REFERANCE_BYTES[1718] ^ block_header[76];
            final_init_img[1722] = NEW_IMAGE_REFERANCE_BYTES[1722] ^ block_header[76];
            final_init_img[1726] = NEW_IMAGE_REFERANCE_BYTES[1726] ^ block_header[76];
            final_init_img[1730] = NEW_IMAGE_REFERANCE_BYTES[1730] ^ block_header[76];
            final_init_img[1734] = NEW_IMAGE_REFERANCE_BYTES[1734] ^ block_header[76];
            final_init_img[1738] = NEW_IMAGE_REFERANCE_BYTES[1738] ^ block_header[76];
            final_init_img[1742] = NEW_IMAGE_REFERANCE_BYTES[1742] ^ block_header[76];
            final_init_img[1746] = NEW_IMAGE_REFERANCE_BYTES[1746] ^ block_header[76];
            final_init_img[1750] = NEW_IMAGE_REFERANCE_BYTES[1750] ^ block_header[76];
            final_init_img[1754] = NEW_IMAGE_REFERANCE_BYTES[1754] ^ block_header[76];
            final_init_img[1758] = NEW_IMAGE_REFERANCE_BYTES[1758] ^ block_header[76];
            final_init_img[1762] = NEW_IMAGE_REFERANCE_BYTES[1762] ^ block_header[76];
            final_init_img[1766] = NEW_IMAGE_REFERANCE_BYTES[1766] ^ block_header[76];
            final_init_img[1770] = NEW_IMAGE_REFERANCE_BYTES[1770] ^ block_header[76];
            final_init_img[1774] = NEW_IMAGE_REFERANCE_BYTES[1774] ^ block_header[76];
            final_init_img[1778] = NEW_IMAGE_REFERANCE_BYTES[1778] ^ block_header[76];
       
		}

        if (last_nonce_bytes[1] != block_header[77]){
            last_nonce_bytes[1] = block_header[77];
            final_init_img[55] = NEW_IMAGE_REFERANCE_BYTES[55] ^ block_header[77];
            final_init_img[59] = NEW_IMAGE_REFERANCE_BYTES[59] ^ block_header[77];
            final_init_img[63] = NEW_IMAGE_REFERANCE_BYTES[63] ^ block_header[77];
            final_init_img[67] = NEW_IMAGE_REFERANCE_BYTES[67] ^ block_header[77];
            final_init_img[71] = NEW_IMAGE_REFERANCE_BYTES[71] ^ block_header[77];
            final_init_img[75] = NEW_IMAGE_REFERANCE_BYTES[75] ^ block_header[77];
            final_init_img[79] = NEW_IMAGE_REFERANCE_BYTES[79] ^ block_header[77];
            final_init_img[83] = NEW_IMAGE_REFERANCE_BYTES[83] ^ block_header[77];
            final_init_img[87] = NEW_IMAGE_REFERANCE_BYTES[87] ^ block_header[77];
            final_init_img[91] = NEW_IMAGE_REFERANCE_BYTES[91] ^ block_header[77];
            final_init_img[95] = NEW_IMAGE_REFERANCE_BYTES[95] ^ block_header[77];
            final_init_img[99] = NEW_IMAGE_REFERANCE_BYTES[99] ^ block_header[77];
            final_init_img[103] = NEW_IMAGE_REFERANCE_BYTES[103] ^ block_header[77];
            final_init_img[107] = NEW_IMAGE_REFERANCE_BYTES[107] ^ block_header[77];
            final_init_img[111] = NEW_IMAGE_REFERANCE_BYTES[111] ^ block_header[77];
            final_init_img[115] = NEW_IMAGE_REFERANCE_BYTES[115] ^ block_header[77];
            final_init_img[119] = NEW_IMAGE_REFERANCE_BYTES[119] ^ block_header[77];
            final_init_img[123] = NEW_IMAGE_REFERANCE_BYTES[123] ^ block_header[77];
            final_init_img[127] = NEW_IMAGE_REFERANCE_BYTES[127] ^ block_header[77];
            final_init_img[131] = NEW_IMAGE_REFERANCE_BYTES[131] ^ block_header[77];
            final_init_img[135] = NEW_IMAGE_REFERANCE_BYTES[135] ^ block_header[77];
            final_init_img[139] = NEW_IMAGE_REFERANCE_BYTES[139] ^ block_header[77];
            final_init_img[143] = NEW_IMAGE_REFERANCE_BYTES[143] ^ block_header[77];
            final_init_img[147] = NEW_IMAGE_REFERANCE_BYTES[147] ^ block_header[77];
            final_init_img[151] = NEW_IMAGE_REFERANCE_BYTES[151] ^ block_header[77];
            final_init_img[155] = NEW_IMAGE_REFERANCE_BYTES[155] ^ block_header[77];
            final_init_img[159] = NEW_IMAGE_REFERANCE_BYTES[159] ^ block_header[77];
            final_init_img[163] = NEW_IMAGE_REFERANCE_BYTES[163] ^ block_header[77];
            final_init_img[167] = NEW_IMAGE_REFERANCE_BYTES[167] ^ block_header[77];
            final_init_img[171] = NEW_IMAGE_REFERANCE_BYTES[171] ^ block_header[77];
            final_init_img[175] = NEW_IMAGE_REFERANCE_BYTES[175] ^ block_header[77];
            final_init_img[179] = NEW_IMAGE_REFERANCE_BYTES[179] ^ block_header[77];
            final_init_img[183] = NEW_IMAGE_REFERANCE_BYTES[183] ^ block_header[77];
            final_init_img[187] = NEW_IMAGE_REFERANCE_BYTES[187] ^ block_header[77];
            final_init_img[191] = NEW_IMAGE_REFERANCE_BYTES[191] ^ block_header[77];
            final_init_img[195] = NEW_IMAGE_REFERANCE_BYTES[195] ^ block_header[77];
            final_init_img[199] = NEW_IMAGE_REFERANCE_BYTES[199] ^ block_header[77];
            final_init_img[203] = NEW_IMAGE_REFERANCE_BYTES[203] ^ block_header[77];
            final_init_img[207] = NEW_IMAGE_REFERANCE_BYTES[207] ^ block_header[77];
            final_init_img[211] = NEW_IMAGE_REFERANCE_BYTES[211] ^ block_header[77];
            final_init_img[215] = NEW_IMAGE_REFERANCE_BYTES[215] ^ block_header[77];
            final_init_img[219] = NEW_IMAGE_REFERANCE_BYTES[219] ^ block_header[77];
            final_init_img[223] = NEW_IMAGE_REFERANCE_BYTES[223] ^ block_header[77];
            final_init_img[227] = NEW_IMAGE_REFERANCE_BYTES[227] ^ block_header[77];
            final_init_img[231] = NEW_IMAGE_REFERANCE_BYTES[231] ^ block_header[77];
            final_init_img[235] = NEW_IMAGE_REFERANCE_BYTES[235] ^ block_header[77];
            final_init_img[239] = NEW_IMAGE_REFERANCE_BYTES[239] ^ block_header[77];
            final_init_img[243] = NEW_IMAGE_REFERANCE_BYTES[243] ^ block_header[77];
            final_init_img[247] = NEW_IMAGE_REFERANCE_BYTES[247] ^ block_header[77];
            final_init_img[251] = NEW_IMAGE_REFERANCE_BYTES[251] ^ block_header[77];
            final_init_img[255] = NEW_IMAGE_REFERANCE_BYTES[255] ^ block_header[77];
            final_init_img[259] = NEW_IMAGE_REFERANCE_BYTES[259] ^ block_header[77];
            final_init_img[263] = NEW_IMAGE_REFERANCE_BYTES[263] ^ block_header[77];
            final_init_img[267] = NEW_IMAGE_REFERANCE_BYTES[267] ^ block_header[77];
            final_init_img[271] = NEW_IMAGE_REFERANCE_BYTES[271] ^ block_header[77];
            final_init_img[275] = NEW_IMAGE_REFERANCE_BYTES[275] ^ block_header[77];
            final_init_img[279] = NEW_IMAGE_REFERANCE_BYTES[279] ^ block_header[77];
            final_init_img[283] = NEW_IMAGE_REFERANCE_BYTES[283] ^ block_header[77];
            final_init_img[287] = NEW_IMAGE_REFERANCE_BYTES[287] ^ block_header[77];
            final_init_img[291] = NEW_IMAGE_REFERANCE_BYTES[291] ^ block_header[77];
            final_init_img[295] = NEW_IMAGE_REFERANCE_BYTES[295] ^ block_header[77];
            final_init_img[299] = NEW_IMAGE_REFERANCE_BYTES[299] ^ block_header[77];
            final_init_img[303] = NEW_IMAGE_REFERANCE_BYTES[303] ^ block_header[77];
            final_init_img[307] = NEW_IMAGE_REFERANCE_BYTES[307] ^ block_header[77];
            final_init_img[311] = NEW_IMAGE_REFERANCE_BYTES[311] ^ block_header[77];
            final_init_img[315] = NEW_IMAGE_REFERANCE_BYTES[315] ^ block_header[77];
            final_init_img[319] = NEW_IMAGE_REFERANCE_BYTES[319] ^ block_header[77];
            final_init_img[323] = NEW_IMAGE_REFERANCE_BYTES[323] ^ block_header[77];
            final_init_img[327] = NEW_IMAGE_REFERANCE_BYTES[327] ^ block_header[77];
            final_init_img[331] = NEW_IMAGE_REFERANCE_BYTES[331] ^ block_header[77];
            final_init_img[335] = NEW_IMAGE_REFERANCE_BYTES[335] ^ block_header[77];
            final_init_img[339] = NEW_IMAGE_REFERANCE_BYTES[339] ^ block_header[77];
            final_init_img[343] = NEW_IMAGE_REFERANCE_BYTES[343] ^ block_header[77];
            final_init_img[347] = NEW_IMAGE_REFERANCE_BYTES[347] ^ block_header[77];
            final_init_img[351] = NEW_IMAGE_REFERANCE_BYTES[351] ^ block_header[77];
            final_init_img[355] = NEW_IMAGE_REFERANCE_BYTES[355] ^ block_header[77];
            final_init_img[359] = NEW_IMAGE_REFERANCE_BYTES[359] ^ block_header[77];
            final_init_img[363] = NEW_IMAGE_REFERANCE_BYTES[363] ^ block_header[77];
            final_init_img[367] = NEW_IMAGE_REFERANCE_BYTES[367] ^ block_header[77];
            final_init_img[371] = NEW_IMAGE_REFERANCE_BYTES[371] ^ block_header[77];
            final_init_img[375] = NEW_IMAGE_REFERANCE_BYTES[375] ^ block_header[77];
            final_init_img[379] = NEW_IMAGE_REFERANCE_BYTES[379] ^ block_header[77];
            final_init_img[383] = NEW_IMAGE_REFERANCE_BYTES[383] ^ block_header[77];
            final_init_img[387] = NEW_IMAGE_REFERANCE_BYTES[387] ^ block_header[77];
            final_init_img[391] = NEW_IMAGE_REFERANCE_BYTES[391] ^ block_header[77];
            final_init_img[395] = NEW_IMAGE_REFERANCE_BYTES[395] ^ block_header[77];
            final_init_img[399] = NEW_IMAGE_REFERANCE_BYTES[399] ^ block_header[77];
            final_init_img[403] = NEW_IMAGE_REFERANCE_BYTES[403] ^ block_header[77];
            final_init_img[407] = NEW_IMAGE_REFERANCE_BYTES[407] ^ block_header[77];
            final_init_img[411] = NEW_IMAGE_REFERANCE_BYTES[411] ^ block_header[77];
            final_init_img[415] = NEW_IMAGE_REFERANCE_BYTES[415] ^ block_header[77];
            final_init_img[419] = NEW_IMAGE_REFERANCE_BYTES[419] ^ block_header[77];
            final_init_img[423] = NEW_IMAGE_REFERANCE_BYTES[423] ^ block_header[77];
            final_init_img[427] = NEW_IMAGE_REFERANCE_BYTES[427] ^ block_header[77];
            final_init_img[431] = NEW_IMAGE_REFERANCE_BYTES[431] ^ block_header[77];
            final_init_img[435] = NEW_IMAGE_REFERANCE_BYTES[435] ^ block_header[77];
            final_init_img[439] = NEW_IMAGE_REFERANCE_BYTES[439] ^ block_header[77];
            final_init_img[443] = NEW_IMAGE_REFERANCE_BYTES[443] ^ block_header[77];
            final_init_img[447] = NEW_IMAGE_REFERANCE_BYTES[447] ^ block_header[77];
            final_init_img[451] = NEW_IMAGE_REFERANCE_BYTES[451] ^ block_header[77];
            final_init_img[455] = NEW_IMAGE_REFERANCE_BYTES[455] ^ block_header[77];
            final_init_img[459] = NEW_IMAGE_REFERANCE_BYTES[459] ^ block_header[77];
            final_init_img[463] = NEW_IMAGE_REFERANCE_BYTES[463] ^ block_header[77];
            final_init_img[467] = NEW_IMAGE_REFERANCE_BYTES[467] ^ block_header[77];
            final_init_img[471] = NEW_IMAGE_REFERANCE_BYTES[471] ^ block_header[77];
            final_init_img[475] = NEW_IMAGE_REFERANCE_BYTES[475] ^ block_header[77];
            final_init_img[479] = NEW_IMAGE_REFERANCE_BYTES[479] ^ block_header[77];
            final_init_img[483] = NEW_IMAGE_REFERANCE_BYTES[483] ^ block_header[77];
            final_init_img[487] = NEW_IMAGE_REFERANCE_BYTES[487] ^ block_header[77];
            final_init_img[491] = NEW_IMAGE_REFERANCE_BYTES[491] ^ block_header[77];
            final_init_img[495] = NEW_IMAGE_REFERANCE_BYTES[495] ^ block_header[77];
            final_init_img[499] = NEW_IMAGE_REFERANCE_BYTES[499] ^ block_header[77];
            final_init_img[503] = NEW_IMAGE_REFERANCE_BYTES[503] ^ block_header[77];
            final_init_img[507] = NEW_IMAGE_REFERANCE_BYTES[507] ^ block_header[77];
            final_init_img[511] = NEW_IMAGE_REFERANCE_BYTES[511] ^ block_header[77];
            final_init_img[515] = NEW_IMAGE_REFERANCE_BYTES[515] ^ block_header[77];
            final_init_img[519] = NEW_IMAGE_REFERANCE_BYTES[519] ^ block_header[77];
            final_init_img[523] = NEW_IMAGE_REFERANCE_BYTES[523] ^ block_header[77];
            final_init_img[527] = NEW_IMAGE_REFERANCE_BYTES[527] ^ block_header[77];
            final_init_img[531] = NEW_IMAGE_REFERANCE_BYTES[531] ^ block_header[77];
            final_init_img[535] = NEW_IMAGE_REFERANCE_BYTES[535] ^ block_header[77];
            final_init_img[539] = NEW_IMAGE_REFERANCE_BYTES[539] ^ block_header[77];
            final_init_img[543] = NEW_IMAGE_REFERANCE_BYTES[543] ^ block_header[77];
            final_init_img[547] = NEW_IMAGE_REFERANCE_BYTES[547] ^ block_header[77];
            final_init_img[551] = NEW_IMAGE_REFERANCE_BYTES[551] ^ block_header[77];
            final_init_img[555] = NEW_IMAGE_REFERANCE_BYTES[555] ^ block_header[77];
            final_init_img[559] = NEW_IMAGE_REFERANCE_BYTES[559] ^ block_header[77];
            final_init_img[563] = NEW_IMAGE_REFERANCE_BYTES[563] ^ block_header[77];
            final_init_img[567] = NEW_IMAGE_REFERANCE_BYTES[567] ^ block_header[77];
            final_init_img[571] = NEW_IMAGE_REFERANCE_BYTES[571] ^ block_header[77];
            final_init_img[575] = NEW_IMAGE_REFERANCE_BYTES[575] ^ block_header[77];
            final_init_img[579] = NEW_IMAGE_REFERANCE_BYTES[579] ^ block_header[77];
            final_init_img[583] = NEW_IMAGE_REFERANCE_BYTES[583] ^ block_header[77];
            final_init_img[587] = NEW_IMAGE_REFERANCE_BYTES[587] ^ block_header[77];
            final_init_img[591] = NEW_IMAGE_REFERANCE_BYTES[591] ^ block_header[77];
            final_init_img[595] = NEW_IMAGE_REFERANCE_BYTES[595] ^ block_header[77];
            final_init_img[599] = NEW_IMAGE_REFERANCE_BYTES[599] ^ block_header[77];
            final_init_img[603] = NEW_IMAGE_REFERANCE_BYTES[603] ^ block_header[77];
            final_init_img[607] = NEW_IMAGE_REFERANCE_BYTES[607] ^ block_header[77];
            final_init_img[611] = NEW_IMAGE_REFERANCE_BYTES[611] ^ block_header[77];
            final_init_img[615] = NEW_IMAGE_REFERANCE_BYTES[615] ^ block_header[77];
            final_init_img[619] = NEW_IMAGE_REFERANCE_BYTES[619] ^ block_header[77];
            final_init_img[623] = NEW_IMAGE_REFERANCE_BYTES[623] ^ block_header[77];
            final_init_img[627] = NEW_IMAGE_REFERANCE_BYTES[627] ^ block_header[77];
            final_init_img[631] = NEW_IMAGE_REFERANCE_BYTES[631] ^ block_header[77];
            final_init_img[635] = NEW_IMAGE_REFERANCE_BYTES[635] ^ block_header[77];
            final_init_img[639] = NEW_IMAGE_REFERANCE_BYTES[639] ^ block_header[77];
            final_init_img[643] = NEW_IMAGE_REFERANCE_BYTES[643] ^ block_header[77];
            final_init_img[647] = NEW_IMAGE_REFERANCE_BYTES[647] ^ block_header[77];
            final_init_img[651] = NEW_IMAGE_REFERANCE_BYTES[651] ^ block_header[77];
            final_init_img[655] = NEW_IMAGE_REFERANCE_BYTES[655] ^ block_header[77];
            final_init_img[659] = NEW_IMAGE_REFERANCE_BYTES[659] ^ block_header[77];
            final_init_img[663] = NEW_IMAGE_REFERANCE_BYTES[663] ^ block_header[77];
            final_init_img[667] = NEW_IMAGE_REFERANCE_BYTES[667] ^ block_header[77];
            final_init_img[671] = NEW_IMAGE_REFERANCE_BYTES[671] ^ block_header[77];
            final_init_img[675] = NEW_IMAGE_REFERANCE_BYTES[675] ^ block_header[77];
            final_init_img[679] = NEW_IMAGE_REFERANCE_BYTES[679] ^ block_header[77];
            final_init_img[683] = NEW_IMAGE_REFERANCE_BYTES[683] ^ block_header[77];
            final_init_img[687] = NEW_IMAGE_REFERANCE_BYTES[687] ^ block_header[77];
            final_init_img[691] = NEW_IMAGE_REFERANCE_BYTES[691] ^ block_header[77];
            final_init_img[695] = NEW_IMAGE_REFERANCE_BYTES[695] ^ block_header[77];
            final_init_img[699] = NEW_IMAGE_REFERANCE_BYTES[699] ^ block_header[77];
            final_init_img[703] = NEW_IMAGE_REFERANCE_BYTES[703] ^ block_header[77];
            final_init_img[707] = NEW_IMAGE_REFERANCE_BYTES[707] ^ block_header[77];
            final_init_img[711] = NEW_IMAGE_REFERANCE_BYTES[711] ^ block_header[77];
            final_init_img[715] = NEW_IMAGE_REFERANCE_BYTES[715] ^ block_header[77];
            final_init_img[719] = NEW_IMAGE_REFERANCE_BYTES[719] ^ block_header[77];
            final_init_img[723] = NEW_IMAGE_REFERANCE_BYTES[723] ^ block_header[77];
            final_init_img[727] = NEW_IMAGE_REFERANCE_BYTES[727] ^ block_header[77];
            final_init_img[731] = NEW_IMAGE_REFERANCE_BYTES[731] ^ block_header[77];
            final_init_img[735] = NEW_IMAGE_REFERANCE_BYTES[735] ^ block_header[77];
            final_init_img[739] = NEW_IMAGE_REFERANCE_BYTES[739] ^ block_header[77];
            final_init_img[743] = NEW_IMAGE_REFERANCE_BYTES[743] ^ block_header[77];
            final_init_img[747] = NEW_IMAGE_REFERANCE_BYTES[747] ^ block_header[77];
            final_init_img[751] = NEW_IMAGE_REFERANCE_BYTES[751] ^ block_header[77];
            final_init_img[755] = NEW_IMAGE_REFERANCE_BYTES[755] ^ block_header[77];
            final_init_img[759] = NEW_IMAGE_REFERANCE_BYTES[759] ^ block_header[77];
            final_init_img[763] = NEW_IMAGE_REFERANCE_BYTES[763] ^ block_header[77];
            final_init_img[767] = NEW_IMAGE_REFERANCE_BYTES[767] ^ block_header[77];
            final_init_img[771] = NEW_IMAGE_REFERANCE_BYTES[771] ^ block_header[77];
            final_init_img[775] = NEW_IMAGE_REFERANCE_BYTES[775] ^ block_header[77];
            final_init_img[779] = NEW_IMAGE_REFERANCE_BYTES[779] ^ block_header[77];
            final_init_img[783] = NEW_IMAGE_REFERANCE_BYTES[783] ^ block_header[77];
            final_init_img[787] = NEW_IMAGE_REFERANCE_BYTES[787] ^ block_header[77];
            final_init_img[791] = NEW_IMAGE_REFERANCE_BYTES[791] ^ block_header[77];
            final_init_img[795] = NEW_IMAGE_REFERANCE_BYTES[795] ^ block_header[77];
            final_init_img[799] = NEW_IMAGE_REFERANCE_BYTES[799] ^ block_header[77];
            final_init_img[803] = NEW_IMAGE_REFERANCE_BYTES[803] ^ block_header[77];
            final_init_img[807] = NEW_IMAGE_REFERANCE_BYTES[807] ^ block_header[77];
            final_init_img[811] = NEW_IMAGE_REFERANCE_BYTES[811] ^ block_header[77];
            final_init_img[815] = NEW_IMAGE_REFERANCE_BYTES[815] ^ block_header[77];
            final_init_img[819] = NEW_IMAGE_REFERANCE_BYTES[819] ^ block_header[77];
            final_init_img[823] = NEW_IMAGE_REFERANCE_BYTES[823] ^ block_header[77];
            final_init_img[827] = NEW_IMAGE_REFERANCE_BYTES[827] ^ block_header[77];
            final_init_img[831] = NEW_IMAGE_REFERANCE_BYTES[831] ^ block_header[77];
            final_init_img[835] = NEW_IMAGE_REFERANCE_BYTES[835] ^ block_header[77];
            final_init_img[839] = NEW_IMAGE_REFERANCE_BYTES[839] ^ block_header[77];
            final_init_img[843] = NEW_IMAGE_REFERANCE_BYTES[843] ^ block_header[77];
            final_init_img[847] = NEW_IMAGE_REFERANCE_BYTES[847] ^ block_header[77];
            final_init_img[851] = NEW_IMAGE_REFERANCE_BYTES[851] ^ block_header[77];
            final_init_img[855] = NEW_IMAGE_REFERANCE_BYTES[855] ^ block_header[77];
            final_init_img[859] = NEW_IMAGE_REFERANCE_BYTES[859] ^ block_header[77];
            final_init_img[863] = NEW_IMAGE_REFERANCE_BYTES[863] ^ block_header[77];
            final_init_img[867] = NEW_IMAGE_REFERANCE_BYTES[867] ^ block_header[77];
            final_init_img[871] = NEW_IMAGE_REFERANCE_BYTES[871] ^ block_header[77];
            final_init_img[875] = NEW_IMAGE_REFERANCE_BYTES[875] ^ block_header[77];
            final_init_img[879] = NEW_IMAGE_REFERANCE_BYTES[879] ^ block_header[77];
            final_init_img[883] = NEW_IMAGE_REFERANCE_BYTES[883] ^ block_header[77];
            final_init_img[887] = NEW_IMAGE_REFERANCE_BYTES[887] ^ block_header[77];
            final_init_img[891] = NEW_IMAGE_REFERANCE_BYTES[891] ^ block_header[77];
            final_init_img[895] = NEW_IMAGE_REFERANCE_BYTES[895] ^ block_header[77];
            final_init_img[899] = NEW_IMAGE_REFERANCE_BYTES[899] ^ block_header[77];
            final_init_img[903] = NEW_IMAGE_REFERANCE_BYTES[903] ^ block_header[77];
            final_init_img[907] = NEW_IMAGE_REFERANCE_BYTES[907] ^ block_header[77];
            final_init_img[911] = NEW_IMAGE_REFERANCE_BYTES[911] ^ block_header[77];
            final_init_img[915] = NEW_IMAGE_REFERANCE_BYTES[915] ^ block_header[77];
            final_init_img[919] = NEW_IMAGE_REFERANCE_BYTES[919] ^ block_header[77];
            final_init_img[923] = NEW_IMAGE_REFERANCE_BYTES[923] ^ block_header[77];
            final_init_img[927] = NEW_IMAGE_REFERANCE_BYTES[927] ^ block_header[77];
            final_init_img[931] = NEW_IMAGE_REFERANCE_BYTES[931] ^ block_header[77];
            final_init_img[935] = NEW_IMAGE_REFERANCE_BYTES[935] ^ block_header[77];
            final_init_img[939] = NEW_IMAGE_REFERANCE_BYTES[939] ^ block_header[77];
            final_init_img[943] = NEW_IMAGE_REFERANCE_BYTES[943] ^ block_header[77];
            final_init_img[947] = NEW_IMAGE_REFERANCE_BYTES[947] ^ block_header[77];
            final_init_img[951] = NEW_IMAGE_REFERANCE_BYTES[951] ^ block_header[77];
            final_init_img[955] = NEW_IMAGE_REFERANCE_BYTES[955] ^ block_header[77];
            final_init_img[959] = NEW_IMAGE_REFERANCE_BYTES[959] ^ block_header[77];
            final_init_img[963] = NEW_IMAGE_REFERANCE_BYTES[963] ^ block_header[77];
            final_init_img[967] = NEW_IMAGE_REFERANCE_BYTES[967] ^ block_header[77];
            final_init_img[971] = NEW_IMAGE_REFERANCE_BYTES[971] ^ block_header[77];
            final_init_img[975] = NEW_IMAGE_REFERANCE_BYTES[975] ^ block_header[77];
            final_init_img[979] = NEW_IMAGE_REFERANCE_BYTES[979] ^ block_header[77];
            final_init_img[983] = NEW_IMAGE_REFERANCE_BYTES[983] ^ block_header[77];
            final_init_img[987] = NEW_IMAGE_REFERANCE_BYTES[987] ^ block_header[77];
            final_init_img[991] = NEW_IMAGE_REFERANCE_BYTES[991] ^ block_header[77];
            final_init_img[995] = NEW_IMAGE_REFERANCE_BYTES[995] ^ block_header[77];
            final_init_img[999] = NEW_IMAGE_REFERANCE_BYTES[999] ^ block_header[77];
            final_init_img[1003] = NEW_IMAGE_REFERANCE_BYTES[1003] ^ block_header[77];
            final_init_img[1007] = NEW_IMAGE_REFERANCE_BYTES[1007] ^ block_header[77];
            final_init_img[1011] = NEW_IMAGE_REFERANCE_BYTES[1011] ^ block_header[77];
            final_init_img[1015] = NEW_IMAGE_REFERANCE_BYTES[1015] ^ block_header[77];
            final_init_img[1019] = NEW_IMAGE_REFERANCE_BYTES[1019] ^ block_header[77];
            final_init_img[1023] = NEW_IMAGE_REFERANCE_BYTES[1023] ^ block_header[77];
            final_init_img[1027] = NEW_IMAGE_REFERANCE_BYTES[1027] ^ block_header[77];
            final_init_img[1031] = NEW_IMAGE_REFERANCE_BYTES[1031] ^ block_header[77];
            final_init_img[1035] = NEW_IMAGE_REFERANCE_BYTES[1035] ^ block_header[77];
            final_init_img[1039] = NEW_IMAGE_REFERANCE_BYTES[1039] ^ block_header[77];
            final_init_img[1043] = NEW_IMAGE_REFERANCE_BYTES[1043] ^ block_header[77];
            final_init_img[1047] = NEW_IMAGE_REFERANCE_BYTES[1047] ^ block_header[77];
            final_init_img[1051] = NEW_IMAGE_REFERANCE_BYTES[1051] ^ block_header[77];
            final_init_img[1055] = NEW_IMAGE_REFERANCE_BYTES[1055] ^ block_header[77];
            final_init_img[1059] = NEW_IMAGE_REFERANCE_BYTES[1059] ^ block_header[77];
            final_init_img[1063] = NEW_IMAGE_REFERANCE_BYTES[1063] ^ block_header[77];
            final_init_img[1067] = NEW_IMAGE_REFERANCE_BYTES[1067] ^ block_header[77];
            final_init_img[1071] = NEW_IMAGE_REFERANCE_BYTES[1071] ^ block_header[77];
            final_init_img[1075] = NEW_IMAGE_REFERANCE_BYTES[1075] ^ block_header[77];
            final_init_img[1079] = NEW_IMAGE_REFERANCE_BYTES[1079] ^ block_header[77];
            final_init_img[1083] = NEW_IMAGE_REFERANCE_BYTES[1083] ^ block_header[77];
            final_init_img[1087] = NEW_IMAGE_REFERANCE_BYTES[1087] ^ block_header[77];
            final_init_img[1091] = NEW_IMAGE_REFERANCE_BYTES[1091] ^ block_header[77];
            final_init_img[1095] = NEW_IMAGE_REFERANCE_BYTES[1095] ^ block_header[77];
            final_init_img[1099] = NEW_IMAGE_REFERANCE_BYTES[1099] ^ block_header[77];
            final_init_img[1103] = NEW_IMAGE_REFERANCE_BYTES[1103] ^ block_header[77];
            final_init_img[1107] = NEW_IMAGE_REFERANCE_BYTES[1107] ^ block_header[77];
            final_init_img[1111] = NEW_IMAGE_REFERANCE_BYTES[1111] ^ block_header[77];
            final_init_img[1115] = NEW_IMAGE_REFERANCE_BYTES[1115] ^ block_header[77];
            final_init_img[1119] = NEW_IMAGE_REFERANCE_BYTES[1119] ^ block_header[77];
            final_init_img[1123] = NEW_IMAGE_REFERANCE_BYTES[1123] ^ block_header[77];
            final_init_img[1127] = NEW_IMAGE_REFERANCE_BYTES[1127] ^ block_header[77];
            final_init_img[1131] = NEW_IMAGE_REFERANCE_BYTES[1131] ^ block_header[77];
            final_init_img[1135] = NEW_IMAGE_REFERANCE_BYTES[1135] ^ block_header[77];
            final_init_img[1139] = NEW_IMAGE_REFERANCE_BYTES[1139] ^ block_header[77];
            final_init_img[1143] = NEW_IMAGE_REFERANCE_BYTES[1143] ^ block_header[77];
            final_init_img[1147] = NEW_IMAGE_REFERANCE_BYTES[1147] ^ block_header[77];
            final_init_img[1151] = NEW_IMAGE_REFERANCE_BYTES[1151] ^ block_header[77];
            final_init_img[1155] = NEW_IMAGE_REFERANCE_BYTES[1155] ^ block_header[77];
            final_init_img[1159] = NEW_IMAGE_REFERANCE_BYTES[1159] ^ block_header[77];
            final_init_img[1163] = NEW_IMAGE_REFERANCE_BYTES[1163] ^ block_header[77];
            final_init_img[1167] = NEW_IMAGE_REFERANCE_BYTES[1167] ^ block_header[77];
            final_init_img[1171] = NEW_IMAGE_REFERANCE_BYTES[1171] ^ block_header[77];
            final_init_img[1175] = NEW_IMAGE_REFERANCE_BYTES[1175] ^ block_header[77];
            final_init_img[1179] = NEW_IMAGE_REFERANCE_BYTES[1179] ^ block_header[77];
            final_init_img[1183] = NEW_IMAGE_REFERANCE_BYTES[1183] ^ block_header[77];
            final_init_img[1187] = NEW_IMAGE_REFERANCE_BYTES[1187] ^ block_header[77];
            final_init_img[1191] = NEW_IMAGE_REFERANCE_BYTES[1191] ^ block_header[77];
            final_init_img[1195] = NEW_IMAGE_REFERANCE_BYTES[1195] ^ block_header[77];
            final_init_img[1199] = NEW_IMAGE_REFERANCE_BYTES[1199] ^ block_header[77];
            final_init_img[1203] = NEW_IMAGE_REFERANCE_BYTES[1203] ^ block_header[77];
            final_init_img[1207] = NEW_IMAGE_REFERANCE_BYTES[1207] ^ block_header[77];
            final_init_img[1211] = NEW_IMAGE_REFERANCE_BYTES[1211] ^ block_header[77];
            final_init_img[1215] = NEW_IMAGE_REFERANCE_BYTES[1215] ^ block_header[77];
            final_init_img[1219] = NEW_IMAGE_REFERANCE_BYTES[1219] ^ block_header[77];
            final_init_img[1223] = NEW_IMAGE_REFERANCE_BYTES[1223] ^ block_header[77];
            final_init_img[1227] = NEW_IMAGE_REFERANCE_BYTES[1227] ^ block_header[77];
            final_init_img[1231] = NEW_IMAGE_REFERANCE_BYTES[1231] ^ block_header[77];
            final_init_img[1235] = NEW_IMAGE_REFERANCE_BYTES[1235] ^ block_header[77];
            final_init_img[1239] = NEW_IMAGE_REFERANCE_BYTES[1239] ^ block_header[77];
            final_init_img[1243] = NEW_IMAGE_REFERANCE_BYTES[1243] ^ block_header[77];
            final_init_img[1247] = NEW_IMAGE_REFERANCE_BYTES[1247] ^ block_header[77];
            final_init_img[1251] = NEW_IMAGE_REFERANCE_BYTES[1251] ^ block_header[77];
            final_init_img[1255] = NEW_IMAGE_REFERANCE_BYTES[1255] ^ block_header[77];
            final_init_img[1259] = NEW_IMAGE_REFERANCE_BYTES[1259] ^ block_header[77];
            final_init_img[1263] = NEW_IMAGE_REFERANCE_BYTES[1263] ^ block_header[77];
            final_init_img[1267] = NEW_IMAGE_REFERANCE_BYTES[1267] ^ block_header[77];
            final_init_img[1271] = NEW_IMAGE_REFERANCE_BYTES[1271] ^ block_header[77];
            final_init_img[1275] = NEW_IMAGE_REFERANCE_BYTES[1275] ^ block_header[77];
            final_init_img[1279] = NEW_IMAGE_REFERANCE_BYTES[1279] ^ block_header[77];
            final_init_img[1283] = NEW_IMAGE_REFERANCE_BYTES[1283] ^ block_header[77];
            final_init_img[1287] = NEW_IMAGE_REFERANCE_BYTES[1287] ^ block_header[77];
            final_init_img[1291] = NEW_IMAGE_REFERANCE_BYTES[1291] ^ block_header[77];
            final_init_img[1295] = NEW_IMAGE_REFERANCE_BYTES[1295] ^ block_header[77];
            final_init_img[1299] = NEW_IMAGE_REFERANCE_BYTES[1299] ^ block_header[77];
            final_init_img[1303] = NEW_IMAGE_REFERANCE_BYTES[1303] ^ block_header[77];
            final_init_img[1307] = NEW_IMAGE_REFERANCE_BYTES[1307] ^ block_header[77];
            final_init_img[1311] = NEW_IMAGE_REFERANCE_BYTES[1311] ^ block_header[77];
            final_init_img[1315] = NEW_IMAGE_REFERANCE_BYTES[1315] ^ block_header[77];
            final_init_img[1319] = NEW_IMAGE_REFERANCE_BYTES[1319] ^ block_header[77];
            final_init_img[1323] = NEW_IMAGE_REFERANCE_BYTES[1323] ^ block_header[77];
            final_init_img[1327] = NEW_IMAGE_REFERANCE_BYTES[1327] ^ block_header[77];
            final_init_img[1331] = NEW_IMAGE_REFERANCE_BYTES[1331] ^ block_header[77];
            final_init_img[1335] = NEW_IMAGE_REFERANCE_BYTES[1335] ^ block_header[77];
            final_init_img[1339] = NEW_IMAGE_REFERANCE_BYTES[1339] ^ block_header[77];
            final_init_img[1343] = NEW_IMAGE_REFERANCE_BYTES[1343] ^ block_header[77];
            final_init_img[1347] = NEW_IMAGE_REFERANCE_BYTES[1347] ^ block_header[77];
            final_init_img[1351] = NEW_IMAGE_REFERANCE_BYTES[1351] ^ block_header[77];
            final_init_img[1355] = NEW_IMAGE_REFERANCE_BYTES[1355] ^ block_header[77];
            final_init_img[1359] = NEW_IMAGE_REFERANCE_BYTES[1359] ^ block_header[77];
            final_init_img[1363] = NEW_IMAGE_REFERANCE_BYTES[1363] ^ block_header[77];
            final_init_img[1367] = NEW_IMAGE_REFERANCE_BYTES[1367] ^ block_header[77];
            final_init_img[1371] = NEW_IMAGE_REFERANCE_BYTES[1371] ^ block_header[77];
            final_init_img[1375] = NEW_IMAGE_REFERANCE_BYTES[1375] ^ block_header[77];
            final_init_img[1379] = NEW_IMAGE_REFERANCE_BYTES[1379] ^ block_header[77];
            final_init_img[1383] = NEW_IMAGE_REFERANCE_BYTES[1383] ^ block_header[77];
            final_init_img[1387] = NEW_IMAGE_REFERANCE_BYTES[1387] ^ block_header[77];
            final_init_img[1391] = NEW_IMAGE_REFERANCE_BYTES[1391] ^ block_header[77];
            final_init_img[1395] = NEW_IMAGE_REFERANCE_BYTES[1395] ^ block_header[77];
            final_init_img[1399] = NEW_IMAGE_REFERANCE_BYTES[1399] ^ block_header[77];
            final_init_img[1403] = NEW_IMAGE_REFERANCE_BYTES[1403] ^ block_header[77];
            final_init_img[1407] = NEW_IMAGE_REFERANCE_BYTES[1407] ^ block_header[77];
            final_init_img[1411] = NEW_IMAGE_REFERANCE_BYTES[1411] ^ block_header[77];
            final_init_img[1415] = NEW_IMAGE_REFERANCE_BYTES[1415] ^ block_header[77];
            final_init_img[1419] = NEW_IMAGE_REFERANCE_BYTES[1419] ^ block_header[77];
            final_init_img[1423] = NEW_IMAGE_REFERANCE_BYTES[1423] ^ block_header[77];
            final_init_img[1427] = NEW_IMAGE_REFERANCE_BYTES[1427] ^ block_header[77];
            final_init_img[1431] = NEW_IMAGE_REFERANCE_BYTES[1431] ^ block_header[77];
            final_init_img[1435] = NEW_IMAGE_REFERANCE_BYTES[1435] ^ block_header[77];
            final_init_img[1439] = NEW_IMAGE_REFERANCE_BYTES[1439] ^ block_header[77];
            final_init_img[1443] = NEW_IMAGE_REFERANCE_BYTES[1443] ^ block_header[77];
            final_init_img[1447] = NEW_IMAGE_REFERANCE_BYTES[1447] ^ block_header[77];
            final_init_img[1451] = NEW_IMAGE_REFERANCE_BYTES[1451] ^ block_header[77];
            final_init_img[1455] = NEW_IMAGE_REFERANCE_BYTES[1455] ^ block_header[77];
            final_init_img[1459] = NEW_IMAGE_REFERANCE_BYTES[1459] ^ block_header[77];
            final_init_img[1463] = NEW_IMAGE_REFERANCE_BYTES[1463] ^ block_header[77];
            final_init_img[1467] = NEW_IMAGE_REFERANCE_BYTES[1467] ^ block_header[77];
            final_init_img[1471] = NEW_IMAGE_REFERANCE_BYTES[1471] ^ block_header[77];
            final_init_img[1475] = NEW_IMAGE_REFERANCE_BYTES[1475] ^ block_header[77];
            final_init_img[1479] = NEW_IMAGE_REFERANCE_BYTES[1479] ^ block_header[77];
            final_init_img[1483] = NEW_IMAGE_REFERANCE_BYTES[1483] ^ block_header[77];
            final_init_img[1487] = NEW_IMAGE_REFERANCE_BYTES[1487] ^ block_header[77];
            final_init_img[1491] = NEW_IMAGE_REFERANCE_BYTES[1491] ^ block_header[77];
            final_init_img[1495] = NEW_IMAGE_REFERANCE_BYTES[1495] ^ block_header[77];
            final_init_img[1499] = NEW_IMAGE_REFERANCE_BYTES[1499] ^ block_header[77];
            final_init_img[1503] = NEW_IMAGE_REFERANCE_BYTES[1503] ^ block_header[77];
            final_init_img[1507] = NEW_IMAGE_REFERANCE_BYTES[1507] ^ block_header[77];
            final_init_img[1511] = NEW_IMAGE_REFERANCE_BYTES[1511] ^ block_header[77];
            final_init_img[1515] = NEW_IMAGE_REFERANCE_BYTES[1515] ^ block_header[77];
            final_init_img[1519] = NEW_IMAGE_REFERANCE_BYTES[1519] ^ block_header[77];
            final_init_img[1523] = NEW_IMAGE_REFERANCE_BYTES[1523] ^ block_header[77];
            final_init_img[1527] = NEW_IMAGE_REFERANCE_BYTES[1527] ^ block_header[77];
            final_init_img[1531] = NEW_IMAGE_REFERANCE_BYTES[1531] ^ block_header[77];
            final_init_img[1535] = NEW_IMAGE_REFERANCE_BYTES[1535] ^ block_header[77];
            final_init_img[1539] = NEW_IMAGE_REFERANCE_BYTES[1539] ^ block_header[77];
            final_init_img[1543] = NEW_IMAGE_REFERANCE_BYTES[1543] ^ block_header[77];
            final_init_img[1547] = NEW_IMAGE_REFERANCE_BYTES[1547] ^ block_header[77];
            final_init_img[1551] = NEW_IMAGE_REFERANCE_BYTES[1551] ^ block_header[77];
            final_init_img[1555] = NEW_IMAGE_REFERANCE_BYTES[1555] ^ block_header[77];
            final_init_img[1559] = NEW_IMAGE_REFERANCE_BYTES[1559] ^ block_header[77];
            final_init_img[1563] = NEW_IMAGE_REFERANCE_BYTES[1563] ^ block_header[77];
            final_init_img[1567] = NEW_IMAGE_REFERANCE_BYTES[1567] ^ block_header[77];
            final_init_img[1571] = NEW_IMAGE_REFERANCE_BYTES[1571] ^ block_header[77];
            final_init_img[1575] = NEW_IMAGE_REFERANCE_BYTES[1575] ^ block_header[77];
            final_init_img[1579] = NEW_IMAGE_REFERANCE_BYTES[1579] ^ block_header[77];
            final_init_img[1583] = NEW_IMAGE_REFERANCE_BYTES[1583] ^ block_header[77];
            final_init_img[1587] = NEW_IMAGE_REFERANCE_BYTES[1587] ^ block_header[77];
            final_init_img[1591] = NEW_IMAGE_REFERANCE_BYTES[1591] ^ block_header[77];
            final_init_img[1595] = NEW_IMAGE_REFERANCE_BYTES[1595] ^ block_header[77];
            final_init_img[1599] = NEW_IMAGE_REFERANCE_BYTES[1599] ^ block_header[77];
            final_init_img[1603] = NEW_IMAGE_REFERANCE_BYTES[1603] ^ block_header[77];
            final_init_img[1607] = NEW_IMAGE_REFERANCE_BYTES[1607] ^ block_header[77];
            final_init_img[1611] = NEW_IMAGE_REFERANCE_BYTES[1611] ^ block_header[77];
            final_init_img[1615] = NEW_IMAGE_REFERANCE_BYTES[1615] ^ block_header[77];
            final_init_img[1619] = NEW_IMAGE_REFERANCE_BYTES[1619] ^ block_header[77];
            final_init_img[1623] = NEW_IMAGE_REFERANCE_BYTES[1623] ^ block_header[77];
            final_init_img[1627] = NEW_IMAGE_REFERANCE_BYTES[1627] ^ block_header[77];
            final_init_img[1631] = NEW_IMAGE_REFERANCE_BYTES[1631] ^ block_header[77];
            final_init_img[1635] = NEW_IMAGE_REFERANCE_BYTES[1635] ^ block_header[77];
            final_init_img[1639] = NEW_IMAGE_REFERANCE_BYTES[1639] ^ block_header[77];
            final_init_img[1643] = NEW_IMAGE_REFERANCE_BYTES[1643] ^ block_header[77];
            final_init_img[1647] = NEW_IMAGE_REFERANCE_BYTES[1647] ^ block_header[77];
            final_init_img[1651] = NEW_IMAGE_REFERANCE_BYTES[1651] ^ block_header[77];
            final_init_img[1655] = NEW_IMAGE_REFERANCE_BYTES[1655] ^ block_header[77];
            final_init_img[1659] = NEW_IMAGE_REFERANCE_BYTES[1659] ^ block_header[77];
            final_init_img[1663] = NEW_IMAGE_REFERANCE_BYTES[1663] ^ block_header[77];
            final_init_img[1667] = NEW_IMAGE_REFERANCE_BYTES[1667] ^ block_header[77];
            final_init_img[1671] = NEW_IMAGE_REFERANCE_BYTES[1671] ^ block_header[77];
            final_init_img[1675] = NEW_IMAGE_REFERANCE_BYTES[1675] ^ block_header[77];
            final_init_img[1679] = NEW_IMAGE_REFERANCE_BYTES[1679] ^ block_header[77];
            final_init_img[1683] = NEW_IMAGE_REFERANCE_BYTES[1683] ^ block_header[77];
            final_init_img[1687] = NEW_IMAGE_REFERANCE_BYTES[1687] ^ block_header[77];
            final_init_img[1691] = NEW_IMAGE_REFERANCE_BYTES[1691] ^ block_header[77];
            final_init_img[1695] = NEW_IMAGE_REFERANCE_BYTES[1695] ^ block_header[77];
            final_init_img[1699] = NEW_IMAGE_REFERANCE_BYTES[1699] ^ block_header[77];
            final_init_img[1703] = NEW_IMAGE_REFERANCE_BYTES[1703] ^ block_header[77];
            final_init_img[1707] = NEW_IMAGE_REFERANCE_BYTES[1707] ^ block_header[77];
            final_init_img[1711] = NEW_IMAGE_REFERANCE_BYTES[1711] ^ block_header[77];
            final_init_img[1715] = NEW_IMAGE_REFERANCE_BYTES[1715] ^ block_header[77];
            final_init_img[1719] = NEW_IMAGE_REFERANCE_BYTES[1719] ^ block_header[77];
            final_init_img[1723] = NEW_IMAGE_REFERANCE_BYTES[1723] ^ block_header[77];
            final_init_img[1727] = NEW_IMAGE_REFERANCE_BYTES[1727] ^ block_header[77];
            final_init_img[1731] = NEW_IMAGE_REFERANCE_BYTES[1731] ^ block_header[77];
            final_init_img[1735] = NEW_IMAGE_REFERANCE_BYTES[1735] ^ block_header[77];
            final_init_img[1739] = NEW_IMAGE_REFERANCE_BYTES[1739] ^ block_header[77];
            final_init_img[1743] = NEW_IMAGE_REFERANCE_BYTES[1743] ^ block_header[77];
            final_init_img[1747] = NEW_IMAGE_REFERANCE_BYTES[1747] ^ block_header[77];
            final_init_img[1751] = NEW_IMAGE_REFERANCE_BYTES[1751] ^ block_header[77];
            final_init_img[1755] = NEW_IMAGE_REFERANCE_BYTES[1755] ^ block_header[77];
            final_init_img[1759] = NEW_IMAGE_REFERANCE_BYTES[1759] ^ block_header[77];
            final_init_img[1763] = NEW_IMAGE_REFERANCE_BYTES[1763] ^ block_header[77];
            final_init_img[1767] = NEW_IMAGE_REFERANCE_BYTES[1767] ^ block_header[77];
            final_init_img[1771] = NEW_IMAGE_REFERANCE_BYTES[1771] ^ block_header[77];
            final_init_img[1775] = NEW_IMAGE_REFERANCE_BYTES[1775] ^ block_header[77];
            final_init_img[1779] = NEW_IMAGE_REFERANCE_BYTES[1779] ^ block_header[77];
		}


        if (last_nonce_bytes[2] != block_header[78]){
            last_nonce_bytes[2] = block_header[78];
            final_init_img[56] = NEW_IMAGE_REFERANCE_BYTES[56] ^ block_header[78];
            final_init_img[60] = NEW_IMAGE_REFERANCE_BYTES[60] ^ block_header[78];
            final_init_img[64] = NEW_IMAGE_REFERANCE_BYTES[64] ^ block_header[78];
            final_init_img[68] = NEW_IMAGE_REFERANCE_BYTES[68] ^ block_header[78];
            final_init_img[72] = NEW_IMAGE_REFERANCE_BYTES[72] ^ block_header[78];
            final_init_img[76] = NEW_IMAGE_REFERANCE_BYTES[76] ^ block_header[78];
            final_init_img[80] = NEW_IMAGE_REFERANCE_BYTES[80] ^ block_header[78];
            final_init_img[84] = NEW_IMAGE_REFERANCE_BYTES[84] ^ block_header[78];
            final_init_img[88] = NEW_IMAGE_REFERANCE_BYTES[88] ^ block_header[78];
            final_init_img[92] = NEW_IMAGE_REFERANCE_BYTES[92] ^ block_header[78];
            final_init_img[96] = NEW_IMAGE_REFERANCE_BYTES[96] ^ block_header[78];
            final_init_img[100] = NEW_IMAGE_REFERANCE_BYTES[100] ^ block_header[78];
            final_init_img[104] = NEW_IMAGE_REFERANCE_BYTES[104] ^ block_header[78];
            final_init_img[108] = NEW_IMAGE_REFERANCE_BYTES[108] ^ block_header[78];
            final_init_img[112] = NEW_IMAGE_REFERANCE_BYTES[112] ^ block_header[78];
            final_init_img[116] = NEW_IMAGE_REFERANCE_BYTES[116] ^ block_header[78];
            final_init_img[120] = NEW_IMAGE_REFERANCE_BYTES[120] ^ block_header[78];
            final_init_img[124] = NEW_IMAGE_REFERANCE_BYTES[124] ^ block_header[78];
            final_init_img[128] = NEW_IMAGE_REFERANCE_BYTES[128] ^ block_header[78];
            final_init_img[132] = NEW_IMAGE_REFERANCE_BYTES[132] ^ block_header[78];
            final_init_img[136] = NEW_IMAGE_REFERANCE_BYTES[136] ^ block_header[78];
            final_init_img[140] = NEW_IMAGE_REFERANCE_BYTES[140] ^ block_header[78];
            final_init_img[144] = NEW_IMAGE_REFERANCE_BYTES[144] ^ block_header[78];
            final_init_img[148] = NEW_IMAGE_REFERANCE_BYTES[148] ^ block_header[78];
            final_init_img[152] = NEW_IMAGE_REFERANCE_BYTES[152] ^ block_header[78];
            final_init_img[156] = NEW_IMAGE_REFERANCE_BYTES[156] ^ block_header[78];
            final_init_img[160] = NEW_IMAGE_REFERANCE_BYTES[160] ^ block_header[78];
            final_init_img[164] = NEW_IMAGE_REFERANCE_BYTES[164] ^ block_header[78];
            final_init_img[168] = NEW_IMAGE_REFERANCE_BYTES[168] ^ block_header[78];
            final_init_img[172] = NEW_IMAGE_REFERANCE_BYTES[172] ^ block_header[78];
            final_init_img[176] = NEW_IMAGE_REFERANCE_BYTES[176] ^ block_header[78];
            final_init_img[180] = NEW_IMAGE_REFERANCE_BYTES[180] ^ block_header[78];
            final_init_img[184] = NEW_IMAGE_REFERANCE_BYTES[184] ^ block_header[78];
            final_init_img[188] = NEW_IMAGE_REFERANCE_BYTES[188] ^ block_header[78];
            final_init_img[192] = NEW_IMAGE_REFERANCE_BYTES[192] ^ block_header[78];
            final_init_img[196] = NEW_IMAGE_REFERANCE_BYTES[196] ^ block_header[78];
            final_init_img[200] = NEW_IMAGE_REFERANCE_BYTES[200] ^ block_header[78];
            final_init_img[204] = NEW_IMAGE_REFERANCE_BYTES[204] ^ block_header[78];
            final_init_img[208] = NEW_IMAGE_REFERANCE_BYTES[208] ^ block_header[78];
            final_init_img[212] = NEW_IMAGE_REFERANCE_BYTES[212] ^ block_header[78];
            final_init_img[216] = NEW_IMAGE_REFERANCE_BYTES[216] ^ block_header[78];
            final_init_img[220] = NEW_IMAGE_REFERANCE_BYTES[220] ^ block_header[78];
            final_init_img[224] = NEW_IMAGE_REFERANCE_BYTES[224] ^ block_header[78];
            final_init_img[228] = NEW_IMAGE_REFERANCE_BYTES[228] ^ block_header[78];
            final_init_img[232] = NEW_IMAGE_REFERANCE_BYTES[232] ^ block_header[78];
            final_init_img[236] = NEW_IMAGE_REFERANCE_BYTES[236] ^ block_header[78];
            final_init_img[240] = NEW_IMAGE_REFERANCE_BYTES[240] ^ block_header[78];
            final_init_img[244] = NEW_IMAGE_REFERANCE_BYTES[244] ^ block_header[78];
            final_init_img[248] = NEW_IMAGE_REFERANCE_BYTES[248] ^ block_header[78];
            final_init_img[252] = NEW_IMAGE_REFERANCE_BYTES[252] ^ block_header[78];
            final_init_img[256] = NEW_IMAGE_REFERANCE_BYTES[256] ^ block_header[78];
            final_init_img[260] = NEW_IMAGE_REFERANCE_BYTES[260] ^ block_header[78];
            final_init_img[264] = NEW_IMAGE_REFERANCE_BYTES[264] ^ block_header[78];
            final_init_img[268] = NEW_IMAGE_REFERANCE_BYTES[268] ^ block_header[78];
            final_init_img[272] = NEW_IMAGE_REFERANCE_BYTES[272] ^ block_header[78];
            final_init_img[276] = NEW_IMAGE_REFERANCE_BYTES[276] ^ block_header[78];
            final_init_img[280] = NEW_IMAGE_REFERANCE_BYTES[280] ^ block_header[78];
            final_init_img[284] = NEW_IMAGE_REFERANCE_BYTES[284] ^ block_header[78];
            final_init_img[288] = NEW_IMAGE_REFERANCE_BYTES[288] ^ block_header[78];
            final_init_img[292] = NEW_IMAGE_REFERANCE_BYTES[292] ^ block_header[78];
            final_init_img[296] = NEW_IMAGE_REFERANCE_BYTES[296] ^ block_header[78];
            final_init_img[300] = NEW_IMAGE_REFERANCE_BYTES[300] ^ block_header[78];
            final_init_img[304] = NEW_IMAGE_REFERANCE_BYTES[304] ^ block_header[78];
            final_init_img[308] = NEW_IMAGE_REFERANCE_BYTES[308] ^ block_header[78];
            final_init_img[312] = NEW_IMAGE_REFERANCE_BYTES[312] ^ block_header[78];
            final_init_img[316] = NEW_IMAGE_REFERANCE_BYTES[316] ^ block_header[78];
            final_init_img[320] = NEW_IMAGE_REFERANCE_BYTES[320] ^ block_header[78];
            final_init_img[324] = NEW_IMAGE_REFERANCE_BYTES[324] ^ block_header[78];
            final_init_img[328] = NEW_IMAGE_REFERANCE_BYTES[328] ^ block_header[78];
            final_init_img[332] = NEW_IMAGE_REFERANCE_BYTES[332] ^ block_header[78];
            final_init_img[336] = NEW_IMAGE_REFERANCE_BYTES[336] ^ block_header[78];
            final_init_img[340] = NEW_IMAGE_REFERANCE_BYTES[340] ^ block_header[78];
            final_init_img[344] = NEW_IMAGE_REFERANCE_BYTES[344] ^ block_header[78];
            final_init_img[348] = NEW_IMAGE_REFERANCE_BYTES[348] ^ block_header[78];
            final_init_img[352] = NEW_IMAGE_REFERANCE_BYTES[352] ^ block_header[78];
            final_init_img[356] = NEW_IMAGE_REFERANCE_BYTES[356] ^ block_header[78];
            final_init_img[360] = NEW_IMAGE_REFERANCE_BYTES[360] ^ block_header[78];
            final_init_img[364] = NEW_IMAGE_REFERANCE_BYTES[364] ^ block_header[78];
            final_init_img[368] = NEW_IMAGE_REFERANCE_BYTES[368] ^ block_header[78];
            final_init_img[372] = NEW_IMAGE_REFERANCE_BYTES[372] ^ block_header[78];
            final_init_img[376] = NEW_IMAGE_REFERANCE_BYTES[376] ^ block_header[78];
            final_init_img[380] = NEW_IMAGE_REFERANCE_BYTES[380] ^ block_header[78];
            final_init_img[384] = NEW_IMAGE_REFERANCE_BYTES[384] ^ block_header[78];
            final_init_img[388] = NEW_IMAGE_REFERANCE_BYTES[388] ^ block_header[78];
            final_init_img[392] = NEW_IMAGE_REFERANCE_BYTES[392] ^ block_header[78];
            final_init_img[396] = NEW_IMAGE_REFERANCE_BYTES[396] ^ block_header[78];
            final_init_img[400] = NEW_IMAGE_REFERANCE_BYTES[400] ^ block_header[78];
            final_init_img[404] = NEW_IMAGE_REFERANCE_BYTES[404] ^ block_header[78];
            final_init_img[408] = NEW_IMAGE_REFERANCE_BYTES[408] ^ block_header[78];
            final_init_img[412] = NEW_IMAGE_REFERANCE_BYTES[412] ^ block_header[78];
            final_init_img[416] = NEW_IMAGE_REFERANCE_BYTES[416] ^ block_header[78];
            final_init_img[420] = NEW_IMAGE_REFERANCE_BYTES[420] ^ block_header[78];
            final_init_img[424] = NEW_IMAGE_REFERANCE_BYTES[424] ^ block_header[78];
            final_init_img[428] = NEW_IMAGE_REFERANCE_BYTES[428] ^ block_header[78];
            final_init_img[432] = NEW_IMAGE_REFERANCE_BYTES[432] ^ block_header[78];
            final_init_img[436] = NEW_IMAGE_REFERANCE_BYTES[436] ^ block_header[78];
            final_init_img[440] = NEW_IMAGE_REFERANCE_BYTES[440] ^ block_header[78];
            final_init_img[444] = NEW_IMAGE_REFERANCE_BYTES[444] ^ block_header[78];
            final_init_img[448] = NEW_IMAGE_REFERANCE_BYTES[448] ^ block_header[78];
            final_init_img[452] = NEW_IMAGE_REFERANCE_BYTES[452] ^ block_header[78];
            final_init_img[456] = NEW_IMAGE_REFERANCE_BYTES[456] ^ block_header[78];
            final_init_img[460] = NEW_IMAGE_REFERANCE_BYTES[460] ^ block_header[78];
            final_init_img[464] = NEW_IMAGE_REFERANCE_BYTES[464] ^ block_header[78];
            final_init_img[468] = NEW_IMAGE_REFERANCE_BYTES[468] ^ block_header[78];
            final_init_img[472] = NEW_IMAGE_REFERANCE_BYTES[472] ^ block_header[78];
            final_init_img[476] = NEW_IMAGE_REFERANCE_BYTES[476] ^ block_header[78];
            final_init_img[480] = NEW_IMAGE_REFERANCE_BYTES[480] ^ block_header[78];
            final_init_img[484] = NEW_IMAGE_REFERANCE_BYTES[484] ^ block_header[78];
            final_init_img[488] = NEW_IMAGE_REFERANCE_BYTES[488] ^ block_header[78];
            final_init_img[492] = NEW_IMAGE_REFERANCE_BYTES[492] ^ block_header[78];
            final_init_img[496] = NEW_IMAGE_REFERANCE_BYTES[496] ^ block_header[78];
            final_init_img[500] = NEW_IMAGE_REFERANCE_BYTES[500] ^ block_header[78];
            final_init_img[504] = NEW_IMAGE_REFERANCE_BYTES[504] ^ block_header[78];
            final_init_img[508] = NEW_IMAGE_REFERANCE_BYTES[508] ^ block_header[78];
            final_init_img[512] = NEW_IMAGE_REFERANCE_BYTES[512] ^ block_header[78];
            final_init_img[516] = NEW_IMAGE_REFERANCE_BYTES[516] ^ block_header[78];
            final_init_img[520] = NEW_IMAGE_REFERANCE_BYTES[520] ^ block_header[78];
            final_init_img[524] = NEW_IMAGE_REFERANCE_BYTES[524] ^ block_header[78];
            final_init_img[528] = NEW_IMAGE_REFERANCE_BYTES[528] ^ block_header[78];
            final_init_img[532] = NEW_IMAGE_REFERANCE_BYTES[532] ^ block_header[78];
            final_init_img[536] = NEW_IMAGE_REFERANCE_BYTES[536] ^ block_header[78];
            final_init_img[540] = NEW_IMAGE_REFERANCE_BYTES[540] ^ block_header[78];
            final_init_img[544] = NEW_IMAGE_REFERANCE_BYTES[544] ^ block_header[78];
            final_init_img[548] = NEW_IMAGE_REFERANCE_BYTES[548] ^ block_header[78];
            final_init_img[552] = NEW_IMAGE_REFERANCE_BYTES[552] ^ block_header[78];
            final_init_img[556] = NEW_IMAGE_REFERANCE_BYTES[556] ^ block_header[78];
            final_init_img[560] = NEW_IMAGE_REFERANCE_BYTES[560] ^ block_header[78];
            final_init_img[564] = NEW_IMAGE_REFERANCE_BYTES[564] ^ block_header[78];
            final_init_img[568] = NEW_IMAGE_REFERANCE_BYTES[568] ^ block_header[78];
            final_init_img[572] = NEW_IMAGE_REFERANCE_BYTES[572] ^ block_header[78];
            final_init_img[576] = NEW_IMAGE_REFERANCE_BYTES[576] ^ block_header[78];
            final_init_img[580] = NEW_IMAGE_REFERANCE_BYTES[580] ^ block_header[78];
            final_init_img[584] = NEW_IMAGE_REFERANCE_BYTES[584] ^ block_header[78];
            final_init_img[588] = NEW_IMAGE_REFERANCE_BYTES[588] ^ block_header[78];
            final_init_img[592] = NEW_IMAGE_REFERANCE_BYTES[592] ^ block_header[78];
            final_init_img[596] = NEW_IMAGE_REFERANCE_BYTES[596] ^ block_header[78];
            final_init_img[600] = NEW_IMAGE_REFERANCE_BYTES[600] ^ block_header[78];
            final_init_img[604] = NEW_IMAGE_REFERANCE_BYTES[604] ^ block_header[78];
            final_init_img[608] = NEW_IMAGE_REFERANCE_BYTES[608] ^ block_header[78];
            final_init_img[612] = NEW_IMAGE_REFERANCE_BYTES[612] ^ block_header[78];
            final_init_img[616] = NEW_IMAGE_REFERANCE_BYTES[616] ^ block_header[78];
            final_init_img[620] = NEW_IMAGE_REFERANCE_BYTES[620] ^ block_header[78];
            final_init_img[624] = NEW_IMAGE_REFERANCE_BYTES[624] ^ block_header[78];
            final_init_img[628] = NEW_IMAGE_REFERANCE_BYTES[628] ^ block_header[78];
            final_init_img[632] = NEW_IMAGE_REFERANCE_BYTES[632] ^ block_header[78];
            final_init_img[636] = NEW_IMAGE_REFERANCE_BYTES[636] ^ block_header[78];
            final_init_img[640] = NEW_IMAGE_REFERANCE_BYTES[640] ^ block_header[78];
            final_init_img[644] = NEW_IMAGE_REFERANCE_BYTES[644] ^ block_header[78];
            final_init_img[648] = NEW_IMAGE_REFERANCE_BYTES[648] ^ block_header[78];
            final_init_img[652] = NEW_IMAGE_REFERANCE_BYTES[652] ^ block_header[78];
            final_init_img[656] = NEW_IMAGE_REFERANCE_BYTES[656] ^ block_header[78];
            final_init_img[660] = NEW_IMAGE_REFERANCE_BYTES[660] ^ block_header[78];
            final_init_img[664] = NEW_IMAGE_REFERANCE_BYTES[664] ^ block_header[78];
            final_init_img[668] = NEW_IMAGE_REFERANCE_BYTES[668] ^ block_header[78];
            final_init_img[672] = NEW_IMAGE_REFERANCE_BYTES[672] ^ block_header[78];
            final_init_img[676] = NEW_IMAGE_REFERANCE_BYTES[676] ^ block_header[78];
            final_init_img[680] = NEW_IMAGE_REFERANCE_BYTES[680] ^ block_header[78];
            final_init_img[684] = NEW_IMAGE_REFERANCE_BYTES[684] ^ block_header[78];
            final_init_img[688] = NEW_IMAGE_REFERANCE_BYTES[688] ^ block_header[78];
            final_init_img[692] = NEW_IMAGE_REFERANCE_BYTES[692] ^ block_header[78];
            final_init_img[696] = NEW_IMAGE_REFERANCE_BYTES[696] ^ block_header[78];
            final_init_img[700] = NEW_IMAGE_REFERANCE_BYTES[700] ^ block_header[78];
            final_init_img[704] = NEW_IMAGE_REFERANCE_BYTES[704] ^ block_header[78];
            final_init_img[708] = NEW_IMAGE_REFERANCE_BYTES[708] ^ block_header[78];
            final_init_img[712] = NEW_IMAGE_REFERANCE_BYTES[712] ^ block_header[78];
            final_init_img[716] = NEW_IMAGE_REFERANCE_BYTES[716] ^ block_header[78];
            final_init_img[720] = NEW_IMAGE_REFERANCE_BYTES[720] ^ block_header[78];
            final_init_img[724] = NEW_IMAGE_REFERANCE_BYTES[724] ^ block_header[78];
            final_init_img[728] = NEW_IMAGE_REFERANCE_BYTES[728] ^ block_header[78];
            final_init_img[732] = NEW_IMAGE_REFERANCE_BYTES[732] ^ block_header[78];
            final_init_img[736] = NEW_IMAGE_REFERANCE_BYTES[736] ^ block_header[78];
            final_init_img[740] = NEW_IMAGE_REFERANCE_BYTES[740] ^ block_header[78];
            final_init_img[744] = NEW_IMAGE_REFERANCE_BYTES[744] ^ block_header[78];
            final_init_img[748] = NEW_IMAGE_REFERANCE_BYTES[748] ^ block_header[78];
            final_init_img[752] = NEW_IMAGE_REFERANCE_BYTES[752] ^ block_header[78];
            final_init_img[756] = NEW_IMAGE_REFERANCE_BYTES[756] ^ block_header[78];
            final_init_img[760] = NEW_IMAGE_REFERANCE_BYTES[760] ^ block_header[78];
            final_init_img[764] = NEW_IMAGE_REFERANCE_BYTES[764] ^ block_header[78];
            final_init_img[768] = NEW_IMAGE_REFERANCE_BYTES[768] ^ block_header[78];
            final_init_img[772] = NEW_IMAGE_REFERANCE_BYTES[772] ^ block_header[78];
            final_init_img[776] = NEW_IMAGE_REFERANCE_BYTES[776] ^ block_header[78];
            final_init_img[780] = NEW_IMAGE_REFERANCE_BYTES[780] ^ block_header[78];
            final_init_img[784] = NEW_IMAGE_REFERANCE_BYTES[784] ^ block_header[78];
            final_init_img[788] = NEW_IMAGE_REFERANCE_BYTES[788] ^ block_header[78];
            final_init_img[792] = NEW_IMAGE_REFERANCE_BYTES[792] ^ block_header[78];
            final_init_img[796] = NEW_IMAGE_REFERANCE_BYTES[796] ^ block_header[78];
            final_init_img[800] = NEW_IMAGE_REFERANCE_BYTES[800] ^ block_header[78];
            final_init_img[804] = NEW_IMAGE_REFERANCE_BYTES[804] ^ block_header[78];
            final_init_img[808] = NEW_IMAGE_REFERANCE_BYTES[808] ^ block_header[78];
            final_init_img[812] = NEW_IMAGE_REFERANCE_BYTES[812] ^ block_header[78];
            final_init_img[816] = NEW_IMAGE_REFERANCE_BYTES[816] ^ block_header[78];
            final_init_img[820] = NEW_IMAGE_REFERANCE_BYTES[820] ^ block_header[78];
            final_init_img[824] = NEW_IMAGE_REFERANCE_BYTES[824] ^ block_header[78];
            final_init_img[828] = NEW_IMAGE_REFERANCE_BYTES[828] ^ block_header[78];
            final_init_img[832] = NEW_IMAGE_REFERANCE_BYTES[832] ^ block_header[78];
            final_init_img[836] = NEW_IMAGE_REFERANCE_BYTES[836] ^ block_header[78];
            final_init_img[840] = NEW_IMAGE_REFERANCE_BYTES[840] ^ block_header[78];
            final_init_img[844] = NEW_IMAGE_REFERANCE_BYTES[844] ^ block_header[78];
            final_init_img[848] = NEW_IMAGE_REFERANCE_BYTES[848] ^ block_header[78];
            final_init_img[852] = NEW_IMAGE_REFERANCE_BYTES[852] ^ block_header[78];
            final_init_img[856] = NEW_IMAGE_REFERANCE_BYTES[856] ^ block_header[78];
            final_init_img[860] = NEW_IMAGE_REFERANCE_BYTES[860] ^ block_header[78];
            final_init_img[864] = NEW_IMAGE_REFERANCE_BYTES[864] ^ block_header[78];
            final_init_img[868] = NEW_IMAGE_REFERANCE_BYTES[868] ^ block_header[78];
            final_init_img[872] = NEW_IMAGE_REFERANCE_BYTES[872] ^ block_header[78];
            final_init_img[876] = NEW_IMAGE_REFERANCE_BYTES[876] ^ block_header[78];
            final_init_img[880] = NEW_IMAGE_REFERANCE_BYTES[880] ^ block_header[78];
            final_init_img[884] = NEW_IMAGE_REFERANCE_BYTES[884] ^ block_header[78];
            final_init_img[888] = NEW_IMAGE_REFERANCE_BYTES[888] ^ block_header[78];
            final_init_img[892] = NEW_IMAGE_REFERANCE_BYTES[892] ^ block_header[78];
            final_init_img[896] = NEW_IMAGE_REFERANCE_BYTES[896] ^ block_header[78];
            final_init_img[900] = NEW_IMAGE_REFERANCE_BYTES[900] ^ block_header[78];
            final_init_img[904] = NEW_IMAGE_REFERANCE_BYTES[904] ^ block_header[78];
            final_init_img[908] = NEW_IMAGE_REFERANCE_BYTES[908] ^ block_header[78];
            final_init_img[912] = NEW_IMAGE_REFERANCE_BYTES[912] ^ block_header[78];
            final_init_img[916] = NEW_IMAGE_REFERANCE_BYTES[916] ^ block_header[78];
            final_init_img[920] = NEW_IMAGE_REFERANCE_BYTES[920] ^ block_header[78];
            final_init_img[924] = NEW_IMAGE_REFERANCE_BYTES[924] ^ block_header[78];
            final_init_img[928] = NEW_IMAGE_REFERANCE_BYTES[928] ^ block_header[78];
            final_init_img[932] = NEW_IMAGE_REFERANCE_BYTES[932] ^ block_header[78];
            final_init_img[936] = NEW_IMAGE_REFERANCE_BYTES[936] ^ block_header[78];
            final_init_img[940] = NEW_IMAGE_REFERANCE_BYTES[940] ^ block_header[78];
            final_init_img[944] = NEW_IMAGE_REFERANCE_BYTES[944] ^ block_header[78];
            final_init_img[948] = NEW_IMAGE_REFERANCE_BYTES[948] ^ block_header[78];
            final_init_img[952] = NEW_IMAGE_REFERANCE_BYTES[952] ^ block_header[78];
            final_init_img[956] = NEW_IMAGE_REFERANCE_BYTES[956] ^ block_header[78];
            final_init_img[960] = NEW_IMAGE_REFERANCE_BYTES[960] ^ block_header[78];
            final_init_img[964] = NEW_IMAGE_REFERANCE_BYTES[964] ^ block_header[78];
            final_init_img[968] = NEW_IMAGE_REFERANCE_BYTES[968] ^ block_header[78];
            final_init_img[972] = NEW_IMAGE_REFERANCE_BYTES[972] ^ block_header[78];
            final_init_img[976] = NEW_IMAGE_REFERANCE_BYTES[976] ^ block_header[78];
            final_init_img[980] = NEW_IMAGE_REFERANCE_BYTES[980] ^ block_header[78];
            final_init_img[984] = NEW_IMAGE_REFERANCE_BYTES[984] ^ block_header[78];
            final_init_img[988] = NEW_IMAGE_REFERANCE_BYTES[988] ^ block_header[78];
            final_init_img[992] = NEW_IMAGE_REFERANCE_BYTES[992] ^ block_header[78];
            final_init_img[996] = NEW_IMAGE_REFERANCE_BYTES[996] ^ block_header[78];
            final_init_img[1000] = NEW_IMAGE_REFERANCE_BYTES[1000] ^ block_header[78];
            final_init_img[1004] = NEW_IMAGE_REFERANCE_BYTES[1004] ^ block_header[78];
            final_init_img[1008] = NEW_IMAGE_REFERANCE_BYTES[1008] ^ block_header[78];
            final_init_img[1012] = NEW_IMAGE_REFERANCE_BYTES[1012] ^ block_header[78];
            final_init_img[1016] = NEW_IMAGE_REFERANCE_BYTES[1016] ^ block_header[78];
            final_init_img[1020] = NEW_IMAGE_REFERANCE_BYTES[1020] ^ block_header[78];
            final_init_img[1024] = NEW_IMAGE_REFERANCE_BYTES[1024] ^ block_header[78];
            final_init_img[1028] = NEW_IMAGE_REFERANCE_BYTES[1028] ^ block_header[78];
            final_init_img[1032] = NEW_IMAGE_REFERANCE_BYTES[1032] ^ block_header[78];
            final_init_img[1036] = NEW_IMAGE_REFERANCE_BYTES[1036] ^ block_header[78];
            final_init_img[1040] = NEW_IMAGE_REFERANCE_BYTES[1040] ^ block_header[78];
            final_init_img[1044] = NEW_IMAGE_REFERANCE_BYTES[1044] ^ block_header[78];
            final_init_img[1048] = NEW_IMAGE_REFERANCE_BYTES[1048] ^ block_header[78];
            final_init_img[1052] = NEW_IMAGE_REFERANCE_BYTES[1052] ^ block_header[78];
            final_init_img[1056] = NEW_IMAGE_REFERANCE_BYTES[1056] ^ block_header[78];
            final_init_img[1060] = NEW_IMAGE_REFERANCE_BYTES[1060] ^ block_header[78];
            final_init_img[1064] = NEW_IMAGE_REFERANCE_BYTES[1064] ^ block_header[78];
            final_init_img[1068] = NEW_IMAGE_REFERANCE_BYTES[1068] ^ block_header[78];
            final_init_img[1072] = NEW_IMAGE_REFERANCE_BYTES[1072] ^ block_header[78];
            final_init_img[1076] = NEW_IMAGE_REFERANCE_BYTES[1076] ^ block_header[78];
            final_init_img[1080] = NEW_IMAGE_REFERANCE_BYTES[1080] ^ block_header[78];
            final_init_img[1084] = NEW_IMAGE_REFERANCE_BYTES[1084] ^ block_header[78];
            final_init_img[1088] = NEW_IMAGE_REFERANCE_BYTES[1088] ^ block_header[78];
            final_init_img[1092] = NEW_IMAGE_REFERANCE_BYTES[1092] ^ block_header[78];
            final_init_img[1096] = NEW_IMAGE_REFERANCE_BYTES[1096] ^ block_header[78];
            final_init_img[1100] = NEW_IMAGE_REFERANCE_BYTES[1100] ^ block_header[78];
            final_init_img[1104] = NEW_IMAGE_REFERANCE_BYTES[1104] ^ block_header[78];
            final_init_img[1108] = NEW_IMAGE_REFERANCE_BYTES[1108] ^ block_header[78];
            final_init_img[1112] = NEW_IMAGE_REFERANCE_BYTES[1112] ^ block_header[78];
            final_init_img[1116] = NEW_IMAGE_REFERANCE_BYTES[1116] ^ block_header[78];
            final_init_img[1120] = NEW_IMAGE_REFERANCE_BYTES[1120] ^ block_header[78];
            final_init_img[1124] = NEW_IMAGE_REFERANCE_BYTES[1124] ^ block_header[78];
            final_init_img[1128] = NEW_IMAGE_REFERANCE_BYTES[1128] ^ block_header[78];
            final_init_img[1132] = NEW_IMAGE_REFERANCE_BYTES[1132] ^ block_header[78];
            final_init_img[1136] = NEW_IMAGE_REFERANCE_BYTES[1136] ^ block_header[78];
            final_init_img[1140] = NEW_IMAGE_REFERANCE_BYTES[1140] ^ block_header[78];
            final_init_img[1144] = NEW_IMAGE_REFERANCE_BYTES[1144] ^ block_header[78];
            final_init_img[1148] = NEW_IMAGE_REFERANCE_BYTES[1148] ^ block_header[78];
            final_init_img[1152] = NEW_IMAGE_REFERANCE_BYTES[1152] ^ block_header[78];
            final_init_img[1156] = NEW_IMAGE_REFERANCE_BYTES[1156] ^ block_header[78];
            final_init_img[1160] = NEW_IMAGE_REFERANCE_BYTES[1160] ^ block_header[78];
            final_init_img[1164] = NEW_IMAGE_REFERANCE_BYTES[1164] ^ block_header[78];
            final_init_img[1168] = NEW_IMAGE_REFERANCE_BYTES[1168] ^ block_header[78];
            final_init_img[1172] = NEW_IMAGE_REFERANCE_BYTES[1172] ^ block_header[78];
            final_init_img[1176] = NEW_IMAGE_REFERANCE_BYTES[1176] ^ block_header[78];
            final_init_img[1180] = NEW_IMAGE_REFERANCE_BYTES[1180] ^ block_header[78];
            final_init_img[1184] = NEW_IMAGE_REFERANCE_BYTES[1184] ^ block_header[78];
            final_init_img[1188] = NEW_IMAGE_REFERANCE_BYTES[1188] ^ block_header[78];
            final_init_img[1192] = NEW_IMAGE_REFERANCE_BYTES[1192] ^ block_header[78];
            final_init_img[1196] = NEW_IMAGE_REFERANCE_BYTES[1196] ^ block_header[78];
            final_init_img[1200] = NEW_IMAGE_REFERANCE_BYTES[1200] ^ block_header[78];
            final_init_img[1204] = NEW_IMAGE_REFERANCE_BYTES[1204] ^ block_header[78];
            final_init_img[1208] = NEW_IMAGE_REFERANCE_BYTES[1208] ^ block_header[78];
            final_init_img[1212] = NEW_IMAGE_REFERANCE_BYTES[1212] ^ block_header[78];
            final_init_img[1216] = NEW_IMAGE_REFERANCE_BYTES[1216] ^ block_header[78];
            final_init_img[1220] = NEW_IMAGE_REFERANCE_BYTES[1220] ^ block_header[78];
            final_init_img[1224] = NEW_IMAGE_REFERANCE_BYTES[1224] ^ block_header[78];
            final_init_img[1228] = NEW_IMAGE_REFERANCE_BYTES[1228] ^ block_header[78];
            final_init_img[1232] = NEW_IMAGE_REFERANCE_BYTES[1232] ^ block_header[78];
            final_init_img[1236] = NEW_IMAGE_REFERANCE_BYTES[1236] ^ block_header[78];
            final_init_img[1240] = NEW_IMAGE_REFERANCE_BYTES[1240] ^ block_header[78];
            final_init_img[1244] = NEW_IMAGE_REFERANCE_BYTES[1244] ^ block_header[78];
            final_init_img[1248] = NEW_IMAGE_REFERANCE_BYTES[1248] ^ block_header[78];
            final_init_img[1252] = NEW_IMAGE_REFERANCE_BYTES[1252] ^ block_header[78];
            final_init_img[1256] = NEW_IMAGE_REFERANCE_BYTES[1256] ^ block_header[78];
            final_init_img[1260] = NEW_IMAGE_REFERANCE_BYTES[1260] ^ block_header[78];
            final_init_img[1264] = NEW_IMAGE_REFERANCE_BYTES[1264] ^ block_header[78];
            final_init_img[1268] = NEW_IMAGE_REFERANCE_BYTES[1268] ^ block_header[78];
            final_init_img[1272] = NEW_IMAGE_REFERANCE_BYTES[1272] ^ block_header[78];
            final_init_img[1276] = NEW_IMAGE_REFERANCE_BYTES[1276] ^ block_header[78];
            final_init_img[1280] = NEW_IMAGE_REFERANCE_BYTES[1280] ^ block_header[78];
            final_init_img[1284] = NEW_IMAGE_REFERANCE_BYTES[1284] ^ block_header[78];
            final_init_img[1288] = NEW_IMAGE_REFERANCE_BYTES[1288] ^ block_header[78];
            final_init_img[1292] = NEW_IMAGE_REFERANCE_BYTES[1292] ^ block_header[78];
            final_init_img[1296] = NEW_IMAGE_REFERANCE_BYTES[1296] ^ block_header[78];
            final_init_img[1300] = NEW_IMAGE_REFERANCE_BYTES[1300] ^ block_header[78];
            final_init_img[1304] = NEW_IMAGE_REFERANCE_BYTES[1304] ^ block_header[78];
            final_init_img[1308] = NEW_IMAGE_REFERANCE_BYTES[1308] ^ block_header[78];
            final_init_img[1312] = NEW_IMAGE_REFERANCE_BYTES[1312] ^ block_header[78];
            final_init_img[1316] = NEW_IMAGE_REFERANCE_BYTES[1316] ^ block_header[78];
            final_init_img[1320] = NEW_IMAGE_REFERANCE_BYTES[1320] ^ block_header[78];
            final_init_img[1324] = NEW_IMAGE_REFERANCE_BYTES[1324] ^ block_header[78];
            final_init_img[1328] = NEW_IMAGE_REFERANCE_BYTES[1328] ^ block_header[78];
            final_init_img[1332] = NEW_IMAGE_REFERANCE_BYTES[1332] ^ block_header[78];
            final_init_img[1336] = NEW_IMAGE_REFERANCE_BYTES[1336] ^ block_header[78];
            final_init_img[1340] = NEW_IMAGE_REFERANCE_BYTES[1340] ^ block_header[78];
            final_init_img[1344] = NEW_IMAGE_REFERANCE_BYTES[1344] ^ block_header[78];
            final_init_img[1348] = NEW_IMAGE_REFERANCE_BYTES[1348] ^ block_header[78];
            final_init_img[1352] = NEW_IMAGE_REFERANCE_BYTES[1352] ^ block_header[78];
            final_init_img[1356] = NEW_IMAGE_REFERANCE_BYTES[1356] ^ block_header[78];
            final_init_img[1360] = NEW_IMAGE_REFERANCE_BYTES[1360] ^ block_header[78];
            final_init_img[1364] = NEW_IMAGE_REFERANCE_BYTES[1364] ^ block_header[78];
            final_init_img[1368] = NEW_IMAGE_REFERANCE_BYTES[1368] ^ block_header[78];
            final_init_img[1372] = NEW_IMAGE_REFERANCE_BYTES[1372] ^ block_header[78];
            final_init_img[1376] = NEW_IMAGE_REFERANCE_BYTES[1376] ^ block_header[78];
            final_init_img[1380] = NEW_IMAGE_REFERANCE_BYTES[1380] ^ block_header[78];
            final_init_img[1384] = NEW_IMAGE_REFERANCE_BYTES[1384] ^ block_header[78];
            final_init_img[1388] = NEW_IMAGE_REFERANCE_BYTES[1388] ^ block_header[78];
            final_init_img[1392] = NEW_IMAGE_REFERANCE_BYTES[1392] ^ block_header[78];
            final_init_img[1396] = NEW_IMAGE_REFERANCE_BYTES[1396] ^ block_header[78];
            final_init_img[1400] = NEW_IMAGE_REFERANCE_BYTES[1400] ^ block_header[78];
            final_init_img[1404] = NEW_IMAGE_REFERANCE_BYTES[1404] ^ block_header[78];
            final_init_img[1408] = NEW_IMAGE_REFERANCE_BYTES[1408] ^ block_header[78];
            final_init_img[1412] = NEW_IMAGE_REFERANCE_BYTES[1412] ^ block_header[78];
            final_init_img[1416] = NEW_IMAGE_REFERANCE_BYTES[1416] ^ block_header[78];
            final_init_img[1420] = NEW_IMAGE_REFERANCE_BYTES[1420] ^ block_header[78];
            final_init_img[1424] = NEW_IMAGE_REFERANCE_BYTES[1424] ^ block_header[78];
            final_init_img[1428] = NEW_IMAGE_REFERANCE_BYTES[1428] ^ block_header[78];
            final_init_img[1432] = NEW_IMAGE_REFERANCE_BYTES[1432] ^ block_header[78];
            final_init_img[1436] = NEW_IMAGE_REFERANCE_BYTES[1436] ^ block_header[78];
            final_init_img[1440] = NEW_IMAGE_REFERANCE_BYTES[1440] ^ block_header[78];
            final_init_img[1444] = NEW_IMAGE_REFERANCE_BYTES[1444] ^ block_header[78];
            final_init_img[1448] = NEW_IMAGE_REFERANCE_BYTES[1448] ^ block_header[78];
            final_init_img[1452] = NEW_IMAGE_REFERANCE_BYTES[1452] ^ block_header[78];
            final_init_img[1456] = NEW_IMAGE_REFERANCE_BYTES[1456] ^ block_header[78];
            final_init_img[1460] = NEW_IMAGE_REFERANCE_BYTES[1460] ^ block_header[78];
            final_init_img[1464] = NEW_IMAGE_REFERANCE_BYTES[1464] ^ block_header[78];
            final_init_img[1468] = NEW_IMAGE_REFERANCE_BYTES[1468] ^ block_header[78];
            final_init_img[1472] = NEW_IMAGE_REFERANCE_BYTES[1472] ^ block_header[78];
            final_init_img[1476] = NEW_IMAGE_REFERANCE_BYTES[1476] ^ block_header[78];
            final_init_img[1480] = NEW_IMAGE_REFERANCE_BYTES[1480] ^ block_header[78];
            final_init_img[1484] = NEW_IMAGE_REFERANCE_BYTES[1484] ^ block_header[78];
            final_init_img[1488] = NEW_IMAGE_REFERANCE_BYTES[1488] ^ block_header[78];
            final_init_img[1492] = NEW_IMAGE_REFERANCE_BYTES[1492] ^ block_header[78];
            final_init_img[1496] = NEW_IMAGE_REFERANCE_BYTES[1496] ^ block_header[78];
            final_init_img[1500] = NEW_IMAGE_REFERANCE_BYTES[1500] ^ block_header[78];
            final_init_img[1504] = NEW_IMAGE_REFERANCE_BYTES[1504] ^ block_header[78];
            final_init_img[1508] = NEW_IMAGE_REFERANCE_BYTES[1508] ^ block_header[78];
            final_init_img[1512] = NEW_IMAGE_REFERANCE_BYTES[1512] ^ block_header[78];
            final_init_img[1516] = NEW_IMAGE_REFERANCE_BYTES[1516] ^ block_header[78];
            final_init_img[1520] = NEW_IMAGE_REFERANCE_BYTES[1520] ^ block_header[78];
            final_init_img[1524] = NEW_IMAGE_REFERANCE_BYTES[1524] ^ block_header[78];
            final_init_img[1528] = NEW_IMAGE_REFERANCE_BYTES[1528] ^ block_header[78];
            final_init_img[1532] = NEW_IMAGE_REFERANCE_BYTES[1532] ^ block_header[78];
            final_init_img[1536] = NEW_IMAGE_REFERANCE_BYTES[1536] ^ block_header[78];
            final_init_img[1540] = NEW_IMAGE_REFERANCE_BYTES[1540] ^ block_header[78];
            final_init_img[1544] = NEW_IMAGE_REFERANCE_BYTES[1544] ^ block_header[78];
            final_init_img[1548] = NEW_IMAGE_REFERANCE_BYTES[1548] ^ block_header[78];
            final_init_img[1552] = NEW_IMAGE_REFERANCE_BYTES[1552] ^ block_header[78];
            final_init_img[1556] = NEW_IMAGE_REFERANCE_BYTES[1556] ^ block_header[78];
            final_init_img[1560] = NEW_IMAGE_REFERANCE_BYTES[1560] ^ block_header[78];
            final_init_img[1564] = NEW_IMAGE_REFERANCE_BYTES[1564] ^ block_header[78];
            final_init_img[1568] = NEW_IMAGE_REFERANCE_BYTES[1568] ^ block_header[78];
            final_init_img[1572] = NEW_IMAGE_REFERANCE_BYTES[1572] ^ block_header[78];
            final_init_img[1576] = NEW_IMAGE_REFERANCE_BYTES[1576] ^ block_header[78];
            final_init_img[1580] = NEW_IMAGE_REFERANCE_BYTES[1580] ^ block_header[78];
            final_init_img[1584] = NEW_IMAGE_REFERANCE_BYTES[1584] ^ block_header[78];
            final_init_img[1588] = NEW_IMAGE_REFERANCE_BYTES[1588] ^ block_header[78];
            final_init_img[1592] = NEW_IMAGE_REFERANCE_BYTES[1592] ^ block_header[78];
            final_init_img[1596] = NEW_IMAGE_REFERANCE_BYTES[1596] ^ block_header[78];
            final_init_img[1600] = NEW_IMAGE_REFERANCE_BYTES[1600] ^ block_header[78];
            final_init_img[1604] = NEW_IMAGE_REFERANCE_BYTES[1604] ^ block_header[78];
            final_init_img[1608] = NEW_IMAGE_REFERANCE_BYTES[1608] ^ block_header[78];
            final_init_img[1612] = NEW_IMAGE_REFERANCE_BYTES[1612] ^ block_header[78];
            final_init_img[1616] = NEW_IMAGE_REFERANCE_BYTES[1616] ^ block_header[78];
            final_init_img[1620] = NEW_IMAGE_REFERANCE_BYTES[1620] ^ block_header[78];
            final_init_img[1624] = NEW_IMAGE_REFERANCE_BYTES[1624] ^ block_header[78];
            final_init_img[1628] = NEW_IMAGE_REFERANCE_BYTES[1628] ^ block_header[78];
            final_init_img[1632] = NEW_IMAGE_REFERANCE_BYTES[1632] ^ block_header[78];
            final_init_img[1636] = NEW_IMAGE_REFERANCE_BYTES[1636] ^ block_header[78];
            final_init_img[1640] = NEW_IMAGE_REFERANCE_BYTES[1640] ^ block_header[78];
            final_init_img[1644] = NEW_IMAGE_REFERANCE_BYTES[1644] ^ block_header[78];
            final_init_img[1648] = NEW_IMAGE_REFERANCE_BYTES[1648] ^ block_header[78];
            final_init_img[1652] = NEW_IMAGE_REFERANCE_BYTES[1652] ^ block_header[78];
            final_init_img[1656] = NEW_IMAGE_REFERANCE_BYTES[1656] ^ block_header[78];
            final_init_img[1660] = NEW_IMAGE_REFERANCE_BYTES[1660] ^ block_header[78];
            final_init_img[1664] = NEW_IMAGE_REFERANCE_BYTES[1664] ^ block_header[78];
            final_init_img[1668] = NEW_IMAGE_REFERANCE_BYTES[1668] ^ block_header[78];
            final_init_img[1672] = NEW_IMAGE_REFERANCE_BYTES[1672] ^ block_header[78];
            final_init_img[1676] = NEW_IMAGE_REFERANCE_BYTES[1676] ^ block_header[78];
            final_init_img[1680] = NEW_IMAGE_REFERANCE_BYTES[1680] ^ block_header[78];
            final_init_img[1684] = NEW_IMAGE_REFERANCE_BYTES[1684] ^ block_header[78];
            final_init_img[1688] = NEW_IMAGE_REFERANCE_BYTES[1688] ^ block_header[78];
            final_init_img[1692] = NEW_IMAGE_REFERANCE_BYTES[1692] ^ block_header[78];
            final_init_img[1696] = NEW_IMAGE_REFERANCE_BYTES[1696] ^ block_header[78];
            final_init_img[1700] = NEW_IMAGE_REFERANCE_BYTES[1700] ^ block_header[78];
            final_init_img[1704] = NEW_IMAGE_REFERANCE_BYTES[1704] ^ block_header[78];
            final_init_img[1708] = NEW_IMAGE_REFERANCE_BYTES[1708] ^ block_header[78];
            final_init_img[1712] = NEW_IMAGE_REFERANCE_BYTES[1712] ^ block_header[78];
            final_init_img[1716] = NEW_IMAGE_REFERANCE_BYTES[1716] ^ block_header[78];
            final_init_img[1720] = NEW_IMAGE_REFERANCE_BYTES[1720] ^ block_header[78];
            final_init_img[1724] = NEW_IMAGE_REFERANCE_BYTES[1724] ^ block_header[78];
            final_init_img[1728] = NEW_IMAGE_REFERANCE_BYTES[1728] ^ block_header[78];
            final_init_img[1732] = NEW_IMAGE_REFERANCE_BYTES[1732] ^ block_header[78];
            final_init_img[1736] = NEW_IMAGE_REFERANCE_BYTES[1736] ^ block_header[78];
            final_init_img[1740] = NEW_IMAGE_REFERANCE_BYTES[1740] ^ block_header[78];
            final_init_img[1744] = NEW_IMAGE_REFERANCE_BYTES[1744] ^ block_header[78];
            final_init_img[1748] = NEW_IMAGE_REFERANCE_BYTES[1748] ^ block_header[78];
            final_init_img[1752] = NEW_IMAGE_REFERANCE_BYTES[1752] ^ block_header[78];
            final_init_img[1756] = NEW_IMAGE_REFERANCE_BYTES[1756] ^ block_header[78];
            final_init_img[1760] = NEW_IMAGE_REFERANCE_BYTES[1760] ^ block_header[78];
            final_init_img[1764] = NEW_IMAGE_REFERANCE_BYTES[1764] ^ block_header[78];
            final_init_img[1768] = NEW_IMAGE_REFERANCE_BYTES[1768] ^ block_header[78];
            final_init_img[1772] = NEW_IMAGE_REFERANCE_BYTES[1772] ^ block_header[78];
            final_init_img[1776] = NEW_IMAGE_REFERANCE_BYTES[1776] ^ block_header[78];
            final_init_img[1780] = NEW_IMAGE_REFERANCE_BYTES[1780] ^ block_header[78];
		}


        if (last_nonce_bytes[3] != block_header[79]){
            last_nonce_bytes[3] = block_header[79];
            final_init_img[57] = NEW_IMAGE_REFERANCE_BYTES[57] ^ block_header[79];
            final_init_img[61] = NEW_IMAGE_REFERANCE_BYTES[61] ^ block_header[79];
            final_init_img[65] = NEW_IMAGE_REFERANCE_BYTES[65] ^ block_header[79];
            final_init_img[69] = NEW_IMAGE_REFERANCE_BYTES[69] ^ block_header[79];
            final_init_img[73] = NEW_IMAGE_REFERANCE_BYTES[73] ^ block_header[79];
            final_init_img[77] = NEW_IMAGE_REFERANCE_BYTES[77] ^ block_header[79];
            final_init_img[81] = NEW_IMAGE_REFERANCE_BYTES[81] ^ block_header[79];
            final_init_img[85] = NEW_IMAGE_REFERANCE_BYTES[85] ^ block_header[79];
            final_init_img[89] = NEW_IMAGE_REFERANCE_BYTES[89] ^ block_header[79];
            final_init_img[93] = NEW_IMAGE_REFERANCE_BYTES[93] ^ block_header[79];
            final_init_img[97] = NEW_IMAGE_REFERANCE_BYTES[97] ^ block_header[79];
            final_init_img[101] = NEW_IMAGE_REFERANCE_BYTES[101] ^ block_header[79];
            final_init_img[105] = NEW_IMAGE_REFERANCE_BYTES[105] ^ block_header[79];
            final_init_img[109] = NEW_IMAGE_REFERANCE_BYTES[109] ^ block_header[79];
            final_init_img[113] = NEW_IMAGE_REFERANCE_BYTES[113] ^ block_header[79];
            final_init_img[117] = NEW_IMAGE_REFERANCE_BYTES[117] ^ block_header[79];
            final_init_img[121] = NEW_IMAGE_REFERANCE_BYTES[121] ^ block_header[79];
            final_init_img[125] = NEW_IMAGE_REFERANCE_BYTES[125] ^ block_header[79];
            final_init_img[129] = NEW_IMAGE_REFERANCE_BYTES[129] ^ block_header[79];
            final_init_img[133] = NEW_IMAGE_REFERANCE_BYTES[133] ^ block_header[79];
            final_init_img[137] = NEW_IMAGE_REFERANCE_BYTES[137] ^ block_header[79];
            final_init_img[141] = NEW_IMAGE_REFERANCE_BYTES[141] ^ block_header[79];
            final_init_img[145] = NEW_IMAGE_REFERANCE_BYTES[145] ^ block_header[79];
            final_init_img[149] = NEW_IMAGE_REFERANCE_BYTES[149] ^ block_header[79];
            final_init_img[153] = NEW_IMAGE_REFERANCE_BYTES[153] ^ block_header[79];
            final_init_img[157] = NEW_IMAGE_REFERANCE_BYTES[157] ^ block_header[79];
            final_init_img[161] = NEW_IMAGE_REFERANCE_BYTES[161] ^ block_header[79];
            final_init_img[165] = NEW_IMAGE_REFERANCE_BYTES[165] ^ block_header[79];
            final_init_img[169] = NEW_IMAGE_REFERANCE_BYTES[169] ^ block_header[79];
            final_init_img[173] = NEW_IMAGE_REFERANCE_BYTES[173] ^ block_header[79];
            final_init_img[177] = NEW_IMAGE_REFERANCE_BYTES[177] ^ block_header[79];
            final_init_img[181] = NEW_IMAGE_REFERANCE_BYTES[181] ^ block_header[79];
            final_init_img[185] = NEW_IMAGE_REFERANCE_BYTES[185] ^ block_header[79];
            final_init_img[189] = NEW_IMAGE_REFERANCE_BYTES[189] ^ block_header[79];
            final_init_img[193] = NEW_IMAGE_REFERANCE_BYTES[193] ^ block_header[79];
            final_init_img[197] = NEW_IMAGE_REFERANCE_BYTES[197] ^ block_header[79];
            final_init_img[201] = NEW_IMAGE_REFERANCE_BYTES[201] ^ block_header[79];
            final_init_img[205] = NEW_IMAGE_REFERANCE_BYTES[205] ^ block_header[79];
            final_init_img[209] = NEW_IMAGE_REFERANCE_BYTES[209] ^ block_header[79];
            final_init_img[213] = NEW_IMAGE_REFERANCE_BYTES[213] ^ block_header[79];
            final_init_img[217] = NEW_IMAGE_REFERANCE_BYTES[217] ^ block_header[79];
            final_init_img[221] = NEW_IMAGE_REFERANCE_BYTES[221] ^ block_header[79];
            final_init_img[225] = NEW_IMAGE_REFERANCE_BYTES[225] ^ block_header[79];
            final_init_img[229] = NEW_IMAGE_REFERANCE_BYTES[229] ^ block_header[79];
            final_init_img[233] = NEW_IMAGE_REFERANCE_BYTES[233] ^ block_header[79];
            final_init_img[237] = NEW_IMAGE_REFERANCE_BYTES[237] ^ block_header[79];
            final_init_img[241] = NEW_IMAGE_REFERANCE_BYTES[241] ^ block_header[79];
            final_init_img[245] = NEW_IMAGE_REFERANCE_BYTES[245] ^ block_header[79];
            final_init_img[249] = NEW_IMAGE_REFERANCE_BYTES[249] ^ block_header[79];
            final_init_img[253] = NEW_IMAGE_REFERANCE_BYTES[253] ^ block_header[79];
            final_init_img[257] = NEW_IMAGE_REFERANCE_BYTES[257] ^ block_header[79];
            final_init_img[261] = NEW_IMAGE_REFERANCE_BYTES[261] ^ block_header[79];
            final_init_img[265] = NEW_IMAGE_REFERANCE_BYTES[265] ^ block_header[79];
            final_init_img[269] = NEW_IMAGE_REFERANCE_BYTES[269] ^ block_header[79];
            final_init_img[273] = NEW_IMAGE_REFERANCE_BYTES[273] ^ block_header[79];
            final_init_img[277] = NEW_IMAGE_REFERANCE_BYTES[277] ^ block_header[79];
            final_init_img[281] = NEW_IMAGE_REFERANCE_BYTES[281] ^ block_header[79];
            final_init_img[285] = NEW_IMAGE_REFERANCE_BYTES[285] ^ block_header[79];
            final_init_img[289] = NEW_IMAGE_REFERANCE_BYTES[289] ^ block_header[79];
            final_init_img[293] = NEW_IMAGE_REFERANCE_BYTES[293] ^ block_header[79];
            final_init_img[297] = NEW_IMAGE_REFERANCE_BYTES[297] ^ block_header[79];
            final_init_img[301] = NEW_IMAGE_REFERANCE_BYTES[301] ^ block_header[79];
            final_init_img[305] = NEW_IMAGE_REFERANCE_BYTES[305] ^ block_header[79];
            final_init_img[309] = NEW_IMAGE_REFERANCE_BYTES[309] ^ block_header[79];
            final_init_img[313] = NEW_IMAGE_REFERANCE_BYTES[313] ^ block_header[79];
            final_init_img[317] = NEW_IMAGE_REFERANCE_BYTES[317] ^ block_header[79];
            final_init_img[321] = NEW_IMAGE_REFERANCE_BYTES[321] ^ block_header[79];
            final_init_img[325] = NEW_IMAGE_REFERANCE_BYTES[325] ^ block_header[79];
            final_init_img[329] = NEW_IMAGE_REFERANCE_BYTES[329] ^ block_header[79];
            final_init_img[333] = NEW_IMAGE_REFERANCE_BYTES[333] ^ block_header[79];
            final_init_img[337] = NEW_IMAGE_REFERANCE_BYTES[337] ^ block_header[79];
            final_init_img[341] = NEW_IMAGE_REFERANCE_BYTES[341] ^ block_header[79];
            final_init_img[345] = NEW_IMAGE_REFERANCE_BYTES[345] ^ block_header[79];
            final_init_img[349] = NEW_IMAGE_REFERANCE_BYTES[349] ^ block_header[79];
            final_init_img[353] = NEW_IMAGE_REFERANCE_BYTES[353] ^ block_header[79];
            final_init_img[357] = NEW_IMAGE_REFERANCE_BYTES[357] ^ block_header[79];
            final_init_img[361] = NEW_IMAGE_REFERANCE_BYTES[361] ^ block_header[79];
            final_init_img[365] = NEW_IMAGE_REFERANCE_BYTES[365] ^ block_header[79];
            final_init_img[369] = NEW_IMAGE_REFERANCE_BYTES[369] ^ block_header[79];
            final_init_img[373] = NEW_IMAGE_REFERANCE_BYTES[373] ^ block_header[79];
            final_init_img[377] = NEW_IMAGE_REFERANCE_BYTES[377] ^ block_header[79];
            final_init_img[381] = NEW_IMAGE_REFERANCE_BYTES[381] ^ block_header[79];
            final_init_img[385] = NEW_IMAGE_REFERANCE_BYTES[385] ^ block_header[79];
            final_init_img[389] = NEW_IMAGE_REFERANCE_BYTES[389] ^ block_header[79];
            final_init_img[393] = NEW_IMAGE_REFERANCE_BYTES[393] ^ block_header[79];
            final_init_img[397] = NEW_IMAGE_REFERANCE_BYTES[397] ^ block_header[79];
            final_init_img[401] = NEW_IMAGE_REFERANCE_BYTES[401] ^ block_header[79];
            final_init_img[405] = NEW_IMAGE_REFERANCE_BYTES[405] ^ block_header[79];
            final_init_img[409] = NEW_IMAGE_REFERANCE_BYTES[409] ^ block_header[79];
            final_init_img[413] = NEW_IMAGE_REFERANCE_BYTES[413] ^ block_header[79];
            final_init_img[417] = NEW_IMAGE_REFERANCE_BYTES[417] ^ block_header[79];
            final_init_img[421] = NEW_IMAGE_REFERANCE_BYTES[421] ^ block_header[79];
            final_init_img[425] = NEW_IMAGE_REFERANCE_BYTES[425] ^ block_header[79];
            final_init_img[429] = NEW_IMAGE_REFERANCE_BYTES[429] ^ block_header[79];
            final_init_img[433] = NEW_IMAGE_REFERANCE_BYTES[433] ^ block_header[79];
            final_init_img[437] = NEW_IMAGE_REFERANCE_BYTES[437] ^ block_header[79];
            final_init_img[441] = NEW_IMAGE_REFERANCE_BYTES[441] ^ block_header[79];
            final_init_img[445] = NEW_IMAGE_REFERANCE_BYTES[445] ^ block_header[79];
            final_init_img[449] = NEW_IMAGE_REFERANCE_BYTES[449] ^ block_header[79];
            final_init_img[453] = NEW_IMAGE_REFERANCE_BYTES[453] ^ block_header[79];
            final_init_img[457] = NEW_IMAGE_REFERANCE_BYTES[457] ^ block_header[79];
            final_init_img[461] = NEW_IMAGE_REFERANCE_BYTES[461] ^ block_header[79];
            final_init_img[465] = NEW_IMAGE_REFERANCE_BYTES[465] ^ block_header[79];
            final_init_img[469] = NEW_IMAGE_REFERANCE_BYTES[469] ^ block_header[79];
            final_init_img[473] = NEW_IMAGE_REFERANCE_BYTES[473] ^ block_header[79];
            final_init_img[477] = NEW_IMAGE_REFERANCE_BYTES[477] ^ block_header[79];
            final_init_img[481] = NEW_IMAGE_REFERANCE_BYTES[481] ^ block_header[79];
            final_init_img[485] = NEW_IMAGE_REFERANCE_BYTES[485] ^ block_header[79];
            final_init_img[489] = NEW_IMAGE_REFERANCE_BYTES[489] ^ block_header[79];
            final_init_img[493] = NEW_IMAGE_REFERANCE_BYTES[493] ^ block_header[79];
            final_init_img[497] = NEW_IMAGE_REFERANCE_BYTES[497] ^ block_header[79];
            final_init_img[501] = NEW_IMAGE_REFERANCE_BYTES[501] ^ block_header[79];
            final_init_img[505] = NEW_IMAGE_REFERANCE_BYTES[505] ^ block_header[79];
            final_init_img[509] = NEW_IMAGE_REFERANCE_BYTES[509] ^ block_header[79];
            final_init_img[513] = NEW_IMAGE_REFERANCE_BYTES[513] ^ block_header[79];
            final_init_img[517] = NEW_IMAGE_REFERANCE_BYTES[517] ^ block_header[79];
            final_init_img[521] = NEW_IMAGE_REFERANCE_BYTES[521] ^ block_header[79];
            final_init_img[525] = NEW_IMAGE_REFERANCE_BYTES[525] ^ block_header[79];
            final_init_img[529] = NEW_IMAGE_REFERANCE_BYTES[529] ^ block_header[79];
            final_init_img[533] = NEW_IMAGE_REFERANCE_BYTES[533] ^ block_header[79];
            final_init_img[537] = NEW_IMAGE_REFERANCE_BYTES[537] ^ block_header[79];
            final_init_img[541] = NEW_IMAGE_REFERANCE_BYTES[541] ^ block_header[79];
            final_init_img[545] = NEW_IMAGE_REFERANCE_BYTES[545] ^ block_header[79];
            final_init_img[549] = NEW_IMAGE_REFERANCE_BYTES[549] ^ block_header[79];
            final_init_img[553] = NEW_IMAGE_REFERANCE_BYTES[553] ^ block_header[79];
            final_init_img[557] = NEW_IMAGE_REFERANCE_BYTES[557] ^ block_header[79];
            final_init_img[561] = NEW_IMAGE_REFERANCE_BYTES[561] ^ block_header[79];
            final_init_img[565] = NEW_IMAGE_REFERANCE_BYTES[565] ^ block_header[79];
            final_init_img[569] = NEW_IMAGE_REFERANCE_BYTES[569] ^ block_header[79];
            final_init_img[573] = NEW_IMAGE_REFERANCE_BYTES[573] ^ block_header[79];
            final_init_img[577] = NEW_IMAGE_REFERANCE_BYTES[577] ^ block_header[79];
            final_init_img[581] = NEW_IMAGE_REFERANCE_BYTES[581] ^ block_header[79];
            final_init_img[585] = NEW_IMAGE_REFERANCE_BYTES[585] ^ block_header[79];
            final_init_img[589] = NEW_IMAGE_REFERANCE_BYTES[589] ^ block_header[79];
            final_init_img[593] = NEW_IMAGE_REFERANCE_BYTES[593] ^ block_header[79];
            final_init_img[597] = NEW_IMAGE_REFERANCE_BYTES[597] ^ block_header[79];
            final_init_img[601] = NEW_IMAGE_REFERANCE_BYTES[601] ^ block_header[79];
            final_init_img[605] = NEW_IMAGE_REFERANCE_BYTES[605] ^ block_header[79];
            final_init_img[609] = NEW_IMAGE_REFERANCE_BYTES[609] ^ block_header[79];
            final_init_img[613] = NEW_IMAGE_REFERANCE_BYTES[613] ^ block_header[79];
            final_init_img[617] = NEW_IMAGE_REFERANCE_BYTES[617] ^ block_header[79];
            final_init_img[621] = NEW_IMAGE_REFERANCE_BYTES[621] ^ block_header[79];
            final_init_img[625] = NEW_IMAGE_REFERANCE_BYTES[625] ^ block_header[79];
            final_init_img[629] = NEW_IMAGE_REFERANCE_BYTES[629] ^ block_header[79];
            final_init_img[633] = NEW_IMAGE_REFERANCE_BYTES[633] ^ block_header[79];
            final_init_img[637] = NEW_IMAGE_REFERANCE_BYTES[637] ^ block_header[79];
            final_init_img[641] = NEW_IMAGE_REFERANCE_BYTES[641] ^ block_header[79];
            final_init_img[645] = NEW_IMAGE_REFERANCE_BYTES[645] ^ block_header[79];
            final_init_img[649] = NEW_IMAGE_REFERANCE_BYTES[649] ^ block_header[79];
            final_init_img[653] = NEW_IMAGE_REFERANCE_BYTES[653] ^ block_header[79];
            final_init_img[657] = NEW_IMAGE_REFERANCE_BYTES[657] ^ block_header[79];
            final_init_img[661] = NEW_IMAGE_REFERANCE_BYTES[661] ^ block_header[79];
            final_init_img[665] = NEW_IMAGE_REFERANCE_BYTES[665] ^ block_header[79];
            final_init_img[669] = NEW_IMAGE_REFERANCE_BYTES[669] ^ block_header[79];
            final_init_img[673] = NEW_IMAGE_REFERANCE_BYTES[673] ^ block_header[79];
            final_init_img[677] = NEW_IMAGE_REFERANCE_BYTES[677] ^ block_header[79];
            final_init_img[681] = NEW_IMAGE_REFERANCE_BYTES[681] ^ block_header[79];
            final_init_img[685] = NEW_IMAGE_REFERANCE_BYTES[685] ^ block_header[79];
            final_init_img[689] = NEW_IMAGE_REFERANCE_BYTES[689] ^ block_header[79];
            final_init_img[693] = NEW_IMAGE_REFERANCE_BYTES[693] ^ block_header[79];
            final_init_img[697] = NEW_IMAGE_REFERANCE_BYTES[697] ^ block_header[79];
            final_init_img[701] = NEW_IMAGE_REFERANCE_BYTES[701] ^ block_header[79];
            final_init_img[705] = NEW_IMAGE_REFERANCE_BYTES[705] ^ block_header[79];
            final_init_img[709] = NEW_IMAGE_REFERANCE_BYTES[709] ^ block_header[79];
            final_init_img[713] = NEW_IMAGE_REFERANCE_BYTES[713] ^ block_header[79];
            final_init_img[717] = NEW_IMAGE_REFERANCE_BYTES[717] ^ block_header[79];
            final_init_img[721] = NEW_IMAGE_REFERANCE_BYTES[721] ^ block_header[79];
            final_init_img[725] = NEW_IMAGE_REFERANCE_BYTES[725] ^ block_header[79];
            final_init_img[729] = NEW_IMAGE_REFERANCE_BYTES[729] ^ block_header[79];
            final_init_img[733] = NEW_IMAGE_REFERANCE_BYTES[733] ^ block_header[79];
            final_init_img[737] = NEW_IMAGE_REFERANCE_BYTES[737] ^ block_header[79];
            final_init_img[741] = NEW_IMAGE_REFERANCE_BYTES[741] ^ block_header[79];
            final_init_img[745] = NEW_IMAGE_REFERANCE_BYTES[745] ^ block_header[79];
            final_init_img[749] = NEW_IMAGE_REFERANCE_BYTES[749] ^ block_header[79];
            final_init_img[753] = NEW_IMAGE_REFERANCE_BYTES[753] ^ block_header[79];
            final_init_img[757] = NEW_IMAGE_REFERANCE_BYTES[757] ^ block_header[79];
            final_init_img[761] = NEW_IMAGE_REFERANCE_BYTES[761] ^ block_header[79];
            final_init_img[765] = NEW_IMAGE_REFERANCE_BYTES[765] ^ block_header[79];
            final_init_img[769] = NEW_IMAGE_REFERANCE_BYTES[769] ^ block_header[79];
            final_init_img[773] = NEW_IMAGE_REFERANCE_BYTES[773] ^ block_header[79];
            final_init_img[777] = NEW_IMAGE_REFERANCE_BYTES[777] ^ block_header[79];
            final_init_img[781] = NEW_IMAGE_REFERANCE_BYTES[781] ^ block_header[79];
            final_init_img[785] = NEW_IMAGE_REFERANCE_BYTES[785] ^ block_header[79];
            final_init_img[789] = NEW_IMAGE_REFERANCE_BYTES[789] ^ block_header[79];
            final_init_img[793] = NEW_IMAGE_REFERANCE_BYTES[793] ^ block_header[79];
            final_init_img[797] = NEW_IMAGE_REFERANCE_BYTES[797] ^ block_header[79];
            final_init_img[801] = NEW_IMAGE_REFERANCE_BYTES[801] ^ block_header[79];
            final_init_img[805] = NEW_IMAGE_REFERANCE_BYTES[805] ^ block_header[79];
            final_init_img[809] = NEW_IMAGE_REFERANCE_BYTES[809] ^ block_header[79];
            final_init_img[813] = NEW_IMAGE_REFERANCE_BYTES[813] ^ block_header[79];
            final_init_img[817] = NEW_IMAGE_REFERANCE_BYTES[817] ^ block_header[79];
            final_init_img[821] = NEW_IMAGE_REFERANCE_BYTES[821] ^ block_header[79];
            final_init_img[825] = NEW_IMAGE_REFERANCE_BYTES[825] ^ block_header[79];
            final_init_img[829] = NEW_IMAGE_REFERANCE_BYTES[829] ^ block_header[79];
            final_init_img[833] = NEW_IMAGE_REFERANCE_BYTES[833] ^ block_header[79];
            final_init_img[837] = NEW_IMAGE_REFERANCE_BYTES[837] ^ block_header[79];
            final_init_img[841] = NEW_IMAGE_REFERANCE_BYTES[841] ^ block_header[79];
            final_init_img[845] = NEW_IMAGE_REFERANCE_BYTES[845] ^ block_header[79];
            final_init_img[849] = NEW_IMAGE_REFERANCE_BYTES[849] ^ block_header[79];
            final_init_img[853] = NEW_IMAGE_REFERANCE_BYTES[853] ^ block_header[79];
            final_init_img[857] = NEW_IMAGE_REFERANCE_BYTES[857] ^ block_header[79];
            final_init_img[861] = NEW_IMAGE_REFERANCE_BYTES[861] ^ block_header[79];
            final_init_img[865] = NEW_IMAGE_REFERANCE_BYTES[865] ^ block_header[79];
            final_init_img[869] = NEW_IMAGE_REFERANCE_BYTES[869] ^ block_header[79];
            final_init_img[873] = NEW_IMAGE_REFERANCE_BYTES[873] ^ block_header[79];
            final_init_img[877] = NEW_IMAGE_REFERANCE_BYTES[877] ^ block_header[79];
            final_init_img[881] = NEW_IMAGE_REFERANCE_BYTES[881] ^ block_header[79];
            final_init_img[885] = NEW_IMAGE_REFERANCE_BYTES[885] ^ block_header[79];
            final_init_img[889] = NEW_IMAGE_REFERANCE_BYTES[889] ^ block_header[79];
            final_init_img[893] = NEW_IMAGE_REFERANCE_BYTES[893] ^ block_header[79];
            final_init_img[897] = NEW_IMAGE_REFERANCE_BYTES[897] ^ block_header[79];
            final_init_img[901] = NEW_IMAGE_REFERANCE_BYTES[901] ^ block_header[79];
            final_init_img[905] = NEW_IMAGE_REFERANCE_BYTES[905] ^ block_header[79];
            final_init_img[909] = NEW_IMAGE_REFERANCE_BYTES[909] ^ block_header[79];
            final_init_img[913] = NEW_IMAGE_REFERANCE_BYTES[913] ^ block_header[79];
            final_init_img[917] = NEW_IMAGE_REFERANCE_BYTES[917] ^ block_header[79];
            final_init_img[921] = NEW_IMAGE_REFERANCE_BYTES[921] ^ block_header[79];
            final_init_img[925] = NEW_IMAGE_REFERANCE_BYTES[925] ^ block_header[79];
            final_init_img[929] = NEW_IMAGE_REFERANCE_BYTES[929] ^ block_header[79];
            final_init_img[933] = NEW_IMAGE_REFERANCE_BYTES[933] ^ block_header[79];
            final_init_img[937] = NEW_IMAGE_REFERANCE_BYTES[937] ^ block_header[79];
            final_init_img[941] = NEW_IMAGE_REFERANCE_BYTES[941] ^ block_header[79];
            final_init_img[945] = NEW_IMAGE_REFERANCE_BYTES[945] ^ block_header[79];
            final_init_img[949] = NEW_IMAGE_REFERANCE_BYTES[949] ^ block_header[79];
            final_init_img[953] = NEW_IMAGE_REFERANCE_BYTES[953] ^ block_header[79];
            final_init_img[957] = NEW_IMAGE_REFERANCE_BYTES[957] ^ block_header[79];
            final_init_img[961] = NEW_IMAGE_REFERANCE_BYTES[961] ^ block_header[79];
            final_init_img[965] = NEW_IMAGE_REFERANCE_BYTES[965] ^ block_header[79];
            final_init_img[969] = NEW_IMAGE_REFERANCE_BYTES[969] ^ block_header[79];
            final_init_img[973] = NEW_IMAGE_REFERANCE_BYTES[973] ^ block_header[79];
            final_init_img[977] = NEW_IMAGE_REFERANCE_BYTES[977] ^ block_header[79];
            final_init_img[981] = NEW_IMAGE_REFERANCE_BYTES[981] ^ block_header[79];
            final_init_img[985] = NEW_IMAGE_REFERANCE_BYTES[985] ^ block_header[79];
            final_init_img[989] = NEW_IMAGE_REFERANCE_BYTES[989] ^ block_header[79];
            final_init_img[993] = NEW_IMAGE_REFERANCE_BYTES[993] ^ block_header[79];
            final_init_img[997] = NEW_IMAGE_REFERANCE_BYTES[997] ^ block_header[79];
            final_init_img[1001] = NEW_IMAGE_REFERANCE_BYTES[1001] ^ block_header[79];
            final_init_img[1005] = NEW_IMAGE_REFERANCE_BYTES[1005] ^ block_header[79];
            final_init_img[1009] = NEW_IMAGE_REFERANCE_BYTES[1009] ^ block_header[79];
            final_init_img[1013] = NEW_IMAGE_REFERANCE_BYTES[1013] ^ block_header[79];
            final_init_img[1017] = NEW_IMAGE_REFERANCE_BYTES[1017] ^ block_header[79];
            final_init_img[1021] = NEW_IMAGE_REFERANCE_BYTES[1021] ^ block_header[79];
            final_init_img[1025] = NEW_IMAGE_REFERANCE_BYTES[1025] ^ block_header[79];
            final_init_img[1029] = NEW_IMAGE_REFERANCE_BYTES[1029] ^ block_header[79];
            final_init_img[1033] = NEW_IMAGE_REFERANCE_BYTES[1033] ^ block_header[79];
            final_init_img[1037] = NEW_IMAGE_REFERANCE_BYTES[1037] ^ block_header[79];
            final_init_img[1041] = NEW_IMAGE_REFERANCE_BYTES[1041] ^ block_header[79];
            final_init_img[1045] = NEW_IMAGE_REFERANCE_BYTES[1045] ^ block_header[79];
            final_init_img[1049] = NEW_IMAGE_REFERANCE_BYTES[1049] ^ block_header[79];
            final_init_img[1053] = NEW_IMAGE_REFERANCE_BYTES[1053] ^ block_header[79];
            final_init_img[1057] = NEW_IMAGE_REFERANCE_BYTES[1057] ^ block_header[79];
            final_init_img[1061] = NEW_IMAGE_REFERANCE_BYTES[1061] ^ block_header[79];
            final_init_img[1065] = NEW_IMAGE_REFERANCE_BYTES[1065] ^ block_header[79];
            final_init_img[1069] = NEW_IMAGE_REFERANCE_BYTES[1069] ^ block_header[79];
            final_init_img[1073] = NEW_IMAGE_REFERANCE_BYTES[1073] ^ block_header[79];
            final_init_img[1077] = NEW_IMAGE_REFERANCE_BYTES[1077] ^ block_header[79];
            final_init_img[1081] = NEW_IMAGE_REFERANCE_BYTES[1081] ^ block_header[79];
            final_init_img[1085] = NEW_IMAGE_REFERANCE_BYTES[1085] ^ block_header[79];
            final_init_img[1089] = NEW_IMAGE_REFERANCE_BYTES[1089] ^ block_header[79];
            final_init_img[1093] = NEW_IMAGE_REFERANCE_BYTES[1093] ^ block_header[79];
            final_init_img[1097] = NEW_IMAGE_REFERANCE_BYTES[1097] ^ block_header[79];
            final_init_img[1101] = NEW_IMAGE_REFERANCE_BYTES[1101] ^ block_header[79];
            final_init_img[1105] = NEW_IMAGE_REFERANCE_BYTES[1105] ^ block_header[79];
            final_init_img[1109] = NEW_IMAGE_REFERANCE_BYTES[1109] ^ block_header[79];
            final_init_img[1113] = NEW_IMAGE_REFERANCE_BYTES[1113] ^ block_header[79];
            final_init_img[1117] = NEW_IMAGE_REFERANCE_BYTES[1117] ^ block_header[79];
            final_init_img[1121] = NEW_IMAGE_REFERANCE_BYTES[1121] ^ block_header[79];
            final_init_img[1125] = NEW_IMAGE_REFERANCE_BYTES[1125] ^ block_header[79];
            final_init_img[1129] = NEW_IMAGE_REFERANCE_BYTES[1129] ^ block_header[79];
            final_init_img[1133] = NEW_IMAGE_REFERANCE_BYTES[1133] ^ block_header[79];
            final_init_img[1137] = NEW_IMAGE_REFERANCE_BYTES[1137] ^ block_header[79];
            final_init_img[1141] = NEW_IMAGE_REFERANCE_BYTES[1141] ^ block_header[79];
            final_init_img[1145] = NEW_IMAGE_REFERANCE_BYTES[1145] ^ block_header[79];
            final_init_img[1149] = NEW_IMAGE_REFERANCE_BYTES[1149] ^ block_header[79];
            final_init_img[1153] = NEW_IMAGE_REFERANCE_BYTES[1153] ^ block_header[79];
            final_init_img[1157] = NEW_IMAGE_REFERANCE_BYTES[1157] ^ block_header[79];
            final_init_img[1161] = NEW_IMAGE_REFERANCE_BYTES[1161] ^ block_header[79];
            final_init_img[1165] = NEW_IMAGE_REFERANCE_BYTES[1165] ^ block_header[79];
            final_init_img[1169] = NEW_IMAGE_REFERANCE_BYTES[1169] ^ block_header[79];
            final_init_img[1173] = NEW_IMAGE_REFERANCE_BYTES[1173] ^ block_header[79];
            final_init_img[1177] = NEW_IMAGE_REFERANCE_BYTES[1177] ^ block_header[79];
            final_init_img[1181] = NEW_IMAGE_REFERANCE_BYTES[1181] ^ block_header[79];
            final_init_img[1185] = NEW_IMAGE_REFERANCE_BYTES[1185] ^ block_header[79];
            final_init_img[1189] = NEW_IMAGE_REFERANCE_BYTES[1189] ^ block_header[79];
            final_init_img[1193] = NEW_IMAGE_REFERANCE_BYTES[1193] ^ block_header[79];
            final_init_img[1197] = NEW_IMAGE_REFERANCE_BYTES[1197] ^ block_header[79];
            final_init_img[1201] = NEW_IMAGE_REFERANCE_BYTES[1201] ^ block_header[79];
            final_init_img[1205] = NEW_IMAGE_REFERANCE_BYTES[1205] ^ block_header[79];
            final_init_img[1209] = NEW_IMAGE_REFERANCE_BYTES[1209] ^ block_header[79];
            final_init_img[1213] = NEW_IMAGE_REFERANCE_BYTES[1213] ^ block_header[79];
            final_init_img[1217] = NEW_IMAGE_REFERANCE_BYTES[1217] ^ block_header[79];
            final_init_img[1221] = NEW_IMAGE_REFERANCE_BYTES[1221] ^ block_header[79];
            final_init_img[1225] = NEW_IMAGE_REFERANCE_BYTES[1225] ^ block_header[79];
            final_init_img[1229] = NEW_IMAGE_REFERANCE_BYTES[1229] ^ block_header[79];
            final_init_img[1233] = NEW_IMAGE_REFERANCE_BYTES[1233] ^ block_header[79];
            final_init_img[1237] = NEW_IMAGE_REFERANCE_BYTES[1237] ^ block_header[79];
            final_init_img[1241] = NEW_IMAGE_REFERANCE_BYTES[1241] ^ block_header[79];
            final_init_img[1245] = NEW_IMAGE_REFERANCE_BYTES[1245] ^ block_header[79];
            final_init_img[1249] = NEW_IMAGE_REFERANCE_BYTES[1249] ^ block_header[79];
            final_init_img[1253] = NEW_IMAGE_REFERANCE_BYTES[1253] ^ block_header[79];
            final_init_img[1257] = NEW_IMAGE_REFERANCE_BYTES[1257] ^ block_header[79];
            final_init_img[1261] = NEW_IMAGE_REFERANCE_BYTES[1261] ^ block_header[79];
            final_init_img[1265] = NEW_IMAGE_REFERANCE_BYTES[1265] ^ block_header[79];
            final_init_img[1269] = NEW_IMAGE_REFERANCE_BYTES[1269] ^ block_header[79];
            final_init_img[1273] = NEW_IMAGE_REFERANCE_BYTES[1273] ^ block_header[79];
            final_init_img[1277] = NEW_IMAGE_REFERANCE_BYTES[1277] ^ block_header[79];
            final_init_img[1281] = NEW_IMAGE_REFERANCE_BYTES[1281] ^ block_header[79];
            final_init_img[1285] = NEW_IMAGE_REFERANCE_BYTES[1285] ^ block_header[79];
            final_init_img[1289] = NEW_IMAGE_REFERANCE_BYTES[1289] ^ block_header[79];
            final_init_img[1293] = NEW_IMAGE_REFERANCE_BYTES[1293] ^ block_header[79];
            final_init_img[1297] = NEW_IMAGE_REFERANCE_BYTES[1297] ^ block_header[79];
            final_init_img[1301] = NEW_IMAGE_REFERANCE_BYTES[1301] ^ block_header[79];
            final_init_img[1305] = NEW_IMAGE_REFERANCE_BYTES[1305] ^ block_header[79];
            final_init_img[1309] = NEW_IMAGE_REFERANCE_BYTES[1309] ^ block_header[79];
            final_init_img[1313] = NEW_IMAGE_REFERANCE_BYTES[1313] ^ block_header[79];
            final_init_img[1317] = NEW_IMAGE_REFERANCE_BYTES[1317] ^ block_header[79];
            final_init_img[1321] = NEW_IMAGE_REFERANCE_BYTES[1321] ^ block_header[79];
            final_init_img[1325] = NEW_IMAGE_REFERANCE_BYTES[1325] ^ block_header[79];
            final_init_img[1329] = NEW_IMAGE_REFERANCE_BYTES[1329] ^ block_header[79];
            final_init_img[1333] = NEW_IMAGE_REFERANCE_BYTES[1333] ^ block_header[79];
            final_init_img[1337] = NEW_IMAGE_REFERANCE_BYTES[1337] ^ block_header[79];
            final_init_img[1341] = NEW_IMAGE_REFERANCE_BYTES[1341] ^ block_header[79];
            final_init_img[1345] = NEW_IMAGE_REFERANCE_BYTES[1345] ^ block_header[79];
            final_init_img[1349] = NEW_IMAGE_REFERANCE_BYTES[1349] ^ block_header[79];
            final_init_img[1353] = NEW_IMAGE_REFERANCE_BYTES[1353] ^ block_header[79];
            final_init_img[1357] = NEW_IMAGE_REFERANCE_BYTES[1357] ^ block_header[79];
            final_init_img[1361] = NEW_IMAGE_REFERANCE_BYTES[1361] ^ block_header[79];
            final_init_img[1365] = NEW_IMAGE_REFERANCE_BYTES[1365] ^ block_header[79];
            final_init_img[1369] = NEW_IMAGE_REFERANCE_BYTES[1369] ^ block_header[79];
            final_init_img[1373] = NEW_IMAGE_REFERANCE_BYTES[1373] ^ block_header[79];
            final_init_img[1377] = NEW_IMAGE_REFERANCE_BYTES[1377] ^ block_header[79];
            final_init_img[1381] = NEW_IMAGE_REFERANCE_BYTES[1381] ^ block_header[79];
            final_init_img[1385] = NEW_IMAGE_REFERANCE_BYTES[1385] ^ block_header[79];
            final_init_img[1389] = NEW_IMAGE_REFERANCE_BYTES[1389] ^ block_header[79];
            final_init_img[1393] = NEW_IMAGE_REFERANCE_BYTES[1393] ^ block_header[79];
            final_init_img[1397] = NEW_IMAGE_REFERANCE_BYTES[1397] ^ block_header[79];
            final_init_img[1401] = NEW_IMAGE_REFERANCE_BYTES[1401] ^ block_header[79];
            final_init_img[1405] = NEW_IMAGE_REFERANCE_BYTES[1405] ^ block_header[79];
            final_init_img[1409] = NEW_IMAGE_REFERANCE_BYTES[1409] ^ block_header[79];
            final_init_img[1413] = NEW_IMAGE_REFERANCE_BYTES[1413] ^ block_header[79];
            final_init_img[1417] = NEW_IMAGE_REFERANCE_BYTES[1417] ^ block_header[79];
            final_init_img[1421] = NEW_IMAGE_REFERANCE_BYTES[1421] ^ block_header[79];
            final_init_img[1425] = NEW_IMAGE_REFERANCE_BYTES[1425] ^ block_header[79];
            final_init_img[1429] = NEW_IMAGE_REFERANCE_BYTES[1429] ^ block_header[79];
            final_init_img[1433] = NEW_IMAGE_REFERANCE_BYTES[1433] ^ block_header[79];
            final_init_img[1437] = NEW_IMAGE_REFERANCE_BYTES[1437] ^ block_header[79];
            final_init_img[1441] = NEW_IMAGE_REFERANCE_BYTES[1441] ^ block_header[79];
            final_init_img[1445] = NEW_IMAGE_REFERANCE_BYTES[1445] ^ block_header[79];
            final_init_img[1449] = NEW_IMAGE_REFERANCE_BYTES[1449] ^ block_header[79];
            final_init_img[1453] = NEW_IMAGE_REFERANCE_BYTES[1453] ^ block_header[79];
            final_init_img[1457] = NEW_IMAGE_REFERANCE_BYTES[1457] ^ block_header[79];
            final_init_img[1461] = NEW_IMAGE_REFERANCE_BYTES[1461] ^ block_header[79];
            final_init_img[1465] = NEW_IMAGE_REFERANCE_BYTES[1465] ^ block_header[79];
            final_init_img[1469] = NEW_IMAGE_REFERANCE_BYTES[1469] ^ block_header[79];
            final_init_img[1473] = NEW_IMAGE_REFERANCE_BYTES[1473] ^ block_header[79];
            final_init_img[1477] = NEW_IMAGE_REFERANCE_BYTES[1477] ^ block_header[79];
            final_init_img[1481] = NEW_IMAGE_REFERANCE_BYTES[1481] ^ block_header[79];
            final_init_img[1485] = NEW_IMAGE_REFERANCE_BYTES[1485] ^ block_header[79];
            final_init_img[1489] = NEW_IMAGE_REFERANCE_BYTES[1489] ^ block_header[79];
            final_init_img[1493] = NEW_IMAGE_REFERANCE_BYTES[1493] ^ block_header[79];
            final_init_img[1497] = NEW_IMAGE_REFERANCE_BYTES[1497] ^ block_header[79];
            final_init_img[1501] = NEW_IMAGE_REFERANCE_BYTES[1501] ^ block_header[79];
            final_init_img[1505] = NEW_IMAGE_REFERANCE_BYTES[1505] ^ block_header[79];
            final_init_img[1509] = NEW_IMAGE_REFERANCE_BYTES[1509] ^ block_header[79];
            final_init_img[1513] = NEW_IMAGE_REFERANCE_BYTES[1513] ^ block_header[79];
            final_init_img[1517] = NEW_IMAGE_REFERANCE_BYTES[1517] ^ block_header[79];
            final_init_img[1521] = NEW_IMAGE_REFERANCE_BYTES[1521] ^ block_header[79];
            final_init_img[1525] = NEW_IMAGE_REFERANCE_BYTES[1525] ^ block_header[79];
            final_init_img[1529] = NEW_IMAGE_REFERANCE_BYTES[1529] ^ block_header[79];
            final_init_img[1533] = NEW_IMAGE_REFERANCE_BYTES[1533] ^ block_header[79];
            final_init_img[1537] = NEW_IMAGE_REFERANCE_BYTES[1537] ^ block_header[79];
            final_init_img[1541] = NEW_IMAGE_REFERANCE_BYTES[1541] ^ block_header[79];
            final_init_img[1545] = NEW_IMAGE_REFERANCE_BYTES[1545] ^ block_header[79];
            final_init_img[1549] = NEW_IMAGE_REFERANCE_BYTES[1549] ^ block_header[79];
            final_init_img[1553] = NEW_IMAGE_REFERANCE_BYTES[1553] ^ block_header[79];
            final_init_img[1557] = NEW_IMAGE_REFERANCE_BYTES[1557] ^ block_header[79];
            final_init_img[1561] = NEW_IMAGE_REFERANCE_BYTES[1561] ^ block_header[79];
            final_init_img[1565] = NEW_IMAGE_REFERANCE_BYTES[1565] ^ block_header[79];
            final_init_img[1569] = NEW_IMAGE_REFERANCE_BYTES[1569] ^ block_header[79];
            final_init_img[1573] = NEW_IMAGE_REFERANCE_BYTES[1573] ^ block_header[79];
            final_init_img[1577] = NEW_IMAGE_REFERANCE_BYTES[1577] ^ block_header[79];
            final_init_img[1581] = NEW_IMAGE_REFERANCE_BYTES[1581] ^ block_header[79];
            final_init_img[1585] = NEW_IMAGE_REFERANCE_BYTES[1585] ^ block_header[79];
            final_init_img[1589] = NEW_IMAGE_REFERANCE_BYTES[1589] ^ block_header[79];
            final_init_img[1593] = NEW_IMAGE_REFERANCE_BYTES[1593] ^ block_header[79];
            final_init_img[1597] = NEW_IMAGE_REFERANCE_BYTES[1597] ^ block_header[79];
            final_init_img[1601] = NEW_IMAGE_REFERANCE_BYTES[1601] ^ block_header[79];
            final_init_img[1605] = NEW_IMAGE_REFERANCE_BYTES[1605] ^ block_header[79];
            final_init_img[1609] = NEW_IMAGE_REFERANCE_BYTES[1609] ^ block_header[79];
            final_init_img[1613] = NEW_IMAGE_REFERANCE_BYTES[1613] ^ block_header[79];
            final_init_img[1617] = NEW_IMAGE_REFERANCE_BYTES[1617] ^ block_header[79];
            final_init_img[1621] = NEW_IMAGE_REFERANCE_BYTES[1621] ^ block_header[79];
            final_init_img[1625] = NEW_IMAGE_REFERANCE_BYTES[1625] ^ block_header[79];
            final_init_img[1629] = NEW_IMAGE_REFERANCE_BYTES[1629] ^ block_header[79];
            final_init_img[1633] = NEW_IMAGE_REFERANCE_BYTES[1633] ^ block_header[79];
            final_init_img[1637] = NEW_IMAGE_REFERANCE_BYTES[1637] ^ block_header[79];
            final_init_img[1641] = NEW_IMAGE_REFERANCE_BYTES[1641] ^ block_header[79];
            final_init_img[1645] = NEW_IMAGE_REFERANCE_BYTES[1645] ^ block_header[79];
            final_init_img[1649] = NEW_IMAGE_REFERANCE_BYTES[1649] ^ block_header[79];
            final_init_img[1653] = NEW_IMAGE_REFERANCE_BYTES[1653] ^ block_header[79];
            final_init_img[1657] = NEW_IMAGE_REFERANCE_BYTES[1657] ^ block_header[79];
            final_init_img[1661] = NEW_IMAGE_REFERANCE_BYTES[1661] ^ block_header[79];
            final_init_img[1665] = NEW_IMAGE_REFERANCE_BYTES[1665] ^ block_header[79];
            final_init_img[1669] = NEW_IMAGE_REFERANCE_BYTES[1669] ^ block_header[79];
            final_init_img[1673] = NEW_IMAGE_REFERANCE_BYTES[1673] ^ block_header[79];
            final_init_img[1677] = NEW_IMAGE_REFERANCE_BYTES[1677] ^ block_header[79];
            final_init_img[1681] = NEW_IMAGE_REFERANCE_BYTES[1681] ^ block_header[79];
            final_init_img[1685] = NEW_IMAGE_REFERANCE_BYTES[1685] ^ block_header[79];
            final_init_img[1689] = NEW_IMAGE_REFERANCE_BYTES[1689] ^ block_header[79];
            final_init_img[1693] = NEW_IMAGE_REFERANCE_BYTES[1693] ^ block_header[79];
            final_init_img[1697] = NEW_IMAGE_REFERANCE_BYTES[1697] ^ block_header[79];
            final_init_img[1701] = NEW_IMAGE_REFERANCE_BYTES[1701] ^ block_header[79];
            final_init_img[1705] = NEW_IMAGE_REFERANCE_BYTES[1705] ^ block_header[79];
            final_init_img[1709] = NEW_IMAGE_REFERANCE_BYTES[1709] ^ block_header[79];
            final_init_img[1713] = NEW_IMAGE_REFERANCE_BYTES[1713] ^ block_header[79];
            final_init_img[1717] = NEW_IMAGE_REFERANCE_BYTES[1717] ^ block_header[79];
            final_init_img[1721] = NEW_IMAGE_REFERANCE_BYTES[1721] ^ block_header[79];
            final_init_img[1725] = NEW_IMAGE_REFERANCE_BYTES[1725] ^ block_header[79];
            final_init_img[1729] = NEW_IMAGE_REFERANCE_BYTES[1729] ^ block_header[79];
            final_init_img[1733] = NEW_IMAGE_REFERANCE_BYTES[1733] ^ block_header[79];
            final_init_img[1737] = NEW_IMAGE_REFERANCE_BYTES[1737] ^ block_header[79];
            final_init_img[1741] = NEW_IMAGE_REFERANCE_BYTES[1741] ^ block_header[79];
            final_init_img[1745] = NEW_IMAGE_REFERANCE_BYTES[1745] ^ block_header[79];
            final_init_img[1749] = NEW_IMAGE_REFERANCE_BYTES[1749] ^ block_header[79];
            final_init_img[1753] = NEW_IMAGE_REFERANCE_BYTES[1753] ^ block_header[79];
            final_init_img[1757] = NEW_IMAGE_REFERANCE_BYTES[1757] ^ block_header[79];
            final_init_img[1761] = NEW_IMAGE_REFERANCE_BYTES[1761] ^ block_header[79];
            final_init_img[1765] = NEW_IMAGE_REFERANCE_BYTES[1765] ^ block_header[79];
            final_init_img[1769] = NEW_IMAGE_REFERANCE_BYTES[1769] ^ block_header[79];
            final_init_img[1773] = NEW_IMAGE_REFERANCE_BYTES[1773] ^ block_header[79];
            final_init_img[1777] = NEW_IMAGE_REFERANCE_BYTES[1777] ^ block_header[79];
            final_init_img[1781] = NEW_IMAGE_REFERANCE_BYTES[1781] ^ block_header[79];
       

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
	for(i=0;i<80;i++)
		output_buff.push_back (block_header[i]);
	

	
	CSHA256().Write((const unsigned char*)&output_buff[0], 1782+80).Finalize((uint8_t *)output);	
	
	
}




int ocv2_test_algo(){
	
/*
	It is necessary to check whether opencv is compiled correctly and verify that the cpu is able to correctly calculate our algorithm.
	Some processors use different float point arithmetic and our algo doesn't work properly on them.
	Also opencv compilation flags ( like avx sse eg..) may break this algorithm.
	So we will test.
*/
	
	
		
		



	char block_headers[] = "\x00\x00\x00\x20\x61\x53\x97\x9f\x8e\xb4\x9f\x13\xa8\xb0\x04\x60\x89\x0b\x97\x2e\x6e\x01\x8a\xe8\x0e\x4e\x47\x24\x8e\x9e\xcf\x0e\x18\x00\x00\x00\x78\xfa\x8e\x7b\x87\xcc\xf4\x28\xa1\xe4\x04\xcd\x16\x1f\x32\xf6\xed\x0f\x50\x19\x1a\xc5\x56\x36\x2e\x86\xf1\xc2\x35\xb0\x76\xd4\xbc\xb1\x88\x61\xef\x58\x4b\x1d\x46\x94\x88\x77\x00\x00\x00\x20\xbd\x41\x10\x7d\x52\x18\x15\x7f\xa7\xa2\x08\xda\xe1\xfc\x59\x19\x58\x2a\x65\x47\x87\x6b\x4b\xa4\x1e\x90\x0c\x1e\x49\x00\x00\x00\x17\xa4\x5a\x5b\xba\x2d\x30\x4c\xf6\x52\xe7\x93\xc8\x9b\x2b\xcc\xd3\x82\x51\x11\xa3\xf8\xb9\xb3\x71\x13\xca\xf1\x8d\xa1\xdc\x5b\xaa\xb2\x88\x61\xef\x58\x4b\x1d\x0c\x1f\x17\x11\x00\x00\x00\x20\x40\x14\xf9\xc5\xf5\x0d\x8c\xae\x32\xa6\x49\xa2\x14\x30\x8b\x24\x1f\x49\x3a\x56\xa5\xd4\x46\x7a\xf4\x97\x62\xd6\x1b\x00\x00\x00\xaa\xea\x81\xc6\xcb\x69\xc5\x65\x24\x08\x66\x18\xb3\x8d\xeb\x25\x81\x3e\x07\xee\xef\x7b\x3e\x17\x76\x8a\x7c\xd1\xdd\x1a\x4b\xe6\xd1\xb3\x88\x61\xef\x58\x4b\x1d\x92\x06\x87\x77\x00\x00\x00\x20\xaa\xb0\xc5\x64\x46\xaf\x33\x6b\x7c\xb0\x06\xf3\x52\x48\xdd\x56\x85\x78\x76\x07\x99\xda\x08\xd3\x27\xbe\x34\x17\x44\x00\x00\x00\xf0\xc6\x90\xb1\x67\xac\xfe\x65\xf8\xe3\x8a\x07\x50\x6c\x9b\xe1\x52\xcc\xaf\x5a\x49\x86\x58\x9e\xb9\x62\x92\xed\x63\x93\x7a\x6d\xeb\xb3\x88\x61\xef\x58\x4b\x1d\x42\x64\x8b\x88\x00\x00\x00\x20\xff\x04\x7c\x46\x78\x18\x0d\xa8\x01\xb9\x7e\x6d\xf6\x2a\xfc\x62\xca\x0b\x5a\x2d\xbc\x6b\x49\x5e\xa8\x3d\x49\x32\x1e\x00\x00\x00\xcd\x3f\xfa\xe9\xb9\x1e\x24\x66\xd2\x41\x36\x4b\x79\xec\xb5\x7f\xbf\x3e\x3c\xba\x0e\xee\xe5\x2e\xd1\x89\x6a\x58\x6d\x18\x73\x9c\x20\xb5\x88\x61\xef\x58\x4b\x1d\x5f\x19\x9c\x99\x00\x00\x00\x20\xbe\x34\xc3\xa2\x45\xb4\x62\x46\x4b\xef\x80\x6b\x9c\x71\x4b\xb3\xb5\x5b\x5e\xd2\x65\xd8\x71\x3a\x83\x87\xc3\xd6\x11\x00\x00\x00\xd5\xbc\x65\xfa\x1c\xc9\x6e\xe7\x06\x2f\x47\xad\x50\xfd\xe5\x07\x5d\xfc\x23\xd3\xda\x63\x52\xf1\xef\x52\x31\xa7\x15\xa0\x0c\xfb\x9f\xb5\x88\x61\xef\x58\x4b\x1d\xc6\xaa\xfb\xdd\x00\x00\x00\x20\xcb\xc4\x6a\x48\x3a\xa1\x90\x36\xb5\xa8\x9e\x56\xa1\xa7\xa0\xb9\x8f\xda\xe1\x3a\xc9\xd6\xcb\x0f\x74\xed\xd9\xde\x11\x00\x00\x00\x24\x2b\xd5\x9a\xbe\x5d\x2a\x6a\xae\xf9\x0e\x5e\x90\x46\x3e\xc9\xaa\xe0\x04\xeb\xd2\xcd\xbd\x28\x7d\x6f\x80\xd6\xb0\xdc\x96\x2e\x17\xb6\x88\x61\xef\x58\x4b\x1d\x86\x8d\xde\xdd\x00\x00\x00\x20\xa9\x58\x29\x9f\x9a\x3d\x11\xd7\xca\x3c\xbd\xf7\x56\xda\xa1\xdd\x4a\x4c\x17\x4a\x6e\xf7\x2f\xe0\xc6\x0a\x1a\x0e\x2d\x00\x00\x00\xc7\xaf\x84\x2b\xf0\x7f\x11\x33\x93\x3c\x2c\xa9\x9e\xc0\xa7\x6a\x3a\x0b\xfd\x3a\x48\xf9\x99\xfb\x4b\xd4\xda\x77\xa0\xbd\xa7\x71\x85\xb7\x88\x61\xef\x58\x4b\x1d\x5a\x88\x7e\x77\x00\x00\x00\x20\x2d\x83\x8e\xa8\xd6\xa7\xe4\xb7\x6d\xe9\x8d\x62\x6c\x4a\xa7\x31\x31\xd5\xc5\x18\xe0\x1a\x64\x7f\x65\xd6\x36\x9f\x3a\x00\x00\x00\x59\x02\x5c\x43\xa4\xd9\xa1\x65\xb8\x5e\xbd\xcc\x8e\x3b\xa5\x1a\x38\x01\xee\xaa\x67\xfe\xa2\x4e\x06\xc1\x96\x6b\x37\xf1\x43\x8f\x1d\xb8\x88\x61\xef\x58\x4b\x1d\x75\xdd\xbb\xbb\x00\x00\x00\x20\x56\x1b\xd4\x6d\x9d\xb5\x66\x23\x6b\xca\xab\x59\x0c\x1f\xa0\xad\x39\x4c\xe0\x25\xc6\x64\x62\xaa\xf5\x87\xbb\xde\x00\x00\x00\x00\x13\xce\xdc\xfa\xab\xae\x47\x76\x16\x81\x66\xc1\x50\x2d\x9b\x3d\x01\xf4\x1b\x72\x5c\x31\x14\x24\x8f\x04\x5a\x4f\x22\x4b\x79\x16\xdb\xb8\x88\x61\xef\x58\x4b\x1d\x37\x1c\xff\xdd\x00\x00\x00\x20\x58\x2d\x27\x8e\x3a\xd6\xcc\x5e\xfa\x60\xd7\xbc\xb8\x15\x90\xe3\xdf\x1b\x34\x9b\x15\x2e\x72\xcc\x58\x36\x7f\x9f\x18\x00\x00\x00\x7a\xb8\xd5\x8e\x2f\x10\x64\x60\x13\xa6\xcd\x4f\xce\xeb\xdf\xda\x45\xf1\xc0\x1f\xec\x41\x51\x4c\x43\x9f\xdb\x90\x92\xac\x60\xdf\x14\xb9\x88\x61\xef\x58\x4b\x1d\xfe\x81\xc1\xbb\x00\x00\x00\x20\x9b\xad\x85\xd9\x09\xe9\x16\x55\x11\xff\xa2\x90\xef\xff\x8c\xb6\xf8\x31\xac\xb5\xfa\xdf\xf8\xc1\x11\x36\x87\xf6\x25\x00\x00\x00\x36\x48\x54\x6e\xd3\xfc\x30\x26\x8a\x1f\xbb\xbd\x29\x92\xb7\xef\x80\x7d\x4f\xaa\x8d\x65\xfc\x0e\x29\x6b\xe9\x2e\xfd\x92\x05\xe4\xbe\xba\x88\x61\xef\x58\x4b\x1d\x44\x92\xa3\x99\x00\x00\x00\x20\xf1\xf2\x75\x14\x1e\x0a\xaf\x73\xc8\x8e\xfb\x40\xe2\x85\xb6\xf3\xbf\x1e\x8d\xc5\x9f\xbb\xe6\x43\x34\x96\x35\x08\x13\x00\x00\x00\x64\x7b\xff\xfe\x1e\xf9\x70\x6f\x80\xf3\x29\x3d\xdf\x76\xdc\x94\x6c\xfb\x1c\x7d\x31\xff\x2c\xe5\xd7\xa1\xcb\x0c\x83\x55\xf2\x0a\xba\xbb\x88\x61\xef\x58\x4b\x1d\x6d\x46\xbf\xbb\x00\x00\x00\x20\xcf\x1b\x2b\x39\xe0\x0c\xf0\xb7\xbc\xe6\xa6\xe8\xf2\x45\x4d\x0b\xb8\x7d\x43\x73\x9d\x8e\x32\x69\x22\x16\xef\x9a\x3b\x00\x00\x00\xa6\x0a\xf9\x13\xa5\x95\x30\x44\x1f\xde\x00\x28\x46\x77\x95\xfa\x75\x68\x2c\x33\x53\xed\xd7\x9d\x19\x64\xd8\x3a\xf4\x56\x68\xb8\x9d\xbc\x88\x61\xef\x58\x4b\x1d\xf2\x0b\x09\x00\x00\x00\x00\x20\xaa\x32\x89\x40\xa5\x2a\x9b\xeb\xca\xd5\x9b\x52\x15\x36\x3c\x5f\x1b\xeb\x3e\xc7\x7d\x66\x01\xac\x61\xd8\x32\x09\x11\x00\x00\x00\x28\x61\x8e\xe5\xf5\x6f\xe9\x07\xcf\xab\xd3\x42\x42\x80\x1f\x8a\x4d\x09\x54\x51\x59\x15\x6e\x89\x4f\xb9\x72\x91\x56\x2c\xa9\x22\x8d\xbd\x88\x61\xef\x58\x4b\x1d\x8b\xcb\x99\x88\x00\x00\x00\x20\x99\x39\x0f\x6f\x92\x4d\x48\x9e\x92\x47\x67\x3e\x4a\x0b\xa8\x02\x8e\x9f\xe6\x01\xeb\x39\x25\xfe\xed\xab\x2b\xc9\x33\x00\x00\x00\x11\xa0\xc8\xf3\xcd\xed\xb9\xb3\xcc\x65\x94\x4f\xac\x60\xc6\xd4\xca\x85\x36\x3a\xd2\x92\x7d\xba\x9d\x89\xd6\x2b\xbd\x2e\x37\x7e\x2c\xbe\x88\x61\xef\x58\x4b\x1d\xa0\x1c\x3c\x22\x00\x00\x00\x20\x7b\xc4\x2a\xf7\x4e\x9e\xdd\xa6\x6a\xc8\xe2\xcb\x54\x21\x5a\x2f\xab\x78\x7c\xd8\x90\x1a\xa5\xb6\xfe\x42\x6c\xa3\x17\x00\x00\x00\x21\xfd\x00\xea\xb7\x45\x0f\xda\x74\x05\x26\x98\x7d\xe0\x2f\x3a\xb3\x04\x15\x30\x7a\xf7\xe8\x9c\x10\x04\xd5\xd1\x38\xe2\x40\xd7\x5d\xbe\x88\x61\xef\x58\x4b\x1d\x39\x4c\x93\x88\x00\x00\x00\x20\xe5\x6f\x03\x03\xf2\x14\x65\xdc\xde\x89\xdd\x0f\xdc\xa6\xe6\x49\x71\x44\x7f\x48\xbc\x20\xec\x88\x63\x9b\x71\xb4\x40\x00\x00\x00\x5c\x4b\x48\x5d\x92\x31\x1d\x09\x11\xb2\xec\xef\x5c\x79\x58\x99\xbb\x61\xcf\x96\x0b\x67\x91\x2b\xaa\xb8\xc1\x70\x74\x4f\xad\x72\x73\xbe\x88\x61\xef\x58\x4b\x1d\xf7\x69\xf3\xee\x00\x00\x00\x20\x21\x17\x89\x3f\x30\x16\x6d\x71\x4c\x6e\x9f\x44\xeb\x31\xff\x72\x69\xce\x25\xa1\x77\xe8\x60\x28\x24\xaf\x96\x57\x2d\x00\x00\x00\x9b\x2f\xde\xed\xa6\x68\xe6\x0a\x4d\x5f\x04\xd3\x88\x05\x76\x2c\x65\x65\x65\xfc\x02\xcd\x52\x7d\xce\x4b\xd1\x15\x71\x20\x11\x9a\x87\xbe\x88\x61\xef\x58\x4b\x1d\x57\xf9\x18\x80\x00\x00\x00\x20\x60\x65\x05\xfa\x2a\x49\xc7\xdf\x9b\xf8\xab\xa4\xcb\x90\xc4\x07\xcd\x77\x56\x98\x86\x09\x00\x0b\xd6\x48\x16\x99\x36\x00\x00\x00\x6f\xf9\xee\x01\x49\x9c\x0c\xab\xda\xc5\xf6\xdf\xe2\xfe\x65\xe3\x01\x50\x4d\x71\xcd\x5a\xf2\x65\x0b\x90\x66\x8d\x5b\xaf\x92\x4f\x25\xbf\x88\x61\xef\x58\x4b\x1d\xad\x63\xce\xcc\x00\x00\x00\x20\x08\x62\xf3\xa3\xc9\xb2\xfb\x14\xa4\xd2\xc7\x8c\x07\xac\x02\xda\xd7\xca\xc6\xa3\x4f\xad\x61\x78\xf3\x39\xd1\x7c\x28\x00\x00\x00\xd2\x25\x3e\x04\xeb\x38\x6f\x18\xef\x68\x62\xae\xcb\x13\xdf\xbd\x12\x5e\x44\xc6\x5d\xec\x22\x2e\x2f\xb2\xde\xa8\xae\x10\xba\x05\xb6\xbf\x88\x61\xef\x58\x4b\x1d\x55\x6f\xe1\xdd\x00\x00\x00\x20\x4d\xe3\x30\x30\xf0\xb2\xbf\x89\xfc\x63\x0a\xbe\xa8\x6b\xd5\xa5\x01\x83\x9c\xa5\xd1\x39\xac\x40\x1d\xc3\x13\x5f\x17\x00\x00\x00\x9c\xf7\xb4\x01\xc9\xab\x6c\x31\x6e\xb8\x2f\x9b\x19\xb5\xd9\x11\x12\xb1\xf5\xe6\x7b\xc4\x3a\x5c\x5d\xae\xbc\xd1\x1e\x4c\xb9\x27\x09\xc2\x88\x61\xef\x58\x4b\x1d\x18\x28\x5d\x95\x00\x00\x00\x20\x13\xc7\xbc\x9d\x96\x6c\x4d\x8f\x1b\xc5\x0f\x2a\xa3\x2d\xa0\x53\x74\x17\x55\xc9\x47\x19\x15\xd8\xbc\x70\x3a\x47\x20\x00\x00\x00\xcc\x5b\x3c\x0e\xdd\xc0\x34\x69\xa3\x69\xf9\x0b\xb7\x8a\xcb\xc4\x95\x9c\xb5\x0f\xc4\x12\x05\x22\x9b\x69\xff\x21\x65\x9a\xdd\x36\x60\xc2\x88\x61\xef\x58\x4b\x1d\x5e\x08\x02\x20\x00\x00\x00\x20\x6a\xfd\x2a\xf6\xb5\x2f\xb9\x8b\x4e\xdb\xa4\x5f\x10\xc8\xa4\xf0\x87\xbe\x5f\x9d\xec\xb2\xfb\xd1\x5f\xe5\x93\x2b\x15\x00\x00\x00\xf1\xb1\x05\xb9\x19\x08\xf8\x97\x05\xb9\x91\x2c\x7f\xd2\xa9\x02\x7f\x07\xde\xa6\xcf\xda\xbe\x13\xee\x8e\x15\x1d\xad\x4f\x81\xcc\x58\xd1\x88\x61\xef\x58\x4b\x1d\xd8\xa1\x05\xc0\x00\x00\x00\x20\xd7\x7b\x56\x4f\x7d\x9e\x3c\x78\x11\xa6\x8b\x37\xae\xe5\xb1\x7f\x15\xd6\x44\x37\xbf\x6b\x25\x43\x0c\xd8\x7c\xc6\x07\x00\x00\x00\xd9\x7e\x66\xcc\xbb\xb5\x4c\x02\xd7\x14\x0c\xb7\x2b\x5c\x99\xbb\x7d\x97\xf3\x06\x93\x66\xf2\x5b\x9c\x17\x39\xdb\x13\xe8\xb5\xd9\xe4\xfa\x88\x61\xef\x58\x4b\x1d\x7c\x83\x03\x20\x00\x00\x00\x20\xf4\xb2\xb7\x82\x84\xd2\xc8\x63\x85\xfe\x4b\xcf\x5c\xb8\x04\x7a\xd1\xe9\xa8\x22\xf7\x4e\xbd\x83\x19\x7e\x38\xf8\x30\x00\x00\x00\x18\x41\x68\x8b\x39\x85\xcc\x2b\xda\x12\xcd\x20\xae\xed\xfd\x3e\xaf\xf6\x8d\xff\xfd\xe4\x11\xf5\x74\x67\xec\x3e\x28\x92\xba\x0e\xf0\xfa\x88\x61\xef\x58\x4b\x1d\x28\x8c\x0a\xe0\x00\x00\x00\x20\xaa\x27\x5c\x10\x1c\xdd\x03\xce\x75\x79\xd8\xf0\x7f\x93\x8b\x74\x64\x70\x46\xc4\xef\x5d\xfd\x95\x8f\xf5\x82\x39\x26\x00\x00\x00\xa8\x1e\x9e\xae\xcf\xe1\x8a\x94\xaf\x5c\xe8\x17\x32\x19\x20\xbe\xae\xdb\xa9\x27\x42\x58\x26\x0d\xae\x1c\x13\x68\xfa\xd1\x37\x8e\x20\xfb\x88\x61\xef\x58\x4b\x1d\xdb\x44\x00\xc0\x00\x00\x00\x20\x8c\x8e\x43\x2c\xab\x2b\xf2\x14\xcd\x92\x8c\x00\xa8\xc4\xde\xf5\xf0\x6b\xad\x07\x53\xc6\xe9\x2f\x9d\xbe\x6d\x42\x0b\x00\x00\x00\xbd\x92\x22\xb0\xa8\x9e\xd0\x07\x32\x12\xb9\xb9\x35\x45\x93\x7c\x40\x7d\xdd\xff\x28\x2c\x52\x41\x3b\xd6\x11\x00\xf3\x65\x19\x1f\x21\xfb\x88\x61\xef\x58\x4b\x1d\xef\x2a\x0c\x00\x00\x00\x00\x20\xd7\xbc\x38\x75\xd9\x09\xb4\xb5\xcc\x18\x0b\x2a\x47\x16\x17\x65\x0b\x3b\x93\xf0\x09\x42\x34\xe5\xd2\x98\xa7\x0e\x2a\x00\x00\x00\x5f\x49\x73\xdb\xb1\x6c\x88\x5d\xf8\x18\x25\xd4\xbd\x53\x72\xec\x98\x8c\xbe\x6b\x75\x7b\x43\x31\x7e\x39\x96\xce\xad\xd9\x49\x2d\x56\xfb\x88\x61\xef\x58\x4b\x1d\xbb\xea\x55\x55\x00\x00\x00\x20\x2d\x57\xac\x7a\x0e\x73\xba\x07\x80\x4b\x36\xfc\x78\x0f\x67\xe4\x88\x98\xcb\x4d\x06\x4d\x95\x22\x04\xc1\x67\x07\x08\x00\x00\x00\xbc\xb0\x0a\x04\xff\x0b\x9f\xa8\x01\x07\x39\xfb\x36\x18\xc4\x8f\x48\xb7\x55\xa2\xe8\xee\x0a\xa0\x92\x17\x02\x56\x6d\x1f\xe2\xf9\x5b\xfb\x88\x61\xef\x58\x4b\x1d\xd2\xd4\x0c\xe0\x00\x00\x00\x20\x41\x58\xe1\x1c\xc3\xc0\x5c\xbc\x01\xd6\x3c\xd8\xa5\xdc\xb9\x71\xe4\xc9\x54\x60\x38\xf7\x98\x7b\x63\xa4\x1d\xf7\x05\x00\x00\x00\xa9\xa2\xba\x35\x83\x97\x38\x76\x7e\x95\x50\x9e\xec\x8f\x8b\xcb\x03\x8a\x04\x39\x8a\xf1\x03\xb6\x2c\x5a\xdf\x17\x56\xd9\xec\xe9\x89\xfb\x88\x61\xef\x58\x4b\x1d\xbc\xb2\x01\x60\x00\x00\x00\x20\xbc\x64\x35\x44\xf2\x6c\x2d\x40\xd3\x88\xd2\x84\x09\x15\x99\x0c\xe6\xff\xf9\x50\xc6\xa2\x41\x2d\x18\x5a\x58\xc3\x2d\x00\x00\x00\x58\x7c\xde\x2d\x05\xd4\x92\x66\x08\x8e\xe4\x0a\x32\x8c\x33\x70\x8e\x72\x2f\x8a\xdb\xc2\x25\xbd\xfd\x28\x75\xf5\xa4\xd1\x31\x98\x91\xfb\x88\x61\xef\x58\x4b\x1d\xdc\x01\x05\x80\x00\x00\x00\x20\x64\xd7\xd5\x01\xb5\x59\x47\xdc\xd4\x79\x63\x63\xc5\x09\xaa\xa7\x1c\x44\x19\x93\x49\x3c\xc5\x50\x70\x97\xe0\x19\x1d\x00\x00\x00\x7f\xbe\x65\xbb\xcc\xd7\x35\x31\x91\x2b\x3e\x3e\xe6\xd6\x02\x1b\x17\x2e\x0b\x9a\xa9\xe0\x7b\xec\xb3\x74\xda\x29\xf0\x27\x29\x92\xa7\xfb\x88\x61\xef\x58\x4b\x1d\xb5\xa8\x04\xc8\x00\x00\x00\x20\xd3\xdc\x01\x3a\xf0\xb2\xa5\x12\xbb\x20\xfa\xa8\xe0\xd2\x4d\x7f\x3e\x6f\xc7\xf5\xfe\x46\xc7\x48\xd4\x4f\xec\x0b\x14\x00\x00\x00\x18\x25\x1f\x92\xf8\xe2\x29\x83\x75\xa6\x4e\x87\x6c\x9f\x4c\x3a\x40\x69\x03\x8d\xa8\x0b\x0d\x98\x19\x33\x64\x37\x18\x26\x2a\x07\x01\xfc\x88\x61\xef\x58\x4b\x1d\x8f\x56\x0d\x00\x00\x00\x00\x20\x9e\xbc\x50\xfc\xc5\x2c\xb8\x1b\xac\x23\x88\x6d\x54\x6f\x97\x47\x85\x76\x47\x4e\x51\x44\x8c\xb0\x6b\x5c\xed\xeb\x41\x00\x00\x00\x12\xf1\x52\x7d\x55\x0b\xcc\xef\xd0\x2f\x7d\x79\xf2\x6d\x31\xa5\x61\x6f\xd7\x43\x98\x0a\xe2\x8b\x20\x33\xd4\x95\x14\xa1\x36\x96\x04\xfc\x88\x61\xef\x58\x4b\x1d\xbf\x75\x00\x80\x00\x00\x00\x20\x12\x59\xdd\x57\xd9\xd2\xb9\xdf\x30\x4c\x84\x49\xd1\x67\x78\xf8\x13\x46\xe8\xb8\xad\x0a\xf7\xdc\x36\x41\xb0\xd5\x18\x00\x00\x00\x86\x4b\xea\x6c\x5d\x90\xa4\xaf\xe1\xc6\xfb\x4f\xae\xc5\xe9\x93\xda\x52\xb6\x21\x8d\x57\xdf\xf1\x62\xec\x9a\xb9\x1e\x2f\xe8\x8d\x08\xfc\x88\x61\xef\x58\x4b\x1d\xe3\x02\x02\x60\x00\x00\x00\x20\xf2\x31\x3a\xbb\x8e\xcb\xf5\x58\xc7\x5e\x4e\x4e\xfb\x89\x57\x64\xac\x19\x1d\xda\x14\x45\x6f\xdf\xf1\xb9\x9a\x6f\x3e\x00\x00\x00\xd6\x87\xdd\xab\xce\x98\xea\x41\x50\x71\xd3\xcd\x61\x71\x99\xa5\x75\x14\xff\x6e\x45\xde\x8e\x91\x10\x7a\x1a\x99\x05\x98\xcc\x5a\x1f\xfc\x88\x61\xef\x58\x4b\x1d\x32\xfa\x0e\xe0\x00\x00\x00\x20\x8e\x3c\x6c\x8a\xde\x3d\xc7\xa2\xe5\x89\x2d\xca\x81\x53\xbd\xa1\xb3\x7d\x31\x19\x15\xf1\xab\xed\x2a\x83\xad\xdb\x42\x00\x00\x00\x8a\x0d\x88\x25\xda\x63\xda\x46\xaa\xd1\xb4\xd0\xbd\x81\x03\x3d\xc6\xa5\xef\xa7\xa7\x4b\x4f\x75\x58\x74\xcc\x37\x1e\xf4\x82\x45\xcc\x69\x89\x61\xef\x58\x4b\x1d\x05\x20\x05\x00\x00\x00\x00\x20\x48\xfd\xdb\xcf\xb5\xb3\x63\x0f\x12\x5e\x84\xe3\xd9\xa9\xb5\xe0\xee\xcf\xc3\x82\x2f\x74\xc9\x1e\xcf\x0b\x24\xef\x3c\x00\x00\x00\xe2\x71\xa1\x8f\x70\x05\x4d\x81\xcd\x29\xe8\xb8\xf5\x86\x89\x13\x46\x4d\xe1\x37\xe6\xa3\xa6\x62\x8e\xb8\x1f\xbd\xa9\xb6\x12\xe2\x2a\x6b\x89\x61\xef\x58\x4b\x1d\x39\x41\x01\x40\x00\x00\x00\x20\x5b\x3b\xab\x19\x4a\xcc\xf4\xc6\x6c\x59\x79\xf3\x8c\x8e\x38\xe6\xfd\x31\xa9\xcb\xbb\xf6\xbc\x56\x84\xd9\x3e\xc2\x38\x00\x00\x00\xc0\x48\x1a\xb7\x84\x62\xba\xa2\x3f\x85\xcb\xb9\x86\x38\x5a\x5c\x8d\xe3\xb8\xb6\x7a\xdc\xc1\xfa\x15\x97\x0c\x7b\x7d\x60\xd0\x31\x75\x74\x89\x61\xef\x58\x4b\x1d\x50\xb2\xac\x6a\x00\x00\x00\x20\x7f\xc0\x59\x75\x0c\x3e\x1e\xca\x75\xae\xc8\xe3\xb8\xc7\x42\x27\x64\x75\x13\x0c\x7c\x1c\x34\x45\x0b\x38\xb6\xae\x18\x00\x00\x00\x0f\x29\x30\x70\xfd\x8b\x05\x4c\x50\x9a\x06\xeb\xd6\x69\x6e\xbf\x56\xe7\xd2\x40\x35\x70\x80\xac\x4a\x92\x5a\x59\x41\x46\xf8\xdd\x81\x74\x89\x61\xef\x58\x4b\x1d\x65\x57\x00\x38\x00\x00\x00\x20\x63\x65\x6c\xea\xb7\xdf\xb4\x5e\x23\xf2\x73\x72\xbf\xc2\x31\x54\xcb\xf0\x10\x01\x64\xad\x86\xd0\x01\x57\xe3\xfd\x0b\x00\x00\x00\xdc\x7b\xbe\xc1\x22\x91\xd4\xfb\x07\x08\xa3\x15\x58\x00\x37\x3f\x27\x00\xcc\x50\x68\xeb\xac\x79\xb1\xaa\xcd\xd3\xf7\xd3\xe3\x39\xa6\x74\x89\x61\xef\x58\x4b\x1d\xe0\x54\xae\x2a\x00\x00\x00\x20\x7d\x26\x91\xea\x73\x26\x4e\xd7\xd2\x52\xca\xae\xd7\x98\x44\x6a\x6d\x5f\xbb\xe2\xa7\xb5\x0a\xd3\xc7\x00\xf9\x93\x2e\x00\x00\x00\x6e\x94\x6a\xe8\xda\x29\x58\x26\xc6\x4e\x56\xe4\xca\xb5\xae\xcb\xae\x6b\x9d\xaa\xd6\x22\x2e\xd5\x24\xcc\xfa\xb1\xbf\x33\xa4\x7d\xc7\x74\x89\x61\xef\x58\x4b\x1d\xe0\xf0\x00\x80\x00\x00\x00\x20\x4e\xd7\xd0\x42\xbe\xd4\x80\x77\x43\xa2\x07\xb8\xf3\x60\xde\xa1\x05\x22\x74\x31\xad\xb7\x56\x3c\x83\xcd\x23\xa4\x42\x00\x00\x00\xfa\x12\x9a\x30\x36\x1a\x85\x94\x95\xfc\x3c\xa2\xe9\x8b\x51\xe2\xb3\x2b\x6f\x87\x18\x5f\xcf\x6c\x9f\x71\x2c\x2a\x75\x15\xee\xb2\xcd\x74\x89\x61\xef\x58\x4b\x1d\xba\x38\x01\x88\x00\x00\x00\x20\xbe\x97\x40\x94\xb1\x57\x39\x87\xf7\x60\x83\x4b\x9d\x84\x88\xaf\xa8\x87\x21\x68\x73\xca\x62\x0e\x8a\xce\xbf\x60\x2a\x00\x00\x00\xd4\x51\x99\x38\x9a\x31\xc6\xfa\x5b\x30\xef\xd2\x32\xc3\x5c\x21\xc5\x47\xce\x5f\x12\x5b\xd8\x5e\x7e\x97\xc1\x91\xf4\x5c\x75\x92\x97\xbc\x89\x61\xef\x58\x4b\x1d\x12\x0d\x0a\x5c\x00\x00\x00\x20\x26\xfa\x8c\x87\xca\x93\xa5\xdd\xea\xdc\x7c\x1f\x71\x23\x9a\x1e\x6b\x19\x7b\xd4\xe4\x57\x6d\xd6\xc2\xef\xd3\xde\x06\x00\x00\x00\xdd\x93\xe1\xe2\xd2\xf8\x04\x54\x3f\x38\x08\xc7\x39\x74\x08\x17\x71\x3a\x0c\x6e\x31\xb1\xa0\x53\x60\xb9\x7a\xa3\x56\x88\xe3\xd1\xbe\xbd\x89\x61\xef\x58\x4b\x1d\x35\xbe\x01\x48\x00\x00\x00\x20\xf3\xb6\xeb\xcd\xd1\x9f\x8a\x60\x81\x1a\xd7\x34\x9d\x44\x20\x02\x96\xaa\xd4\xc5\x84\x75\x0a\x3b\xa0\xaf\xa0\x53\x3f\x00\x00\x00\xcf\x9c\x85\x11\x8b\xce\x54\xc7\x1a\x18\xed\x31\x6e\xc6\x07\x06\xec\x88\x9f\x4d\x25\x4f\x04\xcf\x87\x6e\xa6\xc9\xed\x91\x49\x34\x41\xbe\x89\x61\xef\x58\x4b\x1d\x25\xa1\xbd\xaa\x00\x00\x00\x20\x82\xa9\x7c\x2d\xf5\xdc\xaa\x30\x86\x0d\x20\x37\x24\x47\xd2\xfc\x5e\x09\xba\x17\xe4\xf2\x89\x8e\xca\x25\x07\x12\x39\x00\x00\x00\xeb\x11\x44\x25\xe8\x1b\x04\x81\xa3\xeb\xa8\xfd\x93\xcc\xda\x3b\x5f\x81\xda\x04\xe8\x40\x02\x84\x94\xd1\xc9\x08\xb9\xdf\xcc\xe6\xfa\xc0\x89\x61\xef\x58\x4b\x1d\xb5\x4c\x01\x80\x00\x00\x00\x20\x07\x40\x3b\xa4\x3a\x40\x05\x48\xf6\x3c\x04\xc5\xba\x13\x3e\xef\x59\xc9\x8c\xac\xe2\x75\xf4\x7e\xb2\xbe\x84\xa9\x2b\x00\x00\x00\xf6\x7e\xba\x48\xd3\xc8\xec\xc4\x52\x29\xe7\xb6\xe8\xd9\xd8\xdd\xc8\xa2\xba\xb9\x81\x83\xa6\x32\xb7\xa5\x72\x3a\x29\xdb\x23\x49\x5c\xc1\x89\x61\xef\x58\x4b\x1d\x26\x67\x02\x88\x00\x00\x00\x20\xe6\x36\xf8\xb9\x2e\x30\x2f\x10\xff\x15\xde\xb8\x14\x06\x1e\xcb\x27\xb7\x07\x50\x12\x97\x26\xba\xec\xdc\x40\x31\x06\x00\x00\x00\xb5\xdc\xd1\x00\xed\x75\x54\x39\xaf\x3e\xf6\x13\xc4\x32\x47\x86\x00\x17\x7b\x45\x7b\xa7\x65\x68\xdc\x8d\xed\x71\xa7\x89\x4b\x51\x0e\xc2\x89\x61\xef\x58\x4b\x1d\x6b\x93\x03\xc8\x00\x00\x00\x20\x43\xeb\xfd\x0b\xa0\x19\x9a\xd3\xe5\xb4\xe4\xe5\xb9\x48\x81\xb0\x7f\x6d\xb9\x97\xb1\x36\xe9\x6c\xbe\x57\x21\x29\x39\x00\x00\x00\x97\xbb\x37\xb9\x1b\x2b\xbf\xa4\xe8\x88\x54\xa5\xd5\x39\xef\xac\x1e\x89\x20\xf8\xd0\xf6\x0f\xd8\xcf\xc3\xdb\xa2\x72\xf2\x77\x3e\x11\xc3\x89\x61\xef\x58\x4b\x1d\x37\x31\x01\x20\x00\x00\x00\x20\xf6\x0b\x78\x0c\xf9\xfc\x0b\x9d\x2b\x9b\xda\x47\x84\x3a\x4e\x44\x2d\x81\x8b\xca\x95\xf3\xe6\x1c\x9e\xd3\x63\x8d\x1c\x00\x00\x00\x39\x92\xaf\xd2\xb7\x0c\xec\x8b\x4e\xf6\xf3\x5c\x3f\x23\x93\x64\x93\x8d\x4e\x63\xb9\xef\x31\xfe\x98\x04\x59\xf8\x64\x1b\x3c\x52\x5a\xc3\x89\x61\xef\x58\x4b\x1d\xae\xe0\x0b\x50\x00\x00\x00\x20\x58\x51\x78\x1b\x6d\x3e\x0e\xa7\x12\x5e\xc0\x62\x9a\x43\x67\x24\xb3\x99\xa9\x42\x53\xff\x7c\xff\x73\x05\x46\xce\x43\x00\x00\x00\xf0\xdc\x94\xab\x1e\x28\x96\x37\x22\x36\xc6\x55\x93\x99\x68\x12\xc9\x85\x5a\xe6\xe1\xbd\xc3\x37\x33\xf1\x30\x51\x2c\xde\x7f\xa9\xbb\xc4\x89\x61\xef\x58\x4b\x1d\x5f\x5f\x02\x60\x00\x00\x00\x20\x99\x16\xae\x2d\x3b\xf0\x34\x33\x77\x97\x4d\xf6\xfd\x27\xa2\x2e\xb3\x6b\xb0\x0e\x88\x7d\xab\xa1\xc0\xe3\x1d\xd1\x0d\x00\x00\x00\xf9\x5d\xb4\xe6\xa3\x11\x0a\x89\xdf\xbb\x5c\x24\xf5\x5d\x01\x50\x23\x46\xe0\x9e\x4c\x3c\x6e\x72\xc5\xca\x30\x6b\x58\x4c\x0c\xc8\x05\xc5\x89\x61\xef\x58\x4b\x1d\x07\x45\x5f\x55\x00\x00\x00\x20\xab\xdb\xa7\x53\x92\x49\x8d\xac\x96\xd9\x9e\x24\x61\x25\x0c\x9d\x55\xf0\xc3\xd0\x85\x64\x6c\xbd\x66\x36\x84\x2b\x32\x00\x00\x00\x0a\x15\x13\xe1\xec\x5e\xd0\xc8\x80\xe8\x1f\x97\x75\x69\x11\xa7\xa9\x56\x46\x07\x71\x69\xa0\x45\xc5\x00\x20\xef\x0d\xaa\x02\x21\x69\xc6\x89\x61\xef\x58\x4b\x1d\xd8\xa2\x0b\x10\x00\x00\x00\x20\x79\xe4\x50\xe0\xa4\x45\x2a\x4e\x9d\x33\x46\xc8\xd2\x69\x29\x5d\x6d\x4f\xab\xb9\x60\x91\x05\x8b\x78\x63\x53\x5f\x0a\x00\x00\x00\x62\xa4\x39\x7f\x80\xa9\xdf\x4d\x76\x76\x62\xbd\xda\x49\x66\xf5\xe9\xdf\x59\x24\xbc\x73\xcc\xb7\xe2\x63\x4f\x31\x70\x87\xe0\x7f\xc2\xc7\x89\x61\xef\x58\x4b\x1d\x60\x81\x02\x10\x00\x00\x00\x20\x69\xba\x0d\xb8\xa6\x7b\xcf\x32\xf6\x31\x92\xc7\x5d\xa6\x76\x98\x3b\x8f\x53\x39\xb8\x03\x71\x45\x0e\xa0\x83\xcb\x00\x00\x00\x00\x1e\x88\xd2\x6c\x97\xc2\xf3\xfe\xc6\x69\xe0\x70\x01\xdf\x53\x82\xd8\x73\x2c\x4f\x5c\xb6\x2e\x1e\x21\x76\x98\xa9\xd8\xfe\x99\xf5\x7d\xc8\x89\x61\xef\x58\x4b\x1d\x6a\xb4\x01\x00\x00\x00\x00\x20\x4d\xc2\x73\x80\x0b\xdd\x75\xd4\x34\xe4\x97\x1e\x4c\x5c\xa9\x38\xb2\xcd\x06\x2b\x84\x01\x8e\xa9\x17\x27\x57\x9a\x0b\x00\x00\x00\xa4\x53\x6c\x60\x02\x18\x8b\x1a\x97\xdd\x70\x7f\x41\xbd\xef\x02\xa8\x98\x5b\xa2\x7a\x12\x64\xe5\xf6\xd8\x16\xe1\x15\xc7\x2d\x5e\xe3\xc8\x89\x61\xef\x58\x4b\x1d\xe3\x95\x01\x10\x00\x00\x00\x20\x03\x91\x79\xcc\x7c\x06\x28\x7a\x38\x96\x5f\xb1\x15\x35\xf3\xee\x36\x60\x3a\x3b\x8b\x92\x71\x1a\x18\x92\xe3\x93\x02\x00\x00\x00\xaf\x89\x61\xcd\xf3\xc6\x31\xc6\x1e\xe0\x6e\x36\x61\x5e\x2c\x13\x80\xe6\xf0\x3d\x28\x77\x51\x9a\xf3\x57\xa6\xc9\xc5\x8c\x31\xbf\x1c\xc9\x89\x61\xef\x58\x4b\x1d\x74\x0a\x03\x00\x00\x00\x00\x20\xbd\xc8\xa2\x3f\x8a\xfc\x27\xca\x3b\xb6\x9a\x81\x29\xac\x2b\xbc\xce\x5b\xfc\xa9\x54\x79\x87\xd3\x8c\x7f\xaf\x73\x31\x00\x00\x00\xaf\x65\xf1\x71\xee\x83\x88\x9d\x89\x77\x4e\x8c\xb5\x79\x0f\x29\x10\x08\x97\x9a\xe7\x5a\x98\x54\x96\xdb\x38\x24\xe7\xd3\xd8\x27\xd0\xc9\x89\x61\xef\x58\x4b\x1d\xc7\x3e\x04\x40\x00\x00\x00\x20\x92\x41\xde\xd0\xde\xb5\xb4\xee\x8f\x6f\xc6\x86\xf8\xa4\x7e\xbf\xbb\x7f\x15\x92\x2a\x56\x5a\x7d\xfe\x28\xa3\xeb\x24\x00\x00\x00\xdc\xa7\xb9\x6d\x5b\x94\x42\x08\x63\x6b\xa6\x5e\x2a\x4b\x0d\xf5\xe9\xe5\xd2\xdc\xa2\x93\xbd\x56\x72\x84\xa4\x72\x3d\xbc\xf6\x89\xc8\xca\x89\x61\xef\x58\x4b\x1d\xb1\x9d\x05\x40\x00\x00\x00\x20\x5b\x11\x0c\x26\x90\xad\x9e\xd1\x11\x97\xda\x77\x0b\x06\x88\x1b\x03\xe1\x09\x1a\xff\x6b\x58\xae\x13\x2f\x21\xdf\x12\x00\x00\x00\xdb\x66\x68\xab\xd0\xd0\xf1\x22\x6d\xef\x2d\x8c\xcc\x30\x1c\xc6\x0c\x01\xf4\x3e\x16\x32\x73\x82\x73\xe9\x7c\x5c\xc6\x47\x72\x6b\x1b\xcc\x89\x61\xef\x58\x4b\x1d\x27\xb7\x04\x20\x00\x00\x00\x20\x1a\xbc\x98\x1c\x16\x5b\x0a\x49\x95\x06\x0d\xf6\x15\xad\x5b\x45\x92\x02\x02\x59\x3d\xd5\xa6\x8c\x83\x56\xe8\x7d\x18\x00\x00\x00\x37\x03\x01\x8e\xc7\x6f\xe4\xbb\xc5\x74\x2b\x03\xc4\xfe\xd5\x1d\xb5\xc4\x95\xb5\x5a\x7f\xa3\x50\x05\x90\x8e\x09\x8f\xf2\x62\x94\x83\xcc\x89\x61\xef\x58\x4b\x1d\x8e\xdb\x5a\x95\x00\x00\x00\x20\xfe\x03\x4f\xb5\x80\x58\xed\xc1\xa4\x7c\x32\x78\xce\xa5\x63\x2a\xd5\x02\x55\xbf\x50\x7b\x70\xd2\x19\xb6\x21\x1c\x20\x00\x00\x00\xa8\x45\x06\xd6\x3f\x51\x72\xa7\x1e\x2f\xf7\xfb\xb1\xb2\x1a\x5f\x37\xa2\xf0\x20\x35\x0a\x44\x27\x22\x50\x9c\xc8\xa6\xaf\x3e\xc2\x41\xcd\x89\x61\xef\x58\x4b\x1d\x73\x2b\x01\x54\x00\x00\x00\x20\x99\x90\x54\xf3\xbe\xf4\x8a\xc1\x66\xee\x2e\x8f\xad\x8a\xc3\x59\xee\xaa\x3f\x77\x77\xdc\x50\xa2\x15\xd7\x07\xaf\x2c\x00\x00\x00\x01\x87\x76\x77\x45\x8b\x99\x76\x4b\x22\xf3\x80\x4b\x5e\xd7\x71\xb7\x89\x55\x20\x66\x7f\x0b\xcc\xe1\xea\xc8\x5b\x62\xeb\x48\x93\x67\xcd\x89\x61\xef\x58\x4b\x1d\x2d\xa3\x16\x60\x00\x00\x00\x20\x93\x15\xa1\x2d\x64\xb7\x53\xeb\x86\x59\x72\x64\xe9\xab\xe0\x9f\x92\x9d\x5a\x2b\xa2\xc3\xf0\x25\x95\x08\xb8\xf7\x12\x00\x00\x00\xfa\x87\x29\x5d\x45\x94\x58\x7b\x16\x9f\x32\x3d\x90\x49\x7b\x7c\x47\x3d\x91\x29\x58\xe1\x76\xf0\xee\x89\x48\x63\x7b\x39\x91\x79\x00\xd0\x89\x61\xef\x58\x4b\x1d\x2c\x07\x04\x80\x00\x00\x00\x20\xc5\x20\xb6\xc8\x4a\xa5\x2c\x46\x74\xe7\xb4\xc3\xcb\x12\xe7\xfe\xd5\x39\xf0\x95\x0d\x88\x4e\x4c\x97\xda\x1e\x67\x16\x00\x00\x00\xf8\x59\x05\xda\x6f\x40\xef\x63\x7e\x1d\x32\xf1\x73\x38\xe2\xcb\xf6\x5a\x69\x10\x6b\x1d\x04\x0d\x44\x39\xd7\x85\xe1\xbb\xae\x5b\xf1\xd0\x89\x61\xef\x58\x4b\x1d\x79\x19\x04\x3c\x00\x00\x00\x20\x7a\x17\xcf\x03\x82\x50\x40\x6d\x87\x5f\xd9\xb6\xa9\x23\xea\xfc\xe3\x09\xea\x38\x8e\xdc\x36\x1e\x7f\x21\x4c\x6a\x30\x00\x00\x00\x1d\xca\xe4\x52\x1d\x60\xe0\x91\x66\xe9\x2b\xc3\x2e\xa4\x23\xad\x8d\x46\x44\xbd\xfa\x6c\x93\x15\xf8\x71\xed\x20\x20\xce\xda\x08\x6d\xd1\x89\x61\xef\x58\x4b\x1d\x85\xb5\xbe\x0a\x00\x00\x00\x20\xe3\x55\xca\x60\xc0\x51\x69\xef\x0a\x24\xbf\xdf\xc8\x17\x86\x78\x86\x73\x58\x21\x31\x67\x48\xf4\xfe\xe7\x4f\x16\x49\x00\x00\x00\x91\x67\x9f\x9a\x48\x35\xd1\xb6\x43\xb0\xf8\xda\xd8\xea\xca\x5a\x92\xba\x28\x8f\x13\x79\x4e\x44\x82\xe8\x3b\x74\xa1\x54\x66\xd7\x4d\xd5\x89\x61\xef\x58\x4b\x1d\xe5\x84\xac\x2a\x00\x00\x00\x20\x59\x66\xd9\xe4\xdb\xcd\x46\x2f\x3f\x8a\xda\x3b\x4c\xbb\x03\xa5\xbc\xa0\xf0\x18\xa5\x3b\xbd\x96\xbd\xc1\xc6\xf8\x37\x00\x00\x00\x2a\x5d\x01\x94\x47\xbf\xf9\x30\xd5\xcf\x0d\xc2\xf7\x4b\xfc\x2e\x20\x89\x66\x6d\x17\xc4\x78\xd3\x5c\x9a\x78\x5c\x3a\x8d\xe3\x15\x7f\xd5\x89\x61\xef\x58\x4b\x1d\x39\xbc\x02\x40\x00\x00\x00\x20\x9e\xa1\x50\x81\xea\xb6\x1a\xed\xb9\x93\xad\xc7\xb2\x89\x75\x1e\xa4\xb7\x7f\xd3\x74\x8d\xfb\xb3\x67\xf4\x9c\x99\x23\x00\x00\x00\x6e\x64\x6f\x17\xee\x2c\x93\x0e\xc6\xa3\xd5\x1c\xc0\xa5\x76\x89\xbe\x37\xbe\x58\x0a\x4e\xfb\x6b\xcf\x1c\x0f\x26\xa2\x6a\x9c\x94\x9e\xd6\x89\x61\xef\x58\x4b\x1d\xf7\xd5\x02\x78\x00\x00\x00\x20\xfe\x9f\xc9\xfd\x79\x6f\x9c\xab\xcc\x04\xdf\x76\x05\xc5\xa9\x86\x73\xd2\xf8\x06\x9b\xb7\x99\x35\x33\x51\x8b\x3f\x4a\x00\x00\x00\x28\x93\x6c\x90\x33\xa3\xab\x9c\x79\x27\x6b\xda\xb7\xbe\x15\x6a\x5c\xae\x56\xbc\xb2\x5a\x64\xae\x40\x05\x99\xb2\x5a\x15\x37\x0b\x42\xd7\x89\x61\xef\x58\x4b\x1d\xf0\x36\x0e\x0c\x00\x00\x00\x20\xa7\x13\xc2\x7f\xb5\xca\x13\x11\xa3\x37\x67\x33\xdd\xc0\xcd\xbe\xea\xbd\xe8\x5d\x5e\x32\xc4\x95\x56\xcc\xe8\x93\x27\x00\x00\x00\x4c\x6b\x2a\x20\xcb\xee\x0b\x10\x56\xc6\xce\x5c\x02\x05\x5d\x98\xbd\x59\xbe\xd7\x6a\x8f\xf3\xd0\x67\x99\xf5\x48\xbe\xd6\x93\x15\xf2\xd8\x89\x61\xef\x58\x4b\x1d\x35\xdd\x01\x0c\x00\x00\x00\x20\x31\x3a\x67\xc6\x00\x8d\xe1\xa6\xc9\xb5\x11\x00\xde\x8d\x47\x17\x17\x05\xb8\xb4\x97\xed\x42\x1e\x8b\x51\x0e\xa0\x26\x00\x00\x00\xfe\x89\x9c\xb3\x84\xb9\x07\xf2\x62\xd0\xd3\x4f\xf8\x76\x45\xd2\x6b\xa5\x3c\xb2\x8a\x34\xec\x13\x2f\xaa\x39\x68\xe9\xb2\x8e\xfe\x32\xd9\x89\x61\xef\x58\x4b\x1d\x33\x4c\xef\xee\x00\x00\x00\x20\x58\x3e\x60\x14\x48\xc2\x51\x50\xbc\x9e\xe5\x6b\xc7\xab\x1a\x7b\x9c\xeb\x15\x29\x11\xea\xde\xdd\xf5\xab\xf5\xd1\x11\x00\x00\x00\x56\xe0\x67\x16\x9e\x26\x3f\x0c\xdf\x4b\xf8\xfe\xbc\x93\xf0\x84\xcd\x46\xb8\x5b\xf7\x6a\x5e\xbf\x2c\xbc\xc1\xce\xa1\x79\x43\x60\x3c\xd9\x89\x61\xef\x58\x4b\x1d\xb4\x27\x01\x60\x00\x00\x00\x20\x87\x65\x68\x01\xe5\x1d\xe8\x5f\xdd\xd7\x06\x90\xac\xf1\x82\x96\xf0\x10\x40\x1a\x5f\xf8\x11\x7e\x4c\xd9\x69\x83\x22\x00\x00\x00\x96\x46\xb8\x4d\x1e\xb8\x1e\xa8\x47\x77\x72\x91\xf7\x94\x0c\xaa\xaf\xf8\xbb\xe3\xfe\xb9\xbd\xdc\x10\xbd\x54\xa4\xdd\x5f\xff\xa4\x84\xd9\x89\x61\xef\x58\x4b\x1d\x8f\xd6\x0a\x30\x00\x00\x00\x20\xa9\xbc\x48\x34\x88\x5a\x2e\x1a\xdf\xe4\x2c\x2f\x40\x9d\x27\xd8\x68\x2d\x14\x34\xc9\x4b\x70\xc8\x18\x44\xeb\xef\x38\x00\x00\x00\x31\x6e\x0f\x37\xaf\x74\x32\xbd\xaa\xa7\x75\xd8\x94\x60\x14\x34\xa6\x43\x36\xe9\xd3\xd8\xae\xa1\xc9\x8c\xb1\xde\x36\x16\x98\xc3\x6e\xda\x89\x61\xef\x58\x4b\x1d\x2d\xa7\x07\x68\x00\x00\x00\x20\x96\xf5\x9b\x84\x73\x36\xcb\xfa\x58\x62\x60\x66\xfe\x54\x50\x9f\xf0\x40\x05\x42\x15\x89\xb1\xfe\x98\x11\xd6\xb7\x40\x00\x00\x00\x01\x80\x3e\x32\xf0\xf6\x13\xc8\x63\x84\x86\x94\xe9\x8f\x66\xbb\xf2\x58\x8a\xb9\xf8\xd6\x59\xc4\x4e\x5c\x3c\x1c\x85\xa9\x87\x62\x12\xdb\x89\x61\xef\x58\x4b\x1d\xd3\xd8\x15\x40\x00\x00\x00\x20\x8e\x69\x25\x3f\x7b\xf2\x5b\x27\x9a\x6c\x6b\xfb\xf2\x7e\x79\x3d\x3b\x1a\x21\x38\x32\x69\x06\x54\x9a\x9b\x2d\x87\x1f\x00\x00\x00\xfd\x21\xb0\x49\x70\xd8\x60\xac\x0a\x0e\x98\x91\x3a\xec\xa1\x12\x6e\xfc\xc6\x52\xb3\xae\xab\x12\x78\xc0\x46\x88\x98\x05\xb8\x46\x28\xe0\x89\x61\xef\x58\x4b\x1d\x34\x5b\x13\x20\x00\x00\x00\x20\x2f\x5f\x05\xc0\x35\x9e\x7d\xd0\x22\xfe\x3a\x69\x28\x1d\x1a\xd6\x1f\x20\xaa\x39\xe9\xaf\xfd\xd8\x59\x4d\xd7\xe2\x14\x00\x00\x00\xcc\x6e\xc7\x88\x80\x30\x7c\x4a\xbd\x54\xf2\x79\xaf\xd1\xe7\xc1\xc7\xbd\x99\x88\x42\x3e\x48\x72\x8c\x84\x77\xca\xf5\x1e\xcb\x8f\xbd\xe1\x89\x61\xef\x58\x4b\x1d\x77\xbb\x1b\x38\x00\x00\x00\x20\xaf\x54\x11\xc6\xee\xc7\x3c\x8e\xca\x45\x96\xd4\x1b\x00\xfd\x73\x8e\x19\xc0\x2f\x46\xae\x6e\xaa\x7c\x88\x80\x10\x24\x00\x00\x00\xca\x81\x89\x60\xe9\x4e\x29\x3f\x5a\xa3\x15\xbd\x4b\x8e\x81\x41\x99\xf0\x12\x1d\xb1\x49\x27\xa4\x5f\xfc\x5b\x8e\xed\xd8\x9e\xe3\x07\xe4\x89\x61\xef\x58\x4b\x1d\x64\xdd\x00\xc0\x00\x00\x00\x20\xf0\x9f\xb8\x4d\x09\x24\xc0\xaf\xfb\x9d\x55\xc6\xef\xae\x60\xe4\x5a\x0b\xb0\xa1\xaa\xb7\xc3\x44\x82\xbe\x04\xce\x45\x00\x00\x00\x4c\x6a\x20\x25\x09\x64\x96\x2b\xdc\xfa\x42\xe6\x84\x12\x4a\xee\x7e\xe0\x77\xaf\xd3\x9c\x70\x5d\x6e\x68\x28\xdc\x1b\x6e\x48\x00\x3b\xe4\x89\x61\xef\x58\x4b\x1d\x7c\x13\x03\xe0\x00\x00\x00\x20\x44\xe3\x35\xfc\xd9\xc9\xc9\xcf\x79\x6d\xad\x3d\x3f\xcb\x59\xf6\x33\xaa\xc8\x6a\x94\x52\x11\x07\x03\x94\xd5\xe7\x17\x00\x00\x00\xc7\xab\xc7\xd8\x9f\x96\xde\xa9\x82\x09\xff\x86\xb1\x42\x9c\x99\x3d\xb6\xc0\xe7\x5e\x96\x2c\xbe\xd7\xcf\xc6\x03\x02\x67\xe7\xd6\xf3\xe4\x89\x61\xef\x58\x4b\x1d\xa7\x0c\x04\x70\x00\x00\x00\x20\xc5\xee\xab\x90\xa0\xa1\xe9\x59\xb5\x72\x45\xae\xaf\x84\xee\x32\xec\x60\x91\x40\x84\x49\xa3\xcb\xd6\xa5\xa8\x9a\x44\x00\x00\x00\x8f\x23\x1f\x5c\x03\x6f\xd9\x28\xc7\x55\xd5\x54\xfe\xe3\xc8\x5e\x34\x36\xf0\x07\x70\x74\x70\x58\x59\xd1\x2e\xe4\x92\xd1\x88\xe0\x4b\xe5\x89\x61\xef\x58\x4b\x1d\xb4\x20\x00\x60\x00\x00\x00\x20\x62\xbc\x47\x48\x81\x5d\x1a\xd0\x1a\x28\x3e\x41\x5c\x15\x8e\x1a\x37\x42\xcf\x38\x9a\x1f\xfd\x70\xb5\x04\x32\xaf\x16\x00\x00\x00\x3f\xc5\x54\x48\x37\xd9\x00\xbb\xaf\x68\x73\x7c\x53\x65\x4a\xcc\x85\x5c\x74\xe3\xc1\x3f\x25\xce\x2b\x47\xf7\x4f\x55\xf8\x04\x9a\x53\xe5\x89\x61\xef\x58\x4b\x1d\x32\x44\x08\x80\x00\x00\x00\x20\x65\xff\x38\x73\x24\xbc\xb7\x37\xec\xcd\xe3\x2f\x04\x03\x4d\x13\xd3\xe3\x15\x19\x06\x12\x31\xd7\x48\x5a\x13\xba\x2d\x00\x00\x00\xfe\xbd\x09\x71\x5f\x33\xaf\xde\x24\x59\x45\x98\xcc\xa9\xf8\xc6\x6a\xf0\x1c\x9c\xcf\x8b\x2a\xe3\x19\x82\x9d\xb3\xef\x2a\x10\xd2\x4c\xe7\x89\x61\xef\x58\x4b\x1d\xfa\x10\xbc\xbb\x00\x00\x00\x20\xfc\x48\x7e\xb6\x92\x88\x1c\xcf\x90\xa3\x06\xca\xd0\x77\x07\x52\xcb\x08\x89\x50\x9a\x18\xe8\x83\x4c\x5d\xe6\x34\x4a\x00\x00\x00\x4a\x44\xc9\xd1\xc6\x08\x12\x8f\xc4\x3b\xe2\x33\x0d\x79\x44\x52\xf1\x19\x30\xd5\xcc\xca\xc3\xca\x74\xf8\xf4\x3e\x17\xbf\x89\xbd\x56\xe7\x89\x61\xef\x58\x4b\x1d\x3b\x39\xc2\x0a\x00\x00\x00\x20\x5f\x3e\x18\x96\x1b\xb8\xb6\x9d\x65\x46\x0b\x56\x0a\xd5\x7e\xcd\xfa\x1a\xb1\x2a\x95\x94\xb8\xb2\xbb\xbd\x81\x62\x37\x00\x00\x00\x52\xd9\x97\x62\x78\xff\x77\x21\xdd\xa2\x15\x20\xd6\xf9\xcd\xa1\x9c\xf4\x12\xc8\x58\x83\xfb\x7f\xcd\x0a\x85\xbe\x9c\xe3\xb8\xb8\x97\xea\x89\x61\xef\x58\x4b\x1d\xc8\x7a\x00\x40\x00\x00\x00\x20\xf7\x6e\xa9\x7f\xc0\x6b\x69\x56\xbd\x29\x5b\x62\x53\x0f\x13\xbe\xa2\x89\x9f\x7b\xfc\x5e\x2b\xec\x19\x4a\x26\x4d\x36\x00\x00\x00\x62\x13\x9d\x27\xd1\xa4\xa8\x58\x6a\xa9\xd9\xb2\xc1\x31\x03\x51\x9f\xd6\x26\xdf\x0d\x14\xce\x42\xe5\x2b\x9f\xba\xbf\x86\x30\x5f\x61\xee\x89\x61\xef\x58\x4b\x1d\x8a\x85\x00\x00\x00\x00\x00\x20\xbe\x31\xf0\x1c\x2d\x01\xaf\x1a\x8b\x98\x7e\x66\xc5\x65\x1a\x0b\xf2\x6c\x24\x07\x9f\x29\x0e\x9e\x0e\xe9\xb2\x76\x43\x00\x00\x00\x81\x7f\x64\xcb\x93\x82\x3e\xe6\xe8\x19\x66\xbb\x11\xf6\x28\xc4\x40\x94\xc2\x6e\xb9\xe7\xad\xd0\xf4\xac\x0f\xd8\xac\x07\xac\x7e\x72\xee\x89\x61\xef\x58\x4b\x1d\x0b\x41\x04\x74\x00\x00\x00\x20\x9a\x71\x60\x54\x0b\xf4\xc0\x41\x7d\xc4\xb7\x87\xb1\xc3\x57\x54\xe2\x54\xa0\x71\xc2\x36\xc7\x2a\xae\x25\x53\xb2\x27\x00\x00\x00\x5d\xc0\x96\x67\x41\x78\xe9\xbd\xb5\x4d\x4b\xe1\x04\xc5\x24\x80\xd4\xd0\x28\xf2\x28\xff\x35\xd1\x83\x7c\x6b\x20\xea\x33\xc1\x64\xf7\xee\x89\x61\xef\x58\x4b\x1d\x5b\x1f\x03\x40\x00\x00\x00\x20\x59\xa1\x1b\x85\x53\xf2\xbe\xf0\xef\x8e\x10\x61\x79\x27\xd1\xc7\x99\xa3\x70\x79\xa0\xc3\x80\x49\xb3\x02\xd1\xfc\x37\x00\x00\x00\x83\xce\x59\x96\xdf\x1e\x86\x2e\xc2\x53\xc5\x12\xd8\x4b\xd0\x6a\xce\x96\xbb\x60\xb6\xf3\xd6\x91\x3e\x9f\xc2\x95\x4a\x1c\xac\xca\xb2\xef\x89\x61\xef\x58\x4b\x1d\x80\xe3\x0e\x78\x00\x00\x00\x20\x03\xea\xb1\x34\xc2\x5c\x85\xbd\xc9\xad\xa5\x01\xcb\xd3\xc9\x81\x15\x41\x08\xdf\xef\x7b\x5e\xcb\x0e\xba\x6c\x1f\x02\x00\x00\x00\xfe\x47\xc3\xd5\x07\x82\xb9\x5b\xa4\xdc\x62\xb4\x0c\xa3\x69\x7d\xca\x1f\x7b\x9b\x04\x9f\x39\x51\x5d\x17\xeb\x8b\x75\xf7\x10\xa4\xed\xf0\x89\x61\xef\x58\x4b\x1d\x9f\x2a\x02\x00\x00\x00\x00\x20\x7b\xb6\x95\x58\xea\x00\xab\x88\x27\x67\xdd\xfa\x64\x3e\xbf\x7c\xae\x6f\xd6\x60\xd2\x55\xf8\x0f\xff\x8b\xdc\xcf\x15\x00\x00\x00\x1f\xb0\x73\x9b\x8f\x2b\xe2\x38\x8f\xab\x92\x84\xd9\x68\xcb\x9c\xa4\xde\x22\xcf\x5b\x41\x0c\x5a\x62\xa9\x70\x46\xec\x26\xc6\xcc\x6d\xf1\x89\x61\xef\x58\x4b\x1d\xed\xeb\x04\xe0\x00\x00\x00\x20\x70\x80\xdb\x5d\xb3\x2b\xac\x07\xf5\x39\x32\x83\x79\xe2\x4f\x05\x8b\xa2\x5d\x4f\x9a\xa2\x1c\x53\x69\xbb\x2c\x63\x3e\x00\x00\x00\x27\x52\x29\x72\x18\xae\x0a\xa5\x08\x0c\xe3\xbf\x29\x7e\x2a\x3c\x7d\x12\xdf\x88\x4e\x0a\x48\xee\x05\xe0\x2a\x5a\xff\x64\x37\x6c\x94\xf2\x89\x61\xef\x58\x4b\x1d\x4b\xf4\x01\x98\x00\x00\x00\x20\x69\x11\x40\xef\x92\x11\x01\x5f\x12\xe9\xd5\x71\x86\xf4\xda\x89\x3a\x6c\xf2\x27\xa0\xdb\x9c\x94\xa6\xfc\xab\x90\x3f\x00\x00\x00\x5b\x0c\xb4\xbc\x4d\x8e\x18\x01\x51\x81\x2f\xc1\xaf\x37\x62\x19\xf3\x49\x83\x8a\xdb\x11\x94\x61\xc8\xdb\xf2\x1d\x4c\x38\xf1\x90\x24\xf3\x89\x61\xef\x58\x4b\x1d\x2f\x25\x03\xa0\x00\x00\x00\x20\xfb\x75\x8a\x11\x05\x00\x4a\xd0\x3b\x04\x67\x2c\x2d\xdb\xd0\xad\x23\x12\xb9\x70\xfb\xd1\x0f\x1b\x48\x40\x27\xce\x49\x00\x00\x00\x40\xdc\x02\x58\xc9\x5b\x4b\x61\x07\xec\x25\x77\x17\xcc\x0c\x85\x0c\x4e\x19\x84\x12\x66\x97\x43\x4b\x16\x9f\x79\x0c\x1a\x78\x37\x76\xf5\x89\x61\xef\x58\x4b\x1d\xc3\x65\xef\xee\x00\x00\x00\x20\x8c\x95\x40\xbb\x8a\x62\x30\x70\x84\x83\x83\x17\x34\xb9\xb0\x67\x67\x61\x36\x92\xc9\x3f\xe7\x19\x33\x50\x35\x23\x14\x00\x00\x00\x5b\x45\x7e\x7d\xab\x0f\xed\x3f\x5e\xdb\xe0\x6f\x5f\xee\x86\x2f\xfe\xf2\xcb\x92\xbe\x39\xcf\xf0\x3d\x26\x8c\x7b\xd5\xd8\xe2\xc1\x91\xf6\x89\x61\xef\x58\x4b\x1d\x70\x23\x11\x11\x00\x00\x00\x20\x4e\x89\x8f\x59\x2b\x4a\xa0\xff\x5b\xf2\x25\x7b\x9e\x95\x4d\x12\xc6\x00\xeb\x16\x8c\x7a\xcc\x03\xa0\x98\x5d\xfa\x0c\x00\x00\x00\xc1\xa8\xa4\x1a\xbf\x3d\x01\xec\xdf\xf6\x07\x37\xdd\x26\x42\xc6\xc0\x7a\x33\xd8\x4c\x47\x07\xf8\x05\xed\xa4\xc3\x53\xeb\x99\xd7\x95\xf6\x89\x61\xef\x58\x4b\x1d\x2f\x7a\x02\x40\x00\x00\x00\x20\xfc\xab\xa1\xed\x8f\xd1\x26\x0c\x02\x71\xb0\x41\xb9\x8c\xcc\x70\x36\xea\x85\x4c\xa5\x31\xc5\x1a\xc3\xd9\x2e\x34\x04\x00\x00\x00\xa0\x23\x96\x25\x1c\x10\x00\xa1\x8b\xf8\xdf\xff\xad\x52\x9a\x5d\x6f\x72\xe1\xf3\x1b\x77\x62\xdd\x9e\xab\x72\xa2\x1d\x5f\x1c\x0d\xee\xf6\x89\x61\xef\x58\x4b\x1d\x46\x8f\x00\x80";
	char hashes[] = "\xbd\x41\x10\x7d\x52\x18\x15\x7f\xa7\xa2\x08\xda\xe1\xfc\x59\x19\x58\x2a\x65\x47\x87\x6b\x4b\xa4\x1e\x90\x0c\x1e\x49\x00\x00\x00\x40\x14\xf9\xc5\xf5\x0d\x8c\xae\x32\xa6\x49\xa2\x14\x30\x8b\x24\x1f\x49\x3a\x56\xa5\xd4\x46\x7a\xf4\x97\x62\xd6\x1b\x00\x00\x00\xaa\xb0\xc5\x64\x46\xaf\x33\x6b\x7c\xb0\x06\xf3\x52\x48\xdd\x56\x85\x78\x76\x07\x99\xda\x08\xd3\x27\xbe\x34\x17\x44\x00\x00\x00\xff\x04\x7c\x46\x78\x18\x0d\xa8\x01\xb9\x7e\x6d\xf6\x2a\xfc\x62\xca\x0b\x5a\x2d\xbc\x6b\x49\x5e\xa8\x3d\x49\x32\x1e\x00\x00\x00\xbe\x34\xc3\xa2\x45\xb4\x62\x46\x4b\xef\x80\x6b\x9c\x71\x4b\xb3\xb5\x5b\x5e\xd2\x65\xd8\x71\x3a\x83\x87\xc3\xd6\x11\x00\x00\x00\xcb\xc4\x6a\x48\x3a\xa1\x90\x36\xb5\xa8\x9e\x56\xa1\xa7\xa0\xb9\x8f\xda\xe1\x3a\xc9\xd6\xcb\x0f\x74\xed\xd9\xde\x11\x00\x00\x00\xa9\x58\x29\x9f\x9a\x3d\x11\xd7\xca\x3c\xbd\xf7\x56\xda\xa1\xdd\x4a\x4c\x17\x4a\x6e\xf7\x2f\xe0\xc6\x0a\x1a\x0e\x2d\x00\x00\x00\x2d\x83\x8e\xa8\xd6\xa7\xe4\xb7\x6d\xe9\x8d\x62\x6c\x4a\xa7\x31\x31\xd5\xc5\x18\xe0\x1a\x64\x7f\x65\xd6\x36\x9f\x3a\x00\x00\x00\x56\x1b\xd4\x6d\x9d\xb5\x66\x23\x6b\xca\xab\x59\x0c\x1f\xa0\xad\x39\x4c\xe0\x25\xc6\x64\x62\xaa\xf5\x87\xbb\xde\x00\x00\x00\x00\x58\x2d\x27\x8e\x3a\xd6\xcc\x5e\xfa\x60\xd7\xbc\xb8\x15\x90\xe3\xdf\x1b\x34\x9b\x15\x2e\x72\xcc\x58\x36\x7f\x9f\x18\x00\x00\x00\x9b\xad\x85\xd9\x09\xe9\x16\x55\x11\xff\xa2\x90\xef\xff\x8c\xb6\xf8\x31\xac\xb5\xfa\xdf\xf8\xc1\x11\x36\x87\xf6\x25\x00\x00\x00\xf1\xf2\x75\x14\x1e\x0a\xaf\x73\xc8\x8e\xfb\x40\xe2\x85\xb6\xf3\xbf\x1e\x8d\xc5\x9f\xbb\xe6\x43\x34\x96\x35\x08\x13\x00\x00\x00\xcf\x1b\x2b\x39\xe0\x0c\xf0\xb7\xbc\xe6\xa6\xe8\xf2\x45\x4d\x0b\xb8\x7d\x43\x73\x9d\x8e\x32\x69\x22\x16\xef\x9a\x3b\x00\x00\x00\xaa\x32\x89\x40\xa5\x2a\x9b\xeb\xca\xd5\x9b\x52\x15\x36\x3c\x5f\x1b\xeb\x3e\xc7\x7d\x66\x01\xac\x61\xd8\x32\x09\x11\x00\x00\x00\x99\x39\x0f\x6f\x92\x4d\x48\x9e\x92\x47\x67\x3e\x4a\x0b\xa8\x02\x8e\x9f\xe6\x01\xeb\x39\x25\xfe\xed\xab\x2b\xc9\x33\x00\x00\x00\x7b\xc4\x2a\xf7\x4e\x9e\xdd\xa6\x6a\xc8\xe2\xcb\x54\x21\x5a\x2f\xab\x78\x7c\xd8\x90\x1a\xa5\xb6\xfe\x42\x6c\xa3\x17\x00\x00\x00\xe5\x6f\x03\x03\xf2\x14\x65\xdc\xde\x89\xdd\x0f\xdc\xa6\xe6\x49\x71\x44\x7f\x48\xbc\x20\xec\x88\x63\x9b\x71\xb4\x40\x00\x00\x00\x21\x17\x89\x3f\x30\x16\x6d\x71\x4c\x6e\x9f\x44\xeb\x31\xff\x72\x69\xce\x25\xa1\x77\xe8\x60\x28\x24\xaf\x96\x57\x2d\x00\x00\x00\x60\x65\x05\xfa\x2a\x49\xc7\xdf\x9b\xf8\xab\xa4\xcb\x90\xc4\x07\xcd\x77\x56\x98\x86\x09\x00\x0b\xd6\x48\x16\x99\x36\x00\x00\x00\x08\x62\xf3\xa3\xc9\xb2\xfb\x14\xa4\xd2\xc7\x8c\x07\xac\x02\xda\xd7\xca\xc6\xa3\x4f\xad\x61\x78\xf3\x39\xd1\x7c\x28\x00\x00\x00\x4d\xe3\x30\x30\xf0\xb2\xbf\x89\xfc\x63\x0a\xbe\xa8\x6b\xd5\xa5\x01\x83\x9c\xa5\xd1\x39\xac\x40\x1d\xc3\x13\x5f\x17\x00\x00\x00\x13\xc7\xbc\x9d\x96\x6c\x4d\x8f\x1b\xc5\x0f\x2a\xa3\x2d\xa0\x53\x74\x17\x55\xc9\x47\x19\x15\xd8\xbc\x70\x3a\x47\x20\x00\x00\x00\x6a\xfd\x2a\xf6\xb5\x2f\xb9\x8b\x4e\xdb\xa4\x5f\x10\xc8\xa4\xf0\x87\xbe\x5f\x9d\xec\xb2\xfb\xd1\x5f\xe5\x93\x2b\x15\x00\x00\x00\xd7\x7b\x56\x4f\x7d\x9e\x3c\x78\x11\xa6\x8b\x37\xae\xe5\xb1\x7f\x15\xd6\x44\x37\xbf\x6b\x25\x43\x0c\xd8\x7c\xc6\x07\x00\x00\x00\xf4\xb2\xb7\x82\x84\xd2\xc8\x63\x85\xfe\x4b\xcf\x5c\xb8\x04\x7a\xd1\xe9\xa8\x22\xf7\x4e\xbd\x83\x19\x7e\x38\xf8\x30\x00\x00\x00\xaa\x27\x5c\x10\x1c\xdd\x03\xce\x75\x79\xd8\xf0\x7f\x93\x8b\x74\x64\x70\x46\xc4\xef\x5d\xfd\x95\x8f\xf5\x82\x39\x26\x00\x00\x00\x8c\x8e\x43\x2c\xab\x2b\xf2\x14\xcd\x92\x8c\x00\xa8\xc4\xde\xf5\xf0\x6b\xad\x07\x53\xc6\xe9\x2f\x9d\xbe\x6d\x42\x0b\x00\x00\x00\xd7\xbc\x38\x75\xd9\x09\xb4\xb5\xcc\x18\x0b\x2a\x47\x16\x17\x65\x0b\x3b\x93\xf0\x09\x42\x34\xe5\xd2\x98\xa7\x0e\x2a\x00\x00\x00\x2d\x57\xac\x7a\x0e\x73\xba\x07\x80\x4b\x36\xfc\x78\x0f\x67\xe4\x88\x98\xcb\x4d\x06\x4d\x95\x22\x04\xc1\x67\x07\x08\x00\x00\x00\x41\x58\xe1\x1c\xc3\xc0\x5c\xbc\x01\xd6\x3c\xd8\xa5\xdc\xb9\x71\xe4\xc9\x54\x60\x38\xf7\x98\x7b\x63\xa4\x1d\xf7\x05\x00\x00\x00\xbc\x64\x35\x44\xf2\x6c\x2d\x40\xd3\x88\xd2\x84\x09\x15\x99\x0c\xe6\xff\xf9\x50\xc6\xa2\x41\x2d\x18\x5a\x58\xc3\x2d\x00\x00\x00\x64\xd7\xd5\x01\xb5\x59\x47\xdc\xd4\x79\x63\x63\xc5\x09\xaa\xa7\x1c\x44\x19\x93\x49\x3c\xc5\x50\x70\x97\xe0\x19\x1d\x00\x00\x00\xd3\xdc\x01\x3a\xf0\xb2\xa5\x12\xbb\x20\xfa\xa8\xe0\xd2\x4d\x7f\x3e\x6f\xc7\xf5\xfe\x46\xc7\x48\xd4\x4f\xec\x0b\x14\x00\x00\x00\x9e\xbc\x50\xfc\xc5\x2c\xb8\x1b\xac\x23\x88\x6d\x54\x6f\x97\x47\x85\x76\x47\x4e\x51\x44\x8c\xb0\x6b\x5c\xed\xeb\x41\x00\x00\x00\x12\x59\xdd\x57\xd9\xd2\xb9\xdf\x30\x4c\x84\x49\xd1\x67\x78\xf8\x13\x46\xe8\xb8\xad\x0a\xf7\xdc\x36\x41\xb0\xd5\x18\x00\x00\x00\xf2\x31\x3a\xbb\x8e\xcb\xf5\x58\xc7\x5e\x4e\x4e\xfb\x89\x57\x64\xac\x19\x1d\xda\x14\x45\x6f\xdf\xf1\xb9\x9a\x6f\x3e\x00\x00\x00\x8e\x3c\x6c\x8a\xde\x3d\xc7\xa2\xe5\x89\x2d\xca\x81\x53\xbd\xa1\xb3\x7d\x31\x19\x15\xf1\xab\xed\x2a\x83\xad\xdb\x42\x00\x00\x00\x48\xfd\xdb\xcf\xb5\xb3\x63\x0f\x12\x5e\x84\xe3\xd9\xa9\xb5\xe0\xee\xcf\xc3\x82\x2f\x74\xc9\x1e\xcf\x0b\x24\xef\x3c\x00\x00\x00\x5b\x3b\xab\x19\x4a\xcc\xf4\xc6\x6c\x59\x79\xf3\x8c\x8e\x38\xe6\xfd\x31\xa9\xcb\xbb\xf6\xbc\x56\x84\xd9\x3e\xc2\x38\x00\x00\x00\x7f\xc0\x59\x75\x0c\x3e\x1e\xca\x75\xae\xc8\xe3\xb8\xc7\x42\x27\x64\x75\x13\x0c\x7c\x1c\x34\x45\x0b\x38\xb6\xae\x18\x00\x00\x00\x63\x65\x6c\xea\xb7\xdf\xb4\x5e\x23\xf2\x73\x72\xbf\xc2\x31\x54\xcb\xf0\x10\x01\x64\xad\x86\xd0\x01\x57\xe3\xfd\x0b\x00\x00\x00\x7d\x26\x91\xea\x73\x26\x4e\xd7\xd2\x52\xca\xae\xd7\x98\x44\x6a\x6d\x5f\xbb\xe2\xa7\xb5\x0a\xd3\xc7\x00\xf9\x93\x2e\x00\x00\x00\x4e\xd7\xd0\x42\xbe\xd4\x80\x77\x43\xa2\x07\xb8\xf3\x60\xde\xa1\x05\x22\x74\x31\xad\xb7\x56\x3c\x83\xcd\x23\xa4\x42\x00\x00\x00\xbe\x97\x40\x94\xb1\x57\x39\x87\xf7\x60\x83\x4b\x9d\x84\x88\xaf\xa8\x87\x21\x68\x73\xca\x62\x0e\x8a\xce\xbf\x60\x2a\x00\x00\x00\x26\xfa\x8c\x87\xca\x93\xa5\xdd\xea\xdc\x7c\x1f\x71\x23\x9a\x1e\x6b\x19\x7b\xd4\xe4\x57\x6d\xd6\xc2\xef\xd3\xde\x06\x00\x00\x00\xf3\xb6\xeb\xcd\xd1\x9f\x8a\x60\x81\x1a\xd7\x34\x9d\x44\x20\x02\x96\xaa\xd4\xc5\x84\x75\x0a\x3b\xa0\xaf\xa0\x53\x3f\x00\x00\x00\x82\xa9\x7c\x2d\xf5\xdc\xaa\x30\x86\x0d\x20\x37\x24\x47\xd2\xfc\x5e\x09\xba\x17\xe4\xf2\x89\x8e\xca\x25\x07\x12\x39\x00\x00\x00\x07\x40\x3b\xa4\x3a\x40\x05\x48\xf6\x3c\x04\xc5\xba\x13\x3e\xef\x59\xc9\x8c\xac\xe2\x75\xf4\x7e\xb2\xbe\x84\xa9\x2b\x00\x00\x00\xe6\x36\xf8\xb9\x2e\x30\x2f\x10\xff\x15\xde\xb8\x14\x06\x1e\xcb\x27\xb7\x07\x50\x12\x97\x26\xba\xec\xdc\x40\x31\x06\x00\x00\x00\x43\xeb\xfd\x0b\xa0\x19\x9a\xd3\xe5\xb4\xe4\xe5\xb9\x48\x81\xb0\x7f\x6d\xb9\x97\xb1\x36\xe9\x6c\xbe\x57\x21\x29\x39\x00\x00\x00\xf6\x0b\x78\x0c\xf9\xfc\x0b\x9d\x2b\x9b\xda\x47\x84\x3a\x4e\x44\x2d\x81\x8b\xca\x95\xf3\xe6\x1c\x9e\xd3\x63\x8d\x1c\x00\x00\x00\x58\x51\x78\x1b\x6d\x3e\x0e\xa7\x12\x5e\xc0\x62\x9a\x43\x67\x24\xb3\x99\xa9\x42\x53\xff\x7c\xff\x73\x05\x46\xce\x43\x00\x00\x00\x99\x16\xae\x2d\x3b\xf0\x34\x33\x77\x97\x4d\xf6\xfd\x27\xa2\x2e\xb3\x6b\xb0\x0e\x88\x7d\xab\xa1\xc0\xe3\x1d\xd1\x0d\x00\x00\x00\xab\xdb\xa7\x53\x92\x49\x8d\xac\x96\xd9\x9e\x24\x61\x25\x0c\x9d\x55\xf0\xc3\xd0\x85\x64\x6c\xbd\x66\x36\x84\x2b\x32\x00\x00\x00\x79\xe4\x50\xe0\xa4\x45\x2a\x4e\x9d\x33\x46\xc8\xd2\x69\x29\x5d\x6d\x4f\xab\xb9\x60\x91\x05\x8b\x78\x63\x53\x5f\x0a\x00\x00\x00\x69\xba\x0d\xb8\xa6\x7b\xcf\x32\xf6\x31\x92\xc7\x5d\xa6\x76\x98\x3b\x8f\x53\x39\xb8\x03\x71\x45\x0e\xa0\x83\xcb\x00\x00\x00\x00\x4d\xc2\x73\x80\x0b\xdd\x75\xd4\x34\xe4\x97\x1e\x4c\x5c\xa9\x38\xb2\xcd\x06\x2b\x84\x01\x8e\xa9\x17\x27\x57\x9a\x0b\x00\x00\x00\x03\x91\x79\xcc\x7c\x06\x28\x7a\x38\x96\x5f\xb1\x15\x35\xf3\xee\x36\x60\x3a\x3b\x8b\x92\x71\x1a\x18\x92\xe3\x93\x02\x00\x00\x00\xbd\xc8\xa2\x3f\x8a\xfc\x27\xca\x3b\xb6\x9a\x81\x29\xac\x2b\xbc\xce\x5b\xfc\xa9\x54\x79\x87\xd3\x8c\x7f\xaf\x73\x31\x00\x00\x00\x92\x41\xde\xd0\xde\xb5\xb4\xee\x8f\x6f\xc6\x86\xf8\xa4\x7e\xbf\xbb\x7f\x15\x92\x2a\x56\x5a\x7d\xfe\x28\xa3\xeb\x24\x00\x00\x00\x5b\x11\x0c\x26\x90\xad\x9e\xd1\x11\x97\xda\x77\x0b\x06\x88\x1b\x03\xe1\x09\x1a\xff\x6b\x58\xae\x13\x2f\x21\xdf\x12\x00\x00\x00\x1a\xbc\x98\x1c\x16\x5b\x0a\x49\x95\x06\x0d\xf6\x15\xad\x5b\x45\x92\x02\x02\x59\x3d\xd5\xa6\x8c\x83\x56\xe8\x7d\x18\x00\x00\x00\xfe\x03\x4f\xb5\x80\x58\xed\xc1\xa4\x7c\x32\x78\xce\xa5\x63\x2a\xd5\x02\x55\xbf\x50\x7b\x70\xd2\x19\xb6\x21\x1c\x20\x00\x00\x00\x99\x90\x54\xf3\xbe\xf4\x8a\xc1\x66\xee\x2e\x8f\xad\x8a\xc3\x59\xee\xaa\x3f\x77\x77\xdc\x50\xa2\x15\xd7\x07\xaf\x2c\x00\x00\x00\x93\x15\xa1\x2d\x64\xb7\x53\xeb\x86\x59\x72\x64\xe9\xab\xe0\x9f\x92\x9d\x5a\x2b\xa2\xc3\xf0\x25\x95\x08\xb8\xf7\x12\x00\x00\x00\xc5\x20\xb6\xc8\x4a\xa5\x2c\x46\x74\xe7\xb4\xc3\xcb\x12\xe7\xfe\xd5\x39\xf0\x95\x0d\x88\x4e\x4c\x97\xda\x1e\x67\x16\x00\x00\x00\x7a\x17\xcf\x03\x82\x50\x40\x6d\x87\x5f\xd9\xb6\xa9\x23\xea\xfc\xe3\x09\xea\x38\x8e\xdc\x36\x1e\x7f\x21\x4c\x6a\x30\x00\x00\x00\xe3\x55\xca\x60\xc0\x51\x69\xef\x0a\x24\xbf\xdf\xc8\x17\x86\x78\x86\x73\x58\x21\x31\x67\x48\xf4\xfe\xe7\x4f\x16\x49\x00\x00\x00\x59\x66\xd9\xe4\xdb\xcd\x46\x2f\x3f\x8a\xda\x3b\x4c\xbb\x03\xa5\xbc\xa0\xf0\x18\xa5\x3b\xbd\x96\xbd\xc1\xc6\xf8\x37\x00\x00\x00\x9e\xa1\x50\x81\xea\xb6\x1a\xed\xb9\x93\xad\xc7\xb2\x89\x75\x1e\xa4\xb7\x7f\xd3\x74\x8d\xfb\xb3\x67\xf4\x9c\x99\x23\x00\x00\x00\xfe\x9f\xc9\xfd\x79\x6f\x9c\xab\xcc\x04\xdf\x76\x05\xc5\xa9\x86\x73\xd2\xf8\x06\x9b\xb7\x99\x35\x33\x51\x8b\x3f\x4a\x00\x00\x00\xa7\x13\xc2\x7f\xb5\xca\x13\x11\xa3\x37\x67\x33\xdd\xc0\xcd\xbe\xea\xbd\xe8\x5d\x5e\x32\xc4\x95\x56\xcc\xe8\x93\x27\x00\x00\x00\x31\x3a\x67\xc6\x00\x8d\xe1\xa6\xc9\xb5\x11\x00\xde\x8d\x47\x17\x17\x05\xb8\xb4\x97\xed\x42\x1e\x8b\x51\x0e\xa0\x26\x00\x00\x00\x58\x3e\x60\x14\x48\xc2\x51\x50\xbc\x9e\xe5\x6b\xc7\xab\x1a\x7b\x9c\xeb\x15\x29\x11\xea\xde\xdd\xf5\xab\xf5\xd1\x11\x00\x00\x00\x87\x65\x68\x01\xe5\x1d\xe8\x5f\xdd\xd7\x06\x90\xac\xf1\x82\x96\xf0\x10\x40\x1a\x5f\xf8\x11\x7e\x4c\xd9\x69\x83\x22\x00\x00\x00\xa9\xbc\x48\x34\x88\x5a\x2e\x1a\xdf\xe4\x2c\x2f\x40\x9d\x27\xd8\x68\x2d\x14\x34\xc9\x4b\x70\xc8\x18\x44\xeb\xef\x38\x00\x00\x00\x96\xf5\x9b\x84\x73\x36\xcb\xfa\x58\x62\x60\x66\xfe\x54\x50\x9f\xf0\x40\x05\x42\x15\x89\xb1\xfe\x98\x11\xd6\xb7\x40\x00\x00\x00\x8e\x69\x25\x3f\x7b\xf2\x5b\x27\x9a\x6c\x6b\xfb\xf2\x7e\x79\x3d\x3b\x1a\x21\x38\x32\x69\x06\x54\x9a\x9b\x2d\x87\x1f\x00\x00\x00\x2f\x5f\x05\xc0\x35\x9e\x7d\xd0\x22\xfe\x3a\x69\x28\x1d\x1a\xd6\x1f\x20\xaa\x39\xe9\xaf\xfd\xd8\x59\x4d\xd7\xe2\x14\x00\x00\x00\xaf\x54\x11\xc6\xee\xc7\x3c\x8e\xca\x45\x96\xd4\x1b\x00\xfd\x73\x8e\x19\xc0\x2f\x46\xae\x6e\xaa\x7c\x88\x80\x10\x24\x00\x00\x00\xf0\x9f\xb8\x4d\x09\x24\xc0\xaf\xfb\x9d\x55\xc6\xef\xae\x60\xe4\x5a\x0b\xb0\xa1\xaa\xb7\xc3\x44\x82\xbe\x04\xce\x45\x00\x00\x00\x44\xe3\x35\xfc\xd9\xc9\xc9\xcf\x79\x6d\xad\x3d\x3f\xcb\x59\xf6\x33\xaa\xc8\x6a\x94\x52\x11\x07\x03\x94\xd5\xe7\x17\x00\x00\x00\xc5\xee\xab\x90\xa0\xa1\xe9\x59\xb5\x72\x45\xae\xaf\x84\xee\x32\xec\x60\x91\x40\x84\x49\xa3\xcb\xd6\xa5\xa8\x9a\x44\x00\x00\x00\x62\xbc\x47\x48\x81\x5d\x1a\xd0\x1a\x28\x3e\x41\x5c\x15\x8e\x1a\x37\x42\xcf\x38\x9a\x1f\xfd\x70\xb5\x04\x32\xaf\x16\x00\x00\x00\x65\xff\x38\x73\x24\xbc\xb7\x37\xec\xcd\xe3\x2f\x04\x03\x4d\x13\xd3\xe3\x15\x19\x06\x12\x31\xd7\x48\x5a\x13\xba\x2d\x00\x00\x00\xfc\x48\x7e\xb6\x92\x88\x1c\xcf\x90\xa3\x06\xca\xd0\x77\x07\x52\xcb\x08\x89\x50\x9a\x18\xe8\x83\x4c\x5d\xe6\x34\x4a\x00\x00\x00\x5f\x3e\x18\x96\x1b\xb8\xb6\x9d\x65\x46\x0b\x56\x0a\xd5\x7e\xcd\xfa\x1a\xb1\x2a\x95\x94\xb8\xb2\xbb\xbd\x81\x62\x37\x00\x00\x00\xf7\x6e\xa9\x7f\xc0\x6b\x69\x56\xbd\x29\x5b\x62\x53\x0f\x13\xbe\xa2\x89\x9f\x7b\xfc\x5e\x2b\xec\x19\x4a\x26\x4d\x36\x00\x00\x00\xbe\x31\xf0\x1c\x2d\x01\xaf\x1a\x8b\x98\x7e\x66\xc5\x65\x1a\x0b\xf2\x6c\x24\x07\x9f\x29\x0e\x9e\x0e\xe9\xb2\x76\x43\x00\x00\x00\x9a\x71\x60\x54\x0b\xf4\xc0\x41\x7d\xc4\xb7\x87\xb1\xc3\x57\x54\xe2\x54\xa0\x71\xc2\x36\xc7\x2a\xae\x25\x53\xb2\x27\x00\x00\x00\x59\xa1\x1b\x85\x53\xf2\xbe\xf0\xef\x8e\x10\x61\x79\x27\xd1\xc7\x99\xa3\x70\x79\xa0\xc3\x80\x49\xb3\x02\xd1\xfc\x37\x00\x00\x00\x03\xea\xb1\x34\xc2\x5c\x85\xbd\xc9\xad\xa5\x01\xcb\xd3\xc9\x81\x15\x41\x08\xdf\xef\x7b\x5e\xcb\x0e\xba\x6c\x1f\x02\x00\x00\x00\x7b\xb6\x95\x58\xea\x00\xab\x88\x27\x67\xdd\xfa\x64\x3e\xbf\x7c\xae\x6f\xd6\x60\xd2\x55\xf8\x0f\xff\x8b\xdc\xcf\x15\x00\x00\x00\x70\x80\xdb\x5d\xb3\x2b\xac\x07\xf5\x39\x32\x83\x79\xe2\x4f\x05\x8b\xa2\x5d\x4f\x9a\xa2\x1c\x53\x69\xbb\x2c\x63\x3e\x00\x00\x00\x69\x11\x40\xef\x92\x11\x01\x5f\x12\xe9\xd5\x71\x86\xf4\xda\x89\x3a\x6c\xf2\x27\xa0\xdb\x9c\x94\xa6\xfc\xab\x90\x3f\x00\x00\x00\xfb\x75\x8a\x11\x05\x00\x4a\xd0\x3b\x04\x67\x2c\x2d\xdb\xd0\xad\x23\x12\xb9\x70\xfb\xd1\x0f\x1b\x48\x40\x27\xce\x49\x00\x00\x00\x8c\x95\x40\xbb\x8a\x62\x30\x70\x84\x83\x83\x17\x34\xb9\xb0\x67\x67\x61\x36\x92\xc9\x3f\xe7\x19\x33\x50\x35\x23\x14\x00\x00\x00\x4e\x89\x8f\x59\x2b\x4a\xa0\xff\x5b\xf2\x25\x7b\x9e\x95\x4d\x12\xc6\x00\xeb\x16\x8c\x7a\xcc\x03\xa0\x98\x5d\xfa\x0c\x00\x00\x00\xfc\xab\xa1\xed\x8f\xd1\x26\x0c\x02\x71\xb0\x41\xb9\x8c\xcc\x70\x36\xea\x85\x4c\xa5\x31\xc5\x1a\xc3\xd9\x2e\x34\x04\x00\x00\x00\x87\x5d\xbe\xe1\xb4\x2c\x57\xc1\x55\xe3\x79\x2c\x08\x3e\x84\x81\xfe\x75\x8d\xf5\xbe\xb3\x16\x5b\x6d\xc2\x96\x34\x2c\x00\x00\x00";	
	
	
	int i;
	char tmp_test_arr[32];
	
	for(i=0;i<100;i++){		
		
		
		ocv2_hash(&block_headers[i*80],tmp_test_arr);	
	
		if(	memcmp ( &hashes[i*32] ,tmp_test_arr, 32 ) != 0 )
			return 0;			
		
		
		
	}	

		
		
		
	
	
	return 1;
	
	
	
	
	
	
}
