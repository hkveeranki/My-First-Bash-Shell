
myshell: shellutil.c shell.c cd.c echo.c 
	gcc shellutil.c shell.c cd.c echo.c  -I includes/ -o myshell
clean:
	rm myshell
