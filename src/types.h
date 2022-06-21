#ifndef TYPES_H
#define TYPES_H

#define LOG_ERROR(s) printf("\033[0m\033[1;31m%s\033[0m", (s))
#define LOG_INFO(s)  printf("\033[0m\033[1;32m%s\033[0m\n", (s))

enum ErrorCode {
	VDEC_SUCCESS = 0,
	VDEC_ERROR
};


typedef enum DecoderType {
	VIDEO_CODE_TYPE_H264,
	VIDEO_CODE_TYPE_H265,
}DecoderType;


#endif