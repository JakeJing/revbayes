#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_CharacterDependentCladoBirthDeathProcess.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RateGenerator.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlDistributionMemberFunction.h"
#include "RlCladogeneticSpeciationRateMatrix.h"
#include "RlRateGenerator.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "StateDependentSpeciationExtinctionProcess.h"

using namespace RevLanguage;


Dist_CharacterDependentCladoBirthDeathProcess::Dist_CharacterDependentCladoBirthDeathProcess() : TypedDistribution<TimeTree>()
{
    
}


Dist_CharacterDependentCladoBirthDeathProcess::~Dist_CharacterDependentCladoBirthDeathProcess()
{
    
}



Dist_CharacterDependentCladoBirthDeathProcess* Dist_CharacterDependentCladoBirthDeathProcess::clone( void ) const
{
    
    return new Dist_CharacterDependentCladoBirthDeathProcess( *this );
}


RevBayesCore::TypedDistribution<RevBayesCore::Tree>* Dist_CharacterDependentCladoBirthDeathProcess::createDistribution( void ) const
{
    
    // Get the parameters
    RevBayesCore::TypedDagNode<double>* ra                              = static_cast<const RealPos &>( start_age->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ex     = static_cast<const ModelVector<Real> &>( extinction_rates->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* q          = static_cast<const RateGenerator &>( event_rate_matrix->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>*  r                              = static_cast<const RealPos &>( event_rate->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::Simplex>* rf               = static_cast<const Simplex &>( root_frequencies->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* rh                              = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    const std::string& cond                                             = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    
    // the start condition
    bool uo = ( start_condition == "originAge" ? true : false );

    RevBayesCore::StateDependentSpeciationExtinctionProcess*   d = new RevBayesCore::StateDependentSpeciationExtinctionProcess( ra, ex, q, r, rf, rh, cond, uo );
    
    // set the cladogenetic speciation rate event map
    RevBayesCore::TypedDagNode<RevBayesCore::CladogeneticSpeciationRateMatrix>* cp = static_cast<const CladogeneticSpeciationRateMatrix &>( cladoEvents->getRevObject() ).getDagNode();
    d->setCladogenesisMatrix( cp );
    
    // set the number of time slices for the numeric ODE
    double n = static_cast<const RealPos &>( num_time_slices->getRevObject() ).getValue();
    d->setNumberOfTimeSlices( n );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_CharacterDependentCladoBirthDeathProcess::getClassType(void)
{
    
    static std::string rev_type = "Dist_CharacterDependentCladoBirthDeathProcess";
    
    return rev_type;
}


/* Get class type spec describing type of object. TODO: Check if the correct parent is TypedDistribution or Distribution */
const TypeSpec& Dist_CharacterDependentCladoBirthDeathProcess::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_CharacterDependentCladoBirthDeathProcess::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "CDCladoBDP";
    
    return d_name;
}


MethodTable Dist_CharacterDependentCladoBirthDeathProcess::getDistributionMethods( void ) const
{
    
    MethodTable methods = TypedDistribution<TimeTree>::getDistributionMethods();
    
    ArgumentRules* clampCharDataArgRules = new ArgumentRules();
    clampCharDataArgRules->push_back( new ArgumentRule( "value", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "clampCharData", RlUtils::Void, clampCharDataArgRules ) );
    
    ArgumentRules* getCharDataArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "getCharData", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), getCharDataArgRules ) );
    
    return methods;
}



/* Return member rules */
const MemberRules& Dist_CharacterDependentCladoBirthDeathProcess::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        std::vector<std::string> aliases;
        aliases.push_back("rootAge");
        aliases.push_back("originAge");
        memberRules.push_back( new ArgumentRule( aliases, RealPos::getClassTypeSpec()    , "The start time of the process.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "cladoEventMap",     CladogeneticSpeciationRateMatrix::getClassTypeSpec(),           "The map of speciation rates to cladogenetic event types.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "extinctionRates",   ModelVector<Real>::getClassTypeSpec(),    "The vector of extinction rates for the observed states.",  ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "Q",                 RateGenerator::getClassTypeSpec(),        "The rate matrix of jumping between rate categories.",      ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "delta",             RealPos::getClassTypeSpec(),              "The rate-factor of jumping between rate categories.",      ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "pi",                Simplex::getClassTypeSpec(),              "State frequencies at the root.",                           ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "rho",               Probability::getClassTypeSpec(),          "The taxon sampling probability.",                          ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        //optionsCondition.push_back( "survival" );
        memberRules.push_back( new OptionRule( "condition",           new RlString("time"), optionsCondition,   "The condition of the birth-death process." ) );
        memberRules.push_back( new ArgumentRule( "nTimeSlices",       RealPos::getClassTypeSpec(),              "The number of time slices for the numeric ODE.",           ArgumentRule::BY_VALUE                , ArgumentRule::ANY, new RealPos(500.0) ) );
        
        rules_set = true;
    }
    
    return memberRules;
}


const TypeSpec& Dist_CharacterDependentCladoBirthDeathProcess::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
void Dist_CharacterDependentCladoBirthDeathProcess::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "rootAge" || name == "originAge" )
    {
        start_age = var;
        start_condition = name;
    }
    else if ( name == "cladoEventMap" )
    {
        cladoEvents = var;
    }
    else if ( name == "pi" )
    {
        root_frequencies = var;
    }
    else if ( name == "extinctionRates" )
    {
        extinction_rates = var;
    }
    else if ( name == "Q" )
    {
        event_rate_matrix = var;
    }
    else if ( name == "delta" )
    {
        event_rate = var;
    }
    else if ( name == "rho" )
    {
        rho = var;
    }
    else if ( name == "condition" )
    {
        condition = var;
    }
//    else if ( name == "taxa" )
 //   {
//        taxa = var;
//    }
   else if ( name == "nTimeSlices" )
    {
        num_time_slices = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}

