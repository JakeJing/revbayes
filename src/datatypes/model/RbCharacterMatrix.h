/**
 * @file
 * This file contains the declaration of RbString, which is
 * a REvBayes wrapper around a regular string.
 *
 * @brief Declaration of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbCharacterMatrix_H
#define RbCharacterMatrix_H

#include "RbComplex.h"

#include <string>
#include <vector>

class RbDumpState;
class RbBitset;


class RbCharacterMatrix : public RbComplex {

    public:
        static const StringVector   rbClass;            //!< Static class attribute
        RbBitset*** getCharacterMatrix(void) { return charMatrix; }

		RbCharacterMatrix(const RbCharacterMatrix& a);
	       virtual ~RbCharacterMatrix() {}
	       int getNumTaxa(void) { return numTaxa; }
	       int getNumChar(void) { return numChar; }
	       int getNumStates(void) { return numStates; }
	       std::string getTaxonName(int i) { return taxonNames[i]; }
	       std::vector<std::string>& getTaxonNames(void) { return taxonNames; }
			void	allocateCharMatrix(int nt, int nc, int ns);
			void   freeCharMatrix(void);
			void print(std::ostream& o);
			virtual char convertToChar(RbBitset* bs) = 0;
			virtual void initState(char nuc, RbBitset* bs)=0;
	       
	    virtual RbCharacterMatrix&               operator=(const RbCharacterMatrix& o)=0;
    void 						readFasta(std::string fileName);
    void						readPhylip(std::string fileName);

        //void                    dump(std::ostream& c);                   //!< Dump to ostream c
        //void                    resurrect(const RbDumpState& x);         //!< Resurrect from dumped state

    protected:
    		int numTaxa;
    		int numChar;
    		int numStates;
    		std::vector<std::string> taxonNames;
	         RbCharacterMatrix(std::string fileName, std::string fileType, int ns);   
    		RbBitset				***charMatrix;

};

#endif

