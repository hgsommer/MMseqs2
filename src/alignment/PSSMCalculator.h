#ifndef MMSEQS_PSSM_H
#define MMSEQS_PSSM_H

#include <cstddef>
#include <string>
#include <vector>

#include "Matcher.h"

class SubstitutionMatrix;

class PSSMCalculator {
public:

    struct Profile{
        const char * pssm;
        float * prob;
        const float * neffM;
        const float *gDelOpen;
        const float *gDelClose;
        const float *gIns;
        const float *gapFraction;
        const float gapPseudoCount;
        std::string consensus;

        Profile(char *pssm, float *prob, float *neffM, const float *gDelOpen, const float *gDelClose,
                const float *gIns, const float *gapFraction, const float gapPseudoCount, std::string consensus)
                : pssm(pssm), prob(prob), neffM(neffM), gDelOpen(gDelOpen), gDelClose(gDelClose),
                  gIns(gIns), gapFraction(gapFraction), gapPseudoCount(gapPseudoCount), consensus(consensus) {
        }
    };

    PSSMCalculator(SubstitutionMatrix *subMat, size_t maxSeqLength, size_t maxSetSize, float pca, float pcb, int gapOpen, int gapPseudoCount);

    ~PSSMCalculator();

    Profile computePSSMFromMSA(size_t setSize, size_t queryLength, const char **msaSeqs, bool wg);
    Profile computePSSMFromMSA(size_t setSize, size_t queryLength, const char **msaSeqs, const std::vector<Matcher::result_t> &alnResults, bool wg);

    void printProfile(size_t queryLength);
    void printPSSM(size_t queryLength);

    // prepare pseudocounts
    static void preparePseudoCounts(float *frequency, float *frequency_with_pseudocounts, size_t entrySize, size_t queryLength, const float **R);

    // compute pseudocounts from Neff_M -p log(p) per column
    static void computePseudoCounts(float *profile, float *frequency, float *frequency_with_pseudocounts, size_t entrySize, float *Neff_M, size_t length,float pca, float pcb);

    // Compute weight for sequence based on "Position-based Sequence Weights' (1994)
    static void computeSequenceWeights(float *seqWeight, size_t queryLength, size_t setSize, const char **msaSeqs);

private:
    SubstitutionMatrix * subMat;

    // contains sequence weights (global)
    float * seqWeight;

    // sum of sequence weights
    float seqWeightTotal;

    // contains MSA AA matchWeight
    float * matchWeight;

    // contains MSA AA pseudocount weight
    float * pseudocountsWeight;

    // Entropy of MSA
    float * Neff_M;

    // Profile of MSA
    float * profile;

    // PSSM contains log odds PSSM values
    char * pssm;

    // position-specific gap open penalties for deletions
    std::vector<float> gDelOpen;

    // position-specific gap close penalties for deletions
    std::vector<float> gDelClose;

    // position-specific gap open penalties for insertions
    std::vector<float> gIns;

    // weighted fraction of gaps to include in the profile
    std::vector<float> gapFraction;

    // preallocated memory for computing of gap penalties
    std::vector<float> gapWeightsIns;

    // number of sequences in subalignment i (only for DEBUGGING)
    int *nseqs;

    // weight contribution value for each sequence
    float **w_contrib;

    // weight of sequence k in column i, calculated from subalignment i
    float *wi;

    // number of different amino acids
    int *naa;

    size_t maxSeqLength;

    // default gap opening penalty
    int gapOpen;

    // pseudo count for calculation of gap opening penalties
    int gapPseudoCount;

    // compute position-specific scoring matrix PSSM score
    // 1.) convert PFM to PPM (position probability matrix)
    //     Both PPMs assume statistical independence between positions in the pattern
    // 2.) PSSM Log odds score
    //     M_{aa,pos}={log(M_{aa,pos} / b_{aa}).
    void computeLogPSSM(char *pssm, const float *profile, float bitFactor, size_t queryLength, float scoreBias);

    // compute the Neff_M per column -p log(p)
    void computeNeff_M(float *frequency, float *seqWeight, float *Neff_M, size_t queryLength, size_t setSize, char const **msaSeqs);

    void computeMatchWeights(float * matchWeight, float * seqWeight, size_t setSize, size_t queryLength, const char **msaSeqs);

    void computeContextSpecificWeights(float * matchWeight, float *seqWeight, float * Neff_M, size_t queryLength, size_t setSize, const char **msaSeqs);

    // compute position-specific gap penalties for both deletions and insertions
    void computeGapPenalties(size_t queryLength, size_t setSize, const char **msaSeqs, const std::vector<Matcher::result_t> &alnResults);

    float pca;
    float pcb;

    std::string computeConsensusSequence(float *pDouble, size_t queryLength, double *back, char *num2aa);
};


#endif //MMSEQS_PSSM_H
