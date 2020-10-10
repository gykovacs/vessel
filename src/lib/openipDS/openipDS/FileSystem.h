/**
 * @file FileSystem.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
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
 * This file contains some classes to represent elements of the file system.
 */

#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <string>

namespace openip
{
    class Directory;

    /**
     * File represents the path of a file.
     */
    class File: public std::string
    {
    public:
        /**
         * default constructor
         */
        File();

        /**
         * constructor
         * @param filename as char*
         */
        File(const char* filename);

        /**
         * constructor
         * @param s filename as string
         */
        File(std::string s);

        /**
         * copy constructor
         * @param f instance to copy
         */
        File(const File& f);

        /**
         * destructor
         */
        ~File();
        
        bool operator==(const File& f);

        /**
         * returns the parent directory
         * @return parent directory
         */
        Directory getParentDirectory();

        /**
         * returns the filename without path
         * @return filename without path
         */
        File getFilename();

        /**
         * returns the absolute filename with path s
         * @param s path string
         * @return absolute filename
         */
        std::string getAbsoluteFilename(std::string s);

        /**
         * returns the extension
         * @return file extension as string
         */
        std::string getExtension();
    };

    /**
     * Directory represents the path of a directory
     */
    class Directory: public std::string
    {
    public:
        /**
         * constructor
         * @param dirname directory as char*
         */
        Directory(const char* dirname);

        /**
         * constructor
         * @param s directory as string
         */
        Directory(std::string s);

        /**
         * copy constructor
         * @param d instance to copy
         */
        Directory(const Directory& d);

        /**
         * destructor
         */
        ~Directory();

        /**
         * returns the parent directory
         * @return parent directory
         */
        Directory getParentDirectory();

        /**
         * opens and reads the contents of the directory
         * @return non-zero in case of errors
         */
        int open();

        /**
         * closes the directory
         * @return non-zero in case of errors
         */
        int close();

        /**
         * returns non-zero if not all content have been read
         * @return non-zero until the next file/subdirectory is read
         */
        int hasNextFile();

        /**
         * returns the next file/subdirectory of the directory
         * @return the next file/subdirectory of the directory
         */
        File nextFile();

        /**
         * DIR directory object from dirent.h
         */
        DIR* dir;

        /**
         * struct dirent object from dirent.h
         */
        struct dirent * dirp;
    };
}

#endif	/* FILESYSTEM_H */

