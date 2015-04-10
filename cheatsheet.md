# Initializing ChaiScript

```
chaiscript::ChaiScript chai; // loads stdlib from loadable module on file system
chaiscript::ChaiScript chai(chaiscript::Std_Lib::library()); // compiles in stdlib
```

# Adding Things To The Engine

## Adding a Function / Method / Member

### General

```
chai.add(chaiscript::fun(&function_name), "function_name");
chai.add(chaiscript::fun(&Class::method_name), "method_name");
chai.add(chaiscript::fun(&Class::member_name), "member_name");
```

### With Overloads 

#### Preferred

```
chai.add(chaiscript::fun<ReturnType (ParamType1, ParamType2)>(&function_with_overloads), "function_name");
```

#### Alternative

```
chai.add(chaiscript::fun(std::static_cast<ReturnType (*)(ParamType1, ParamType2)>(&function_with_overloads)), "function_name");
```

### Lambda

```
chai.add(
  chaiscript::fun<std::string (bool)>(
    [](bool type) { 
      if (type) { return "x"; } 
      else { return "y"; } 
    }), "function_name");
```

### Constructors

```
chai.add(chaiscript::constructor<MyType ()>(), "MyType");
chai.add(chaiscript::constructor<MyType (const MyType &)>(), "MyType");
```

## Adding Types

It's not strictly necessary to add types, but it helps with many things. Cloning, better errors, etc.

```
chai.add(chaiscript::user_type<MyClass>, "MyClass");
```

## Adding Objects

```
chai.add(chaiscript::var(somevar), "somevar"); // copied in
chai.add(chaiscript::var(std::ref(somevar), "somevar"); // by reference, shared between C++ and chai
auto shareddouble = std::make_shared<double>(4.3);
chai.add(chaiscript::var(shareddouble), "shareddouble"); // by shared_ptr, shared between c++ and chai
chai.add(chaiscript::const_var(somevar), "somevar"); // copied in and made const
chai.add_global_const(chaiscript::const_var(somevar), "somevar"); // global const. Throws if value is non-const
chai.add_global(chaiscript::var(somevar), "somevar"); // global non-const
```

# Executing Script

## General

```
chai.eval("print(\"Hello World\")");
chai.eval(R"(print("Hello World"))");
```

## Unboxing Return Values

### Prefered

```
chai.eval<double>("5.3 + 2.1"); // returns 7.4 as a C++ double
```

### Alternative

```
auto v = chai.eval("5.3 + 2.1");
chai.boxed_cast<double>(v); // extracts double value from boxed_value and applies known conversions
chaiscript::boxed_cast<double>(v); // free function version, does not know about conversions
```

### Converting Between Algebraic Types

```
chaiscript::Boxed_Number(chai.eval("5.3 + 2.1")).get_as<int>(); // works with any number type
// which is equivalent to, but much more automatic than:
static_cast<int>(chai.eval<double>("5.3+2.1")); // this version only works if we know that it's a double
```

## Sharing Values

```
double &d = chai.eval("var i = 5.2; i"); // d is now a reference to i in the script
std::shared_ptr<double> d = chai.eval("var i = 5.2; i"); // same result but reference counted

d = 3;
chai.eval("print(i)"); // prints 3
```

## Catching Eval Errors

```
try {
  chai.eval("2.3 + \"String\"");
} catch (const chaiscript::exception::eval_error &e) {
  std::cout << "Error\n" << e.pretty_print() << '\n';
}
```

## Catching Errors Thrown From Script

```
try {
  chai.eval("throw(runtime_error(\"error\"))", chaiscript::exception_specification<int, double, float, const std::string &, const std::exception &>());
} catch (const double e) {
} catch (int) {
} catch (float) {
} catch (const std::string &) {
} catch (const std::exception &e) {
  // This is the one what will be called in the specific throw() above
}
```

## Sharing Functions


```
auto p = chai.eval<std::function<std::string (double)>>("to_string"); 
p(5); // calls chaiscript's 'to_string' function, returning std::string("5")
```

Note: backtick treats operators as normal functions

```
auto p = chai.eval<std::function<int (int, int)>>(`+`); 
p(5, 6); // calls chaiscript's '+' function, returning 11
```

```
auto p = chai.eval<std::function<std::string (int, double)>>(fun(x,y) { to_string(x) + to_string(y); });
p(3,4.2); // evaluates the lambda function, returning the string "34.2" to C++
```

# Language Reference

## Variables

```
var i; // uninitialized variable, can take any value on first assignment;
auto j; // equiv to var

var k = 5; // initialized to 5 (integer)
var l := k; // reference to k
auto &m = k; // reference to k
```

## Built in Types

```
var v = [1,2,3u,4ll,"16", `+`]; // creates vector of heterogenous values
var m = ["a":1, "b":2]; // map of string:value pairs
```

## Functions

### General 

```
def myfun(x, y) { x + y; } // last statement in body is the return value
def myfun(x, y) { return x + y; } // equiv 
```

### Optionally Typed

```
def myfun(x, int y) { x + y; } // requires y to be an int
```

### With Guards

```
def myfun(x, int y) : y > 5 { x - y; } // only called if y > 5
```

### Methods

Methods and functions are mostly equivalent

```
def string::add(int y) { this + to_string(y); }
def add(string s, int y) { s + to_string(y); } //equiv functionality

// calling new function/method
"a".add(1); // returns a1
add("a", 1); // returns a1, either calling syntax works with either def above
```

### Lambdas

```
var l = fun(x) { x * 15; }
l(2) // returns 30

var a = 13
var m = fun[a](x) { x * a; } 
m(3); // a was captured (by reference), returns 39

var n = bind(fun(x,y) { x * y; }, _, 10);
n(2); // returns 20 
```



## ChaiScript Defined Types

Define a type called "MyType" with one member value "a" and a getter

### Preferred

```
class MyType {
  var value;
  def MyType() { this.value = "a"; }
  def get_value() { "Value Is: " + this.value; }
};
```

### Alternative 

```
attr MyType::value;
def MyType::MyType() { this.value = "a"; }
def MyType::get_value() { "Value Is: " + this.value; }
```

### Using

```
var m = MyType(); // calls constructor
print(m.get_value()); // prints "Value Is: a"
print(get_value(m)); // prints "Value Is: a"
```



# Built In Functions

## Evaluation

```
eval("4 + 5") // dynamically eval script string and returns value of last statement
eval_file("filename") // evals file and returns value of last statement
use("filename") // evals file exactly once and returns value of last statement
                // if the file had already been 'used' nothing happens and undefined is returned
```

Both `use` and `eval_file` search the 'usepaths' passed to the ChaiScript constructor


