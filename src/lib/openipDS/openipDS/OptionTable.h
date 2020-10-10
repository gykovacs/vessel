/**
 * @file OptionTable.h
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
 * OptionTable is a simple tool in to process command line arguments
 */

#ifndef _OPTIONTABLE_H
#define	_OPTIONTABLE_H

#include <iostream>
#include <string>
#include <vector>

#include <openipDS/OpenIPObject.h>

namespace openip
{
    /**
     * Enum for possible Option types
     */
    typedef enum
    {
        OPTION_INT,
        OPTION_FLOAT,
        OPTION_CHAR,
        OPTION_DOUBLE,
        OPTION_BOOL,
        OPTION_HELP,
        OPTION_SEPARATOR,
        OPTION_LONGINT,
    } OptionType;

    /**
     *Option class specify a possible option in the command line argument vector.
     */
    class Option: public OpenIPObject
    {
        public:
            /**
             * Constructor of the Option class.
             * @param nameL option name
             * @param optionL type of the option
             * @param pL pointer to the option
             * @param nL number of parameters after the switch
             * @param descriptionL description of the option
             */
            Option(std::string nameL, OptionType optionL, char* pL, unsigned int nL, std::string descriptionL);

            virtual ~Option();
            
            /**
             * toString method to describe Option object
             * @return string descriptor
             */
            std::string toString() const;

            /**
             * pointer to the argument
             */
            char* p;

            /**
             * number of parameters following the switch in the command line
             */
            unsigned int n;

            /**
             * name of the option
             */
            std::string name;

            /**
             * type of the option
             */
            OptionType optionType;

            /**
             * description of the option
             */
            std::string description;
    };

    /**
     * OptionTable class to cover a set of Options.
     */
    class OptionTable: public std::vector<Option*>, public OpenIPObject
    {
    public:

        /**
         * Constructor for the option table.
         */
        OptionTable();

        OptionTable(const OptionTable& o);

        ~OptionTable();
        /**
         * Method for adding a new option to the table.
         * @param name option name
         * @param option option type
         * @param p option pointer
         * @param n number of parameters after the command line switch
         * @param description description of the option
         */
        void addOption(std::string name, OptionType option, char* p, unsigned int n, std::string description);

        /**
         * Method to process the command line arguments.
         * @param argc number of command line arguments
         * @param argv pointer to command line argument array
         */
        int processArgs(int* argc, char** argv);

        /**
         * dump method of the optionTable class.
         */
        void dump();

        /**
         * setting the prefix text.
         * @param prefixL table prefix text
         */
        void setPrefix(std::string prefixL);

        /**
         * setting the postfix text.
         * @param postfixL table postfix text
         */
        void setPostfix(std::string postfixL);
        
        void addUsage(std::string usecase);

        /**
         * toString method to describe OptionTable object
         * @return descriptor string
         */
        std::string toString() const;

        /**
         * prefix text to print out with the option table
         */
        std::string prefix;

        /**
         * postfix text to print out with the option table
         */
        std::string postfix;
        
                    
        std::vector<std::string> usages;
    };
}

#endif	/* _OPTIONTABLE_H */

