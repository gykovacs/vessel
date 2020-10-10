/* 
 * File:   mincIOc.h
 * Author: gykovacs
 *
 * Created on 2011. július 13., 10:27
 */

#ifdef USE_MINC2

#ifndef MINCIOC_H
#define	MINCIOC_H

#ifdef	__cplusplus
extern "C" {
#endif

int micreatex(char*, int cmode, struct mi2opts* opts_ptr);

int micopy(int old_fd, int new_fd, char *new_history, int is_template);

int minc1to2(char* filename);

#ifdef	__cplusplus
}
#endif

#endif

#endif	/* MINCIOC_H */

