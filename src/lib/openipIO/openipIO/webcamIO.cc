/* 
 * File:   WebCam.cc
 * Author: Iván János István
 * 
 * Created on 2010. augusztus 9., 11:48
 */

#include <openipIO/webcamIO.h>

#ifdef USE_WEBCAM

namespace openip
{
    WebCam::WebCam(int index)
    {
        this->index= index;

        this->width= 640;
        this->height= 480;

        this->frame= 0;
    }

    WebCam::WebCam(int index, int width, int height)
    {
        this->index= index;

        this->width= width;
        this->height= height;

        this->frame= 0;
    }
    
    #ifdef WIN32
        void WebCam::closeHIC()
        {
            if ( hic )
            {
                ICDecompressEnd( hic );
                ICClose( hic );
                hic= 0;
            }
        }

        LRESULT PASCAL WebCam::frameCallback( HWND hWnd, VIDEOHDR* _hdr )
        {
            WebCam* capture= 0;

            if (!hWnd) return FALSE;

            capture= (WebCam*) capGetUserData(hWnd);
            capture->hdr= _hdr;

            return (LRESULT)TRUE;
        }

        bool WebCam::grab()
        {

            if ( hwnd )
            {
                capGrabFrameNoStop(hwnd);
                return true;
            }

            return false;
        }

        unsigned char* WebCam::retrieveFrame()
        {
            BITMAPINFO vfmt;
            memset( &vfmt, 0, sizeof(vfmt));
            BITMAPINFOHEADER& vfmt0= vfmt.bmiHeader;

            int format, preWidth, preHeight;
            int bitDeep= 24;

            if ( !hwnd )
                return 0;

            format= capGetVideoFormat( hwnd, &vfmt, sizeof(vfmt));
            preWidth= width;
            preHeight= height;

            if ( !hdr || hdr->lpData == 0 || format == 0 )
                return 0;

            if ( !frame || width != vfmt0.biWidth || height != vfmt0.biHeight )
            {
                delete frame;
                frame= new unsigned char[vfmt0.biWidth * vfmt0.biHeight * (int)(bitDeep / 8)];
                width= vfmt0.biWidth;
                height= vfmt0.biHeight;
            }

            if ( vfmt.bmiHeader.biCompression != BI_RGB || vfmt.bmiHeader.biBitCount != bitDeep )
            {

                BITMAPINFOHEADER vfmt1;
                memset(&vfmt1, 0, sizeof(BITMAPINFOHEADER));
                vfmt1.biSize= sizeof(BITMAPINFOHEADER);
                vfmt1.biHeight= vfmt0.biHeight;
                vfmt1.biWidth= vfmt0.biWidth;
                vfmt1.biCompression= BI_RGB;
                vfmt1.biPlanes= 1;
                vfmt1.biBitCount= bitDeep;

                if ( hic == 0 || preWidth != vfmt0.biWidth || preHeight != vfmt0.biHeight )
                {
                    closeHIC();
                    hic= ICOpen( MAKEFOURCC('V','I','D','C'),
                                  vfmt0.biCompression, ICMODE_DECOMPRESS );
                    if ( hic )
                    {
                        if ( ICDecompressBegin( hic, &vfmt0, &vfmt1 ) != ICERR_OK )
                        {
                            closeHIC();
                            return 0;
                        }

                        ICDecompressGetFormat(hic, &vfmt0, &vfmt1);
                    }
                }

                if ( !hic || ICDecompress( hic, 0, &vfmt0, hdr->lpData,
                    &vfmt1, frame ) != ICERR_OK )
                {
                    closeHIC();
                    return 0;
                }

                closeHIC();
            }
            else
            {
                width= vfmt0.biWidth;
                height= vfmt0.biHeight;
                frame= new unsigned char[vfmt0.biWidth * vfmt0.biHeight * (int)(bitDeep / 8)];

                for (int i= 0; i < hdr->dwBufferLength; i++)
                    frame[i]= hdr->lpData[i];
            }

            return frame;
        }
    #else
        void WebCam::xioctl(int fh, int request, void *arg)
        {
                int r;

                do
                {
                    r= v4l2_ioctl(fh, request, arg);
                } while (r == -1 && ((errno == EINTR)));

                if (r == -1)
                    return;
        };
    #endif


    bool WebCam::close()
    {
        #ifdef WIN32
            if ( hwnd )
            {
                capSetCallbackOnFrame( hwnd, NULL);
                capDriverDisconnect( hwnd );
                DestroyWindow( hwnd );
            }

            return true;
        #else
            if (frame)
                free(frame);

            v4l2_close(fd);

            return true;
        #endif
    }

    bool WebCam::open()
    {
        #ifdef WIN32

            hwnd= 0;
            close();

            fflush(stdout);

            hwnd= capCreateCaptureWindow ( TEXT("Capture Window"),
                    WS_POPUP | WS_CHILD, 0, 0, width, height, 0, 0);

            fflush(stdout);

            if (hwnd == 0)
                return false;

            if ( !capDriverConnect (hwnd, index))
            {
                DestroyWindow( hwnd );
                hwnd= 0;

                return false;
            }

            hdr= 0;
            hic= 0;
            
            memset( &caps, 0, sizeof(caps));
            capDriverGetCaps( hwnd, &caps, sizeof(&caps));
            
            capSetUserData( hwnd, (size_t)this );
            capSetCallbackOnFrame( hwnd, frameCallback );

            CAPTUREPARMS p;
            capCaptureGetSetup(hwnd, &p, sizeof(CAPTUREPARMS));

            // p.dwRequestMicroSecPerFrame= 100000;	// 10 frames per sec
            p.dwRequestMicroSecPerFrame= 66667;	// 15 frames per sec
            // p.dwRequestMicroSecPerFrame= 40000;	// 25 frames per sec

            capCaptureSetSetup(hwnd, &p, sizeof(CAPTUREPARMS));
            capPreviewScale(hwnd, FALSE);
            capPreviewRate(hwnd, 1);

            return true;

        #else
            fd= -1;

            dev_name= (char*)malloc( sizeof( char ) * 40);
            sprintf(dev_name, "/dev/video%d", index);

            fd= v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);

            if (fd < 0)
                   return false;
    
            memset(&(fmt), 0, sizeof(fmt));
            fmt.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
            fmt.fmt.pix.width= width;
            fmt.fmt.pix.height= height;
            fmt.fmt.pix.pixelformat= V4L2_PIX_FMT_RGB24;
            fmt.fmt.pix.field= V4L2_FIELD_INTERLACED;

            xioctl(fd, VIDIOC_S_FMT, &fmt);

            if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) 
                   return false;

            v4lconvert_data= v4lconvert_create(fd);
            v4lconvert_try_format(v4lconvert_data, &fmt, &src_fmt);

            xioctl(fd, VIDIOC_S_FMT, &src_fmt);

            frame= (unsigned char*) malloc(fmt.fmt.pix.sizeimage);

            memset(&(req), 0, sizeof(req));
            req.count= 2;
            req.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
            req.memory= V4L2_MEMORY_MMAP;
            xioctl(fd, VIDIOC_REQBUFS, &req);

            buffers= (buffer*)calloc(req.count, sizeof(*buffers));
            for (n_buffers= 0; n_buffers < req.count; n_buffers++)
            {
                   memset(&(buf), 0, sizeof(buf));

                   buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
                   buf.memory= V4L2_MEMORY_MMAP;
                   buf.index= n_buffers;

                   xioctl(fd, VIDIOC_QUERYBUF, &buf);

                   buffers[n_buffers].length= buf.length;
                   buffers[n_buffers].start= v4l2_mmap(NULL, buf.length,
                                 PROT_READ | PROT_WRITE, MAP_SHARED,
                                 fd, buf.m.offset);

                   if (MAP_FAILED == buffers[n_buffers].start) 
                       return false;
            }

            for (int i= 0; i < n_buffers; i++)
            {
                   memset(&(buf), 0, sizeof(buf));
                   buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
                   buf.memory= V4L2_MEMORY_MMAP;
                   buf.index= i;
                   xioctl(fd, VIDIOC_QBUF, &buf);
            }

            type= V4L2_BUF_TYPE_VIDEO_CAPTURE;

            xioctl(fd, VIDIOC_STREAMON, &type);

            return true;
        #endif
    }

    bool WebCam::grabFrame(int vf_const, unsigned char* colorChanel)
    {
        #ifdef WIN32
            memset( &caps, 0, sizeof(caps) );

            if (!grab())
                return false;

            frame= retrieveFrame();

            if (!frame)
                return false;

            int pixel= 0;

            for (int i= height - 1, j; i >= 0 ; i--)
                for (j= 0; j < width * 3; j+= 3)
                {
                    if (vf_const == VF_RED)
                        colorChanel[pixel]= frame[i * width * 3 + j + 2];

                    if (vf_const == VF_GREEN)
                        colorChanel[pixel]= frame[i * width * 3 + j + 1];

                    if (vf_const == VF_BLUE)
                        colorChanel[pixel]= frame[i * width * 3 + j];

                    pixel++;
                }

            return true;
        #else
            int count= 0;

            while(count != 2)
            {
                do
                {
                        FD_ZERO(&fds);
                        FD_SET(fd, &fds);

                        tv.tv_sec= 2;
                        tv.tv_usec= 0;

                        rr= select(fd + 1, &fds, NULL, NULL, &tv);
                } while (rr == -1 && (errno == EINTR));

                if (rr == -1)
                    return false;

                memset(&(buf), 0, sizeof(buf));
                buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory= V4L2_MEMORY_MMAP;
                xioctl(fd, VIDIOC_DQBUF, &buf);


                v4lconvert_convert(v4lconvert_data, &src_fmt, &fmt,
                            (unsigned char*)buffers[buf.index].start,
                            buf.bytesused,
                            frame, fmt.fmt.pix.sizeimage);

                frame= (unsigned char*) malloc(fmt.fmt.pix.sizeimage);
                memmove(frame, frame, fmt.fmt.pix.sizeimage);

                xioctl(fd, VIDIOC_QBUF, &buf);
                count++;
            }

            int pixel= 0;

            for (int i= 0, j; i < height ; i++)
                for (j= 0; j < width * 3; j+= 3)
                {
                    if (vf_const == VF_RED)
                        colorChanel[pixel]= frame[i * width * 3 + j + 0];

                    if (vf_const == VF_GREEN)
                        colorChanel[pixel]= frame[i * width * 3 + j + 1];

                    if (vf_const == VF_BLUE)
                        colorChanel[pixel]= frame[i * width * 3 + j + 2];

                    pixel++;
                }

            return true;
        #endif
    }

    bool WebCam::grabFrame(unsigned char* r, unsigned char* g, unsigned char* b)
    {
        #ifdef WIN32
            memset( &caps, 0, sizeof(caps) );

            if (!grab())
                return false;

            frame= retrieveFrame();

            if (!frame)
                return false;

            int pixel= 0;

            for (int i= height - 1, j; i >= 0 ; i--)
                for (j= 0; j < width * 3; j+= 3)
                {
                    b[pixel]= frame[i * width * 3 + j];
                    g[pixel]= frame[i * width * 3 + j + 1];
                    r[pixel]= frame[i * width * 3 + j + 2];

                    pixel++;
                    
                }

            return true;
        #else
            int count= 0;

            while(count != 2)
            {
                do
                {
                        FD_ZERO(&fds);
                        FD_SET(fd, &fds);

                        tv.tv_sec= 2;
                        tv.tv_usec= 0;

                        rr= select(fd + 1, &fds, NULL, NULL, &tv);
                } while (rr == -1 && (errno == EINTR));

                if (rr == -1)
                    return false;

                memset(&(buf), 0, sizeof(buf));
                buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory= V4L2_MEMORY_MMAP;
                xioctl(fd, VIDIOC_DQBUF, &buf);

                v4lconvert_convert(v4lconvert_data, &src_fmt, &fmt,
                            (unsigned char*)buffers[buf.index].start,
                            buf.bytesused,
                            frame, fmt.fmt.pix.sizeimage);

                xioctl(fd, VIDIOC_QBUF, &buf);
                count++;
            }

            int pixel= 0;

            for (int i= 0, j; i < height ; i++)
                for (j= 0; j < width * 3; j+= 3)
                {
                    r[pixel]= frame[i * width * 3 + j + 0];
                    g[pixel]= frame[i * width * 3 + j + 1];
                    b[pixel]= frame[i * width * 3 + j + 2];

                    pixel++;
                }

            return true;
        #endif
    }

    int WebCam::getWidth()
    {
        return this->width;
    }

    int WebCam::getHeight()
    {
        return this->height;
    }

    void WebCam::setWidth(int width)
    {
        this->width= width;
    }

    void WebCam::setHeight(int height)
    {
        this->height= height;
    }
}

#endif