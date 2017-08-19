PLATFORM_TARGET := out/Platform/lib$(BOARD_NAME)-Platform.a

.PHONY: all Application clean

all: Application

Platform: $(PLATFORM_TARGET)

$(PLATFORM_TARGET):
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Platform -Bout/Platform
	make -C out/Platform

Application: $(PLATFORM_TARGET)
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Application -Bout/Application
	make -C out/Application

clean:
	rm -rf out
