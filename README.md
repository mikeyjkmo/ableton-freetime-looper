# ableton-freetime-looper
A utility that allows freetime looping in session view of Ableton Live without the use of an audible metronome.

There is a limitation in Ableton Live such that multiple loops cannot be recorded live without quantization to a metronome. This is fine when the user has enough audio outputs to have a stereo audio output + metronome output, but not everyone has this luxury.

This utilty transforms the Ableton Live session view into a looper that operates in a similar fashion to a guitar looping pedal -- the initial loop sets the "loop length" and all subsequence loops are quantized to this. In this scenario, an audible metronome is not required!

#### Building the Visual Studio 2015 Solution

The #includes need to see files in a directory above the .sln file. Add an additional include directory

    Project Properties > C/C++ > All Options
    Add the folder that contains the source files (ableton-freetime-looper)
