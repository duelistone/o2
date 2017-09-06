    // The common code for the eval functions is here.
    // This is meant to be included into the eval functions in board.c
    u64 taken = black | white;
    u64 empty = ~taken;

    // Corners
    double eeC = 0;
    // Poison squares (squares that give up corner, or risky x-squares)
    // This computation is finished in the C-squares section
    u64 poisonBlack = 0;
    u64 poisonWhite = 0;
    u64 xPoisonBlack = 0;
    u64 xPoisonWhite = 0;
    // X-square check
    #define XSQC(b) \
    if (black & (b)) {eeC-=2;}\
    else if (white & (b)) {eeC+=2;}\
    else {xPoisonBlack |= (b); xPoisonWhite |= (b);}
    // C squares
    // More isolated c-squares are worse
    // Macro for modifying c-square score based on corner a and adjacent c-square b
    #define CSQS(b, c, shifter) \
    if ((taken & ((b) | (c))) == (c)) {\
        if (white & (c)) poisonBlack |= (b);\
        else poisonWhite |= (b);\
    }\
    else if (black & (b)) {\
        u64 c_region = b;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        if (PC(c_region) < 6) {\
            eeC -= 2 * (0.8 - PC(c_region) * 0.1);\
            c_region = shifter(c_region) & empty & ~CORNERS;\
            if (c_region && (shifter(c_region) & white)) poisonBlack |= c_region;\
        }\
    }\
    else if (white & (b)) {\
        u64 c_region = (b);\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        if (PC(c_region) < 6) {\
            eeC += 2 * (0.8 - PC(c_region) * 0.1);\
            c_region = shifter(c_region) & empty & ~CORNERS;\
            if (c_region && (shifter(c_region) & black)) poisonWhite |= c_region;\
        }\
    }

    // Score for corner patterns
    #define SCPB(x) ((black & (x)) == (x))
    #define SCPW(x) ((white & (x)) == (x))
    if (taken & A1) {
        if (black & A1) {
            eeC += 2 + 2 * SCPB(A1 | B1);
            eeC += 2 * SCPB(A1 | A2);
            eeC += SCPB(A1 | B1 | C1);
            eeC += SCPB(A1 | A2 | A3);
            eeC += SCPB(A1 | B1 | C1 | D1);
            eeC += SCPB(A1 | A2 | A3 | A4);
        }
        else {
            eeC -= 2 + 2 * SCPW(A1 | B1);
            eeC -= 2 * SCPW(A1 | A2);
            eeC -= SCPW(A1 | B1 | C1);
            eeC -= SCPW(A1 | A2 | A3);
            eeC -= SCPW(A1 | B1 | C1 | D1);
            eeC -= SCPW(A1 | A2 | A3 | A4);
        }
    }
    else {
        XSQC(B2);
        CSQS(B1, C1, SHIFT_RIGHT);
        CSQS(A2, A3, SHIFT_DOWN);
    }
    if (taken & H1) {
        if (black & H1) {
            eeC += 2 + 2 * SCPB(H1 | G1);
            eeC += 2 * SCPB(H1 | H2);
            eeC += SCPB(H1 | G1 | F1);
            eeC += SCPB(H1 | H2 | H3);
            eeC += SCPB(H1 | G1 | F1 | E1);
            eeC += SCPB(H1 | H2 | H3 | H4);
        }
        else {
            eeC -= 2 + 2 * SCPW(H1 | G1);
            eeC -= 2 * SCPW(H1 | H2);
            eeC -= SCPW(H1 | G1 | F1);
            eeC -= SCPW(H1 | H2 | H3);
            eeC -= SCPW(H1 | G1 | F1 | E1);
            eeC -= SCPW(H1 | H2 | H3 | H4);
        }
    }
    else {
        XSQC(G2);
        CSQS(G1, F1, SHIFT_LEFT);
        CSQS(H2, H3, SHIFT_DOWN);
    }
    if (taken & A8) {
        if (black & A8) {
            eeC += 2 + 2 * SCPB(A8 | B8);
            eeC += 2 * SCPB(A8 | A7);
            eeC += SCPB(A8 | B8 | C8);
            eeC += SCPB(A8 | A7 | A6);
            eeC += SCPB(A8 | B8 | C8 | D8);
            eeC += SCPB(A8 | A7 | A6 | A5);
        }
        else {
            eeC -= 2 + 2 * SCPW(A8 | B8);
            eeC -= 2 * SCPW(A8 | A7);
            eeC -= SCPW(A8 | B8 | C8);
            eeC -= SCPW(A8 | A7 | A6);
            eeC -= SCPW(A8 | B8 | C8 | D8);
            eeC -= SCPW(A8 | A7 | A6 | A5);
        }
    }
    else {
        XSQC(B7);
        CSQS(B8, C8, SHIFT_RIGHT);
        CSQS(A7, A6, SHIFT_UP);
    }
    if (taken & H8) {
        if (black & H8) {
            eeC += 2 + 2 * SCPB(H8 | G8);
            eeC += 2 * SCPB(H8 | H7);
            eeC += SCPB(H8 | G8 | F8);
            eeC += SCPB(H8 | H7 | H6);
            eeC += SCPB(H8 | G8 | F8 | E8);
            eeC += SCPB(H8 | H7 | H6 | H5);
        }
        else {
            eeC -= 2 + 2 * SCPW(H8 | G8);
            eeC -= 2 * SCPW(H8 | H7);
            eeC -= SCPW(H8 | G8 | F8);
            eeC -= SCPW(H8 | H7 | H6);
            eeC -= SCPW(H8 | G8 | F8 | E8);
            eeC -= SCPW(H8 | H7 | H6 | H5);
        }
    }
    else {
        XSQC(G7);
        CSQS(G8, F8, SHIFT_LEFT);
        CSQS(H7, H6, SHIFT_UP);
    }

    // Scale down corner and c-square scores
    eeC /= (1 + PC(EDGES & ~CORNERS & taken)) / 1.2;
    
    // Frontier
    // Idea: double frontier (2 away and unshielded)?
    // Old idea: simple frontier ratio
    //double blackFrontierRatio = PC(frontier(black, white)) / (double) PC(black);
    //double whiteFrontierRatio = PC(frontier(white, black)) / (double) PC(white);
    // New idea: Weighted frontiers
    double eeF = frontierScore(white, black, poisonBlack | xPoisonBlack) - frontierScore(black, white, poisonWhite | xPoisonWhite);

    // Mobility
    u64 lmWhite = findLegalMoves(white, black);
    int lmNumBlack = PC(lmBlack);
    int lmNumWhite = PC(lmWhite);
    int diff = lmNumBlack - lmNumWhite - 1; // The -1 is a penalty for having to move
    double eeM = 4 * mobilityConstant[35 * lmNumBlack + lmNumWhite];
    eeM = (diff > 0) ? eeM : -eeM;

    // Putting it all together
    double cornerWeight = 0.51;
    double remainingWeight = (1 - cornerWeight);
    double frontierWeight = (0.3 / 0.85) * remainingWeight;
    double mobilityWeight = (0.55 / 0.85) * remainingWeight;
    double ee = 1024 * (eeF * frontierWeight + eeC * cornerWeight + eeM * mobilityWeight);
    double eeSumAbs = 1024 * (fabs(eeF) * frontierWeight + fabs(eeC) * cornerWeight + fabs(eeM) * mobilityWeight);
