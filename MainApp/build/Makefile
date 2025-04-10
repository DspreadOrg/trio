# Output file basename

BIN = output
OBJ = obj
SRC =../source
INC =../include
LIB =../libs
CROSS = riscv-none-embed-

# Compilation tools
CC = $(CROSS)gcc 
LD = $(CROSS)gcc

SIZE = $(CROSS)size
STRIP = $(CROSS)strip
OBJCOPY = $(CROSS)objcopy

CFLAGS = -D__riscv_float_abi_soft -march=rv32imc -mabi=ilp32 -std=gnu99 -fno-strict-aliasing -fsigned-char
CFLAGS += -ffunction-sections -fdata-sections -fstack-usage
#CFLAGS += -fstack-usage
#CFLAGS += -Os -g3 -c -I $(SRC)/system/inc -I $(SRC)  -I $(INC) 
CFLAGS += -Os -g3 -c -I $(SRC) -I $(INC)
CFLAGS += -DAPP_NAME='"$(NAME)"' -DAPP_VER='"$(VER)"'
CFLAGS += -I ../include/third_lib
CFLAGS += -I ../source/tms-dspread

LIBS = -Wl,--start-group $(LIB)/os/LvosApiLib.a   $(LIB)/os/LiteMapiLib.a  -lm -Wl,--end-group
LIBS += $(LIB)/farsi_font/FarsiFontlib_1.01.a 	  $(LIB)/mbedtls/libmbedtls1.0.a 

LFLAGS  = -Tlink_ram.lds -nostartfiles -march=rv32imc -mabi=ilp32 
LFLAGS  +=  -fsigned-char -ffunction-sections -fdata-sections  -g3 -Xlinker --gc-sections -Wl,-Map,"risc-v.map"


#project file directory
VPATH += $(wildcard $(SRC)) $(wildcard $(SRC)/*/*)
VPATH += ../source/tms-dspread

ASM_OBJS = 

C_OBJS =  stb_truetype.o  app.o emv.o emvproc.o EmvL2SysApi.o file.o func.o lcddrawbmp.o trans.o ui.o utils.o paypassparam.o AE_param.o
C_OBJS += printer.o QrCode.o wifi_test.o wlan_test.o
C_OBJS += miniz_tdef.o miniz_tinfl.o miniz_zip.o miniz.o spng.o
C_OBJS += Liteui.o draw_png.o 
C_OBJS += lcdFontTest.o
C_OBJS += ota.o tms_dspread.o tms_dspread_platform.o lring.o
C_OBJS += tls_test.o


# Append OBJ and BIN directories to output filename
OUTPUT := $(BIN)/$(NAME)_$(VER)

C_OBJS_OUTPUT = $(addprefix $(OBJ)/, $(C_OBJS))
ASM_OBJS_OUTPUT = $(addprefix $(OBJ)/, $(ASM_OBJS))

all: $(ASM_OBJS_OUTPUT) $(C_OBJS_OUTPUT)
	echo $(LD) $(LFLAGS)  -o $(OUTPUT).elf $^ $(LIBS) $(LDFLAGS)
	echo start link....
	$(LD) $(LFLAGS)  -o $(OUTPUT).elf $^ $(LIBS) $(LDFLAGS)
#	$(STRIP) --strip-debug  --remove-section=.comment $^
#	$(STRIP) --strip-debug  --remove-section=.comment $(OUTPUT).elf
	$(OBJCOPY) -O binary $(OUTPUT).elf $(OUTPUT).bin
	$(CROSS)objdump -D $(OUTPUT).elf > $(BIN)\asm.txt

$(C_OBJS_OUTPUT): $(OBJ)/%.o: %.c  
	echo CC $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(ASM_OBJS_OUTPUT): $(OBJ)/%.o: %.S 
	echo CC $<
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f $(OBJ)/*.o  $(OBJ)/image/*.o $(BIN)/*.*
 

 
 

