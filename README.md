# nesmus3
NESMUS3 is the work-in-progress third rewrite of my NES / Famicom sound driver. This document serves equally as both documentation and as a specification. Any behavior deviating from this is *probably* a bug.

## Copyright

The NESMUS3 engine and demo project, as software, are released under the MIT license.

The music showcased in the demo project is my original composition. As a musical work, it is released under a Creative Commons Attribution license.

The sound effects in the demo project are released to the public domain.

## Features
NESMUS3 supports eight "streams" of sound, numbered 0 to 7. Streams 0-3 are music streams on each of the NES's four PSG channels (pulse, triangle, noise; in that order). Streams 4-7 are sound effect streams which play over the corresponding music stream.

Note data supports a gamut of 80 semitones from A0 to E7. Notes can be detuned by a signed 8-bit value.

DPCM support is *not* planned.

Volume envelopes are supported on streams 0, 1, and 3. Up to 128 volume envelopes are supported, shared between the pulse streams 0 and 1.

Stream 3, played on the noise channel, is the "drum" stream. It uses a separate set of up to 80 "drum" envelopes, which have a separate data format from the pulse envelopes. The note number played on this stream selects the drum envelope.

Sound effects do not support envelopes; instead volume and duty should be given directly in the sound effect note data.

Software vibrato and sweeps are supported on the pulse and triangle channels.

Hardware sweep support for the pulse channels is stably implemented but currently broken.

All noise pitches are "inverted" before output, so that $-0 is the lowest-pitched noise and $-F is the highest.

## Data formats

### Note data
The following is a complete listing of all note commands used by NESMUS3.

    Bytes           Desc
    00-4F           Play a note. Note range is A0 (byte $00) to E7 (byte $4F).
                    For stream 3, each note is associated with a different drum envelope.
                    For stream 7, (byte & $0F) is the inverted noise pitch. 
                    
    5x / 50 xx      Cut note and rest for xx ticks.
    6x / 60 xx      Release note and rest for xx ticks.
    7x / 70 xx      Hold note for xx ticks.
    8x / 80 xx      Set length in ticks for following notes.
    9x / 90 xx      Set length in ticks for next note only.
    A0-BF           Set note volume to (byte & $1F). $A0 silences the channel.
                    $AF is the typical, default "max" volume.
                    Values $B0-$BF will make the channel louder.
    C1-DF           Set tempo in frames per tick to (byte & $1F).
    Ex              Set envelope for music pulse streams (0,1), or set duty cycle for sfx pulse streams (4,5).
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

The following bytes in a chunk are like `$pv` where `p` is the (inverted) noise period, and `v` is the volume.

Bytes $00-$02 are special commands: `$00` = hold envelope value, `$01 xx` = jump to offset, `$02` = cut note.

## Building
NESMUS3 is written in 6502 assembly for the [WLA DX] (https://github.com/vhelin/wla-dx) toolchain. It is intended to be incorporated as a library into other programs.

The engine uses 16 bytes in the zero page and 171 bytes in the stack page, and requires around 1,300 to 2,500 CPU cycles (around 30 scanlines) depending mostly on how many note commands are being processed. In the future I may try to optimize this.

To assemble NESMUS3 as a WLA DX library file use the command

    wla-6502 -l nesmus.l nesmus.65s

A program using NESMUS3 must supply the labels `nesmus.envelopeTable`, `nesmus.envReleaseOffsTable`, `nesmus.drumTable`. These are used as lookup tables by the envelope system.

The file `demo.65s` contains a simple host program demonstrating select features of NESMUS3, like how notes and envelopes work and how to play music and sound effects.

## Known current issues

Support for hardware sweep is broken.
