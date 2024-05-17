# S6 project: Carcassonne in C

## how to compil

To compile the general game followed the instruction below:

 - check where is Igraph library on your computer
 - write this path in IGRAPH_PATH then write make
 - $> IGRAPH_PATH=/your_path_to_libigraph_repertory/ make
 
example on faustine's computer
IGRAPH_PATH=/home/faustine/Documents/lib/libigraph/ make

To compile tests followed the instruction below:

 - check where is Igraph library on your computer
 - write this path in IGRAPH_PATH then write make alltests
 - $> IGRAPH_PATH=/your_path_to_libigraph_repertory/ make

## how to execute

To execute the general game write the commande below:
 - $> ./install/server install/player1.so install/player2.so

To execute the general game with debug mode write the commande below :
 - $> ./install/server install/player1.so install/player2.so -d

To execute the tests write the commande below :
 - $> ./install/alltests

## how to display the graph :

To display the graph write the commande below :
 - $> neato -Tx11 server_graph.dot &

## main link:

link of subject :

https://www.labri.fr/perso/renault/working/teaching/projets/2023-24-S6-C-Carcassonne.php

link of thor :

https://thor.enseirb-matmeca.fr/ruby/projects/pr105-carc
