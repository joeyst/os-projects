files:
	gcc -Wall -Wextra -o pcseml pcseml.c eventbuf.c -lpthread

.SILENT:
run:
	make files
	./pcseml 20 20 20 20
	rm pcseml
