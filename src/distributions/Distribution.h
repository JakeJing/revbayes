/**
 * @file
 * This file contains the interface for distributions.
 * A distribution is taken in the statistical sense.
 * This interface specifies the pdf, cdf, quantile and rv function with its parameters which need to be provided by any distribution.
 *
 * @brief Declaration of Interface Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Distribution
 * @extends RbObject
 * @package distributions
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include <set>
#include <string>
#include "RbObject.h"
#include "StringVector.h"

class RandomNumberGenerator;
class DAGNode;

class Distribution: public RbObject {

public:
    static const StringVector   rbClass;            //!< Static class attribute

	virtual ~Distribution() { }                        //!< Destructor does nothing

	std::set<DAGNode*>&            getParents(void);
    std::string                    getReturnType() const { return returnType; }    //!< Get return type
	virtual double                 lnPdf(RbObject* o) = 0;       //!< Ln probability density function
	virtual double                 pdf(RbObject* o) = 0;         //!< Probability density function
	virtual RbObject*              rv()  = 0;


protected:
	Distribution(RandomNumberGenerator* r) :
		RbObject() {
		rng = r;
	}

	RandomNumberGenerator* 			rng;
	std::set<DAGNode*>             parents;
	std::string                    returnType;
};

#endif
