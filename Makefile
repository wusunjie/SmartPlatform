
.PHONY: all Platform clean

all: Platform

Platform:
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake -Bout
	make -C out

clean:
	rm -rf out
