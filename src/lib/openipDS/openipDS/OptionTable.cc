#ifndef _OPTIONTABLE_CC
#define	_OPTIONTABLE_CC

#include <openipDS/OptionTable.h>
#include <sstream>
#include <cstring>

openip::Option::Option(std::string nameL, OptionType optionL, char* pL, unsigned int nL, std::string descriptionL)
{
    p= pL;
    n= nL;
    name= nameL;
    optionType= optionL;
    description= descriptionL;
    /*switch (optionType) {
        case OPTION_INT:
            *((int *) p) = 0;
            break;
        case OPTION_FLOAT:
            *((float *) p) = 0.0f;
            break;
        case OPTION_CHAR:
            *((char *) p) = 0;
            break;
        case OPTION_DOUBLE:
            *((double *) p) = 0.0;
            break;
        case OPTION_BOOL:
            *((bool *)p) = false;
            break;
        case OPTION_HELP:
            break;
    }*/
}

openip::Option::~Option()
{
}

std::string openip::Option::toString() const
{
    char tmp[name.size() + description.size() + 100];

    switch (optionType)
    {
        case OPTION_INT:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %d)", name.c_str(), n, "int", description.c_str(), *(int*)p);
            break;
        case OPTION_LONGINT:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %ld)", name.c_str(), n, "long int", description.c_str(), *(long int*)p);
            break;
        case OPTION_FLOAT:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %.2f)", name.c_str(), n, "float", description.c_str(), *(float*)p);
            break;
        case OPTION_DOUBLE:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %.2f)", name.c_str(), n, "double", description.c_str(), *(double*)p);
            break;
        case OPTION_BOOL:
            sprintf(tmp, "\t%-20s%-4d%-8s%s", name.c_str(), n, "bool", description.c_str());
            break;
        case OPTION_CHAR:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %s)", name.c_str(), n, "char", description.c_str(), (char*)p);
            break;
        case OPTION_HELP:
            sprintf(tmp, "\n\t%s\n", description.c_str());
            break;
        case OPTION_SEPARATOR:
            sprintf(tmp, "\n\t%s", description.c_str());
    }

    return std::string(tmp);
}

openip::OptionTable::OptionTable()
: std::vector<Option*>()
{
    prefix= std::string("");
    postfix= std::string("");
}

openip::OptionTable::OptionTable(const openip::OptionTable& o)
: std::vector<Option*>(o)
{
}

openip::OptionTable::~OptionTable()
{
    for ( unsigned int i= 0; i < this->size(); ++i )
        delete (*this)[i];
}

void openip::OptionTable::addOption(std::string name, OptionType option, char* p, unsigned int n, std::string description)
{
    this->push_back(new Option(name, option, p, n, description));
}

int openip::OptionTable::processArgs(int* argc, char** argv)
{
    std::vector<Option*>::iterator it= this->begin();
    unsigned int i, j;

    for (i = 0; (int)i < *argc; ++i)
    {
        if (std::strcmp(argv[i], "--help") == 0)
        {
            dump();
            return 1;
        }
    }

    while (it != this->end())
    {
        Option *o = *it;
        for (i = 0; (int)i < *argc; ++i)
        {
            if (std::strcmp(argv[i], o->name.c_str()) == 0 && o->optionType != OPTION_SEPARATOR)
            {
                for (j = 0; j <= o->n; ++j)
                    if (i + j <= (unsigned int)(*argc))
                        switch (o->optionType)
                        {
                            case OPTION_LONGINT:
                                if (j != 0)
                                    sscanf(argv[i + j], "%ld", ((long int *) (o->p)) + j - 1);
                                break;
                            case OPTION_INT:
                                if (j != 0)
                                    sscanf(argv[i + j], "%d", ((int *) (o->p)) + j - 1);
                                break;
                            case OPTION_FLOAT:
                                if (j != 0)
                                    sscanf(argv[i + j], "%f", ((float *) (o->p)) + j - 1);
                                break;
                            case OPTION_CHAR:
                                if (j != 0)
                                    sscanf(argv[i + j], "%s", ((char *) (o->p)) + j - 1);
                                break;
                            case OPTION_BOOL:
                                *((bool *)(o->p)) = true;
                                break;
                            case OPTION_DOUBLE:
                                if (j != 0) {
                                    double d;
                                    sscanf(argv[i + j], "%lf", &d);
                                    *(((double *) (o->p) + j - 1)) = d;
                                }
                                break;
                            case OPTION_HELP:
                                break;
                            case OPTION_SEPARATOR:
                                break;
                        };

                for (unsigned int k = i; k < *argc - j; ++k)
                {
                    argv[k] = argv[k + j];
                }
                *argc -= j;
                break;
            }
        }
        ++it;
    }
    
    for ( int i= 1; i < *argc; ++i )
        if ( argv[i][0] == '-' && ( argv[i][1] == '-' || argv[i][1] < '0' || argv[i][1] > '9' )  )
        {
            tprintf("Error: unknown argument %s\n", argv[i]);
            return 1;
        }
    
    return 0;
}

std::string openip::OptionTable::toString() const
{
    std::stringstream ss;

    ss << prefix;
    ss << std::endl;
    ss << std::endl;

    ss << "Parameters (parameter name, number of arguments, type of arguments, description):";
    ss << std::endl;

    for (std::vector<Option*>::const_iterator it= this->begin(); it != this->end(); ++it )
    {
        ss << "\t" << (**it);
        ss << std::endl;
    }

    ss << std::endl;
    ss << std::string("\tUsages") << std::endl;
    for ( std::vector<std::string>::const_iterator it= usages.begin(); it != usages.end(); ++it)
    {
        ss << "\t\t" << (*it);
        ss << std::endl;
    }
    ss << std::endl;
    if ( postfix.compare(std::string("")) != 0 )
    {
        ss << postfix;
        ss << std::endl;
    }

    return ss.str();
}

void openip::OptionTable::dump()
{
    cout << this->toString();
}

void openip::OptionTable::setPrefix(std::string prefixL)
{
    prefix= prefixL;
}


void openip::OptionTable::setPostfix(std::string postfixL)
{
    postfix= postfixL;
}

void openip::OptionTable::addUsage(std::string usecase)
{
    usages.push_back(usecase);
}

#endif	/* _OPTIONTABLE_CC */

