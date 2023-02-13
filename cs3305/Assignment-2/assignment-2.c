/* ------------------------
SHUBH FAGERIA
ASSIGNMENT 2
CS3305
251005738
------------------------- */

#include <stdio.h>
#include <unistd.h> //fork()
#include <fcntl.h>  //open() (unbuffered open)
#include <sys/wait.h> //wait()
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

int main( int argc, char* argv[])
{
    int numA;
    int numB;
    
    //exit statement if the 2 numbers are not supplied
    if (argc < 3) {
        return 0;
    }

    //setting the command line arguments as number a and number b
    numA = atoi(argv[1]);
    numB = atoi(argv[2]);
    printf("\nNumbers entered are: %d and %d\n", numA, numB);
    
    int a1, b1, a2, b2;

    //splitting number a into a1*10^(n/2)+a2 where n is 4
    a1 = floor(numA / 100);
    a2 = numA - (a1*100);

    //splitting number b into b1*10^(n/2)+b2 where n is 4
    b1 = floor(numB / 100);
    b2 = numB - (b1*100);

    //creating two pipes
    //one for parent to write for child to read (port1)
    //other for child to write for parent to read (port2)
    int port1[2];
    int port2[2];

    //exit statements should pipe creation cause an error
    if (pipe(port1) < 0) {
        exit(1);
    }
    
    if (pipe(port2) < 0) {
        exit(1);
    }

    //forking child
    pid_t pid = fork();

    //parent
    if (pid > 0) {
        
        /*
            Calculating X
        */
        printf("###\n# Calculating X\n###\n");

        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);
        write(port1[1], &a1, sizeof(int)); //writing a1 to child through pipe port1
        write(port1[1], &b1, sizeof(int)); //writing b1 to child through pipe port1  
        
        int A;
        read(port2[0], &A, sizeof(A));  //reading A from pipe port2. A is equal to a1*b1
        printf("Parent (PID %d): Recieved %d from child\n", getpid(), A);
        
        double X = A * pow(10,4); //getting X value

        /*
            Calculating Y
        */
        
        printf("\n###\n# Calculating Y\n###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);        
        write(port1[1], &a2, sizeof(int));  //writing a2 to child through pipe port1
        write(port1[1], &b1, sizeof(int));  //writing b1 to child through pipe port1

        int B;
        read(port2[0], &B, sizeof(B)); //recieving B from pipe port2. B is equal to a2*b1
        printf("Parent (PID %d): Recieved %d from child\n", getpid(), B);
        
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);
        write(port1[1], &a1, sizeof(int));  //writing a1 to child through pipe port1
        write(port1[1], &b2, sizeof(int));  //writing b2 to child through pipe port1

        int C;
        read(port2[0], &C, sizeof(C)); //recieveing C from pipe port2. C is equal to a1*b2
        printf("Parent (PID %d): Recieved %d from child\n", getpid(), C);
        double Y = (B+C) * pow(10,2);

        
        printf("\n###\n# Calculating Z\n###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);
        write(port1[1], &a2, sizeof(int)); //writing a2 to child through pipe port1
        write(port1[1], &b2, sizeof(int)); //writing b2 to child through pipe port1

        /*
            Calculating Z
        */
        
        int Z;
        read(port2[0], &Z, sizeof(Z)); //recieving Z from pipe port2. Z is equal to a2*b2
        printf("Parent (PID %d): Recieved %d from child\n", getpid(), Z);
        
        
        wait(NULL); //waiting for child to finish to exit

        printf("\n%d*%d == %.0f + %.0f + %d == %.0f\n", numA, numB, X, Y, Z, X+Y+Z); //printing final value X+Y+Z

    }

    //child
    if (pid == 0) {
        int mul1;
        int mul2;

        /*
            Calculating X
        */

        read(port1[0], &mul1, sizeof(int));  //reading a1 from parent through pipe port1
        
        read(port1[0], &mul2, sizeof(int));  //reading b1 from parent through pipe port1
        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul1);
        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul2);
       
        int send = mul1 * mul2;  //calculating value to return -> A = a1*b1

        
        write(port2[1], &send, sizeof(send));  //writing return value A to pipe port[2] for parent to read
        printf("    Child (PID %d): Sending %d to parent\n", getpid(), send);

        /*
            Calculating Y
        */
        
        read(port1[0], &mul1, sizeof(int));  //reading a2 from parent through pipe port1
        
        read(port1[0], &mul2, sizeof(int));  //reading b1 from parent through pipe port1
        
        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul1);
        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul2);

        send = mul1 * mul2;  //calculating value to return -> B = a2*b1

        write(port2[1], &send, sizeof(send));  //writing return value B to pipe port[2] for parent to read
        printf("    Child (PID %d): Sending %d to parent\n", getpid(), send);


        //read a1,b2
        read(port1[0], &mul1, sizeof(int));  //reading a1 from parent through pipe port1
        read(port1[0], &mul2, sizeof(int));  //reading b2 from parent through pipe port1

        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul1);
        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul2);
        send = mul1 * mul2;  //calculating value to return -> C = a1*b2


        write(port2[1], &send, sizeof(send));  //writing return value C to pipe port[2] for parent to read
        printf("    Child (PID %d): Sending %d to parent\n", getpid(), send);

        /*
            Calculating Z
        */
       
        read(port1[0], &mul1, sizeof(int));  //reading a2 from parent through pipe port1
        read(port1[0], &mul2, sizeof(int));  //reading b2 from parent through pipe port1

        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul1);
        printf("    Child (PID %d): Recieved %d from parent\n", getpid(), mul2);
        send = mul1 * mul2;  //calculating value to return -> D = a1*b2

        write(port2[1], &send, sizeof(send));  //writing return value D to pipe port[2] for parent to read
        printf("    Child (PID %d): Sending %d to parent\n", getpid(), send);
    }

    return 1;
}


