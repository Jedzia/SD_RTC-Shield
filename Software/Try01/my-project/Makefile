PROJECT = awesomesauce
BUILD_DIR = bin

SHARED_DIR = ../my-common-code
CFILES = my-project.c ds1307.c ds_common.c common_lib/utils.c common_lib/i2c.c common_lib/usart.c
CFILES += api.c
AFILES += api-asm.S

# TODO - you will need to edit these two lines!
DEVICE=stm32f401ret6
OOCD_FILE = board/st_nucleo_f4.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
