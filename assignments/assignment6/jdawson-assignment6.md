1. The following algorithms shows another solution to the dining philosophers
problem using monitors. Compare this solution to the solution to the dining
philosophers using monitors on the slides. Report your conclusions.

![question-1](images/question-1.png)

2. Consider a sharable resource with the following characteristics:

(1) As long as there are fewer than three processes using the resource, new processes can start
using it right away.

(2) Once there are three process using the resource, all three must leave before any new
processes can begin using it. We realize that counters are needed to keep track of how many
processes are waiting and active, and that these counters are themselves shared resources that
must be protected with mutual exclusion. So we might create the following solution:

![question-2](images/question-2.png)

The solution appears to do everything right: All accesses to the shared variables are protected
by mutual exclusion, processes do not block themselves while in the mutual exclusion,
new processes are prevented from using the resource if there are (or were) three active users,
and the last process to depart unblocks up to three waiting processes.
a. The program is nevertheless incorrect. Explain why.
b. Suppose we change the if in line 6 to a while. Does this solve any problem in the
program? Do any difficulties remain?
c. Propose a solution that fixes the above algorithm and explain why it is correct.

3. Given the following state for the Banker’s Algorithm.

6 processes P0 through P5

4 resource types: A (15 instances); B (6 instances); C (9 instances); D (10 instances)
Snapshot at time T0:

![question-3](images/question-3.png)

a) Verify that the Available array has been calculated correctly.

b) Calculate the Need matrix.

c) Show that the current state is safe, that is, show a safe sequence of processes. In
addition, to the sequence show how the Available (working array) changes as each
process terminates.

d) Given the request (3,2,3,3) from Process P5. Should this request be granted? Why
or why not?

4. Consider a system with a total of 150 units of memory, allocated to three processes as shown:

![question-4](images/question-4.png)

Apply the banker’s algorithm to determine whether it would be safe to grant each of the
following requests. If yes, indicate a sequence of terminations that could be guaranteed possible.
If no, show the reduction of the resulting allocation table.
a. A fourth process arrives, with a maximum memory need of 60 and an initial needof 25 units.
b. A fourth process arrives, with a maximum memory need of 60 and an initial need of 35 units.

5. Suppose the following two processes, foo and bar are executed concurrently and share the
semaphore variables S and R (each initialized to 1) and the integer variable x (initialized to 0).

![question-5](images/question-5.png)

a) Can the concurrent execution of these two processes result in one or both being blocked
forever? If yes, give an execution sequence in which one or both are blocked forever.

b) Can the concurrent execution of these two processes result in the indefinite postponement of
one of them? If yes, give an execution sequence in which one is indefinitely postponed.
