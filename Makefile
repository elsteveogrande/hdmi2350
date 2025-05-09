BOOT_LLS := \
	build/init/image_def.ll \
	build/init/reset.ll \
	build/util/Util.ll \
	build/hdmi.ll \

all: _build_dirs build/hdmi2350.bin.uf2

_build_dirs:
	mkdir -p \
			build \
			build/base \
			build/hardware \
			build/init \
			build/util \
		|| true

build/hdmi2350.bin.uf2: build/hdmi2350.bin scripts/bin2uf2.py
	scripts/bin2uf2.py $<

build/hdmi2350.bin: build/hdmi2350.elf
	llvm-objcopy -D -O binary $< $@

build/hdmi2350.elf: build/hdmi2350.s misc/layout.ld
	clang++ @etc/link_flags.txt @etc/compile_flags.txt -xassembler -o $@ build/hdmi2350.s

build/hdmi2350.s: build/hdmi2350.opt.ll
	llc -o $@ $<

build/hdmi2350.opt.ll: build/hdmi2350.ll
	opt @etc/opt_flags.txt -S -o $@ $<

build/hdmi2350.ll: $(BOOT_LLS)
	llvm-link -S -v -o $@ $(BOOT_LLS)

build/%.ll: %.cc **/*.h
	clang++ @etc/compile_flags.txt -xc++ -S -emit-llvm -o $@ $<

clean:
	rm -rf build/
