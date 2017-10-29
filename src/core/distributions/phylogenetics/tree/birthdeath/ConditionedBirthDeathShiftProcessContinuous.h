#ifndef ConditionedBirthDeathShiftProcessContinuous_H
#define ConditionedBirthDeathShiftProcessContinuous_H

#include "CharacterHistoryContinuous.h"
#include "MemberObject.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class ConditionedBirthDeathShiftProcessContinuous : public TypedDistribution<Tree>, public MemberObject< RbVector<long> >, public MemberObject< RbVector<double> > {
        
    public:
        ConditionedBirthDeathShiftProcessContinuous(const TypedDagNode<double> *a,
                                    const TypedDistribution<double> *s,
                                    const TypedDistribution<double> *e,
                                    const TypedDagNode<double> *ev,
                                    const TypedDagNode<double> *r,
                                    const std::string &cdt,
                                    const std::vector<Taxon> &n);                                                       //!< Constructor
        
        virtual                                            ~ConditionedBirthDeathShiftProcessContinuous(void);          //!< Virtual destructor
        
        // public member functions
        ConditionedBirthDeathShiftProcessContinuous*        clone(void) const;                                          //!< Create an independent clone
        double                                              computeLnProbability(void);                                 //!< Compute ln prob of current value
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<long> &rv) const;     //!< Map the member methods to internal function calls
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const;     //!< Map the member methods to internal function calls
        CharacterHistoryContinuous&                         getCharacterHistory(void);                                  //!< Get the character histories
        const CharacterHistoryContinuous&                   getCharacterHistory(void) const;                                  //!< Get the character histories
        void                                                redrawValue(void);                                          //!< Draw a new random value from distribution
        void                                                setValue(Tree *v, bool force);
        
    protected:
        // Parameter management functions
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        double                                              computeNodeProbability(const TopologyNode &n, size_t nIdx);
        double                                              computeRootLikelihood(void);
        
        
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times, double T);
        void                                                buildRandomBinaryHistory(std::vector<TopologyNode *> &tips);
        double                                              computeStateValue(size_t i, size_t j, double time) const;
        double                                              computeStartValue(size_t i, size_t j) const;
        void                                                simulateTree(void);
        void                                                runMCMC(void);
//        void                                                updateBranchProbabilitiesNumerically(std::vector<double> &state, double begin, double end, const RbVector<double> &s, const RbVector<double> &e, double r, size_t i);
        
        // members
        const TypedDagNode<double>*                         root_age;
        const TypedDistribution<double>*                    speciation;
        const TypedDistribution<double>*                    extinction;
        const TypedDagNode<double>*                         event_rate;
        const TypedDagNode<double>*                         rho;                                                                                                //!< Sampling probability of each species.
        
        CharacterHistoryContinuous                          branch_histories;
        
        std::string                                         condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        size_t                                              num_taxa;
        std::vector<Taxon>                                  taxa;
        
        
//        std::vector<size_t>                                 active_likelihood;
//        mutable std::vector<bool>                           changed_nodes;
//        mutable std::vector<bool>                           dirty_nodes;
//        mutable std::vector<std::vector<std::vector<double> > >       nodeStates;
//        mutable std::vector<std::vector<double> >           scaling_factors;
//        mutable double                                      total_scaling;
        
        double                                              log_tree_topology_prob;                                                                                //!< Log-transformed tree topology probability (combinatorial constant).
        
    };
    
}

#endif

