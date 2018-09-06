demo.nes:	nesmus.l demo.65s
	wla-6502 demo.65s
	wlalink -S linkfile demo.nes
nesmus.l:	nesmus.65s $(wildcard nesmus_*)
	wla-6502 -l nesmus.l nesmus.65s
	
clean:
	cmd /c "del nesmus.l demo.o demo.sym demo.nes"