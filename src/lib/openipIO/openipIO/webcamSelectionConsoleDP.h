/**
 * @file WebCamSelectionConsoleDP.h
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
 * Consol based dialog primitive for WebCam device selection.
 */

#ifndef WEBCAMSELECTIONCONSOLEDP_H
#define	WEBCAMSELECTIONCONSOLEDP_H

#ifdef USE_WEBCAM

#ifdef WIN32
    #include <vfw.h>
#else
    #include <sys/ioctl.h>
    #include <sys/poll.h>
    #include <linux/videodev2.h>
    #include <fcntl.h>
    #include <stdlib.h>
    #include <string.h>
    #include <libv4l2.h>
#endif

namespace openip
{
    /** Consol based dialog primitive for WebCam device selection*/
    class WebCamSelectionConsoleDP {
    public:
        /**
         * webcam selection function
         * @return device index
         */
        static int ask();
    private:

    };
}

#endif

#endif	/* WEBCAMSELECTIONCONSOLEDP_H */

