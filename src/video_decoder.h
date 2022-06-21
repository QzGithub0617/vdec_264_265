#ifndef VIDEO_DECODER_H
#define VIDEO_DECODER_H

#include "types.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>
#include <libswscale/swscale.h>
}

typedef void(*VideoCallback)(unsigned char* data_y, unsigned char* data_u, unsigned char* data_v, int line1, int line2, int line3, int width, int height, long pts);

class VideoDecoder {
public:
    VideoDecoder(DecoderType codecType_) :  codecType(codecType_) {}
    ~VideoDecoder() ;
    ErrorCode init();
    void setCallBack(VideoCallback callback);
    ErrorCode decodeData(unsigned char* data, size_t data_size) ;

private:
    ErrorCode decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, AVFrame* outFrame);
    ErrorCode copyFrameData(AVFrame* src, AVFrame* dst) ;
    ErrorCode flushDecoder() ;
    ErrorCode closeDecoder();

private:
    const AVCodec* codec;
    AVCodecParserContext* parser;
    AVCodecContext* context = NULL;
    AVPacket* pkt;
    AVFrame* frame;
    AVFrame* outFrame;
    VideoCallback videoCallback = NULL;
    DecoderType codecType;

};






#endif