//
//  PreMeasureAnalyzer.cpp
//  MelodyAnalyzer
//
//  Created by Jack Atherton on 10/14/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#include <iostream>
#include "PreMeasureAnalyzer.h"

PreMeasureAnalyzer::PreMeasureAnalyzer(MidiMessageSequence const& melody) {
    init(melody);
}

void PreMeasureAnalyzer::init(MidiMessageSequence const& melody) {
    MidiMessageSequence copy (melody);
    for (int i = 0; i < copy.getNumEvents(); i++) {
        if (!copy.getEventPointer(i)->message.isNoteOn()) {
            copy.deleteEvent(i, false);
            i--;
        }
    }
    analyzeAndDeleteFirstMeasure(copy);
    
    analyzeTatumCounts(copy);
    
    analyzePitchCounts(copy);
    
    analyzeIntervalCounts(copy);    
    
}

void PreMeasureAnalyzer::analyzeAndDeleteFirstMeasure(MidiMessageSequence& copy) {
    ticksPerSixteenth = (copy.getEventTime(1) - copy.getEventTime(0))/4.0;
    ticksPerMeasure = (copy.getEventTime(1) - copy.getEventTime(0))*4.0;
    offsetToC = (120 - copy.getEventPointer(0)->message.getNoteNumber()) % 12;
    
    copy.addTimeToMessages(-1 * copy.getEventTime(0));
    
    for(int i = 0; i < 4; i++) {
        copy.deleteEvent(0, false);
    }
    
    copy.addTimeToMessages(-1 * ticksPerMeasure);
}

void PreMeasureAnalyzer::analyzeTatumCounts(MidiMessageSequence& copy) {
    for (int i = 0; i < 16; i++) {
        tatumCounts[i] = 0;
    }
    double sumTatums = 0;
    for (int i = 0; i < copy.getNumEvents(); i++) {
        int tatum = ((int) copy.getEventTime(i)%(int) ticksPerMeasure + 1)/ticksPerSixteenth;
        tatumCounts[tatum]++;
        sumTatums++;
    }
    for (int i = 0; i < 16; i++) {
        tatumCounts[i] = tatumCounts[i] / sumTatums;
    }
}

void PreMeasureAnalyzer::analyzePitchCounts(MidiMessageSequence& copy) {
    for (int i = 0; i < 12; i++) {
        pitchCounts[i] = 0;
    }
    double sumPitches = 0;
    for (int i = 0; i < copy.getNumEvents(); i++) {
        int pitch = (copy.getEventPointer(i)->message.getNoteNumber() + offsetToC) % 12;
        pitchCounts[pitch]++;
        sumPitches++;
    }
    for (int i = 0; i < 12; i++) {
        pitchCounts[i] = pitchCounts[i]/sumPitches;
    }
}

void PreMeasureAnalyzer::analyzeIntervalCounts(MidiMessageSequence& copy) {
    for (int i = 0; i < 13; i++) {
        intervalCounts[i] = 0;
    }
    double sumIntervals = 0;
    for (int i = 0, j = 1; j < copy.getNumEvents(); i++, j++) {
        if (copy.getEventTime(i) != copy.getEventTime(j)) {
            int interval = (copy.getEventPointer(j)->message.getNoteNumber() - copy.getEventPointer(i)->message.getNoteNumber()) % 12;
            if (interval < 0) {
                interval *= -1;
            }
            intervalCounts[interval]++;
        } else {
            twoNoteHelper(copy, i);
            //i and j need +=2 but have ++ in for-loop statement
            i++;
            j++;
        }
        sumIntervals++;
    }
    for (int i = 0; i < 13; i++) {
        intervalCounts[i] = intervalCounts[i] / sumIntervals;
    }
}

void PreMeasureAnalyzer::twoNoteHelper(MidiMessageSequence& copy, int startIndex) {
    int interval1, interval2;
    if (startIndex + 2 >= copy.getNumEvents()) {
        return;
    }
    if ((startIndex + 3 < copy.getNumEvents()) && copy.getEventTime(startIndex+2) == copy.getEventTime(startIndex+3)) {
        int high1st, low1st, high2nd, low2nd;
        high1st = std::max(copy.getEventPointer(startIndex)->message.getNoteNumber(),copy.getEventPointer(startIndex+1)->message.getNoteNumber());
        low1st  = std::min(copy.getEventPointer(startIndex)->message.getNoteNumber(),copy.getEventPointer(startIndex+1)->message.getNoteNumber());
        high2nd = std::max(copy.getEventPointer(startIndex+2)->message.getNoteNumber(),copy.getEventPointer(startIndex+3)->message.getNoteNumber());
        low2nd  = std::min(copy.getEventPointer(startIndex+2)->message.getNoteNumber(),copy.getEventPointer(startIndex+3)->message.getNoteNumber());
        interval1 = (high2nd - high1st) % 12;
        interval2 = (low2nd - low1st) % 12;        
    } else {
        interval1 = (copy.getEventPointer(startIndex+2)->message.getNoteNumber() - copy.getEventPointer(startIndex)->message.getNoteNumber()) % 12;
        interval2 = (copy.getEventPointer(startIndex+2)->message.getNoteNumber() - copy.getEventPointer(startIndex+1)->message.getNoteNumber()) % 12;
    }
    if (interval1 < 0) {
        interval1 *= -1;
    }
    if (interval2 < 0) {
        interval2 *= -1;
    }
    intervalCounts[interval1]++;
    intervalCounts[interval2]++;
}


