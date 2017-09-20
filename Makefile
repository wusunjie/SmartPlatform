
.PHONY: all Platform clean

all: Platform

Platform:
	cmake -DSMART_PLATFORM_BOARD_NAME=$(BOARD_NAME) cmake -Bout
	make -C out

clean:
	rm -rf out
