# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
FROM ubuntu:latest

# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/vessel
WORKDIR /usr/src/vessel

# This command compiles your app using GCC, adjust for your source code
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y gcc g++ libpng16-16 libpng-dev libjpeg8 libjpeg8-dev libz3-4 libz3-dev libtiff5 libtiff5-dev gsl-bin libgsl-dev cmake
WORKDIR /usr/src/vessel/src
RUN cmake .
RUN make
RUN make install

# This command runs your application, comment out this line to compile only
CMD ["vessel help"]

LABEL Name=vessel Version=0.0.1
