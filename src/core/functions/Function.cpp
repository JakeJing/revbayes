#include "DagNode.h"
#include "Function.h"
#include "RbException.h"

using namespace RevBayesCore;



Function::Function(void)  :
    parameters()
{
    
}


Function::Function(const Function &f)  :
    parameters( f.parameters )
{
    
    for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
    {
        (*it)->incrementReferenceCount();
    }
    
}


Function::~Function( void )
{
    
    for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
    {
        const DagNode *theNode = *it;
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete theNode;
        }
        
    }
    
}



Function& Function::operator=(const Function &f)
{

    if ( this != &f )
    {
        
        for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
        {
            const DagNode *theNode = *it;
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
        }
        parameters.clear();
        
        parameters = f.parameters;
        
        for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
        {
            (*it)->incrementReferenceCount();
        }
        
    }
    
    return *this;
}



/**
 * Add this parameter to our set of parameters.
 */
void Function::addParameter(const DagNode *p)
{
    
    // only if the parameter is not NULL
    if ( p != NULL )
    {
        std::vector<const DagNode*>::iterator pos = std::find(parameters.begin(), parameters.end(), p);
        if ( pos == parameters.end() )
        {
            parameters.push_back( p );
            
            // increment reference count
            p->incrementReferenceCount();
            
        }
        
    }

}


void Function::getAffected(std::set<DagNode *> &affected, DagNode* affecter)
{
    
    // do nothing
}


/**
 * Get a const reference to the set of parameters for this function.
 */
const std::vector<const DagNode*>& Function::getParameters( void ) const
{
    
    return parameters;
}


/**
 * Method stumb that can be overwritten for specialized treatment.
 */
void Function::keep( DagNode* affecter )
{

}


/* Method stub: override for specialized treatment. */
void Function::reInitialized( void )
{
    // do nothing
}


/**
 * Remove this parameter from our list of parameters.
 */
void Function::removeParameter(const RevBayesCore::DagNode *p)
{
    
    // only if the parameter is not NULL
    if ( p != NULL )
    {
        
        std::vector<const DagNode *>::iterator it = std::find( parameters.begin(), parameters.end(), p );
        if ( it != parameters.end() )
        {
            parameters.erase( it );
            if ( p->decrementReferenceCount() == 0 )
            {
                delete p;
            }
            
        }
        
    }
    
}


/* Method stub that can be overwritten for specialized treatment. */
void Function::restore( DagNode *restorer )
{
    
    // nothing to change here in the base class
    
}

/**
 * Swap the old parameter with a new one.
 * This will be called for example when the entire model graph is cloned or
 * when we replace a variable with the same name (re-assignment).
 * Here we update our set and delegate to the derived class.
 */
void Function::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
    std::vector<const DagNode *>::iterator position = std::find(parameters.begin(), parameters.end(), oldP);
    if ( position != parameters.end() )
    {
//        parameters.erase( position );
//        parameters.push_back( newP );
        (*position) = newP;
        swapParameterInternal( oldP, newP );
        
        // increment and decrement the reference counts
        newP->incrementReferenceCount();
        if ( oldP->decrementReferenceCount() == 0 )
        {
            throw RbException("Memory leak in function. Please report this bug to Sebastian.");
        }
        
    }
    else
    {
        
        throw RbException("Could not find the function parameter to be swapped: " + oldP->getName());
    
    }
    
}


/* Method stub that can be overwritten for specialized treatment. */
void Function::touch( DagNode *toucher )
{
    // do nothing
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Function& f)
{
    
    o << "f(x)";
    
    return o;
}
