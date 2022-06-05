# Linux-kernel-c-projects

**NOTE** You can only run these projects inside a linux environment, otherwise they wont work. Also, recommended compiler is GCC.<br />

This is me experimenting with the linux kernel and system calls.<br />
Every file is an exploration of some of the stuff we can do with the linux kernel via Sys calls.<br />

Zad1 -> An experiment with doing sys calls for running linux commands in C.<br />
Zad2 -> A look at how we can create a program with a list of executable programms and run them upon selection.<br />

**The next set of programms is all about concurrent computing and exploring ways we solve famous problems when running multiple things at once**<br />
Zad3 -> Playing around with the famous Dekker's algorithm, making 2 threads and making them enter the same critical section<br />
Zad4 -> Solving the readers/writers problem using a concept called Semaphores<br />
Zad5 -> This is just experimenting with different ways to solve the "Dining philosophers problem", basically - what happens when we try to run multiple (5 in this example) threads at the same time.<br />
Zad5_semaphore -> solving Zad5 problem using a concept called semaphores<br />
Zad5_monitor -> solving Zad5 problem using a concept called monitors<br />

**RUNNING THE PROGRAMMS**<br />
* To generate an exe file from any of the c programms, open linux terminal in the same folder where you cloned your project and run:<br />
  gcc -o the_name_of_the_executable_programm file_name.c<br />
  the_name_of_the_executable_programm --> can be any name you like<br />
* To run the generated executable, in the same terminal type:<br />
  ./the_name_of_the_executable_programm<br />
