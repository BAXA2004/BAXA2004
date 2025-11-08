# Calculator Program

This project implements a simple calculator that can parse and evaluate mathematical expressions. It is structured to separate the implementation, header files, and tests for better organization and maintainability.


## Getting Started
Useful commands

Going to the folder where is our file located
Example: cd /c/Users/user/Desktop/"Lab 1"/calc-lab1

compilation
gcc -std=c17 -O2 -Wall -Wextra src/calc.c -o calc.exe -lm

creating test file
echo "here you can write your problem. Example: 2+4*8" > input.txt

run the code

./calc.exe input.txt


### Prerequisites

- A C compiler (e.g., GCC)
- Make utility
- A code editor (e.g., Visual Studio Code)

### Building the Project

To compile the project, you can use the provided Makefile. Run the following command in the terminal:

```
make
```

This will compile the source files and create the executable.

### Running the Calculator

After building the project, you can run the calculator executable. The program accepts mathematical expressions as input and evaluates them.

### Running Tests

To ensure the calculator functions as expected, you can run the test cases defined in `tests/test_calc.c`. Use the following command:

```
make test
```

### Debugging

You can debug the project using the configuration specified in `.vscode/launch.json`. Open the debugger in your code editor and start debugging the `calc` executable.

## Contributing

Feel free to contribute to this project by submitting issues or pull requests. Your feedback and contributions are welcome!

## License

This project is licensed under the MIT License - see the LICENSE file for details.