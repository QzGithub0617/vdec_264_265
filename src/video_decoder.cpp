#include "video_decoder.h"

VideoDecoder::~VideoDecoder() {		
    flushDecoder();
	closeDecoder();
}

ErrorCode VideoDecoder::init() 
{
  	avcodec_register_all();
	/* find the video decoder */
	if (codecType == VIDEO_CODE_TYPE_H264) {
		codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	} else if (codecType == VIDEO_CODE_TYPE_H265)  {
		codec = avcodec_find_decoder(AV_CODEC_ID_H265);
	}

	if (!codec) {
		LOG_ERROR("Codec not found\n");
		return  VDEC_ERROR;
	}

	parser = av_parser_init(codec->id);
	if (!parser) {
		LOG_ERROR("parser not found\n");
		return VDEC_ERROR;
	}

	context = avcodec_alloc_context3(codec);
	if (!context) {
		LOG_ERROR("Could not allocate video codec context\n");
		return VDEC_ERROR;
	}

	if (avcodec_open2(context, codec, NULL) < 0) {
		LOG_ERROR("Could not open codec\n");
		return VDEC_ERROR;
	}

	frame = av_frame_alloc();
	if (!frame) {
		LOG_ERROR("Could not allocate video frame\n");
		return VDEC_ERROR;
	}

	outFrame = av_frame_alloc();
	if (!outFrame) {
		LOG_ERROR("Could not allocate video frame\n");
		return VDEC_ERROR;
	}

	pkt = av_packet_alloc();
	if (!pkt) {
		LOG_ERROR("Could not allocate video packet\n");
		return VDEC_ERROR;
	}	
	return VDEC_SUCCESS;
}

ErrorCode VideoDecoder::decodeData(unsigned char* data, size_t data_size) 
{
	ErrorCode ret = VDEC_SUCCESS;
	while (data_size > 0) {
		int size = av_parser_parse2(parser, context, &pkt->data, &pkt->size,
			data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
		if (size < 0) {
			LOG_ERROR("Error while parsing\n");
			ret = VDEC_ERROR;
			break;
		}
		data += size;
		data_size -= size;

		if (pkt->size) {
			ret = decode(context, frame, pkt, outFrame);
			if (ret != VDEC_SUCCESS) {
                break;
            }
		}
	}
	return ret;
}

ErrorCode VideoDecoder::decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, AVFrame* outFrame)
{
	ErrorCode res = VDEC_SUCCESS;
	char buf[1024];
	int ret;

	ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0) {
		LOG_ERROR("Error sending a packet for decoding\n");
		res = VDEC_ERROR;
	}
	else {
		while (ret >= 0) {
			ret = avcodec_receive_frame(dec_ctx, frame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {	
				break;
			}
			else if (ret < 0) {
				LOG_ERROR("Error during decoding\n");
				res = VDEC_ERROR;
				break;
			}
			LOG_INFO("Receive data");
			res = copyFrameData(frame, outFrame);
			if (res != VDEC_SUCCESS) {
				break;
			}
            if (videoCallback) {
                videoCallback(outFrame->data[0],
                                            outFrame->data[1],
                                            outFrame->data[2], 
                                            outFrame->linesize[0], 
                                            outFrame->linesize[1], 
                                            outFrame->linesize[2], 
                                            outFrame->width, 
                                            outFrame->height, 
                                            outFrame->pts);
            } else {
				LOG_ERROR("The callback function is not set\n");
            }

		}
	}
	return res;
}


ErrorCode VideoDecoder::copyFrameData(AVFrame* src, AVFrame* dst) 
{
	ErrorCode ret = VDEC_SUCCESS;
	memcpy(dst->data, src->data, sizeof(src->data));
	dst->linesize[0] = src->linesize[0];
	dst->linesize[1] = src->linesize[1];
	dst->linesize[2] = src->linesize[2];
	dst->width = src->width;
	dst->height = src->height;
	return ret;
}

void VideoDecoder::setCallBack( VideoCallback callback)
{
    videoCallback = callback;
}


ErrorCode VideoDecoder::flushDecoder() {
	/* flush the decoder */
	return decode(context, frame, NULL, outFrame);
}

ErrorCode VideoDecoder::closeDecoder() {
	ErrorCode ret = VDEC_SUCCESS;


	if (parser != NULL) {
		av_parser_close(parser);
		LOG_INFO("Video codec context closed.");
	}
	if (context != NULL) {
		avcodec_free_context(&context);
		LOG_INFO("Video codec context closed.");
	}
	if (frame != NULL) {
		av_frame_free(&frame);
	}
	if (pkt != NULL) {
		av_packet_free(&pkt);
	}
	if (outFrame != NULL) {
		av_frame_free(&outFrame);
	}
	LOG_INFO("All buffer released.");


	return VDEC_SUCCESS;
}