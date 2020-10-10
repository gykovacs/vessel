/**
 * @file WebCam.h
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
 * The WebCam class represents a webcam device
 */

#ifndef WEBCAM_H
#define	WEBCAM_H

#ifdef USE_WEBCAM

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
    #include <windows.h>
    #include <commctrl.h>
    #include <vfw.h>
    #include <conio.h>
#else
    #include <string.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <sys/mman.h>
    #include <linux/videodev2.h>
    #include <libv4l2.h>
    #include <libv4lconvert.h>
    #include <cstring>
#endif

namespace openip 
{
    /** red channel identifier */
    #define VF_RED 1
    /** green channel identifier */
    #define VF_GREEN 2
    /** blue channel identifier */
    #define VF_BLUE 3

    /**
     * WebCam class represents a webcam device
     * @return
     */
    class WebCam
    {
    public:
        /**
         * constructor
         * @param index device index
         */
        WebCam(int index);

        /**
         * constructor
         * @param index device index
         * @param width number of columns
         * @param height number of rows
         */
        WebCam(int index, int width, int height);

        /**
         * opens the stream
         * @return false in case of errors, true otherwise
         */
        bool open();

        /**
         * closes the stream
         * @return false in case of errors, true otherwise
         */
        bool close();

        /**
         * grabFrame
         * @param rgb_const rgb channel index
         * @param colorChanel buffer to read the frame
         * @return false in case of errors, true otherwise
         */
        bool grabFrame(int rgb_const, unsigned char* colorChanel);

        /**
         * grabFrame
         * @param r buffer for red channel
         * @param g buffer for green channel
         * @param b buffer for blue channel
         * @return false in case of errors, true otherwise
         */
        bool grabFrame(unsigned char* r, unsigned char* g,unsigned char* b);

        /**
         * returns the number of columns in the stream
         * @return number of columns
         */
        int getWidth();

        /**
         * returns the number of rows in the stream
         * @return number of rows
         */
        int getHeight();

        /**
         * sets the number of columns in the stream
         * @param width number of columns
         */
        void setWidth(int width);

        /**
         * sets the number of rows in the stream
         * @param height number of rows
         */
        void setHeight(int height);

    private:

        /** device index */
        int index;

        /** number of columns in the stream */
        int width;

        /** number of rows in the stream */
        int height;

        /** frame buffer */
        unsigned char* frame;

        #ifdef WIN32
            /** dx related parameter */
            CAPDRIVERCAPS caps;
            /** dx related parameter */
            VIDEOHDR* hdr;
            /** dx related parameter */
            HWND hwnd;
            /** dx parameter */
            HIC    hic;

            /** dx related function */
            static LRESULT PASCAL frameCallback( HWND hWnd, VIDEOHDR* _hdr );
            /** dx related function */
            void closeHIC();
            /**
             * grab function
             * @return false in case of errors, true otherwise
             */
            bool grab();

            /**
             * gets next frame
             * @return buffer pointer
             */
            unsigned char* retrieveFrame();
        #else
            /** v4l related parameter */
            struct v4lconvert_data* v4lconvert_data;
            /** v4l related parameter */
            struct v4l2_format              fmt, src_fmt;
            /** v4l related parameter */
            struct v4l2_buffer              buf;
            /** v4l related parameter */
            struct v4l2_requestbuffers      req;
            /** v4l related parameter */
            enum v4l2_buf_type              type;
            /** v4l related parameter */
            fd_set                          fds;
            /** v4l related parameter */
            struct timeval                  tv;
            /** v4l related parameter */
            int                             rr, fd;
            /** v4l related parameter */
            unsigned int                    n_buffers;
            /** v4l related parameter */
            char                            *dev_name;

            /** v4l related parameter */
            struct buffer {
                    void   *start;
                    size_t length;
            };

            /** v4l related parameter */
            struct buffer                   *buffers;

            /**
             * v4l related function
             * @param fh v4l related parameter
             * @param request v4l related parameter
             * @param arg v4l related parameter
             */
            void xioctl(int fh, int request, void *arg);
        #endif
    };
}

#endif

#endif	/* WEBCAM_H */
