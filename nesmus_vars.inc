.define curStream    $F0
.define curChannel   $F1
.define outputOffs   $F2
.define envX         $F3
.define pitchX       $F4
.define sweepX       $F5
.define outputMask   $F6
.define effectMask   $F7
.define tmpvar       $F8
.define newNoteFlags $FC
.define dataPtr      $FE

.define NOTE_STATE      %00000011
.define NOTE_OFF        %00000000
.define NOTE_ATTACK     %00000001
.define NOTE_SUSTAIN    %00000010
.define NOTE_RELEASE    %00000011
.define PFX_OFF         %00000000
.define PFX_DETUNE      %00010000
.define PFX_ENABLE      %00010000
.define PFX_VIBRATO     %00010100
.define PFX_PORTA_UP    %00011000
.define PFX_PORTA_DOWN  %00011100
.define PFX_MASK        %00011100
.define PFX_VIBRATO_DIR %00100000   ; 0 = up, 1 = down
.define PFX_VIBRATO_TOG %01000000
.define STREAM_OFF      %00000000
.define STREAM_ENABLED  %10000000

.enum $100 EXPORT
    ; the following apply to all channels
    nesmus.notePtrL         ds 8
    nesmus.notePtrH         ds 8
    nesmus.noteRetPtrL      ds 8
    nesmus.noteRetPtrH      ds 8
    nesmus.noteLoopPtrL     ds 8
    nesmus.noteLoopPtrH     ds 8
    nesmus.noteLoopCount    ds 8
                            
    nesmus.streamFlags      ds 8    
    nesmus.streamTempo      ds 8
    nesmus.streamTimer      ds 8
    nesmus.streamVolume     ds 8
    nesmus.noteLength       ds 8
    nesmus.noteTimer        ds 8
    nesmus.curNote          ds 8
    nesmus.noteVolume       ds 8
    
    ; the following only apply to streams 0,1,3 (music, pulse & noise channels)
    nesmus.envId            ds 2    ; Envelope (pulse) or drum (noise) id
    nesmus.drumId           db
    nesmus.envOffs          ds 2    ; Envelope data offset
    nesmus.drumOffs         db
    nesmus.envValue         ds 2    ; Envelope value
    nesmus.drumValue        db
    nesmus.envTimer         ds 2    ; Envelope timer
    nesmus.drumTimer        db
    nesmus.drumBytes        db
    nesmus.drumLength       db
    
    ; the following only apply to pulse and triangle channels (streams 0,1,2,4,5,6)
    nesmus.periodL          ds 6
    nesmus.periodH          ds 6
    
    nesmus.noteDetune       ds 6    ; detune amount
    
    ; the following only apply to streams 0,1,2
    nesmus.pitchbendSpeed   ds 3    ; 
    nesmus.pitchbendDelta   ds 3    ; vibrato / sweep depth
    nesmus.pitchbendTimer   ds 3    ; 
    
    ; the following only apply to the pulse channels (streams 0,1,4,5)
    nesmus.pulseSweep       ds 4    ; hardware sweep registers ($4001, $4005)
    nesmus.prevPeriodH      ds 2    ; previous output to $4003 and $4007
    nesmus.prevPulseSweep   ds 2    ; previous output to sweep unit
    
    nesmus.sfxDuty          ds 2
.ende
