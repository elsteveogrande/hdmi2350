OBJS := \
	build/rp2350/Common.cc.o     		\
	build/rp2350/ImageDef.cc.o   		\
	build/rp2350/ResetHandler.cc.o  \
	build/rp2350/VecTable.cc.o   		\
	build/runtime/ABI.cc.o     			\
	build/runtime/Panic.cc.o     		\
	build/runtime/String.cc.o    		\
	build/rp2350/Faults.s.o         \
	build/hdmi/hdmi.cc.o

all: _build_dirs build/hdmi2350.bin.uf2

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
