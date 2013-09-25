//
//  PreMeasureAnalyzer.h
//  MelodyAnalyzer
//
//  Created by Jack Atherton on 10/14/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#ifndef MelodyAnalyzer_PreMeasureAnalyzer_h
#define MelodyAnalyzer_PreMeasureAnalyzer_h

#include "JuceHeader.h"

/** Analyzes a MidiMessageSequence that begins with four quarter notes on 
        the tonic and is quantized to the 16th note.
    Can produce:
        -Histogram of pitches used, transposed to C / c
        -Histogram of intervals used, from unison to octave
        -Histogram of tatum use on the 16th-note level (tactus quarter note) 
    Can handle one or two notes at once but behavior is undefined beyond that
 */

class PreMeasureAnalyzer {
public:
    PreMeasureAnalyzer(MidiMessageSequence const& melody);

    double pitchCounts[12];
    double intervalCounts[13];
    double tatumCounts[16];
private:
    double ticksPerSixteenth;
    double ticksPerMeasure; //Is 16 times ticksPerSixteenth
    int offsetToC;
    
    void init(MidiMessageSequence const& melody);
    void analyzeAndDeleteFirstMeasure(MidiMessageSequence& copy);
    void analyzeTatumCounts(MidiMessageSequence& copy);
    void analyzePitchCounts(MidiMessageSequence& copy);
    void analyzeIntervalCounts(MidiMessageSequence& copy);
    void twoNoteHelper(MidiMessageSequence& copy, int startIndex);
    
};

#endif
