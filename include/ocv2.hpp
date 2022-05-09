#ifndef OCV2_H
#define OCV2_H

#ifdef __cplusplus
extern "C" {
#endif
 
void ocv2_hash(char* block_header,char* output);
void ocv2_init_image(char* block_header,char* NEW_IMAGE_REFERANCE_BYTES,char* final_init_img,char* last_nonce_bytes);
void ocv2_calculate_hash(char* block_header,char* NEW_IMAGE_REFERANCE_BYTES,char* final_init_img,char* last_nonce_bytes,char* output);
int ocv2_test_algo();
 
#ifdef __cplusplus
}
#endif


#endif






