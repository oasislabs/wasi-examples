
%.wasm.full: %.c
	clang $< -g -o $@

%.wasm.dwarf: %.wasm.full
	llvm-dwarfdump $< > $@

%.wasm: %.wasm.full %.wasm.dwarf
	../vendor/emscripten/tools/wasm-sourcemap.py $< -w $@ -p $(CURDIR) -s -u ./$(@:.wasm=.wasm.map) -o $(@:.wasm=.wasm.map) --dwarfdump-output=$(@:.wasm=.wasm.dwarf)


all: rot13.wasm
.PHONY: all