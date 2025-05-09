BOOT_LLS := \
	build/init/image_def.ll \
	build/init/reset.ll \
	build/main.ll \
	build/Util.ll \

all: _build_dirs build/hdmi.bin.uf2

_build_dirs:
	mkdir -p build build/init || true

build/hdmi.bin.uf2: build/hdmi.bin bin2uf2.py
	./bin2uf2.py $<

build/hdmi.bin: build/hdmi.elf
	llvm-objcopy -D -O binary $< $@

build/hdmi.elf: build/hdmi.s misc/layout.ld
	clang++ @link_flags.txt @compile_flags.txt -xassembler -o $@ build/hdmi.s

build/hdmi.s: build/hdmi.opt.ll
	llc -o $@ $<

build/hdmi.opt.ll: build/hdmi.ll
	opt -S @opt_flags.txt -o $@ $<

build/hdmi.ll: $(BOOT_LLS)
	llvm-link -S -v -o $@ $(BOOT_LLS)

build/%.ll: src/%.cc src/**/*.h
	clang++ @compile_flags.txt -xc++ -S -emit-llvm -o $@ $<

clean:
	rm -rf build/
