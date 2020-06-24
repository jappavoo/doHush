UBOOTREPO=https://gitlab.denx.de/u-boot/u-boot.git

.phony: clean

doHush: main.c u-boot/common/cli_hush.o u-boot/common/command.o u-boot/lib/ctype.o
	cc -I u-boot/include -g -Wl,-Map=doHush.map -Wl,--verbose  -o $@ $?

doHushNoLIBS: main.c u-boot/common/cli_hush.o u-boot/common/command.o u-boot/lib/ctype.o
	cc --static -I u-boot/include -g -D NOLIBS  -Wl,-Map=doHush.map -Wl,--verbose  -nodefaultlibs  -o $@ $?

u-boot/common/cli_hush.o: u-boot/common/cli_hush.c
	cd u-boot
	make sandbox_defconfig common NO_SDL=1

u-boot/common/command.o:
	cd u-boot
	make sandbox_defconfig common NO_SDL=1

u-boot/lib/ctype.o: u-boot/lib/ctype.c
	cd u-uboot
	make sandbox_defconfig lib NO_SDL=1

u-boot/common/cli_hush.c:
	git clone ${UBOOTREPO}


clean:
	-rm main.o doHush doHushNoLIBS *.map
