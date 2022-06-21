#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "video_decoder.h"

#define INBUF_SIZE 4096

int callbackIndex = 0;

void vcb_frame(unsigned char* data_y, unsigned char* data_u, unsigned char* data_v, int line1, int line2, int line3, int width, int height, long pts) {
	printf("[\033[0m\033[1;32m vdec callback \033[0m]  frame: [%d], size = %d * %d\n", ++callbackIndex, width, height);
	int i = 0;
	unsigned char* src = NULL;
	static int cnt = 0;
	char fileName[32];
	sprintf(fileName, "frame%02d.yuv", cnt++);
	FILE* dst = fopen(fileName, "a");
	for (i = 0; i < height; i++) {
		src = data_y + i * line1;
		fwrite(src, width, 1, dst);
	}

	for (i = 0; i < height / 2; i++) {
		src = data_u + i * line2;
		fwrite(src, width/2, 1, dst);
	}

	for (i = 0; i < height / 2; i++) {
		src = data_v + i * line3;
		fwrite(src, width/2, 1, dst);
	}

	fclose(dst);
}

int main(int argc, char** argv)
{
	VideoDecoder vdec(VIDEO_CODE_TYPE_H265);
	// const char* filename = "482*720_test.264";
	const char* filename = "FourPeople_1280x720_60_1M.265";
	vdec.init();	
	vdec.setCallBack(vcb_frame);
	FILE* f;
	size_t   data_size;
	unsigned char* buffer;
	buffer = (unsigned char*)malloc(sizeof(unsigned char) * (INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE));
	if (buffer == NULL) {
		LOG_ERROR("Memory error");
		exit(2);
	}

	f = fopen(filename, "rb");
	if (!f) {
		printf("Could not open %s\n", filename);
		exit(1);
	}

	while (!feof(f)) {
		data_size = fread(buffer, 1, INBUF_SIZE, f);

		if (!data_size)
			break;
		vdec.decodeData(buffer, data_size);
	}
	fclose(f);
	free(buffer);

	return 0;
}