    // The common code for the eval functions is here.
    // This is meant to be included into the eval functions in board.c
    u64 taken = black | white;
    u64 empty = ~taken;
    // Corners
    double eeC = (PC(black & CORNERS) - PC(white & CORNERS));
    // Score for corner patterns
    #define SCP(x) ((black & (x)) == (x)) - ((white & (x)) == (x));
    eeC += SCP(A1 | B1);
    eeC += SCP(A1 | A2);
    eeC += SCP(H1 | G1);
    eeC += SCP(H1 | H2);
    eeC += SCP(A8 | B8);
    eeC += SCP(A8 | A7);
    eeC += SCP(H8 | G8);
    eeC += SCP(H8 | H7);
    eeC *= 2; // So eeC so far is worth twice as much; we'll divide by 2 later
    eeC += SCP(A1 | B1 | C1);
    eeC += SCP(A1 | A2 | A3);
    eeC += SCP(H1 | G1 | F1);
    eeC += SCP(H1 | H2 | H3);
    eeC += SCP(A8 | B8 | C8);
    eeC += SCP(A8 | A7 | A6);
    eeC += SCP(H8 | G8 | F8);
    eeC += SCP(H8 | H7 | H6);
    eeC += SCP(A1 | B1 | C1 | D1);
    eeC += SCP(A1 | A2 | A3 | A4);
    eeC += SCP(H1 | G1 | F1 | E1);
    eeC += SCP(H1 | H2 | H3 | H4);
    eeC += SCP(A8 | B8 | C8 | D8);
    eeC += SCP(A8 | A7 | A6 | A5);
    eeC += SCP(H8 | G8 | F8 | E8);
    eeC += SCP(H8 | H7 | H6 | H5);
    eeC /= 2;

    // Poison squares (squares that give up corner, or risky x-squares)
    // This computation is finished in the C-squares section
    u64 poisonBlack = 0;
    u64 poisonWhite = 0;
    u64 xPoisonBlack = 0;
    u64 xPoisonWhite = 0;
    // X-square check
    #define XSQC(a, b) \
    if ((taken & ((a) | (b))) == 0) {xPoisonBlack |= (b); xPoisonWhite |= (b);}\
    else if ((black & ((a) | (b))) == (b)) {eeC--;}\
    else if ((white & ((a) | (b))) == (b)) {eeC++;}
    XSQC(A1, B2);
    XSQC(H1, G2);
    XSQC(A8, B7);
    XSQC(H8, G7);
    // Empty c-square check
    #define EMPTY_CSQ(a, b, c) \
    if ((taken & ((a) | (b))) == 0) {\
        if (white & (c)) poisonBlack |= (b);\
        else if (black & (c)) poisonWhite |= (b);\
    }
    EMPTY_CSQ(A1, B1, C1);
    EMPTY_CSQ(A1, A2, A3);
    EMPTY_CSQ(H1, H2, H3);
    EMPTY_CSQ(H1, G1, F1);
    EMPTY_CSQ(A8, B8, C8);
    EMPTY_CSQ(A8, A7, A6);
    EMPTY_CSQ(H8, H7, H6);
    EMPTY_CSQ(H8, G8, F8);

    // C squares
    // More isolated c-squares are worse
    // Macro for modifying c-square score based on corner a and adjacent c-square b
    #define CSQS(a, b, shifter) \
    if ((black & ((a) | (b))) == (b)) {\
        u64 c_region = b;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        eeC -= 0.8 - PC(c_region) * 0.1;\
        c_region = shifter(c_region) & empty;\
        if (c_region && (shifter(c_region) & white)) poisonBlack |= c_region;\
    }\
    else if ((white & ((a) | (b))) == (b)) {\
        u64 c_region = (b);\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        eeC += 0.8 - PC(c_region) * 0.1;\
        c_region = shifter(c_region) & empty;\
        if (c_region && (shifter(c_region) & black)) poisonWhite |= c_region;\
    }
    CSQS(A1, B1, SHIFT_RIGHT);
    CSQS(A1, A2, SHIFT_DOWN);
    CSQS(H1, G1, SHIFT_LEFT);
    CSQS(H1, H2, SHIFT_DOWN);
    CSQS(A8, B8, SHIFT_RIGHT);
    CSQS(A8, A7, SHIFT_UP);
    CSQS(H8, G8, SHIFT_LEFT);
    CSQS(H8, H7, SHIFT_UP);

    // Scale down corner and c-square scores
    eeC /= (1 + PC(EDGES & ~CORNERS & taken)) / 2.4;
    
    // Frontier
    // Idea: double frontier (2 away and unshielded)?
    // Old idea: simple frontier ratio
    //double blackFrontierRatio = PC(frontier(black, white)) / (double) PC(black);
    //double whiteFrontierRatio = PC(frontier(white, black)) / (double) PC(white);
    // New idea: Weighted frontiers
    double eeF = frontierScore(white, black, poisonBlack | xPoisonBlack) - frontierScore(black, white, poisonWhite | xPoisonWhite);

    // Mobility
    u64 lmBlack = findLegalMoves(black, white);
    u64 lmWhite = findLegalMoves(white, black);
    int lmNumBlack = PC(lmBlack);
    int lmNumWhite = PC(lmWhite);
    int diff = lmNumBlack - lmNumWhite - 1; // The -1 is a penalty for having to move
    int absDiff = (diff > 0) ? diff : -diff;
    double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - lmNumBlack - lmNumWhite);
    double eeM = 1 - int_pow(1 - 0.01 * absDiff, exponent); // No explanation for this formula yet, still testing
    eeM = (diff > 0) ? eeM : -eeM;

    // Regional mobility
    double eeRM = 0;
    u64 regions[15];
    u8 numRegions = findRegions(~(black | white), regions);
    for (int i = 0; i < numRegions; i++) {
        u64 region = regions[i];

        // Calculate legal move number for each region
        // Poison legal moves are treated as negative legal moves
        // Perhaps X-squares should only be subtracted once, since a sacrifice of a 
        // corner along an X-square is more common than a sacrifice along an edge.
        double lmNumBlack = PC(lmBlack & region);
        double lmNumWhite = PC(lmWhite & region);
        int total = lmNumBlack + lmNumWhite; // Total should not be affected by poison squares
        if (total == 0) continue;
        lmNumBlack -= 1.5 * PC(lmBlack & region & poisonBlack);
        lmNumWhite -= 1.5 * PC(lmWhite & region & poisonWhite);
        lmNumBlack -= 0.75 * PC(lmBlack & region & xPoisonBlack);
        lmNumWhite -= 0.75 * PC(lmWhite & region & xPoisonWhite);

        // Calculations (no explanations for these yet, but we use an exponential)
        double diff = lmNumBlack - lmNumWhite;
        double absDiff = (diff > 0) ? diff : -diff;
        double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - total);
        double score = 1 - int_pow(1 - 0.01 * absDiff, exponent); 
        score = (diff > 0) ? score : -score;
        if (PC(region & CORNERS) == 1) score *= 1.5;
        double factor = 2.1 - 0.1 * PC(region);
        factor = (factor > 1) ? factor : 1;
        score *= factor;
        eeRM += score;
    }
    eeRM /= numRegions;

    // Putting it all together
    double cornerWeight = 0.51;
    double remainingWeight = 1 - cornerWeight;
    double frontierWeight = 0.35 * remainingWeight;
    double mobilityWeight = 0.5 * remainingWeight;
    double rmWeight = 0.15 * remainingWeight;
    double ee = 1024 * (eeF * frontierWeight + eeC * cornerWeight + eeM * mobilityWeight + eeRM * rmWeight);
    double eeSumAbs = 1024 * (fabs(eeF) * frontierWeight + fabs(eeC) * cornerWeight + fabs(eeM) * mobilityWeight + fabs(eeRM) * rmWeight);
