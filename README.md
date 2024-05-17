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

To execute the general game write the command below (default case: no meeple mode):
 - $> ./install/server install/player1.so install/player2.so

To execute the general game with particular seed you can write:
 - $> ./install/server install/player1.so install/player2.so -s=your_number

To execute the general game with infinit meeple you can write:
 - $> ./install/server install/player1.so install/player2.so -m 1

To execute the general game with seven meeple you can write:
 - $> ./install/server install/player1.so install/player2.so -m 2

To execute the general game with debug informations you can write:
 - $> ./install/server install/player1.so install/player2.so -d


You can mix option s, m and d. For exemple you can write :
./install/server install/player1.so install/player2.so -s=1715953665 -m 1 -d


To execute the tests write the command below :
 - $> ./install/alltests

## how to display the graph :

To display the graph write after execution of server the command below :
 - $> neato -Tx11 server_graph.dot &

## how to display the coverage :

To display the coverage write after execution of server the command below :
 - $> gcov *.o

## main link:

link of subject :

https://www.labri.fr/perso/renault/working/teaching/projets/2023-24-S6-C-Carcassonne.php

link of thor :

https://thor.enseirb-matmeca.fr/ruby/projects/pr105-carc
