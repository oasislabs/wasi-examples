NAME=rot13

$(NAME).wasm: $(NAME).c
	clang $(NAME).c -g -o $(NAME).wasm.full
	llvm-dwarfdump $(NAME).wasm.full > $(NAME).wasm.dwarf
	../vendor/emscripten/tools/wasm-sourcemap.py $(NAME).wasm.full -w $(NAME).wasm -p $(CURDIR) -s -u ./$(NAME).wasm.map -o $(NAME).wasm.map --dwarfdump-output=$(NAME).wasm.dwarf
