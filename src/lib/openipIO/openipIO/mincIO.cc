/* 
 * File:   Minc2.cc
 * Author: dzsekob
 * 
 * Created on 2011. május 26., 13:35
 */

#ifdef USE_MINC2

#include <openipIO/mincIO.h>
#include <openipIO/mincIOc.h>
#include <openipDS/Stopper.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

namespace openip
{
    Minc2::Minc2(unsigned int size_z, unsigned int size_y, unsigned int size_x)
    {            
	this->sizes[0]= size_z;
	this->sizes[1]= size_y;
	this->sizes[2]= size_x;
		    
	slab= (double*) malloc(sizeof(double) * size_x * size_y * size_z);
		    
	for (unsigned int i= 0; i < size_x * size_y * size_z; i++)
	    slab[i] = 0;
    }	

    Minc2::Minc2(const char* filename)
    {
	    strcpy(this->filename, filename);
    }

    Minc2::~Minc2()
    {
    }

    bool Minc2::open()
    {
        int flags= minc1to2(filename);

        int result;
        if ( flags )
        {
            //tprintf("MNC version 1\n");
            mincVersion= 1;
            strcpy(filename, "tmp_mnc_open.mnc");
        }
        else
        {
            //tprintf("MNC version 2\n");
            mincVersion= 2;
        }

        result= miopen_volume(filename, MI2_OPEN_READ, &minc_volume);

        if (result != MI_NOERROR)
        {
                fprintf(stderr, "Error opening input file: %d.\n", result);
                return false;
        }

        midimhandle_t dimensions[3];
        miget_volume_dimensions(minc_volume, MI_DIMCLASS_SPATIAL, MI_DIMATTR_ALL, MI_DIMORDER_FILE,3, dimensions);

        miget_dimension_sizes(dimensions, 3, this->sizes);
        miget_dimension_starts(dimensions, MI_ORDER_FILE, 3, this->starts);
        miget_dimension_separations(dimensions, MI_ORDER_FILE, 3, this->steps);
        //printf("%f %f %f\n", steps[0], steps[1], steps[2]); fflush(stdout);
        double valid_min, valid_max;
        miget_volume_valid_range(minc_volume, &valid_max, &valid_min);

        /*  hyperlab    */

        slab =(double*) malloc(sizeof(double) * sizes[0] * sizes[1] * sizes[2]);

        unsigned long start[3], count[3];
        start[0]= start[1]= start[2]= 0;

        for (int i=0; i < 3; i++) {
            count[i]= (unsigned long) sizes[i];
        }

        if (miget_real_value_hyperslab(minc_volume, MI_TYPE_DOUBLE,
            start, count, slab) != MI_NOERROR)
        {
            fprintf(stderr, "Error getting hyperslab\n");
            return false;
        }

        double vmin= DBL_MAX, vmax= -DBL_MAX;
        for ( int i= 0; i < sizes[0]*sizes[1]*sizes[2]; ++i )
        {
            if ( slab[i] < vmin )
                vmin= slab[i];
            if ( slab[i] > vmax )
                vmax= slab[i];
        }

        if ( flags )
            remove("tmp_mnc_open.mnc");

        return true;
    }

    bool Minc2::save(const char* filename)
    {
        mihandle_t    new_volume;
        midimhandle_t *dimensions_new;

        midimhandle_t dimensions[3];
        miget_volume_dimensions(minc_volume, MI_DIMCLASS_SPATIAL, MI_DIMATTR_ALL, MI_DIMORDER_FILE,3, dimensions);

        dimensions_new= (midimhandle_t*) malloc(sizeof(midimhandle_t) * 3);

        for(int i=0; i < 3; i++) {
            micopy_dimension(dimensions[i], &dimensions_new[i]);
        }

        if (micreate_volume(filename, 3, dimensions_new, MI_TYPE_SHORT,
                MI_CLASS_REAL, NULL, &new_volume) != MI_NOERROR)
        {
            fprintf(stderr, "Error creating new volume\n");

            return false;
        }

        if (micreate_volume_image(new_volume) != MI_NOERROR) {
            fprintf(stderr, "Error creating volume data\n");

            return false;
        }

        double min, max;
        unsigned long loc[3];
        loc[0]= loc[1] =loc[2]= 0;


        this->getSliceMinMax(loc, min, max);
        miset_volume_range(new_volume, max, min);
        miset_volume_valid_range(new_volume, 255, 0);

        unsigned long start[3], count[3];
        start[0]= start[1]= start[2]= 0;

        for (int i=0; i < 3; i++) {
            count[i]= (unsigned long) sizes[i];
        }

        if (miset_real_value_hyperslab(new_volume, MI_TYPE_DOUBLE,
            start, count, slab) != MI_NOERROR) {
            fprintf(stderr, "Error setting hyperslab\n");

            return false;
        }

        miclose_volume(new_volume);

        //free(dimensions);
        free(dimensions_new);

        return true;
    }

    bool Minc2::save2(const char* filename)
    {
        if ( mincVersion != 1 )
        {
            mihandle_t    new_volume;
            int result;
            mivolumeprops_t props;

            result = minew_volume_props(&props);
            if ( result != MI_NOERROR )
                printf("error\n");

	    //printf("bbb"); fflush(stdout);
            result = miset_props_multi_resolution(props, 1, 3);

            if (result < 0) {
                printf("failed");
            }

            midimhandle_t dim[3];

	    //printf("ccc %d %d %d %f %f %f\n", sizes[0], sizes[1], sizes[2], steps[0], steps[1], steps[2]); fflush(stdout);
	    
            micreate_dimension("zspace",MI_DIMCLASS_SPATIAL,
                                  MI_DIMATTR_REGULARLY_SAMPLED,
                                  sizes[0],&dim[0]);
            micreate_dimension("yspace",MI_DIMCLASS_SPATIAL,
                                  MI_DIMATTR_REGULARLY_SAMPLED,
                                  sizes[1],&dim[1]);
            micreate_dimension("xspace",MI_DIMCLASS_SPATIAL,
                                  MI_DIMATTR_REGULARLY_SAMPLED,
                                  sizes[2],&dim[2]);

            miset_dimension_start(dim[0], starts[0]);

            miset_dimension_start(dim[1], starts[1]);
            miset_dimension_start(dim[2], starts[2]);
            miset_dimension_separation(dim[0], steps[0]);
            miset_dimension_separation(dim[1], steps[1]);
            miset_dimension_separation(dim[2], steps[2]);

	    //printf("ddd"); fflush(stdout);
            if (micreate_volume(filename, 3, dim, MI_TYPE_SHORT,
                    MI_CLASS_REAL, props, &new_volume) != MI_NOERROR)
            {
                fprintf(stderr, "Error creating new volume\n");
                return false;
            }

            if (micreate_volume_image(new_volume) != MI_NOERROR) {
                fprintf(stderr, "Error creating volume data\n");

                return false;
            }

            //printf("eee"); fflush(stdout);
            double min= DBL_MAX, max= DBL_MIN;
            for ( unsigned int i= 0; i < sizes[0]*sizes[1]*sizes[2]; ++i )
            {
                if ( slab[i] < min )
                    min= slab[i];
                if ( slab[i] > max )
                    max= slab[i];
            }

            //printf("fff"); fflush(stdout);
            miset_volume_range(new_volume, max, min);
            miset_volume_valid_range(new_volume, max, min);

            unsigned long start[3], count[3];
            start[0]= start[1]= start[2]= 0;

            for (int i=0; i < 3; i++) {
                count[i]= (unsigned long) sizes[i];
            }

            //printf("ggg"); fflush(stdout);
            if (miset_real_value_hyperslab(new_volume, MI_TYPE_DOUBLE,
                start, count, slab) != MI_NOERROR) {
                    fprintf(stderr, "Error setting hyperslab\n");

                    return false;
            }

            //printf("hhh"); fflush(stdout);
            miclose_volume(new_volume);
	    //printf("jjj"); fflush(stdout);

            return true;
        }
        else if ( mincVersion == 1 )
        {
            mihandle_t    new_volume;
            int result;
            mivolumeprops_t props;

            result = minew_volume_props(&props);
            if ( result != MI_NOERROR )
                printf("error\n");

            result = miset_props_multi_resolution(props, 1, 3);

            if (result < 0) {
                printf("failed");
            }

            midimhandle_t dim[3];

            micreate_dimension("zspace",MI_DIMCLASS_SPATIAL,
                                  MI_DIMATTR_REGULARLY_SAMPLED,
                                  sizes[0],&dim[0]);
            micreate_dimension("yspace",MI_DIMCLASS_SPATIAL,
                                  MI_DIMATTR_REGULARLY_SAMPLED,
                                  sizes[1],&dim[1]);
            micreate_dimension("xspace",MI_DIMCLASS_SPATIAL,
                                  MI_DIMATTR_REGULARLY_SAMPLED,
                                  sizes[2],&dim[2]);

            miset_dimension_start(dim[0], starts[0]);

            miset_dimension_start(dim[1], starts[1]);
            miset_dimension_start(dim[2], starts[2]);
            miset_dimension_separation(dim[0], steps[0]);
            miset_dimension_separation(dim[1], steps[1]);
            miset_dimension_separation(dim[2], steps[2]);

            if (micreate_volume("tmp_mnc_write.mnc", 3, dim, MI_TYPE_SHORT,
                    MI_CLASS_REAL, props, &new_volume) != MI_NOERROR)
            {
                fprintf(stderr, "Error creating new volume\n");
                return false;
            }

            if (micreate_volume_image(new_volume) != MI_NOERROR) {
                fprintf(stderr, "Error creating volume data\n");

                return false;
            }

            double min= DBL_MAX, max= DBL_MIN;
            for ( unsigned int i= 0; i < sizes[0]*sizes[1]*sizes[2]; ++i )
            {
                if ( slab[i] < min )
                    min= slab[i];
                if ( slab[i] > max )
                    max= slab[i];
            }

            miset_volume_range(new_volume, max, min);
            miset_volume_valid_range(new_volume, max, min);

            unsigned long start[3], count[3];
            start[0]= start[1]= start[2]= 0;

            for (int i=0; i < 3; i++) {
                count[i]= (unsigned long) sizes[i];
            }

            if (miset_real_value_hyperslab(new_volume, MI_TYPE_DOUBLE,
                start, count, slab) != MI_NOERROR) {
                    fprintf(stderr, "Error setting hyperslab\n");

                    return false;
            }

            miclose_volume(new_volume);

            int old_fd= miopen("tmp_mnc_write.mnc", NC_NOWRITE);
            struct mi2opts opts;
            opts.struct_version= MI2_OPTS_V1;
            char f[1000];
            strcpy(f, filename);
            int new_fd = micreatex(f, MI2_OPTS_V1, &opts);
            micopy(old_fd, new_fd, NULL, 0);
            miclose(old_fd);
            miclose(new_fd);

            remove("tmp_mnc_write.mnc");

            return true;
        }
    }

    void Minc2::getRealCoordinate1D(int n, double& x, double& y, double& z)
    {
        double        dvoxel_location[3];
        double        world_location[3];

        dvoxel_location[0]= n / (sizes[1] * sizes[2]);
        int n2= n % (sizes[1] * sizes[2]);

        dvoxel_location[1]= n2 / sizes[2];
        dvoxel_location[2]= n2 % sizes[2];

        miconvert_voxel_to_world(minc_volume, dvoxel_location, world_location);

        x= world_location[0];
        y= world_location[1];
        z= world_location[2];
    }

    void Minc2::getRealCoordinate3D(int v_x, int v_y, int v_z, double &r_x, double &r_y, double &r_z)
    {
        double        dvoxel_location[3];
        double        world_location[3];

        dvoxel_location[0]= (double) v_x;
        dvoxel_location[1]= (double) v_y;
        dvoxel_location[2]= (double) v_z;

        miconvert_voxel_to_world(this->minc_volume, dvoxel_location, world_location);

        r_x= world_location[0];
        r_y= world_location[1];
        r_z= world_location[2];
    }

    double Minc2::getRealValue1D(int n)
    {
        return slab[n];
    }

    double Minc2::getRealValue3D(int z, int y, int x)
    {
        return slab[z * sizes[2] * sizes[1] + y * sizes[2] + x];
    }

    void Minc2::setRealValue1D(int n, double value)
    {
        slab[n]= value;
    }

    void Minc2::setRealValue3D(int z, int y, int x, double value)
    {
        //printf("%d %d %d - %d %d %d - %d\n", sizes[0], sizes[1], sizes[2], z, y, x, z * sizes[2] * sizes[1] + y * sizes[2] + x);
        slab[z * sizes[2] * sizes[1] + y * sizes[2] + x] = value;
    }

    void Minc2::getVolumeMinMax(double &min, double &max)
    {
        miget_volume_valid_range(this->minc_volume, &max, &min);
    }

    void Minc2::setVolumeMinMax(double min, double max)
    {
        miset_volume_valid_range(this->minc_volume, max, min);
    }

    void Minc2::getSliceMinMax(const unsigned long location[], double &min, double &max)
    {
        miget_slice_range(this->minc_volume, location, sizes[1] * sizes[2], &max, &min);
    }

    void Minc2::setSliceMinMax(const unsigned long location[], double min, double max)
    {
        miset_slice_range(this->minc_volume, location, sizes[1] * sizes[2], max, min);
    }

    int Minc2::getSlices()
    {
        return this->sizes[0];
    }

    int Minc2::getRows()
    {
        return this->sizes[1];
    }

    int Minc2::getColumns()
    {
        return this->sizes[2];
    }

	void Minc2::close()
	{
            miclose_volume(this->minc_volume);

            free(slab);
        }
}

#endif
