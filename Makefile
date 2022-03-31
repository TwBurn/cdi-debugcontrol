#Configuration, name, folders
NAME    = debugctl
OUTPUT  = out
BUILD   = build
SRC     = src

#COMPILER AND LIBRARY LOCATIONS
OS9C    = C:\CDI\OS9C
CLIB    = $(OS9C)\lib
MTOOLS  = C:\CDI\MASTER
PATH    = $(OS9C)\bin;$(MTOOLS);%PATH%

#COMPILER CONFIGURATION
CC      = xcc
CCFLAGS = -S -R=$(OUTPUT) -T=$(OUTPUT) -TO=osk -TP=68kI -l
ASFLAGS = -O=0 -S -R=$(OUTPUT) -T=$(OUTPUT) -TO=osk -TP=68kI
LD      = l68
MASTER  = vcdmastr.exe

#FILES TO COMPILE
FILES   = $(OUTPUT)\main.r $(OUTPUT)\video.r $(OUTPUT)\graphics.r $(OUTPUT)\input.r

#LINKER CONFIGURATION
LDPARAM = -a $(CLIB)\cstart.r $(FILES) -l=$(CLIB)\cdi.l -l=$(CLIB)\cdisys.l -l=$(CLIB)\clib.l -l=$(CLIB)\cio.l -l=$(CLIB)\math.l -l=$(CLIB)\sys.l -l=$(CLIB)\usr.l

cd: link
	$(MASTER) build.cd

rebuild: clean cd

cdi_link: $(FILES)
	crt_link -n=play $(LDPARAM) -o=$(BUILD)\$(NAME).app
	$(LD) -z=link.txt
	fixmod -ua=80ff $(BUILD)\$(NAME).app

link: $(FILES)
	crt_link -n=cdi_$(NAME) -o=$(BUILD)\$(NAME) $(LDPARAM)
	$(LD) -z=link.txt
	fixmod -uo=0.0 $(BUILD)\$(NAME)

$(OUTPUT)\graphics.r : $(SRC)\graphics.c
	$(CC) $(CCFLAGS) -O=2 $(SRC)\graphics.c

$(OUTPUT)\input.r : $(SRC)\input.c
	$(CC) $(CCFLAGS) -O=2 $(SRC)\input.c

$(OUTPUT)\main.r : $(SRC)\main.c
	$(CC) $(CCFLAGS) -O=2 $(SRC)\main.c

$(OUTPUT)\video.r : $(SRC)\video.c
	$(CC) $(CCFLAGS) -O=2 $(SRC)\video.c

clean:
	-@erase $(OUTPUT)\cm*
	-@erase $(OUTPUT)\*.tmp
	-@erase $(OUTPUT)\*.r
	-@erase $(BUILD)\$(NAME)

purge:
	-@erase $(OUTPUT)\cm*
	-@erase $(OUTPUT)\*.tmp
