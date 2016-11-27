include ../../config.mk
include config.mk

CFLAGS := -O3 -Wall -I"../../" $(CFLAGS)
NVFLAGS := -O3 -I"../../" $(NVFLAGS)

.PHONY: release debug all lib clean cmd libnnc-cmd.o $(CUDA_CMD_LIB)

release: all

debug: CFLAGS += -g -fno-omit-frame-pointer -fsanitize=address
debug: all

all: lib

lib: libnnc-cmd.o

clean:
	find . -type f -name '*.o' -exec rm {} +

cmd:
	./build-cmd.rb .

libnnc-cmd.o: $(CMD_OBJS) $(CUDA_CMD_LIB)
	ld -r $^ -o $@

libnnc-cmd-cuda.o: $(CUDA_CMD_OBJS)
	ld -r $^ -o $@

%.o: %.c ../../ccv.h ../../ccv_internal.h ../ccv_nnc_tfb.h ../ccv_nnc.h ../ccv_nnc_easy.h ../ccv_nnc_internal.h
	$(CC) $< -o $@ -c $(CFLAGS)

%.o: %.cu ../../ccv.h ../ccv_nnc_tfb.h ../ccv_nnc.h ../ccv_nnc_easy.h ../ccv_nnc_internal.h
	$(NVCC) $< -o $@ -c $(NVFLAGS)