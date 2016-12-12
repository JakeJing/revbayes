#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DelimitedDataReader.h"
#include "HomologousDiscreteCharacterData.h"
#include "Ellipsis.h"
#include "Func_readDataDelimitedFile.h"
#include "ModelObject.h"
#include "ModelVector.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "Natural.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlContinuousCharacterData.h"
#include "RlMatrixReal.h"
#include "RlStandardState.h"
#include "RlString.h"
#include "StringUtilities.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_readDataDelimitedFile* Func_readDataDelimitedFile::clone( void ) const
{
    
    return new Func_readDataDelimitedFile( *this );
}


std::string Func_readDataDelimitedFile::bitToState(const std::string &s)
{
    
    std::stringstream ss;
    char* ptr;
    long parsed = strtol(s.c_str(), &ptr, 2);
    
    if (parsed > RbConstants::Integer::max)
    {
        throw RbException("ERROR: readTSVBitsetData token " + s + " too large to store as NaturalNumber");
    }
    
    if (s.find_first_not_of("01") != std::string::npos)
    {
        throw RbException("ERROR: readTSVBitsetData token " + s + " contains non-binary characters");
    }
    
    ss << parsed;
    
    return ss.str();
}


/** Execute function */
RevPtr<RevVariable> Func_readDataDelimitedFile::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string& fn  = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    bool header            = static_cast<const RlBoolean&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string& del = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    
    // get data from file
    RevBayesCore::DelimitedDataReader* tsv_data = new RevBayesCore::DelimitedDataReader(fn, del[0], header);
    const std::vector<std::vector<std::string> >&data = tsv_data->getChars();

    WorkspaceVector<WorkspaceVector<AbstractModelObject> > matrix;

    enum Datatype { UNBOUNDED   = 0b0001,
                    UNCOUNTABLE = 0b0010,
                    NUMERIC     = 0b0100,
                    ALPHA       = 0b1000,

                    NATURAL     = 0x4,
                    INTEGER     = 0x5,
                    REALPOS     = 0x6,
                    REAL        = 0x7,
                    STRING      = 0x8
                  };

    int matrix_type = 0;

    for (size_t i = 0; i < data.size(); ++i)
    {
        WorkspaceVector<AbstractModelObject> row;

        // make row
        for (size_t j= 0; j < data[i].size(); ++j)
        {
            int elemtype = 0;

            if( StringUtilities::isNumber(data[i][j]) )
            {
                elemtype |= NUMERIC;

                // integer
                if( StringUtilities::isIntegerNumber(data[i][j]) )
                {
                    int val = atoi(data[i][j].c_str() );

                    // negative integer
                    if( val < 0)
                    {
                        elemtype |= UNBOUNDED;
                        row.push_back( Integer( val ) );
                    }
                    // positive integer
                    else
                    {
                        row.push_back( Natural( val ) );
                    }
                }
                // real
                else
                {
                    elemtype |= UNCOUNTABLE;
                    double val = atof(data[i][j].c_str() );

                    // negative real
                    if( val < 0)
                    {
                        elemtype |= UNBOUNDED;
                        row.push_back( Real( val ) );
                    }
                    // positive real
                    else
                    {
                        row.push_back( RealPos( val ) );
                    }
                }
            }
            // string
            else
            {
                elemtype = STRING;
                row.push_back( RlString( data[i][j]) );
            }

            matrix_type = matrix_type | elemtype;
        }

        matrix.push_back(row);
    }

    if(matrix_type <= STRING)
    {
        if(matrix_type == REALPOS)
        {
            ModelVector<ModelVector<RealPos> > m;
            for(size_t i = 0; i<matrix.size(); i++)
            {
                ModelVector<RealPos> r;
                for(size_t j = 0; j < matrix[i].size(); j++)
                {
                    if( !matrix[i][j].isType(RealPos::getClassTypeSpec()) )
                    {
                        r.push_back( *(RealPos*)(matrix[i][j].convertTo(RealPos::getClassTypeSpec())) );
                    }
                    else
                    {
                        r.push_back(matrix[i][j]);
                    }
                }
                m.push_back(r);
            }
            return new RevVariable( new ModelVector<ModelVector<RealPos> >(m) );
        }
        else if(matrix_type == REAL)
        {
            ModelVector<ModelVector<Real> > m;
            for(size_t i = 0; i<matrix.size(); i++)
            {
                ModelVector<Real> r;
                for(size_t j = 0; j < matrix[i].size(); j++)
                {
                    if( !matrix[i][j].isType(Real::getClassTypeSpec()) )
                    {
                        r.push_back( *(Real*)(matrix[i][j].convertTo(Real::getClassTypeSpec())) );
                    }
                    else
                    {
                        r.push_back(matrix[i][j]);
                    }
                }
                m.push_back(r);
            }
            return new RevVariable( new ModelVector<ModelVector<Real> >(m) );
        }
        else if(matrix_type == NATURAL)
        {
            ModelVector<ModelVector<Natural> > m;
            for(size_t i = 0; i<matrix.size(); i++)
            {
                ModelVector<Natural> r;
                for(size_t j = 0; j < matrix[i].size(); j++)
                {
                    r.push_back(matrix[i][j]);
                }
                m.push_back(r);
            }
            return new RevVariable( new ModelVector<ModelVector<Natural> >(m) );
        }
        else if(matrix_type == INTEGER)
        {
            ModelVector<ModelVector<Integer> > m;
            for(size_t i = 0; i<matrix.size(); i++)
            {
                ModelVector<Integer> r;
                for(size_t j = 0; j < matrix[i].size(); j++)
                {
                    r.push_back(matrix[i][j]);
                }
                m.push_back(r);
            }
            return new RevVariable( new ModelVector<ModelVector<Integer> >(m) );
        }
        else if(matrix_type == STRING)
        {
            ModelVector<ModelVector<RlString> > m;
            for(size_t i = 0; i<matrix.size(); i++)
            {
                ModelVector<RlString> r;
                for(size_t j = 0; j < matrix[i].size(); j++)
                {
                    r.push_back(matrix[i][j]);
                }
                m.push_back(r);
            }
            return new RevVariable( new ModelVector<ModelVector<RlString> >(m) );
        }
    }

    return new RevVariable( new WorkspaceVector<WorkspaceVector<AbstractModelObject> >(matrix) );
}


/** Get argument rules */
const ArgumentRules& Func_readDataDelimitedFile::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "file",      RlString::getClassTypeSpec(), "The name of the file to read in.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "header", RlBoolean::getClassTypeSpec(), "Skip first line?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ));
        argumentRules.push_back( new ArgumentRule( "delimiter", RlString::getClassTypeSpec(), "The delimiter between columns.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString( "\t" ) ) );
        rules_set = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readDataDelimitedFile::getClassType(void)
{
    
    static std::string revType = "Func_readDataDelimitedFile";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readDataDelimitedFile::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readDataDelimitedFile::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readDataDelimitedFile";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readDataDelimitedFile::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readDataDelimitedFile::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = WorkspaceVector<WorkspaceVector<AbstractModelObject> >::getClassTypeSpec();
    return returnTypeSpec;
}




