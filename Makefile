all: _build_dir hdmi.bin.uf2

hdmi.bin.uf2: hdmi.bin binsum.py bin2uf2.py
	./binsum.py hdmi.bin
	./bin2uf2.py hdmi.bin

hdmi.bin: hdmi.elf
	llvm-objcopy -O binary hdmi.elf hdmi.bin

_build_dir:
	mkdir -p build

clean:
	rm -f build/

# hdmi.elf: boot2.o Boot.o hdmi.o layout.ld
# 	clang++ @link_flags.txt \
# 		-o hdmi.elf \
# 		boot2.o \
# 		boot.o \
# 		hdmi.o

# Boot.o: Boot.s
# 	clang++ @compile_flags.txt -xassembler -c -o $@ $<

# hdmi.o: hdmi.s
# 	clang++ @compile_flags.txt -xassembler -c -o hdmi.o hdmi.s

# hdmi.s: hdmi.cc **/*.h
# 	clang++ @compile_flags.txt -xc++ -c -S -o hdmi.s hdmi.cc

# boot2.o: boot/boot2.s
# 	clang++ @compile_flags.txt -xassembler -c -o boot2.o $<

# Boot.s: boot/Boot.cc hdmi.cc **/*.h
# 	clang++ @compile_flags.txt -xc++ -c -S -o $@ -Oz $<
