# ultra-precise calculator
console calculator with controlled calculation precision

## Two types of project files

### 1a. calculator.pro (qmake)
`mkdir build && cd build && qmake .. && make DEFINES=-DUSE_VALUE`

### 1b. CMakeLists.txt (cmake)
`mkdir build && cd build && cmake .. && make CXX_DEFINES=-DUSE_VALUE`

#### (2). cpp/CMakeLists.txt (cmake + flex + bison)

#### (3). java/make.sh (jflex + byaccj)

*For 1a and 1b you must choose one of the following macros:*

- USE_VALUE
- USE_VERYLONG - Very Long Integer Class from *SymbolicC++ : An object oriented computer algebra system written in C++* by Tan Kiat Shi, Willi-Hans Steeb, Yorick Hardy
- USE_BIGINT - unsigned integers using a fixed number of bits from *C++ Cookbook* by D. Ryan Stephens, Christopher Diggins, Jonathan Turkanis, Jeff Cogswell

![calculator](https://user-images.githubusercontent.com/20319403/118359144-bc0cc580-b58a-11eb-83e1-3d3f0aabc0d8.png)

**Brief description:** a program for calculating the value of a given 
arithmetic expression, operating on numbers with unlimited degree of accuracy.

**Input:** the input is a string containing an arithmetic expression.

**Output:** the calculated value of the given expression and display it on the 
screen. After the output, the program goes into the mode of waiting for the 
input of the next expressions.

**Supported operations:**
- `+` *(addition)*
- `-` *(subtraction)*
- `*` *(multiplication)*
- `/` *(division)*
- `-` *(unary minus)*
- `^` *(exponentiation)*
- `!` *(factorial)*

**Supported functions:**
- `sin()` - sine
- `cos()` - cosine
- `tan()` - tangent
- `sqrt()` - square root

An expression with a unary minus must be enclosed in parentheses so that there 
is no two consecutive operation signs. For example:
```
3+(-2)
```
Before performing calculations and during calculations, it is necessary catch 
all kinds of errors. Among them, for example:

- *Division by zero*
- *The exponent must be an integer*
- *The expression under the factorial must be a natural number*
- *The square root function parameter must be a positive number*

Numeric constants can be integer or fractional. Separator of integer and the 
fractional part is the dot. In the presence of a fractional part, the whole can 
go down. For example:
```
1 56 3.5 .001
```
Additionally, the program specifies the degree of accuracy of calculations for 
division and calculation of function values. The number of characters after 
comma, rounding the result to the nearest. For example, the calculation 
accuracy is set to 5 decimal places, then the result dividing 1 by 3 will be 
equal to:
```
0.33333
```
