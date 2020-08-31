// scp archivo.c pi@192.168.43.39:~/embebidos/

fe_serie
printf

dato = 0;
for(i = 0; i < N; i++)
{
	read(fd_serie, &dato, 1);
	printf("%x \n", dato);
	pulso[i] = dato;
	i  = i & 0xFFF;
	// Ventaneo, autocorr, 
}

guardaDatos(pulso);
close(fd_serie);