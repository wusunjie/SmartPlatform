Platform:
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Platform -BPlatform_out
	make -C Platform_out

Application: Platform
	cmake -DSTM32_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake/Application -BApplication_out
	make -C Application_out

clean:
	rm -rf Platform_out Application_out
