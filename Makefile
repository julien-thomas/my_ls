CC=gcc
CFLAGS=-Wall -Wextra -Werror
DEPS=listing.h sort.h
EXEC=my_ls

all : $(EXEC)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

my_ls : my_ls.o listing.o sort.o
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm -rf *.o

fclean : clean
	rm -rf *.o $(EXEC)

re : fclean all

help :
	@echo "Options disponibles :"
	@echo "    make all      : Compiler le projet"
	@echo "    make clean    : Supprimer les fichiers objets"
	@echo "    make fclean   : Supprimer les fichiers objets et l'exécutable"
	@echo "    make re       : Supprimer les fichiers objets et l'exécutable et compiler le projet"
	@echo "    make help     : Afficher cette aide"