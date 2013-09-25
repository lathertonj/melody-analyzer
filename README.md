melody-analyzer
===============

A small program using the JUCE Midi framework to analyze melodies that have 
been prepared in a certain protocol.

These files need to be combined with other files from the JUCE framework in
order to be compiled and run.  When run, this program takes an input midi file
and outputs some statistical information about the melody contained in the
file to a local .txt file.  It finds the key and tempo of the song via the
first measure, which should be a measure of quarter notes of the tonic
(first note of the scale).  This measure is ignored when computing the
analysis, which is transposed to C / c.

This program is probably not very useful to download and run, but could be
useful to look at.  As it was originally intended only to collect data for a
larger research project, the code is not very well documented and is provided
as-is.  However, if you have any questions, feel free to contact me and I will
be happy to respond.
