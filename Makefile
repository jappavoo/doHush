UBOOTREPO=https://gitlab.denx.de/u-boot/u-boot.git

.phony: clean

doHush: main.c u-boot/common/cli_hush.o  u-boot/cmd/help.o u-boot/common/command.o u-boot/lib/ctype.o 
	cc -Wl,-T u-boot/u-boot.lds -I u-boot/include -g -Wl,-Map=doHush.map -Wl,--verbose  -o $@ $? u-boot/cmd/echo.o

doHushNoLIBS: main.c u-boot/common/cli_hush.o u-boot/common/command.o u-boot/lib/ctype.o
	cc --static -I u-boot/include -g -D NOLIBS  -Wl,-Map=doHush.map -Wl,--verbose  -nodefaultlibs  -o $@ $?

u-boot/common/cli_hush.o: u-boot/common/cli_hush.c
	make -C u-boot sandbox_defconfig defconfig NO_SDL=1
	make -C u-boot KBUILD_VERBOSE=1 sandbox_defconfig common NO_SDL=1

u-boot/common/command.o:
	make -C u-boot sandbox_defconfig defconfig NO_SDL=1
	make -C u-boot sandbox_defconfig KBUILD_VERBOSE=1 common NO_SDL=1

u-boot/lib/ctype.o: u-boot/lib/ctype.c
	make -C u-boot sandbox_defconfig defconfig NO_SDL=1
	make -C u-boot sandbox_defconfig KBUILD_VERBOSE=1 lib NO_SDL=1

u-boot/cmd/help.o: u-boot/cmd/help.c
	make -C u-boot sandbox_defconfig defconfig NO_SDL=1
	make -C u-boot sandbox_defconfig KBUILD_VERBOSE=1 cmd NO_SDL=1

u-boot/common/cli_hush.c:
	git clone ${UBOOTREPO}


clean:
	make -C u-boot clean
	-rm main.o doHush doHushNoLIBS *.map
