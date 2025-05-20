LLS := \
	build/RP2350/Common.cc.ll     \
	build/RP2350/ImageDef.cc.ll   \
	build/RP2350/VecTable.cc.ll   \
	build/FrameBuffer.rs.ll       \
	build/hdmi.cc.ll

all: _build_dirs build/hdmi2350.bin.uf2

build/hdmi2350.bin.uf2: build/hdmi2350.bin etc/bin2uf2.py
	etc/bin2uf2.py $<

build/hdmi2350.bin: build/hdmi2350.elf
	llvm-objcopy -D -O binary $< $@

build/hdmi2350.elf: build/hdmi2350.s etc/layout.ld etc/link_flags.txt etc/compile_flags.txt
	clang++ @etc/link_flags.txt @etc/compile_flags.txt -xassembler -o $@ build/hdmi2350.s

build/hdmi2350.s: build/hdmi2350.opt.ll
	llc -o $@ $<

build/hdmi2350.opt.ll: build/hdmi2350.ll etc/opt_flags.txt
	opt @etc/opt_flags.txt -S -o $@ $<

build/hdmi2350.ll: $(LLS)
	llvm-link -S -v -o $@ $(LLS)

build/%.cc.ll: %.cc **/*.h etc/compile_flags.txt
	clang++ @etc/compile_flags.txt -xc++ -S -emit-llvm -o $@ $<

build/%.rs.ll: %.rs etc/rustc_flags.txt
	rustc @etc/rustc_flags.txt -o $@ $<

_build_dirs:
	mkdir -p build build/RP2350

clean:
	rm -rf build/
