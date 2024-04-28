<--------------------------------------------------README-------------------------------------------------->
The program can be compiled using the 'make' command.
The program can then be run using './findcomp [arguments]'.

An example command would be:
	./findcomp -s 50 10000 -t 171 -p -w out.pgm chess.pgm

The program takes some command line arguments:
-s <int> <int>: The minimum and maximum component size in pixels
-t <int>: The threshold value
-p: If included will print the component data to the console
-w <string>: If included will write the components to the output file specified
inputFile: The .pgm file from which the components will be extracted

The options -s, -t, -p, and -w are optional and can be specified in any order.
The inputFile is required and must be specifed last.

Output images will be generated in the directory.

The 'make clean' command can be run to clean the .o and findcomp files.

<---------------------------------------------------------------------------------------------------------->