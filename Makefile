CC := g++

DED_FLAGS := -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ \
        -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual \
        -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body \
        -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness \
        -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked \
        -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion \
        -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default \
        -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
        -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast \
        -Wno-varargs -Wstack-protector -fsized-deallocation -fstack-protector \
		-fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr # -fcheck-new


all:
	@mkdir File_png
	@mkdir File_txt
	@g++ Dif.cpp -o a.out
	@./a.out

clean:
	@rm -rf a.out
	@rm -rf File_png
	@rm -rf File_txt
	@rm -rf Log_File.html
	@rm -rf a.out.dSYM
	@clear
	@clear
