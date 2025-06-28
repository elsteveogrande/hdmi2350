LLS := \
	build/RP2350/Common.cc.ll     		\
	build/RP2350/ImageDef.cc.ll   		\
	build/RP2350/ResetHandler.cc.ll   \
	build/RP2350/VecTable.cc.ll   		\
	build/runtime/Panic.cc.ll     		\
	build/runtime/String.cc.ll    		\
	build/hdmi/hdmi.cc.ll

all: _build_dirs build/hdmi2350.bin.uf2

build/hdmi2350.bin.uf2: build/hdmi2350.bin etc/bin2uf2.py
	etc/bin2uf2.py $<

build/hdmi2350.bin: build/hdmi2350.elf
	llvm-objcopy -D -O binary $< $@

build/hdmi2350.elf: build/hdmi2350.s rp2350/exceptions.s etc/layout.ld etc/link_flags.txt etc/compile_flags.txt
	clang++ \
		@etc/link_flags.txt \
		@etc/compile_flags.txt \
		-L build \
		-Wl,-strip-all \
		-g \
		-o $@ \
		-xassembler \
		rp2350/exceptions.s \
		build/hdmi2350.s

build/hdmi2350.s: build/hdmi2350.opt.ll
	llc -mcpu=cortex-m33 -o $@ $<

build/hdmi2350.opt.ll: build/hdmi2350.ll etc/opt_flags.txt
	# opt @etc/opt_flags.txt -S -o $@ $<
	cp build/hdmi2350.ll build/hdmi2350.opt.ll

build/hdmi2350.ll: $(LLS)
	llvm-link -S -v -o $@ $(LLS)

build/%.cc.ll: %.cc **/*.h etc/compile_flags.txt
	clang++ @etc/compile_flags.txt -xc++ -S -emit-llvm -o $@ $<

_build_dirs:
	mkdir -p build build/RP2350 build/runtime build/hdmi

clean:
	rm -rf build/
