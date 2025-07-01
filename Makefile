# MacOS:
#   brew install llvm lldb lld openocd

OBJS := \
	build/rp2350/Common.cc.o     		\
	build/rp2350/Faults.s.o         \
	build/rp2350/ImageDef.cc.o   		\
	build/rp2350/Init.cc.o      		\
	build/rp2350/ResetHandler.cc.o  \
	build/rp2350/SysTick.cc.o    	  \
	build/rp2350/VecTable.cc.o   		\
	build/runtime/ABI.cc.o     			\
	build/runtime/Init.cc.o     		\
	build/runtime/Panic.cc.o     		\
	build/runtime/RuntimeData.cc.o  \
	build/runtime/String.cc.o    		\
	build/hdmi/hdmi.cc.o

all: _build_dirs build/hdmi2350.bin.uf2

start_openocd:
	# See "Debug with a second Pico or Pico 2" here:
	#   https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
	#
	# NOTE:  My brew-installed openocd failed to establish a connection with lldb.
	# A build from source worked better: get openocd from git, and do:
	#   brew install automake jimtcl ; cd openocd ; ./bootstrap ; ./configure --prefix=/opt
	#
	/opt/openocd/bin/openocd \
		-f interface/cmsis-dap.cfg \
		-f target/rp2350.cfg \
		-c "adapter speed 1000"

flash: build/hdmi2350.elf
	# Uploads .ELF directly (not bin / uf2) via openocd
	echo "program build/hdmi2350.elf verify" | nc localhost 4444

lldb: build/hdmi2350.elf
	# Also see: https://web.navan.dev/posts/2024-08-08-openocd-macos-lldb.html
	#   platform select remote-gdb-server
	#   platform connect connect://localhost:3333
	lldb build/hdmi2350.elf

build/hdmi2350.bin.uf2: build/hdmi2350.bin etc/bin2uf2.py
	etc/bin2uf2.py $<

build/hdmi2350.bin: build/hdmi2350.elf
	llvm-objcopy -D -O binary $< $@

build/hdmi2350.elf: $(OBJS) etc/layout.ld etc/link_flags.txt etc/compile_flags.txt
	clang++ \
		@etc/link_flags.txt \
		-L build \
		-o $@ \
		$(OBJS)

build/%.cc.o: %.cc **/*.h etc/compile_flags.txt
	clang++ @etc/compile_flags.txt -c -o $@ $<

build/%.s.o: %.s
	clang++ @etc/compile_flags.txt -xassembler -c -o $@ $<

_build_dirs:
	mkdir -p build build/rp2350 build/runtime build/hdmi

clean:
	rm -rf build/
