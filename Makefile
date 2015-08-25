
myshell: *.c
	gcc *.c -I includes/ -o myshell
clean:
	rm myshell
