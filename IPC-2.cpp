#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>


bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}


int get_nth_prime(int n) {
    int count = 0;
    int num = 2;
    while (count < n) {
        if (is_prime(num)) {
            ++count;
        }
        ++num;
    }
    return num - 1; 
}

int main() {
    int pipe1[2], pipe2[2]; 

    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Pipe creation failed!" << std::endl;
        return 1;
    }

    
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    }

    if (pid == 0) { 
        close(pipe1[1]);  
        close(pipe2[0]);  

        int m;
        read(pipe1[0], &m, sizeof(m));  

        
        std::cout << "[Child] Calculating " << m << "-th prime number..." << std::endl;
        int prime = get_nth_prime(m);

        
        write(pipe2[1], &prime, sizeof(prime));
        std::cout << "[Child] Sending calculation result of prime(" << m << ")..." << std::endl;

        close(pipe1[0]);  
        close(pipe2[1]);  
        exit(0);
    }
    else { 
        close(pipe1[0]);  
        close(pipe2[1]);  

        std::string input;
        while (true) {
            std::cout << "[Parent] Please enter the number: ";
            std::getline(std::cin, input);

            if (input == "exit") {
                std::cout << "[Parent] Exiting program..." << std::endl;
                break;
            }

            
            int m;
            std::stringstream(input) >> m;

            
            std::cout << "[Parent] Sending " << m << " to the child process..." << std::endl;
            write(pipe1[1], &m, sizeof(m));

            
            int result;
            std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;
            read(pipe2[0], &result, sizeof(result));
            std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << std::endl;
        }

        close(pipe1[1]);  
        close(pipe2[0]);  

        
        waitpid(pid, nullptr, 0);
    }

    return 0;
}
