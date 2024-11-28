.PHONY: x clean tar

EJECUTABLE= practica_final

MODULOS:= practica_final.o\
         objetos_final.o\
         file_ply_stl.o\
      
LIBS:= -lglut -lGLU -lGL -lsupc++ -lm -lc -lX11 -lstdc++ -lpthread -L/usr/lib/nvidia-331
CPPFLAGS:= -Wall -fmax-errors=2 -g

CC= g++

x: $(EJECUTABLE)
	./$(EJECUTABLE) beethoven

$(EJECUTABLE): $(MODULOS)	
	$(CC) -o $(EJECUTABLE) $(LDFLAGS) $(MODULOS) $(LIBS) 

clean:
	rm -rf *.o $(EJECUTABLE)

