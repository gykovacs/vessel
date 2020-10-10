/**
 * @file Video.h
 * @author Istvan Janos Ivan <dzsekob@gmail.com>
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
 * The Video class represens a video stream
 */

#ifndef VIDEO_H
#define	VIDEO_H

#ifdef USE_VIDEO

#ifndef INT64_C
/** v4l related stuff */
#define INT64_C(c) (c ## LL)
/** v4l related stuff */
#define UINT64_C(c) (c ## ULL)
#endif

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

namespace openip
{
    /** red channel identifier */
    #define VF_RED 1
    /** green channel identifier */
    #define VF_GREEN 2
    /** blue channel identifier */
    #define VF_BLUE 3

    /**
     * Video class represents a video stream
     */
    class Video {
    public:
        /**
         * constructor
         * @param filename the name of the video file
         */
        Video(const char* filename);

        /**
         * opens the video stream
         * @return false in case of errors, true otherwise
         */
        bool open();

        /**
         * closes the video stream
         * @return false in case of errors, true otherwise
         */
        bool close();

        /**
         * reads the next video frame
         * @param rgb_const channel identifier
         * @param colorChanel the frame is read to this pointer
         * @return false in case of errors, true otherwise
         */
        bool nextFrame(int rgb_const, unsigned char* colorChanel);

        /**
         * reads the next video frame
         * @param r red channel pointer
         * @param g green channel pointer
         * @param b blue channel pointer
         * @return false in case of errors, true otherwise
         */
        bool nextFrame(unsigned char* r, unsigned char* g, unsigned char* b);

        /**
         * returns the number of columns in the video stream
         * @return number of columns
         */
        int getWidth();

        /**
         * returns the number of rows in the video stream
         * @return number of rows
         */
        int getHeight();

    private:
        /** the filename of the video stream */
        const char* filename;

        /** ffmpeg parameter */
        AVFormatContext *pFormatCtx;
        /** ffmpeg parameter */
        int             videoStream;
        /** ffmpeg parameter */
        AVCodecContext  *pCodecCtx;
        /** ffmpeg parameter */
        AVCodec         *pCodec;
        /** ffmpeg parameter */
        AVFrame         *pFrame;
        /** ffmpeg parameter */
        AVFrame         *pFrameRGB;
        /** ffmpeg parameter */
        AVPacket        packet;
        /** ffmpeg parameter */
        int             frameFinished;
        /** ffmpeg parameter */
        int             numBytes;
        /** temporary buffer */
        unsigned char*  buffer;

        /** number of columns of the frames */
        int width;
        /** number of rows of the frames */
        int height;
    };
}

#endif

#endif	/* VIDEO_H */
