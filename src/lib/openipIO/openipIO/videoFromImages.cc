/* 
 * File:   CreateVideoFromImages.cc
 * Author: Dzsekob
 * 
 * Created on 2010. szeptember 20., 16:06
 */

#ifdef USE_VIDEO

#include <openipIO/videoFromImages.h>

namespace openip
{
    CreateVideoFromImages::CreateVideoFromImages()
    {
        sws_flags= SWS_BICUBIC;
        stream_duration= 4.0;
        frame_count= 0;
    }

    bool CreateVideoFromImages::createVideo(Vector<Image<unsigned char>*> gray, const char* outputFilename, int fps)
    {
        return createVideo(gray, gray, gray, outputFilename, fps);
    }

    bool CreateVideoFromImages::createVideo(Vector<Image<unsigned char>*> r, Vector<Image<unsigned char>*> g, Vector<Image<unsigned char>*> b, const char* outputFilename, int fps)
    {
        AVOutputFormat *fmt;
        AVFormatContext *oc;
        AVStream *video_st;
        double video_pts;
        unsigned int i;

        stream_frame_rate= fps;
        stream_nb_frames= stream_duration * stream_frame_rate;

        av_register_all();

        fmt= guess_format(NULL, outputFilename, NULL);

        if (!fmt) {
            printf("Could not deduce output format from file extension: using MPEG.\n");
            fmt= guess_format("mpeg", NULL, NULL);
        }
        if (!fmt) {
            fprintf(stderr, "Could not find suitable output format\n");
            return false;
        }

        /* allocate the output media context */
        oc= avformat_alloc_context();
        if (!oc) {
            fprintf(stderr, "Memory error\n");
            return false;
        }

        oc->oformat= fmt;
        snprintf(oc->filename, sizeof(oc->filename), "%s", outputFilename);

        video_st= NULL;
        if (fmt->video_codec != CODEC_ID_NONE) {
            video_st= add_video_stream(oc, fmt->video_codec, r[0]);
        }

        if (av_set_parameters(oc, NULL) < 0) {
            fprintf(stderr, "Invalid output format parameters\n");
            return false;
        }

        dump_format(oc, 0, outputFilename, 1);

        if (video_st)
            if (!open_video(oc, video_st))
                    return false;

        if (!(fmt->flags & AVFMT_NOFILE)) {
            if (url_fopen(&oc->pb, outputFilename, URL_WRONLY) < 0) {
                fprintf(stderr, "Could not open '%s'\n", outputFilename);
                return false;
            }
        }

        av_write_header(oc);

        for(;;) {
            if (video_st)
                video_pts= (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
            else
                video_pts= 0.0;

            if ((!video_st || video_pts >= stream_duration))
                break;

                if(!write_video_frame(oc, video_st, r[frame_count], g[frame_count], b[frame_count]))
                    return false;
        }

        av_write_trailer(oc);

        if (video_st)
            close_video(video_st);

        for(i= 0; i < oc->nb_streams; i++) {
            av_freep(&oc->streams[i]->codec);
            av_freep(&oc->streams[i]);
        }

        if (!(fmt->flags & AVFMT_NOFILE)) {
            url_fclose(oc->pb);
        }

        av_free(oc);

        return true; 
    }

    AVStream* CreateVideoFromImages::add_video_stream(AVFormatContext *oc, enum CodecID codec_id, Image<unsigned char>* r)
    {
        AVCodecContext *c;
        AVStream *st;

        st= av_new_stream(oc, 0);
        if (!st)
        {
            fprintf(stderr, "Could not alloc stream\n");
            return false;
        }

        c= st->codec;
        c->codec_id= codec_id;
        c->codec_type= CODEC_TYPE_VIDEO;

        c->bit_rate= 400000;

        c->width= r->columns;
        c->height= r->rows;

        c->time_base.den= stream_frame_rate;
        c->time_base.num= 1;
        c->gop_size= 12;
        c->pix_fmt= STREAM_PIX_FMT;
        if (c->codec_id == CODEC_ID_MPEG2VIDEO) 
            c->max_b_frames= 2;

        if (c->codec_id == CODEC_ID_MPEG1VIDEO)
            c->mb_decision=2;

        if(oc->oformat->flags & AVFMT_GLOBALHEADER)
            c->flags |= CODEC_FLAG_GLOBAL_HEADER;

        return st;
    }

    AVFrame* CreateVideoFromImages::alloc_picture(int pix_fmt, int width, int height)
    {
        AVFrame *picture;
        uint8_t *picture_buf;
        int size;

        picture= avcodec_alloc_frame();
        if (!picture)
            return NULL;

        size= avpicture_get_size((PixelFormat)pix_fmt, width, height);
        picture_buf= (uint8_t*) av_malloc(size);

        if (!picture_buf)
        {
            av_free(picture);
            return NULL;
        }
        avpicture_fill((AVPicture *)picture, picture_buf,
                       (PixelFormat)pix_fmt, width, height);
        return picture;
    }

    bool CreateVideoFromImages::open_video(AVFormatContext *oc, AVStream *st)
    {
        AVCodec *codec;
        AVCodecContext *c;

        c= st->codec;

        codec= avcodec_find_encoder(c->codec_id);
        if (!codec)
        {
            fprintf(stderr, "codec not found\n");
            return false;
        }

        if (avcodec_open(c, codec) < 0)
        {
            fprintf(stderr, "could not open codec\n");
            return false;
        }

        video_outbuf= NULL;
        if (!(oc->oformat->flags & AVFMT_RAWPICTURE))
        {
            video_outbuf_size= 200000;
            video_outbuf= (uint8_t*) av_malloc(video_outbuf_size);
        }

        picture= alloc_picture(c->pix_fmt, c->width, c->height);
        if (!picture)
        {
            fprintf(stderr, "Could not allocate picture\n");
            return false;
        }

        tmp_picture= NULL;
        if (c->pix_fmt != PIX_FMT_YUV420P)
        {
            tmp_picture= alloc_picture(PIX_FMT_YUV420P, c->width, c->height);

            if (!tmp_picture)
            {
                fprintf(stderr, "Could not allocate temporary picture\n");
                return false;
            }
        }

        return true;
    }

    void CreateVideoFromImages::fill_yuv_image(AVFrame *pict, Image<unsigned char>* r, Image<unsigned char>* g, Image<unsigned char>* b)
    {
        int x, y, i;
        const int c= r->n;

        unsigned char *uu, *vv;
        uu = (unsigned char*) malloc(c * sizeof (unsigned char));
        vv = (unsigned char*) malloc(c * sizeof (unsigned char));

        /* Y */
        for (y= 0, i= 0; y < r->rows; y++)
            for (x= 0; x < r->columns; x++, i++)
            {
                pict->data[0][y * pict->linesize[0] + x]= r->data()[i] * 0.299000 + g->data()[i] * 0.587000 + b->data()[i] * 0.114000;
                uu[i]= r->data()[i] * -0.168736 + g->data()[i] * -0.331264 + b->data()[i] * 0.500000 + 128;
                vv[i]= r->data()[i] * 0.500000 + g->data()[i] * -0.418688 + b->data()[i] * -0.081312 + 128;
            }

        /* Cb and Cr */
        for (y= 0; y < r->rows / 2; y++)
        {
            for (x= 0; x < r->columns / 2; x++)
            {
                pict->data[1][y * pict->linesize[1] + x]= (uu[2 * y * r->columns + 2 * x] + uu[2 * y * r->columns + 2 * x + 1] + uu[2 * (y + 1) * r->columns + 2 * x] + uu[2 * (y + 1) * r->columns + 2 * x + 1]) / 4;
                pict->data[2][y * pict->linesize[2] + x]= (vv[2 * y * r->columns + 2 * x] + vv[2 * y * r->columns + 2 * x + 1] + vv[2 * (y + 1) * r->columns + 2 * x] + vv[2 * (y + 1) * r->columns + 2 * x + 1]) / 4;
            }
        }
    }

    bool CreateVideoFromImages::write_video_frame(AVFormatContext *oc, AVStream *st, Image<unsigned char>* r, Image<unsigned char>* g, Image<unsigned char>* b)
    {
        int out_size, ret;
        AVCodecContext *c;
        static struct SwsContext *img_convert_ctx;

        c= st->codec;
        if (frame_count < stream_nb_frames) {
            if (c->pix_fmt != PIX_FMT_YUV420P) {
                if (img_convert_ctx == NULL) {
                    img_convert_ctx= sws_getContext(c->width, c->height,
                                                     PIX_FMT_YUV420P,
                                                     c->width, c->height,
                                                     c->pix_fmt,
                                                     sws_flags, NULL, NULL, NULL);
                    if (img_convert_ctx == NULL) {
                        fprintf(stderr, "Cannot initialize the conversion context\n");
                        return false;
                    }
                }
                fill_yuv_image(tmp_picture, r, g, b);
                sws_scale(img_convert_ctx, tmp_picture->data, tmp_picture->linesize,
                          0, c->height, picture->data, picture->linesize);
            } else {
                fill_yuv_image(picture, r, g, b);
            }
        }

        if (oc->oformat->flags & AVFMT_RAWPICTURE) {
            AVPacket pkt;
            av_init_packet(&pkt);

            pkt.flags |= PKT_FLAG_KEY;
            pkt.stream_index= st->index;
            pkt.data= (uint8_t *)picture;
            pkt.size= sizeof(AVPicture);

            ret= av_interleaved_write_frame(oc, &pkt);
        } else {
            out_size= avcodec_encode_video(c, video_outbuf, video_outbuf_size, picture);

            if (out_size > 0) {
                AVPacket pkt;
                av_init_packet(&pkt);

                if (c->coded_frame->pts != AV_NOPTS_VALUE)
                    pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
                if(c->coded_frame->key_frame)
                    pkt.flags |= PKT_FLAG_KEY;
                pkt.stream_index= st->index;
                pkt.data= video_outbuf;
                pkt.size= out_size;

                ret= av_interleaved_write_frame(oc, &pkt);
            } else {
                ret= 0;
            }
        }
        if (ret != 0) {
            fprintf(stderr, "Error while writing video frame\n");
            return false;
        }
        frame_count++;

        return true;
    }

    void CreateVideoFromImages::close_video(AVStream *st)
    {
        avcodec_close(st->codec);
        av_free(picture->data[0]);
        av_free(picture);
        if (tmp_picture) {
            av_free(tmp_picture->data[0]);
            av_free(tmp_picture);
        }
        av_free(video_outbuf);
    }
}

#endif