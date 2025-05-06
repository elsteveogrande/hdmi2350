BOOT_OBJS := \
	build/init/image_def.o \
	build/init/reset.o \

all: _build_dirs build/hdmi.bin.uf2

_build_dirs:
	mkdir -p build build/init || true

build/hdmi.bin.uf2: build/hdmi.bin bin2uf2.py
	./bin2uf2.py $<

build/hdmi.bin: build/hdmi.elf
	llvm-objcopy -D -O binary $< $@

build/hdmi.elf: $(BOOT_OBJS) misc/layout.ld
	clang++ @link_flags.txt -o $@ $(BOOT_OBJS)

build/%.o: build/%.s
	clang++ @compile_flags.txt -xassembler -c -o $@ $<

build/%.s: src/%.cc src/**/*.h
	clang++ @compile_flags.txt -xc++ -S -o $@ $<

clean:
	rm -rf build/
