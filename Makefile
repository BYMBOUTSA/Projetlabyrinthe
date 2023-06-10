CC = g++             # le compilateur à utiliser
GTKMM_CF = `pkg-config gtkmm-3.0 --cflags`
GTKMM_LD = `pkg-config gtkmm-3.0 --libs`
CFLAGS = $(GTKMM_CF) -Wall -pedantic # les options du compilateur
LDFLAGS = $(GTKMM_LD)      # les options pour l'éditeur de liens
SRC = main.cpp modele.cpp vue.cpp grille.cpp #controller.cpp# les fichiers sources
PROG = main.out     # nom de l'exécutable
OBJS =  $(SRC:.cpp=.o) # les .o qui en découlent
.SUFFIXES: .cpp .o     # lien entre les suffixes

all: $(PROG)

# étapes de compilation et d'édition de liens
# $@ la cible  $^ toutes les dépendances	
$(PROG): $(OBJS)
	$(CC)  -o $@ $^ $(LDFLAGS)


# le lien entre .o et .c
# $< dernière dépendance	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# pour faire propre
.PHONY: clean
clean:
	rm -f *.o *~ core $(PROG)



