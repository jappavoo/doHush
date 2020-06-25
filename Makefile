UBOOTREPO=https://gitlab.denx.de/u-boot/u-boot.git

.phony: clean


doHush:  u-boot/u-boot.lds main.o hello.o
	cc -Wl,-T u-boot/u-boot.lds -g -Wl,-Map=doHush.map -Wl,--verbose  -o $@ u-boot/common/cli_hush.o  u-boot/cmd/help.o u-boot/common/command.o u-boot/lib/ctype.o u-boot/cmd/echo.o hello.o main.o 

main.o: main.c
	cc  -I u-boot/include -g -c $?

hello.o: hello.c
	cc  -I u-boot/include -g -c $?

doHushNoLIBS: main.c u-boot/common/cli_hush.o u-boot/common/command.o u-boot/lib/ctype.o
	cc -Wl,-T u-boot/u-boot.lds -g -Wl,-Map=doHushNoLIBS.map -Wl,--verbose --static -I u-boot/include -g -D NOLIBS  -nodefaultlibs  -o $@ $?

u-boot/include/config.h: u-boot/common/cli_hush.c
	make -C u-boot sandbox_defconfig defconfig NO_SDL=1

u-boot/u-boot.lds: u-boot/include/config.h
	make -C u-boot KBUILD_VERBOSE=1 sandbox_defconfig all NO_SDL=1

u-boot/common/cli_hush.c:
	git clone ${UBOOTREPO}

clean:
	make -C u-boot clean
	-rm main.o doHush doHushNoLIBS *.map
