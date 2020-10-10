#ifdef USE_MINC2

#include <openipIO/mincIOc.h>
#define MINC2 1
#include <minc.h>
#include <minc2.h>

int micopy(int old_fd, int new_fd, char *new_history, int is_template)
{
    if (is_template) {
        /* Tell NetCDF that we don't want to allocate the data until written.
         */
        ncsetfill(new_fd, NC_NOFILL);
    }

    /* Copy all variable definitions (and global attributes).
     */
    micopy_all_var_defs(old_fd, new_fd, 0, NULL);

    /* Append the updated history.
     */
    //miappend_history(new_fd, new_history);

    if (!is_template) {
        ncendef(new_fd);
        micopy_all_var_values(old_fd, new_fd, 0, NULL);
    }
    else {
        /* This isn't really standard, but flag this as a template file. 
         */
        miattputstr(new_fd, NC_GLOBAL, "class", "template");
    }
    return (MI_NOERROR);
}

int minc1to2(char* filename)
{
    int old_fd;
    int new_fd;
    int flags= 0;
    struct mi2opts opts;
 
    old_fd= miopen(filename, NC_NOWRITE);
 
    if ( !MI2_ISH5OBJ(old_fd) )
    {
        flags|= MI2_CREATE_V2;

        opts.struct_version= MI2_OPTS_V1;
        opts.comp_type= MI2_COMP_NONE;
        opts.chunk_type= MI2_CHUNK_OFF;
 
        new_fd= micreatex("tmp_mnc_open.mnc", flags, &opts);
 
        micopy(old_fd, new_fd, NULL, 0);
 
        //miclose(old_fd);
        miclose(new_fd);
     }
 
    return flags;
}

#endif
