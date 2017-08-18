lib$(BOARD_NAME)-Platform.a:
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Platform -Bout/Platform
	make -C out/Platform

Application: out/Platform/lib$(BOARD_NAME)-Platform.a
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Application -Bout/Application
	make -C out/Application

clean:
	rm -rf out
