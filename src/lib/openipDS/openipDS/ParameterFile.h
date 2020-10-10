/**
 * @file ParameterFile.h
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
 * ParameterFile represents a simple class for files containing parameters.
 */

#ifndef _PARAMETERFILE_H
#define	_PARAMETERFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace openip
{
    /**
     * parameter types
     */
    typedef enum
    {
        PARAMETER_INT,
        PARAMETER_FLOAT,
        PARAMETER_DOUBLE,
        PARAMETER_BOOL,
        PARAMETER_STRING,
        PARAMETER_LINE,
    } ParameterType;

    /**
     * Parameter represents one possible parameter in the file
     */
    class Parameter
    {
    public:
        /**
         * default constructor
         */
        Parameter();

        /**
         * copy constructor
         * @param p instance to copy
         */
        Parameter(const Parameter& p);

        /**
         * constructor
         * @param parameterName the identifier of the parameter
         * @param pt the type of the parameter
         * @param numberOfParameters the number of arguments of pt type following the parameterName identifier
         */
        Parameter(std::string parameterName, ParameterType pt, int numberOfParameters);

        /**
         * destructor
         */
        ~Parameter();

        /** parameter name */
        std::string parameterName;
        /** parameter type */
        ParameterType parameterType;
        /** number of parameters */
        int numberOfParameters;
    };

    /**
     * ParameterFile represents a simple set of Parameter objects
     */
    class ParameterFile: public std::vector<Parameter>
    {
    public:
        /**
         * default constructor
         */
        ParameterFile();

        /**
         * copy constructor
         * @param pf instance to copy
         */
        ParameterFile(const ParameterFile& pf);

        /**
         * constructor
         * @param filename name of the file containing parameters
         */
        ParameterFile(std::string filename);

        /**
         * destructor
         */
        ~ParameterFile();

        /**
         * adds a parameter to the set
         * @param pt the parameter to add to the set
         */
        void add(Parameter pt);

        /**
         * adds a parameter to the set
         * @param parameterName name of the parameter
         * @param pt type of the parameter
         * @param numberOfParameters number of parameters of type pt following the parameterName identifier
         */
        void add(std::string parameterName, ParameterType pt, int numberOfParameters);

        /**
         * extracting the next parameter from the file, with identifier parameterName
         * @param parameterName the next parameter with identifier parameterName will be extracted
         * @return non-zero, if the extraction was successful, zero, if no more parameters with parameterName identifier can be extracted
         */
        int next(std::string parameterName);

        /**
         * extracts the next parameter having the identifier in the set
         * @return non-zero, if successful, zero, if no more parameters can be extracted with identifier in the set
         */
        int next();

        /**
         * reopens the file
         */
        void reopen();

        /**
         * closes the file
         */
        void close();

        /**
         * the identifier of the parameter extracted
         */
        std::string token;
        /**
         * the type of the parameter extracted
         */
        ParameterType tokenType;

        /** the extracted int parameter */
        int tokenInt;
        /** the extracted float parameter */
        float tokenFloat;
        /** the extracted string or line parameter */
        std::string tokenString;
        /** the extracted double parameter */
        double tokenDouble;
        /** the extracted bool parameter */
        bool tokenBool;

        /** the extracted int parameter vector */
        std::vector<int> tokenVectorInt;
        /** the extracted float parameter vector */
        std::vector<float> tokenVectorFloat;
        /** the extracted string parameter vector */
        std::vector<std::string> tokenVectorString;
        /** the extracted double parameter vector */
        std::vector<double> tokenVectorDouble;
        /** the extracted bool parameter vector */
        std::vector<bool> tokenVectorBool;

        /** input file stream */
        std::ifstream file;
        /** filename */
        std::string filename;
    };

    /**
     * Image database descriptor parameter file
     */
    class DatabaseDescriptorParameterFile: public ParameterFile
    {
    public:
        /**
         * constructor
         * @param filename name of the file
         */
        DatabaseDescriptorParameterFile(std::string filename);

        /**
         * copy constructor
         * @param ddpf instance to copy
         */
        DatabaseDescriptorParameterFile(const DatabaseDescriptorParameterFile& ddpf);

        /**
         * destructor
         */
        ~DatabaseDescriptorParameterFile();
    };
    
    /**
     * Image database descriptor parameter file
     */
    class RegionDatabaseDescriptorParameterFile: public ParameterFile
    {
    public:
        /**
         * constructor
         * @param filename name of the file
         */
        RegionDatabaseDescriptorParameterFile(std::string filename);

        /**
         * copy constructor
         * @param ddpf instance to copy
         */
        RegionDatabaseDescriptorParameterFile(const RegionDatabaseDescriptorParameterFile& ddpf);

        /**
         * destructor
         */
        ~RegionDatabaseDescriptorParameterFile();
    };

    /**
     * Feature descriptor parameter file
     */
    class FeatureDescriptorParameterFile: public ParameterFile
    {
    public:
        /**
         * constructor
         * @param filename name of the file
         */
        FeatureDescriptorParameterFile(std::string filename);

        /**
         * copy constructor
         * @param ddpf parameter to copy
         */
        FeatureDescriptorParameterFile(const FeatureDescriptorParameterFile& ddpf);

        /**
         * destructor
         */
        ~FeatureDescriptorParameterFile();
    };

    class BestFeatureDescriptorParameterFile: public ParameterFile
    {
    public:
        /**
         * constructor
         * @param filename name of the file
         */
        BestFeatureDescriptorParameterFile(std::string filename);

        /**
         * copy constructor
         * @param ddpf parameter to copy
         */
        BestFeatureDescriptorParameterFile(const BestFeatureDescriptorParameterFile& ddpf);

        /**
         * destructor
         */
        ~BestFeatureDescriptorParameterFile();
    };

    /**
     * Validation descriptor parameter file
     */
    class ValidationDescriptorParameterFile: public ParameterFile
    {
    public:
        /**
         * constructor
         * @param filename name of the file
         */
        ValidationDescriptorParameterFile(std::string filename);

        /**
         * copy constructor
         * @param vdpf instance to copy
         */
        ValidationDescriptorParameterFile(const ValidationDescriptorParameterFile& vdpf);

        /**
         * destructor
         */
        ~ValidationDescriptorParameterFile();
    };

    /**
     * Validation descriptor parameter file
     */
    class ValidatorDescriptorParameterFile: public ParameterFile
    {
    public:
        /**
         * constructor
         * @param filename name of the file
         */
        ValidatorDescriptorParameterFile(std::string filename);

        /**
         * copy constructor
         * @param vdpf instance to copy
         */
        ValidatorDescriptorParameterFile(const ValidatorDescriptorParameterFile& vdpf);

        /**
         * destructor
         */
        ~ValidatorDescriptorParameterFile();
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param p ParameterFile object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    //std::ostream& operator<<(std::ostream& o, const ParameterFile& p);
}

#endif	/* _PARAMETERFILE_H */

