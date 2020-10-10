/*
 * File:   VolumeMinc2.h
 * Author: Iván János István
 *
 * Created on 2011. május 26., 12:29
 */

#ifndef MINC2IO_H
#define	MINC2IO_H

#ifdef USE_MINC2

#define MINC2 1
#include <minc2.h>
#include <minc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <minc_compat.h>
#include <openipIO/mincIOc.h>
#include <openipDS/Volume.h>
#include <openipDS/FramedVolume.h>

//MNCAPI int micreatex(char*, int, struct mi2opts*);

namespace openip
{
    template<typename T>
    int readMNC(const char* filename, Volume<T>& v, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
    
    template<typename T>
    int readMNC(const char* filename, Volume<T>& v, Border3 b);
    
    template<typename T>
    int readMNC(const char* filename, FramedVolume<T>& v, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
    
    template<typename T>
    int readMNC(const char* filename, FramedVolume<T>& v, Border3 b);

    template<typename T>
    int writeMNC(const char* filename, const Volume<T>& v);
    
    template<typename T>
    int writeMNC(const char* filename, const FramedVolume<T>& v);
    
    
    
    class Minc2
    {
    public:
        Minc2(const char* filename);
        Minc2(unsigned int size_x, unsigned int size_y, unsigned int size_z);
        
        ~Minc2();
        
        bool open();
        bool save(const char* filename);
        bool save2(const char* filename);
        void close();

        void getRealCoordinate1D(int n, double& x, double& y, double& z);
        void getRealCoordinate3D(int v_x, int v_y, int v_z, double &r_x, double &r_y, double &r_z);

        double getRealValue1D(int n);
        double getRealValue3D(int x, int y, int z);

        void setRealValue1D(int n, double value);
        void setRealValue3D(int x, int y, int z, double value);

        void getVolumeMinMax(double &min, double &max);
        void getSliceMinMax(const unsigned long location[], double &min, double &max);

        void setVolumeMinMax(double min, double max);
        void setSliceMinMax(const unsigned long location[], double min, double max);

        int getSlices();
        int getRows();
        int getColumns();

    public:
        char filename[1000];

        mihandle_t      minc_volume;
        unsigned int    sizes[3];
        double          steps[3];
        double          starts[3];
        double          *slab;
        int mincVersion;
    };

    template<typename T>
    int readMNC(const char* filename, Volume<T>& v,  int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        Minc2 minc(filename);

        if ( minc.open() == false )
        {
            printf("cant open minc file\n");
            return 1;
        }
        
        //printf("%f %f %f\n", minc.starts[0], minc.starts[1], minc.starts[2]);
        //printf("%f %f %f\n", minc.steps[0], minc.steps[1], minc.steps[2]);
        
        v.resizeVolume(minc.sizes[0] + frontBorder + backBorder, minc.sizes[1] + topBorder + bottomBorder, minc.sizes[2] + leftBorder + rightBorder);
        v.topBorder= topBorder;
        v.bottomBorder= bottomBorder;
        v.leftBorder= leftBorder;
        v.rightBorder= rightBorder;
        v.frontBorder= frontBorder;
        v.backBorder= backBorder;
        v.borderMode= borderMode;
        
        if ( typeid(T) == typeid(unsigned char) )
        {
            double d;
            for ( unsigned int i= 0; i < minc.sizes[0]; ++i )
                for ( unsigned int j= 0; j < minc.sizes[1]; ++j )
                    for ( unsigned int k= 0; k < minc.sizes[2]; ++k )
                    {
                        d= minc.getRealValue3D(i,j,k);
                        v(frontBorder + i, topBorder + j, leftBorder + k)= floor(d + 0.5);
                    }
        }
        else
        {
            for ( unsigned int i= 0; i < minc.sizes[0]; ++i )
                for ( unsigned int j= 0; j < minc.sizes[1]; ++j )
                    for ( unsigned int k= 0; k < minc.sizes[2]; ++k )
                        v(frontBorder + i, topBorder + j, leftBorder + k)= minc.getRealValue3D(i,j,k);
        }
            
        minc.close();
	
	v.fillBorder();

        v.mincVersion= minc.mincVersion;
        v.filename= std::string(filename);
        return 0;
    }

    template<typename T>
    int readMNC(const char* filename, Volume<T>& v, Border3 b)
    {
        return readMNC(filename, v, b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
    }

    template<typename T>
    int readMNC(const char* filename, FramedVolume<T>& v, int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        Minc2 minc(filename);

        if ( minc.open() == false )
        {
            printf("cant open minc file\n");
            return 1;
        }
        
        v.resizeVolume(minc.sizes[0] + frontBorder + backBorder, minc.sizes[1] + topBorder + bottomBorder, minc.sizes[2] + leftBorder + rightBorder);
        v.topBorder= topBorder;
        v.bottomBorder= bottomBorder;
        v.leftBorder= leftBorder;
        v.rightBorder= rightBorder;
        v.frontBorder= frontBorder;
        v.backBorder= backBorder;
        v.borderMode= borderMode;
        
        for ( unsigned int i= 0; i < minc.sizes[0]; ++i )
            for ( unsigned int j= 0; j < minc.sizes[1]; ++j )
                for ( unsigned int k= 0; k < minc.sizes[2]; ++k )
                    v(frontBorder + i, topBorder + j, leftBorder + k)= minc.getRealValue3D(i,j,k);
        
        v.origin= Vector3<float>(float(minc.starts[0]), float(minc.starts[1]), float(minc.starts[2]));
        v.baseX= Vector3<float>(minc.steps[0], 0, 0);
        v.baseY= Vector3<float>(0, minc.steps[1], 0);
        v.baseZ= Vector3<float>(0, 0, minc.steps[2]);

        minc.close();
	
	v.fillBorder();
	
        v.mincVersion= minc.mincVersion;
        v.filename= std::string(filename);

        return 0;
    }

    template<typename T>
    int readMNC(const char* filename, FramedVolume<T>& v, Border3 b)
    {
        return readMNC(filename, v, b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
    }

    
    template<typename T>
    int writeMNC(const char* filename, const Volume<T>& v)
    {
        //printf("writeMNC: %s\n", filename);
        Minc2 minc(v.slices - v.frontBorder - v.backBorder, v.rows - v.topBorder - v.bottomBorder, v.columns - v.leftBorder - v.rightBorder);
        for ( unsigned int i= v.frontBorder; i < v.slices - v.backBorder; ++i )
            for ( unsigned int j= v.topBorder; j < v.rows - v.bottomBorder; ++j )
                for ( unsigned int k= v.leftBorder; k < v.columns - v.rightBorder; ++k )
                    minc.setRealValue3D(i - v.frontBorder, j - v.topBorder, k - v.leftBorder, (float)v(i,j,k));
        minc.mincVersion= v.mincVersion;
        minc.save2(filename);
        return 0;
    }
    
    template<typename T>
    int writeMNC(const char* filename, const FramedVolume<T>& v)
    {
        //printf("%d %d %d %d %d %d\n", v.frontBorder, v.backBorder, v.topBorder, v.bottomBorder, v.leftBorder, v.rightBorder);
        Minc2 minc(v.slices - v.frontBorder - v.backBorder, v.rows - v.topBorder - v.bottomBorder, v.columns - v.leftBorder - v.rightBorder);
        for ( unsigned int i= v.frontBorder; i < v.slices - v.backBorder; ++i )
            for ( unsigned int j= v.topBorder; j < v.rows - v.bottomBorder; ++j )
                for ( unsigned int k= v.leftBorder; k < v.columns - v.rightBorder; ++k )
                    minc.setRealValue3D(i - v.frontBorder, j - v.topBorder, k - v.leftBorder, (float)v(i,j,k));
            
        minc.starts[0]= v.origin.x();
        minc.starts[1]= v.origin.y();
        minc.starts[2]= v.origin.z();
        minc.steps[0]= v.baseX.norm();
        minc.steps[1]= v.baseY.norm();
        minc.steps[2]= v.baseZ.norm();
        minc.mincVersion= v.mincVersion;
        minc.save2(filename);
        return 0;
    }
}

#endif

#endif	/* MINC2_H */

