/* 
 * File:   Video.cc
 * Author: Iván János István
 * 
 * Created on 2010. augusztus 31., 14:23
 */

#ifdef USE_VIDEO

#include <openipIO/videoIO.h>

namespace openip
{
	Video::Video(const char* filename)
	{
		this->filename = filename;
	}

	bool Video::open()
	{
                // Register all formats and codecs
		av_register_all();

		// Open video file
		if (av_open_input_file(&pFormatCtx, filename, 0, 0, 0) != 0)
			return false; // Couldn't open file

		// Retrieve stream information
		if (av_find_stream_info(pFormatCtx) < 0)
			return false; // Couldn't find stream information

		// Dump information about file onto standard error
		//dump_format(pFormatCtx, 0, filename, false);

		// Find the first video stream
		videoStream= -1;
		for (unsigned int i= 0; i < pFormatCtx->nb_streams; i++)
			if (pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)
			{
				videoStream= i;
				break;
			}

		if (videoStream == -1)
			return false; // Didn't find a video stream

		// Get a pointer to the codec context for the video stream
		pCodecCtx= pFormatCtx->streams[videoStream]->codec;

		// Find the decoder for the video stream
		pCodec= avcodec_find_decoder(pCodecCtx->codec_id);
		if (pCodec == 0)
			return false; // Codec not found

		// Open codec
		if (avcodec_open(pCodecCtx, pCodec) < 0)
			return false; // Could not open codec

		// Hack to correct wrong frame rates that seem to be generated by some codecs
		if (pCodecCtx->time_base.num > 1000 && pCodecCtx->time_base.den == 1)
			pCodecCtx->time_base.den= 1000;

		// Allocate video frame
		pFrame= avcodec_alloc_frame();

		// Allocate an AVFrame structure
		pFrameRGB= avcodec_alloc_frame();
		if (pFrameRGB == 0)
			return false;

		width = pCodecCtx->width;
		height = pCodecCtx->height;

		// Determine required buffer size and allocate buffer
		numBytes= avpicture_get_size(PIX_FMT_RGB24, width, height);

		buffer= (uint8_t*) malloc(numBytes);

		// Assign appropriate parts of buffer to image planes in pFrameRGB
		avpicture_fill((AVPicture*)pFrameRGB, buffer, PIX_FMT_RGB24, width, height);

                return true;
	}

	bool Video::close()
	{
		// Free the RGB image
		free(buffer);
		av_free(pFrameRGB);

		// Free the YUV frame
		av_free(pFrame);

		// Close the codec
		avcodec_close(pCodecCtx);

		// Close the video file
		av_close_input_file(pFormatCtx);

		return true;
	}

	bool Video::nextFrame(int rgb_const, unsigned char* colorChanel)
	{
            if (av_read_frame(pFormatCtx, &packet) < 0)
                    return false;

            // Is this a packet from the video stream?
            if (packet.stream_index != videoStream)
            {
                av_free_packet(&packet);
                return false;
            }

            // Decode video frame
            avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,
                    packet.data, packet.size);

            // Did we get a video frame?
            if(frameFinished)
            {
                static struct SwsContext *img_convert_ctx;

                // Convert the image into YUV format that SDL uses
                if(img_convert_ctx == NULL)
                {
                    img_convert_ctx = sws_getContext(width, height,
                                                        pCodecCtx->pix_fmt,
                                                        width, height, PIX_FMT_RGB24, SWS_BICUBIC,
                                                        NULL, NULL, NULL);
                    if(img_convert_ctx == NULL)
                    {
                            return false;
                    }
                }
                sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0,
                                  pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                for (int i= 0, j, pixel= 0; i < height; i++)
                {
                    for (j= 0; j < width * 3; j+= 3)
                    {
                        if (rgb_const == VF_RED)
                                colorChanel[pixel]= pFrameRGB->data[0][i * pFrameRGB->linesize[0] + j];

                        if (rgb_const == VF_GREEN)
                                colorChanel[pixel]= pFrameRGB->data[0][i * pFrameRGB->linesize[0] + j + 1];

                        if (rgb_const == VF_BLUE)
                                colorChanel[pixel]= pFrameRGB->data[0][i * pFrameRGB->linesize[0] + j + 2];

                        pixel++;
                    }
                }

                av_free_packet(&packet);
                return true;
            }

            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);
            return false;
	}

	bool Video::nextFrame(unsigned char* r, unsigned char* g, unsigned char* b)
	{	
            if (av_read_frame(pFormatCtx, &packet) < 0)
                return false;
            
            // Is this a packet from the video stream?
            if (packet.stream_index != videoStream)
            {
                av_free_packet(&packet);
                return false;
            }

            // Decode video frame
            avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,
                    packet.data, packet.size);

            // Did we get a video frame?
            if(frameFinished)
            {
                static struct SwsContext *img_convert_ctx;

                // Convert the image into YUV format that SDL uses
                if(img_convert_ctx == NULL)
                {
                    img_convert_ctx = sws_getContext(width, height,
                                                        pCodecCtx->pix_fmt,
                                                        width, height, PIX_FMT_RGB24, SWS_BICUBIC,
                                                        NULL, NULL, NULL);
                    if(img_convert_ctx == NULL)
                    {
                            return false;
                    }
                }
//printf("Next 4\n");fflush(stdout);
                sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0,
                                  pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
//printf("Next 5\n");fflush(stdout);
                for (int i= 0, j, pixel= 0; i < height; i++)
                {
                    for (j= 0; j < width * 3; j+= 3)
                    {
                            r[pixel]= pFrameRGB->data[0][i * pFrameRGB->linesize[0] + j];
                            g[pixel]= pFrameRGB->data[0][i * pFrameRGB->linesize[0] + j + 1];
                            b[pixel]= pFrameRGB->data[0][i * pFrameRGB->linesize[0] + j + 2];

                            pixel++;
                    }
                }

                av_free_packet(&packet);
                return true;
            }

            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);
            return false;
	}

	int Video::getWidth()
	{
		return width;
	}

	int Video::getHeight()
	{
		return height;
	}
}

#endif