# nesmus3
NESMUS3 is the work-in-progress third rewrite of my NES / Famicom sound driver. This document serves equally as both documentation and as a specification. Any behavior deviating from this is *probably* a bug.

## Features
NESMUS3 supports:
* music and one sound effect on each of the NES's four PSG channels. DPCM support is not planned.
* up to 128 volume envelopes for music on pulse channels.
* a separate set of up to 80 "drum" envelopes for music on the noise channel.
* vibrato and software sweep effects.

## Data formats

### Note data
The following is a complete listing of all note commands used by NESMUS3.

    Bytes           Desc
    00-4F           Play a note. Note range is A0 (byte $00) to E6 (byte $4F).
                    On drum channel, each note is associated with a different drum envelope.
    5x / 50 xx      Cut note and rest for xx ticks.
    6x / 60 xx      Release note and rest for xx ticks.
    7x / 70 xx      Hold note for xx ticks.
    8x / 80 xx      Set length in ticks for following notes.
    9x / 90 xx      Set length in ticks for next note only.
    Ax              Set note volume. $A0 silences the channel.
    Bx              Set envelope for music pulse streams (0,1), or set duty cycle for sfx pulse streams (4,5).
    C1-DF           Set tempo in frames per tick to (byte & $1F).
    F0 xxxx         Call subroutine
    F1              Return from subroutine
    F2 xx           Begin loop, set loop counter
    F3              End loop
    F4 xx           Cut note and release for xx frames.
    F5 xx           Release note for xx frames.
    F6 xx           Hold note for xx frames.
    F7 xxxx         Jump to address
    F8              Disable pitch effect
    F9              Vibrato effect
    FA              Software sweep up
    FB              Software sweep down
    FC xy           Set parameters for pitch effect: speed x, depth y
    FD xx           Set hardware sweep for pulse channels
    FE xx           Set note detune (signed)
    FF              End of stream data. Stream is automatically stopped upon reading this.
    
### Pulse envelope data
Envelope data for the pulse channels consists of a string of envelope commands.
These take the form of values to be ORed with #$30 and written to $4000 or $4004. 

Special commands are: `$FF` = hold forever, `$FE xx` = jump to offset, `$FD` = cut note.
If bit 4 (#$10) of an envelope byte is set, the following byte specifies the number of frames to hold that value.

### Drum envelope data
Drum envelope data is formatted differently from pulse envelope data. Drum envelopes are structured in chunks of 1 to 16 bytes. The first byte has the format `$xy` where `x` is the number of bytes following in the chunk, and `y` is the number of frames to hold each.

The following bytes in a chunk are like `$pv` where `p` is the noise period and `v` is the volume.

Bytes $00-$02 are special commands: $00 = hold envelope value, $01 = jump to offset, $02 = cut note.

## Building
NESMUS3 is written in 6502 assembly for the [WLA DX] (https://github.com/vhelin/wla-dx) toolchain. It is intended to be incorporated as a library into other programs.

To assemble NESMUS3 as a WLA DX library file use the command

    wla-6502 -l nesmus.l nesmus.65s

A program using NESMUS3 must supply the labels `nesmus.envelopeTable`, `nesmus.envReleaseOffsTable`, `nesmus.drumTable`. These are used as lookup tables by the envelope system.

The file `demo.65s` contains a simple host program demonstrating select features of NESMUS3.

## Known current issues

The hardware sweep command, $FD, is broken. Apparently 

