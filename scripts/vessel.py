#! /usr/bin/python

import os
import ntpath
import sys
import shutil

scriptpath= os.path.dirname(os.path.realpath(__file__))

# DRIVE model parameters
DRIVEMODEL="%s/../share/vessel/drive-model/" % scriptpath
DRIVEROI="267"

ST2DMAXIT="36"
ST2DNW="3.145"
ST2DDYN="10.5"
ST4DWA="8.32"
ST4DSTH0="19"
ST4DAP="0.529"

# STARE model parameters
STAREMODEL="%s/../share/vessel/stare-model/" % scriptpath
STAREROI="326"

ST2SMAXIT="42"
ST2SNW="3.12"
ST2SDYN="10"
ST4SWA="10.57"
ST4SSTH0="42"
ST4SAP="0.592"

def simpleSegmentation(inp, outp, model, st2maxit, st2nw, st2dyn, st4wa, st4sth0, st4ap):
    command= "vessel --vessel.stage0 %s roi-s1.bmp extended.bmp support.bmp roi.bmp" % inp
    print("command to execute: %s" % command)
    os.system(command)
    
    command= "vessel --vessel.stage1 %s/trained-features.fdf extended.bmp roi-s1.bmp support.bmp stage1.bmp" % model
    print("command to execute: %s" % command)
    os.system(command)
    
    command= "vessel --vessel.stage2 --vessel.stage2.ws 1 --vessel.stage2.shift 1 --vessel.stage2.maxit %s --vessel.stage2.nw %s --vessel.stage2.dynth %s --vessel.stage2.relint %s extended.bmp stage1.bmp roi-s1.bmp stage2.bmp" % (st2maxit, st2nw, st2dyn, model)
    print("command to execute: %s" % command)
    os.system(command)
    
    command= "vessel --vessel.stage4 %s/trained-features.fdf --vessel.stage4.wa %s --vessel.stage4.sizeth0 %s --vessel.stage4.ap %s extended.bmp stage2.bmp roi-s1.bmp support.bmp stage4.bmp" % (model, st4wa, st4sth0, st4ap)
    print("command to execute: %s" % command)
    os.system(command)
    
    shutil.copyfile("stage4.bmp", outp)

def driveFunction(inp, outp):
    simpleSegmentation(inp, outp, DRIVEMODEL, ST2DMAXIT, ST2DNW, ST2DDYN, ST4DWA, ST4DSTH0, ST4DAP)

def stareFunction(inp, outp):
    simpleSegmentation(inp, outp, STAREMODEL, ST2SMAXIT, ST2SNW, ST2SDYN, ST4SWA, ST4SSTH0, ST4SAP)

def scaleFunction(images, roi, model):
    scales="scales.txt"
    
    for im in images:
        name= os.path.basename(im)
        id= os.path.splitext(name)[0]
        
        command= "vessel --vessel.stage0 %s roi-s1-%s.bmp extended-%s.bmp support-%s.bmp roi-%s.bmp" % (im, id, id, id, id)
        print("command to execute: %s" % command)
        os.system(command)
        
        command= "vessel --vessel.stage1 --vessel.stage1.roiradius %s %s/trained-features.fdf extended-%s.bmp roi-s1-%s.bmp support-%s.bmp stage1-%s.bmp --unknown 1 2>> %s" % (roi, model, id, id, id, id, scales)
        print("command to execute: %s" % command)
        os.system(command)

    scale= os.popen("vessel --gmean %s" % scales).read()
    
    print("IMGSCALE: %s" % scale)
    
    f= open('imgscale.txt', 'w')
    MULT= f.write(scale);
    f.close();
    
def scaleDriveFunction(images):
    scaleFunction(images, DRIVEROI, DRIVEMODEL)
    
def scaleStareFunction(images):
    scaleFunction(images, STAREROI, STAREMODEL)

def cnFunction(inp, imgscale, outp, model, roi, st2maxit, st2nw, st2dyn, st4wa, st4sth0, st4ap):
    command= "vessel --vessel.stage0 %s roi-s1.bmp extended.bmp support.bmp roi.bmp" % inp
    print("command to execute: %s" % command)
    os.system(command)

    command= "vessel --vessel.stage1 --vessel.stage1.roiradius %s --vessel.stage1.imgScale %s %s/trained-features.fdf extended.bmp roi-s1.bmp support.bmp stage1.bmp --unknown 0 --vessel.stage1.th1mult 1 --vessel.stage1.th2mult 1" % (roi, imgscale, model)
    print("command to execute: %s" % command)
    os.system(command)

    command= "vessel --vessel.stage2 --vessel.stage1.roiradius %s --vessel.stage2.ws %s --vessel.stage2.dynth %s --vessel.stage2.relint %s --vessel.stage2.nw %s --vessel.stage2.maxit %s extended.bmp stage1.bmp roi.bmp stage2.bmp --unknown 0" % (roi, imgscale, st2dyn, model, st2nw, st2maxit)
    print("command to execute: %s" % command)
    os.system(command)

    MULT=1
    command= "vessel --unknown 0 --vessel.stage4 --vessel.stage1.roiradius %s --vessel.stage4.ws %s %s/trained-features.fdf extended.bmp stage2.bmp roi.bmp support.bmp stage4.bmp --vessel.stage4.sizeth0 %s --vessel.stage4.ap %s --vessel.stage1.th1mult %s --vessel.stage1.th2mult %s --vessel.stage4.wa %s" % (roi, imgscale, model, st4sth0, st4ap, MULT, MULT, st4wa)
    print("command to execute: %s" % command)
    os.system(command)
    
    shutil.copyfile("stage4.bmp", outp)
    
def driveCNFunction(inp, imgscale, outp):
    cnFunction(inp, imgscale, outp, DRIVEMODEL, DRIVEROI, ST2DMAXIT, ST2DNW, ST2DDYN, ST4DWA, ST4DSTH0, ST4DAP)
    
def stareCNFunction(inp, imgscale, outp):
    cnFunction(inp, imgscale, outp, STAREMODEL, STAREROI, ST2SMAXIT, ST2SNW, ST2SDYN, ST4SWA, ST4SSTH0, ST4SAP)    
    
def caFunction(inp, imgscale, outp, model, roi, st2maxit, st2nw, st2dyn, st4wa, st4sth0, st4ap):
    command= "vessel --vessel.stage0 %s roi-s1.bmp extended.bmp support.bmp roi.bmp" % inp
    print("command to execute: %s" % command)
    os.system(command)

    command= "vessel --vessel.stage1 --vessel.stage1.roiradius %s --vessel.stage1.imgScale %s %s/trained-features.fdf extended.bmp roi-s1.bmp support.bmp stage1.bmp --unknown 2 --vessel.stage1.th1mult 1 --vessel.stage1.th2mult 1" % (roi, imgscale, model)
    print("command to execute: %s" % command)
    os.system()
    
    command= "vessel --vessel.stage2 --vessel.stage1.roiradius %s --vessel.stage2.ws %s --vessel.stage2.dynth %s --vessel.stage2.relint %s --vessel.stage2.nw %s --vessel.stage2.maxit %s extended.bmp stage1.bmp roi.bmp stage2.bmp --unknown 2" % (roi, imgscale, st2dyn, model, st2nw, st2maxit)
    print("command to execute: %s" % command)
    os.system(command)
    
    f= open('multiplier.txt', 'r')
    MULT= f.read()
    f.close()
    
    command= "vessel --unknown 1 --vessel.stage4 --vessel.stage1.roiradius %s --vessel.stage4.ws %s %s/trained-features.fdf extended.bmp stage2.bmp roi.bmp support.bmp stage4.bmp --vessel.stage4.sizeth0 %s --vessel.stage4.ap %s --vessel.stage1.th1mult %s --vessel.stage1.th2mult %s --vessel.stage4.wa %s" % (roi, imgscale, model, st4sth0, st4ap, MULT, MULT, st4wa)
    print("command to execute: %s" % command)
    os.system()
    
    shutil.copyfile("stage4.bmp", outp)
    
def driveCAFunction(inp, imgscale, outp):
    caFunction(inp, imgscale, outp, DRIVEMODEL, DRIVEROI, ST2DMAXIT, ST2DNW, ST2DDYN, ST4DWA, ST4DSTH0, ST4DAP)

def stareCAFunction(inp, imgscale, outp):
    caFunction(inp, imgscale, outp, STAREMODEL, STAREROI, ST2SMAXIT, ST2SNW, ST2SDYN, ST4SWA, ST4SSTH0, ST4SAP)

def helpFunction():
    print("Usages:")
    print("vessel.py drive <input> <output>")
    print("vessel.py stare <input> <output>")
    print("vessel.py drive-scale <image0> [<image1>]...")
    print("vessel.py stare-scale <image0> [<image1>]...")
    print("vessel.py drive-cn <input> <scale> <output>")
    print("vessel.py stare-cn <input> <scale> <output>")
    print("vessel.py drive-ca <input> <scale> <output>")
    print("vessel.py stare-ca <input> <scale> <output>")
    print("vessel.py help")
    
files_before= os.listdir(".")

outp=""

if len(sys.argv) < 2:
    helpFunction()
elif sys.argv[1] == "drive":
    outp= sys.argv[3]
    driveFunction(sys.argv[2], sys.argv[3])
elif sys.argv[1] == "stare":
    outp= sys.argv[3]
    stareFunction(sys.argv[2], sys.argv[3])
elif sys.argv[1] == "drive-scale":
    scaleDriveFunction(sys.argv[2:len(sys.argv)])
elif sys.argv[1] == "stare-scale":
    scaleStareFunction(sys.argv[2:len(sys.argv)])
elif sys.argv[1] == "drive-cn":
    outp= sys.argv[4]
    imgscale= sys.argv[3]
    if sys.argv[3] == "imgscale.txt":
        f= open('imgscale.txt', 'r')
        imgscale= f.read()
        f.close()
    driveCNFunction(sys.argv[2], imgscale, sys.argv[4])
elif sys.argv[1] == "stare-cn":
    outp= sys.argv[4]
    imgscale= sys.argv[3]
    if sys.argv[3] == "imgscale.txt":
        f= open('imgscale.txt', 'r')
        imgscale= f.read()
        f.close()
    stareCNFunction(sys.argv[2], imgscale, sys.argv[4])
elif sys.argv[1] == "drive-ca":
    outp= sys.argv[4]
    imgscale= sys.argv[3]
    if sys.argv[3] == "imgscale.txt":
        f= open('imgscale.txt', 'r')
        imgscale= f.read()
        f.close()
    driveCAFunction(sys.argv[2], imgscale, sys.argv[4])
elif sys.argv[1] == "stare-ca":
    outp= sys.argv[4]
    imgscale= sys.argv[3]
    if sys.argv[3] == "imgscale.txt":
        f= open('imgscale.txt', 'r')
        imgscale= f.read()
        f.close()
    stareCAFunction(sys.argv[2], imgscale, sys.argv[4])
elif sys.argv[1] == "help":
    helpFunction()
else:
    helpFunction()

files_after= os.listdir(".")

for f in files_after:
    if (not (f in files_before)) and (f != outp) and (f != 'imgscale.txt':
        os.remove(f)
