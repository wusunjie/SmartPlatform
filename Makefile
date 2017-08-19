
.PHONY: all Platform clean

all: Platform

Platform:
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Platform -Bout/Platform
	make -C out/Platform

clean:
	rm -rf out
