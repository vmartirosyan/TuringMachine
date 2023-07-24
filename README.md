A turing machine simulator alog with a compiler which compiles code written in some "high level" language (see example below) to a Turing machine configuration.

Code exmaple: 

<pre>
while 1
    go right
if *
    write 1
    go right
while 1
    go right
if E
    go left
if 1
    write E
    go left
if 1
    write E
    go left
while 1
    go left
</pre>

The code will be translated into something like this:

<pre>
*, 1, E
q0, q1, q2, q3, q4


 1, q1, >	 | 1, q0, >	 |	X	 
	X	 | 1, q1, >	 | E, q2, <	 
	X	 | E, q3, <	 |	X	 
	X	 | E, q4, <	 |	X	 
	X	 | 1, q4, <	 |	X	
</pre>

This configuration file can be consumed by the simulator.
