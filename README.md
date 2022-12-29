# calculator
simple console calculator

## Two types of project files

### 1a. calculator.pro (qmake)

### 1b. CMakeLists.txt (cmake)

#### (2). cpp/CMakeLists.txt (cmake + flex + bison)

#### (3). java/make.sh (jflex + byaccj)

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
