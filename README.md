# convex-hull
Experimentation with parallelizing convex hull and applying it to some shortest paths problems

###Command line parameters
* <code>-d</code> -> specifies to run in debug mode.
* <code>-D</code> -> runs unit tests.
* <code>-f filepath</code> -> specifies a file from which to load input data.
* <code>-m integer</code> -> the minimum x and y coordinate value for auto-generated data.
* <code>-M integer</code> -> the maximum x and y coordinate value for auto-generated data.
* <code>-n integer</code> -> the number of elements to process.
* <code>-o filepath</code> -> specifies a file to write the auto generated data to. This can be used in later process runs by using the <code>-f</code> parameter.
* <code>-r integer</code> -> the number of times to run each algorithm. If this is greater than one, a summary section will display the statistics for each algorithm.
* <code>-t integer</code> -> the number of threads to run.

###Example
This will run the process (named a.out) with 3200 points with 32 threads and 11 runs and restricts the coordinates to the rectangle defined by (-100, -100) to (100, 100) and run unit tests. The order of the parameters does not matter.

<code>./a.out -n 3200 -t 32 -r 11 -m -100 -M 100 -D</code>
