CXX = $(shell [ `g++ -v 2>&1 | grep -o 'version [0-9]\+' | cut -d' ' -f2` -le 9 ] && echo g++-10 || echo g++)
# Si la ligne précédente provoque une erreur, la supprimer et décommenter la ligne suivante
# CXX = g++-10

# Sur Mac, ou en général si make ne marche pas, décommentez les lignes suivantes
#CXXFLAGS += $(shell fltk-config --cxxflags | sed 's/-I/-isystem/g')
#LDFLAGS  += $(shell fltk-config --ldflags)

### MINI TUTO MAKEFILE - PARTIE 3 ###
# Voir le Makefile des TP précédents pour le début.
#
# Le Makefile du dernier TP contenait ces deux recettes.
# 
# %: %.cpp
# 	${CXX} ${CXXFLAGS} $< -o $@ ${LDFLAGS}
# lab3: lab3.cpp
# 	${CXX} ${CXXFLAGS} $< -o $@ ${LDFLAGS}
#
# La première est une recette universelle, mais qui ne compte pas comme recette par défaut.
# Avec seulement la première cette, il aurait fallut faire "make lab3" à chaque fois, plutôt qu'un simple "make".
# On a donc été obligé d'ajouter la deuxième recette, avec les mêmes étapes.
#
# Une meille solution est la recette vide.
# Si je fais une recette vide, make va comme d'habitude d'abord créer ou mettre à jour les dépendance, puis faire la recette, c'est-à-dire rien.
# La dernière fois on aurait pu donc écrire la recette:
#
# default: lab3
#
# Le seul problème avec cette technique: si il existe un fichier default, make peut croire qu'il n'a pas besoin de recompiler.
# Ou de façon général, si la recette ne produit pas la cible (indiquée avant le ":"), alors make doit en être informé.
# Pour le faire, on dit que la cible est une dépendance de la cible spéciale ".PHONY"
#
# Ce Makefile contient des cibles PHONY.
# Vous verrez qu'elles sont bien pratiqued pour faire certaines actions avec "make cible", alors que "cible" n'est pas quelque chose à créer.
#
# Suite au prochain Makefile.
#####################################

# Variables
CXXFLAGS += -std='c++20' -Wall -Wextra -pedantic 
LDFLAGS  += -lfltk -lfltk_images

# Première recette = recette par défaut.
.PHONY: default # Attention: cette recette ne fabrique pas la cible (pas de fichier "default" créé)
default: lab4 # Par défaut, faire le lab4

# Recette magique
%: %.cpp
	${CXX} ${CXXFLAGS} $< -o $@ ${LDFLAGS}

# make clean supprime les fichiers objets
.PHONY: clean
clean:
	rm -f *.o

# make mrclean supprime les fichiers objets et les exécutables
.PHONY: mrclean
mrclean: clean
	rm -f lab1 lab2 lab3 lab4 lab1sol lab2sol lab3sol lab4sol lab10sol main

