//
//  GeographicGridRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 11/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__GeographicGridRateModifier__
#define __rb_mlandis__GeographicGridRateModifier__

#include "AbstractCharacterHistoryRateModifier.h"
#include "GeographicArea.h"
#include "StochasticNode.h"
#include <string>

namespace RevBayesCore
{
    class GeographicGridRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        GeographicGridRateModifier(void);
        GeographicGridRateModifier(const GeographicGridRateModifier& p);
        double computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState); // ... or pass value to computeRateModifier
        GeographicGridRateModifier* clone(void) const;
        void update(void);
        
    private:
        std::string distanceType;
        std::vector<GeographicArea*> gridAreas;
        
        // helper variables
        int numAreas;
    };
}

#endif /* defined(__rb_mlandis__GeographicGridRateModifier__) */
