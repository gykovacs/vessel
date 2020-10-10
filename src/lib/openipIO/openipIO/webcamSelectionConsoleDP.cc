/* 
 * File:   WebCamSelectionConsoleDP.cc
 * Author: Iván János István
 * 
 * Created on 2010. augusztus 11., 11:59
 */

#ifdef USE_WEBCAM

#include <openipIO/webcamSelectionConsoleDP.h>
#include <stdio.h>

namespace openip 
{
    int WebCamSelectionConsoleDP::ask()
    {
        #ifdef WIN32
            char szDeviceName[40];
            char szDeviceVersion[40];
        #else
            struct v4l2_capability capability;
            char* dev_name;
            int fd= -1;
        #endif
        int index= 0;

        for ( ; index < 10; index++)
        {
            #ifdef WIN32
                if ( capGetDriverDescription( index,
                        (WCHAR*)szDeviceName, sizeof (szDeviceName),
                        (WCHAR*)szDeviceVersion, sizeof (szDeviceVersion)))
                {
                    printf("%d: %s %s\n", index + 1, szDeviceName, szDeviceVersion);
                }
            #else

                dev_name= (char*)malloc( sizeof( char ) * 20);
                sprintf(dev_name, "/dev/video%d", index);

                fd= v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);

                memset (&capability, 0, sizeof (capability));
                if (-1 != ioctl (fd, VIDIOC_QUERYCAP, &capability))
                {
                    printf ("%d: %s %s\n", index + 1, capability.card , capability.bus_info);
                    v4l2_close(fd);
                }
            #endif
        }

        int res= scanf("%d", &index);

        if (res != 1 || index < 0 || index > 10 )
            return -1;

        return index - 1;
    }
}

#endif