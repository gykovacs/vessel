#include <openipDS/ParameterFile.h>

namespace openip
{
    Parameter::Parameter()
    : parameterName(""), parameterType(PARAMETER_BOOL), numberOfParameters(0)
    {
    }

    Parameter::Parameter(const Parameter& p)
    : parameterName(p.parameterName), parameterType(p.parameterType), numberOfParameters(p.numberOfParameters)
    {
    }

    Parameter::Parameter(std::string pname, ParameterType ptype, int numberofp)
    : parameterName(pname), parameterType(ptype), numberOfParameters(numberofp)
    {
    }

    Parameter::~Parameter()
    {
    }

    ParameterFile::ParameterFile()
    : std::vector<Parameter>()
    {
    }

    ParameterFile::ParameterFile(const ParameterFile& pf)
    : std::vector<Parameter>(pf)
    {
        this->filename= pf.filename;
        file.open(filename.c_str(), std::ios::in);
    }

    ParameterFile::ParameterFile(std::string filename)
    {
        this->filename= filename;
        file.open(filename.c_str(), std::ios::in);
	if ( !file.good() )
	  fprintf(stderr, "file is not good\n");
    }

    ParameterFile::~ParameterFile()
    {
        //file.close();
    }

    void ParameterFile::reopen()
    {
        file.close();
        file.open(filename.c_str(), std::ios::in);
    }

    void ParameterFile::close()
    {
        file.close();
    }

    void ParameterFile::add(Parameter p)
    {
        this->push_back(p);
    }

    void ParameterFile::add(std::string parameterName, ParameterType pt, int numberOfParameters)
    {
        this->push_back(Parameter(parameterName, pt, numberOfParameters));
    }

    int ParameterFile::next(std::string pattern)
    {
        if ( ! file.good() )
            return 0;

        token= std::string("");
        while ( token.compare(pattern) != 0 && file.good() )
        {
            this->next();
        }

        if ( file.good() )
            return 1;
        return 0;
    }

    int ParameterFile::next()
    {
        char c;
        token= std::string("");

        while ( 1 )
        {
            if ( ! file.good() )
                return 0;
            while ( file.good() )
            {
                c= file.get();
                if ( c == '#' )
                {
                    while ( file.get() != '\n' && file.good() );
                    continue;
                }
                else
                {
                    file.putback(c);
                    break;
                }
            }
            file >> token;
            for ( unsigned int i= 0; i < this->size(); ++i )
                if ( token.compare((*this)[i].parameterName) == 0 )
                {
                    tokenType= (*this)[i].parameterType;
                    if ( (*this)[i].numberOfParameters == 1 )
                    {
                        switch((*this)[i].parameterType)
                        {
                            case PARAMETER_INT:
                                file >> tokenInt;
                                break;
                            case PARAMETER_FLOAT:
                                file >> tokenFloat;
                                break;
                            case PARAMETER_DOUBLE:
                                file >> tokenDouble;
                                break;
                            case PARAMETER_BOOL:
                                file >> tokenBool;
                                break;
                            case PARAMETER_STRING:
                                file >> tokenString;
                                break;
                            case PARAMETER_LINE:
                                char c;
                                file.get(c);
                                getline(file, tokenString);
                                break;
                        }
                        return 1;
                    }
                    else
                    {

                        for ( int k= 0; k < (*this)[i].numberOfParameters; ++k )
                        {
                            switch((*this)[i].parameterType)
                            {
                                case PARAMETER_INT:
                                {
                                    int x;
                                    file >> x;
                                    tokenVectorInt.push_back(x);
                                    break;
                                }
                                case PARAMETER_FLOAT:
                                {
                                    float xx;
                                    file >> xx;
                                    tokenVectorFloat.push_back(xx);
                                    break;
                                }
                                case PARAMETER_DOUBLE:
                                {
                                    double xxx;
                                    file >> xxx;
                                    tokenVectorDouble.push_back(xxx);
                                    break;
                                }
                                case PARAMETER_BOOL:
                                {
                                    bool xxxx;
                                    file >> xxxx;
                                    tokenVectorBool.push_back(xxxx);
                                    break;
                                }
                                case PARAMETER_STRING:
                                {
                                    std::string yy;
                                    file >> yy;
                                    tokenVectorString.push_back(yy);
                                    break;
                                }
                                case PARAMETER_LINE:
                                {
                                    std::string y;
                                    char c;
                                    file.get(c);
                                    getline(file, y);
                                    tokenVectorString.push_back(y);
                                    break;
                                }
                            }
                        }
                    }
                    return 1;
                }
        }
    }

    DatabaseDescriptorParameterFile::DatabaseDescriptorParameterFile(std::string filename)
    : ParameterFile(filename)
    {
        this->add("class", PARAMETER_STRING, 1);
        this->add("label", PARAMETER_INT, 1);
        this->add("image", PARAMETER_STRING, 1);
        this->add("mask", PARAMETER_STRING, 1);
        this->add("roi", PARAMETER_STRING, 1);
        this->add("support", PARAMETER_STRING, 1);
    }

    DatabaseDescriptorParameterFile::DatabaseDescriptorParameterFile(const DatabaseDescriptorParameterFile& d)
    : ParameterFile(d)
    {
    }

    DatabaseDescriptorParameterFile::~DatabaseDescriptorParameterFile()
    {
    }
    
    RegionDatabaseDescriptorParameterFile::RegionDatabaseDescriptorParameterFile(std::string filename)
    : ParameterFile(filename)
    {
        this->add("class", PARAMETER_STRING, 1);
        this->add("label", PARAMETER_INT, 1);
        this->add("image", PARAMETER_STRING, 1);
        this->add("mask", PARAMETER_STRING, 1);
        this->add("region", PARAMETER_STRING, 1);
    }

    RegionDatabaseDescriptorParameterFile::RegionDatabaseDescriptorParameterFile(const RegionDatabaseDescriptorParameterFile& d)
    : ParameterFile(d)
    {
    }

    RegionDatabaseDescriptorParameterFile::~RegionDatabaseDescriptorParameterFile()
    {
    }

    std::ostream& operator<<(std::ostream& o, const ParameterFile&)
    {
        o << "";

        return o;
    }

    FeatureDescriptorParameterFile::FeatureDescriptorParameterFile(std::string filename)
    : ParameterFile(filename)
    {
        this->add("preprocessing", PARAMETER_LINE, 1);
        this->add("feature", PARAMETER_LINE, 1);
    }

    FeatureDescriptorParameterFile::FeatureDescriptorParameterFile(const FeatureDescriptorParameterFile& d)
    : ParameterFile(d)
    {
    }

    FeatureDescriptorParameterFile::~FeatureDescriptorParameterFile()
    {
    }

    BestFeatureDescriptorParameterFile::BestFeatureDescriptorParameterFile(std::string filename)
    : ParameterFile(filename)
    {
        this->add("preprocessing", PARAMETER_LINE, 1);
        this->add("feature", PARAMETER_LINE, 1);
        this->add("correlation", PARAMETER_FLOAT, 1);
    }

    BestFeatureDescriptorParameterFile::BestFeatureDescriptorParameterFile(const BestFeatureDescriptorParameterFile& d)
    : ParameterFile(d)
    {
    }

    BestFeatureDescriptorParameterFile::~BestFeatureDescriptorParameterFile()
    {
    }

    ValidationDescriptorParameterFile::ValidationDescriptorParameterFile(std::string filename)
    : ParameterFile(filename)
    {
        this->add("test", PARAMETER_STRING, 1);
        this->add("etalon", PARAMETER_STRING, 1);
    }

    ValidationDescriptorParameterFile::ValidationDescriptorParameterFile(const ValidationDescriptorParameterFile& d)
    : ParameterFile(d)
    {
    }

    ValidationDescriptorParameterFile::~ValidationDescriptorParameterFile()
    {
    }

    ValidatorDescriptorParameterFile::ValidatorDescriptorParameterFile(std::string filename)
    : ParameterFile(filename)
    {
        this->add("mode", PARAMETER_LINE, 1);
        this->add("classifier", PARAMETER_LINE, 1);
        this->add("validator", PARAMETER_LINE, 1);
        this->add("feature", PARAMETER_LINE, 1);
    }

    ValidatorDescriptorParameterFile::ValidatorDescriptorParameterFile(const ValidatorDescriptorParameterFile& d)
    : ParameterFile(d)
    {
    }

    ValidatorDescriptorParameterFile::~ValidatorDescriptorParameterFile()
    {
    }
}
