=================
= vessel MANUAL =
=================

The package contains the implementation of the vessel segmentation method described in the paper:

Gy. Kovacs and A. Hajdu. "A Self-Calibrating Approach for the Segmentation of Retinal Vessels
by Template Matching and Contour Reconstruction". In: Medical Image Analysis 29(4) (2016).
pp. 24-46. DOI: 10.1016/j.media.2015.12.003

If you use the method, please cite the paper.

Bibtex:

@article{DBLP:journals/mia/KovacsH16,
  author    = {Gy{\"{o}}rgy Kov{\'{a}}cs and
               Andr{\'{a}}s Hajdu},
  title     = {A self-calibrating approach for the segmentation of retinal vessels
               by template matching and contour reconstruction},
  journal   = {Medical Image Analysis},
  volume    = {29},
  pages     = {24--46},
  year      = {2016},
  url       = {http://dx.doi.org/10.1016/j.media.2015.12.003},
  doi       = {10.1016/j.media.2015.12.003},
  timestamp = {Tue, 01 Mar 2016 11:42:40 +0100},
  biburl    = {http://dblp.uni-trier.de/rec/bib/journals/mia/KovacsH16},
  bibsource = {dblp computer science bibliography, http://dblp.org}
}

The software was tested on Ubuntu 16.10 and Windows 10.

================
= PREREQUISITS =
================

Linux (Ubuntu):
    1) libpng12
    2) libz
    3) libjpeg8
    4) libtiff5
    5) gsl-bin
    6) python3

Windows:
    1) libpng (http://gnuwin32.sourceforge.net/packages/libpng.htm)
    2) libz (http://gnuwin32.sourceforge.net/packages/zlib.htm)
    3) libjpeg (http://gnuwin32.sourceforge.net/packages/jpeg.htm)
    4) libtiff (http://gnuwin32.sourceforge.net/packages/tiff.htm)
    5) gsl (http://gnuwin32.sourceforge.net/packages/gsl.htm)
    6) mingw32-pthreads-w32
    7) python 3.5 (https://www.python.org/downloads/windows/)

In order to get things work in Windows, it is highly recommended to set-up a working
MinGW environment (http://www.mingw.org/), and install the mingw32-pthreads-w32 in it using
the graphical installer. 

Be sure that all the dlls installed with the packages above and with MinGW are present
in Windows' PATH variable.

=========================
= FILES BEING INSTALLED =
=========================

The package installs:
    1) some dlls containing general image processing methods,
    2) an application called "vessel"/"vessel.exe", which contains the implementation
        of the segmentation method,
    3) a Python script which is intended to be used as the main interface of the method,
    4) the model descriptor files trained on the STARE and DRIVE databases.

=======================================
= CHECKING THAT THE ENVIRONMENT WORKS =
=======================================

If everything is installed and the PATH variables are set properly, opening a command line
the command

> vessel.py

should write the help and usages on the screen:

Usages:
vessel.py drive <input> <output>
vessel.py stare <input> <output>
vessel.py drive-scale <image0> [<image1>]...
vessel.py stare-scale <image0> [<image1>]...
vessel.py drive-cn <input> <scale> <output>
vessel.py stare-cn <input> <scale> <output>
vessel.py drive-ca <input> <scale> <output>
vessel.py stare-ca <input> <scale> <output>
vessel.py help

=============
= USE CASES =
=============

Generally, the algorithm writes temporary files to the directory in which it is
executed. It is highly recommended to create an empty directory and call the vessel.py
method in that empty directory. When the segmentation is finished, the vessel.py
scripts removes every file it created, so there is no need to worry about the disk
space.

Simple segmentation
*******************

In this case the models trained on DRIVE or STARE are used for segmentation.
This is useful only if the size of the image content to be segmented is similar to
that of the images in DRIVE or STARE. Using the DRIVE model:

> vessel.py drive dir1/<input> dir2/<output>

Note that the input and output images reside in another directory than the working
directory. 

Using the STARE model:

> vessel.py stare dir1/<input> dir2/<output>

This usecase is not the one which can be used in practice, since it gives the best results
only if the content of the images has the same scale as the training images of DRIVE or
STARE.

Estimating the scale
********************

In order to apply the method to an unseen input image, one has to estimate the scaling
factor between the image content of the training images and that of the unseen image.

This can be done manually or in an automated way, as well.

Coarse/manual estimations: a simple, coarse estimation of the scale factor can be achieved by
simply dividing the number of columns in the training images by the number of columns
in the unseen input image. For example, for the DRIVE model, one must divide the width
of the DRIVE training images (584 pixels) by the width of the image to be segmented.
Say, the image to be segmented has 3000 pixels in each row. Then, the scale factor is
determined as 584/3000 = 0.194. Another way is to divide the radius of the ROI in the
training images by the radius of ROI in the image to be segmented.

Clearly, this estimation has to be done only once for one source of images. However,
the coarse/manual estimations can be time consuming and erroneous as the ratio of
image widths may deviate from the real scale of the patterns (like the width of the
largest vessels) in the images. To overcome this issue, the proposed method can be
used to determine the right scaling factor automatedly.

Automated estimation: the method needs a couple of images from the same source and
tries to determine the scaling factor between the images of the training dataset and
that of the unseen images. The method writes the estimated scale on the console and
also into the file imgscale.txt. When the scaling factor for the DRIVE model is
determined:

> vessel.py drive-scale <image1> <image2> ... <imageN>

The proper number of images to be used is 5-10.

For the STARE model:

> vessel.py stare-scale <image1> <image2> ... <imageN>

Segmenting unseen images without further calibration
****************************************************

Once the scaling factor is estimated either manually or using the proposed automated
technique, the unseen image can be segmented by the following command using the DRIVE
model:

> vessel.py drive-cn <input> <scale> <output>

Or using the STARE model:

> vessel.py stare-cn <input> <scale> <output>

The postfix cn refers to "no calibration", in this case the automated calibration
techniques described in the paper are not applied. The segmentation is relatively
fast, but it is not the bast one achievable using the vessel package.

Segmenting unseen images with further calibration
*************************************************

Using the DRIVE model, one can carry out the segmentation with the blind calibration
techniques as

> vessel.py drive-ca <input> <scale> <output>

Or using the STARE model:

> vessel.py stare-ca <input> <scale> <output>

The postfix ca refers to "all calibrations".
