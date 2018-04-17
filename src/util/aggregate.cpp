#include "Debug.h"
#include "Parameters.h"
#include "Aggregation.h"

int aggregate(int argc, const char **argv, const Command& command) {
    Parameters &par = Parameters::getInstance();
    par.parseParameters(argc, argv, command, 2, true, true);
    Aggregation *aggregFunction = nullptr;

    if(par.mode=="bestHit"){
        aggregFunction = new bestHitAggregator(par.db1, par.db2, par.db3, (unsigned int)par.threads) ;
    }
    else if (par.mode=="pval"){
        aggregFunction = new pvalAggregator(par.db1, par.db2, (unsigned int)par.threads, par.db3, par.db4) ;
    }
    else
        return 1 ;

    aggregFunction->runAggregate();

    return 0 ;
}
