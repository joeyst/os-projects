
mkfs: simfs.a mkfs.o 
	gcc -o $@ $^

simfs.a: image.o block.o free.o inode.o pack.o mkfs.o 
	ar rcs $@ $^

image.o: image.c
	gcc -Wall -Wextra -c $<

block.o: block.c
	gcc -Wall -Wextra -c $<

free.o: free.c
	gcc -Wall -Wextra -c $<

inode.o: inode.c
	gcc -Wall -Wextra -c $<

mkfs.o: mkfs.c
	gcc -Wall -Wextra -c $<

pack.o: pack.c
	gcc -Wall -Wextra -c $<

simfs_test: simfs_test.c simfs.a
	gcc -Wall -Wextra -o $@ $^ -DCTEST_ENABLE 

.PHONY: clean test

clean:
	rm -f *.o
	rm -f filename.txt
	rm -f simfs_test
	rm -f simfs.a
	rm -f image_file.txt

test: simfs_test
	./simfs_test

.SILENT: run
run:
	make clean
	make simfs_test
	./simfs_test
	make clean 
	