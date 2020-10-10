/**
 * @file CreateVideoFromImages.h
 * @author Janos Istvan Ivan <dzsekob@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The CreateVideoFromImages objects are capable to create video streams from
 * a sequence of Image objects.
 */

#ifndef CreateVideoFromImages_H
#define	CreateVideoFromImages_H

#ifdef USE_VIDEO

/** using stdc constant macros */
#define __STDC_CONSTANT_MACROS

#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavutil/avutil.h"
    #include "libavcodec/opt.h"
    #include "libavutil/avstring.h"
    #include "libavutil/fifo.h"
}

#include <openipDS/Image.h>
#include <openipDS/Vector.h>

namespace openip
{
    #ifndef M_PI
    /**
     * pi
     */
    #define M_PI 3.14159265358979323846
    #endif

    /**
     * pixelformat
     */
    #define STREAM_PIX_FMT PIX_FMT_YUV420P

    /**
     * CreateVideoFromImages objects are capable to create video streams from
     * sequences of Image objects.
     */
    class CreateVideoFromImages {
    public:
        /**
         * default constructor
         */
        CreateVideoFromImages();

        /**
         * method for grayscale video
         * @param gray intensity image sequence
         * @param outputFilename the output filename
         * @param fps number of frames per second
         * @return false in case of errors, true otherwise
         */
        bool createVideo(Vector<Image<unsigned char>*> gray, const char* outputFilename, int fps);

        /**
         * method for color video
         * @param r red channel image sequence
         * @param g green channel image sequence
         * @param b blue channel image sequence
         * @param outputFilename the output filename
         * @param fps number of frames per second
         * @return false in case of errors, true otherwise
         */
        bool createVideo(Vector<Image<unsigned char>*> r, Vector<Image<unsigned char>*> g, Vector<Image<unsigned char>*> b, const char* outputFilename, int fps);

    private:
        /** ffmpeg parameter */
        double stream_duration;
        /** ffmpeg parameter */
        int stream_frame_rate;
        /** ffmpeg parameter */
        int stream_nb_frames;
        /** ffmpeg parameter */
        unsigned int sws_flags;

        /** ffmpeg parameter */
        AVFrame *picture, *tmp_picture;
        /** ffmpeg parameter */
        uint8_t *video_outbuf;
        /** ffmpeg parameter */
        int frame_count, video_outbuf_size;

        /**
         * adds an image to the video stream
         * @param oc AVFormatContext pointer
         * @param codec_id codec ID
         * @param r image pointer
         * @return AVStream pointer
         */
        AVStream *add_video_stream(AVFormatContext *oc, enum CodecID codec_id, Image<unsigned char>* r);

        /**
         * allocates space for a new image
         * @param pix_fmt pixel format
         * @param width number of columns
         * @param height number of rows
         * @return AVFrame pointer
         */
        AVFrame *alloc_picture(int pix_fmt, int width, int height);

        /**
         * opens a new video
         * @param oc AVFormatContext pointer
         * @param st AVStream pointer
         * @return false in case of errors, true otherwise
         */
        bool open_video(AVFormatContext *oc, AVStream *st);

        /**
         * fills the parameter frame
         * @param pict AVFrame pointer
         * @param r red channel image
         * @param g green channel image
         * @param b blue channel image
         */
        void fill_yuv_image(AVFrame *pict, Image<unsigned char>* r, Image<unsigned char>* g, Image<unsigned char>* b);

        /**
         * writes the frame into the video stream
         * @param oc AVFormatContext pointer
         * @param st AVStream pointer
         * @param r red channel
         * @param g green channel
         * @param b blue channel
         * @return false in case of errors, true otherwise
         */
        bool write_video_frame(AVFormatContext *oc, AVStream *st, Image<unsigned char>* r, Image<unsigned char>* g, Image<unsigned char>* b);

        /**
         * closes the video stream
         * @param st AVStream pointer
         */
        void close_video(AVStream *st);
    };
}

#endif

#endif	/* CreateVideoFromImages_H */

