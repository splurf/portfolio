prog: libimg.so src/main.c src/img.h
	gcc -o prog src/main.c -L . -l img

libimg.so: src/img.c src/img.h
	gcc src/img.c -fPIC -shared -o libimg.so