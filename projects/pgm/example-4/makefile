prog: src/main.c libimg.so
	gcc -o prog -I . -L . -fPIC src/main.c -l:libimg.so -l:libnetpbm.so.10

libimg.so: src/img.c src/img.h
	gcc -I . -fPIC -l:libnetpbm.so.10 -shared -o libimg.so src/img.c