/**
 * @file
 * This file contains the implementation of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Implementation of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "RbFunction_readCharacterMatrix.h"
#include "RbCharacterMatrix.h"
#include "RbDnaAlignment.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "StringVector.h"
#include "RbString.h"
#include "RbNames.h"
#include <cmath>

const StringVector RbFunction_readCharacterMatrix::rbClass = StringVector(RbNames::ReadAlignment::name) + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_ReadAlignment = SymbolTable::globalTable().add("sqrt", new RbFunction_readCharacterMatrix());


/** Default constructor, allocate workspace */
RbFunction_readCharacterMatrix::RbFunction_readCharacterMatrix(void)
    : RbFunction() {

	argRules.push_back( ArgumentRule(RbNames::ReadAlignment::fileName, RbNames::String::name) );
	argRules.push_back( ArgumentRule(RbNames::ReadAlignment::fileType, RbNames::String::name) );
	argRules.push_back( ArgumentRule(RbNames::ReadAlignment::alignmentType, RbNames::String::name) );
	returnType = RbNames::CharacterMatrix::name;
} 

/** Copy constructor */
RbFunction_readCharacterMatrix::RbFunction_readCharacterMatrix(const RbFunction_readCharacterMatrix& s)
    : RbFunction(s) {
    
	argRules.push_back( ArgumentRule(RbNames::ReadAlignment::fileName, RbNames::String::name) );
	argRules.push_back( ArgumentRule(RbNames::ReadAlignment::fileType, RbNames::String::name) );
	argRules.push_back( ArgumentRule(RbNames::ReadAlignment::alignmentType, RbNames::String::name) );
	returnType = RbNames::CharacterMatrix::name;
}

/** Destructor, delete workspace */
RbFunction_readCharacterMatrix::~RbFunction_readCharacterMatrix() {

}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* RbFunction_readCharacterMatrix::clone(void) const {

    RbObject *x = new RbFunction_readCharacterMatrix( *this );
    return x;
}

RbObject& RbFunction_readCharacterMatrix::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_readCharacterMatrix& x = dynamic_cast<const RbFunction_readCharacterMatrix&> (obj);

        RbFunction_readCharacterMatrix& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_readCharacterMatrix& x = dynamic_cast<const RbFunction_readCharacterMatrix&> (*(obj.convertTo(RbNames::ReadAlignment::name)));

            RbFunction_readCharacterMatrix& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::ReadAlignment::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_readCharacterMatrix& RbFunction_readCharacterMatrix::operator=(const RbFunction_readCharacterMatrix& obj) {
    argRules = obj.argRules;
    returnType = obj.returnType;
    return (*this);
}

void RbFunction_readCharacterMatrix::printValue(std::ostream &o) const {

    //o << "Printing character matrix" << std::endl;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void RbFunction_readCharacterMatrix::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_readCharacterMatrix not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void RbFunction_readCharacterMatrix::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_readCharacterMatrix not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_readCharacterMatrix::toString(void) const {

    return RbNames::ReadAlignment::name;
}


/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_readCharacterMatrix::equals(const RbObject* o) const {

    return false;
}

/** Execute function */
RbObject* RbFunction_readCharacterMatrix::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    std::string fName = ((RbString*) arguments[0]->getValue())->getString();
    std::string fType = ((RbString*) arguments[1]->getValue())->getString();
    std::string aType = ((RbString*) arguments[2]->getValue())->getString();

	RbCharacterMatrix* m;
	if (aType == RbNames::AlignmentType::dna)
		m = new RbDnaAlignment(fName,fType);
	else if (aType == RbNames::AlignmentType::aa)
		;
		
    return m;
}
