Okay, I have reviewed the provided text "Audio Signal Processing in FAUST" by Julius O. Smith III. It's an excellent introduction and covers many fundamental aspects of the language.

Based on this text, I can enhance the previous LLM guide with several clarifications, specific examples, and nuances that are crucial for avoiding common errors.

Here is the **updated** Faust Language Syntax and Structure Guide, incorporating insights from the JOS tutorial:

---

## Faust Language Syntax and Structure Guide for Code-Generating LLMs (Enhanced)

**Objective:** This document provides a structured guide to the Faust (Functional Audio Stream) programming language's syntax and core structural elements. It is designed to help Large Language Models (LLMs) generate syntactically correct and structurally sound Faust code, minimizing common errors. Insights from J.O. Smith's "Audio Signal Processing in FAUST" tutorial are integrated.

**Core Philosophy:** Faust describes signal processors using a functional approach based on **block diagram algebra**. Understand that Faust code *describes a signal flow graph*, not a sequence of imperative commands. Every expression defines a block diagram with a specific number of inputs and outputs.

---

### 1. Fundamental Concepts

1.  **Signals:** The basic entity is a `signal`, representing a stream of samples over time. Signals are processed and transformed by functions/expressions.
2.  **Functions/Expressions:** These represent signal processors (block diagrams). They take a fixed number of input signals and produce a fixed number of output signals.
3.  **Immutability:** Signals themselves are immutable. Functions create *new* signals based on their inputs.
4.  **State Handling:** State is handled explicitly via recursion (`~`) or specific stateful primitives (`mem`, `@`, `rdtable`, `rwtable`). Avoid imperative-style variable updates.
5.  **Block Diagram Algebra:** The core of Faust structure relies on combining expressions using specific operators:
    *   **Sequential Composition (`:`):** Pipes the output(s) of the left expression (`A`) to the input(s) of the right expression (`B`). `A : B` means signal flows through A, then B. **Constraint:** `outputs(A) == inputs(B)`. `outputs(A:B) = outputs(B)`. Precedence: Highest.
    *   **Parallel Composition (`,`):** Places two expressions (`A`, `B`) side-by-side. `A , B` creates a combined processor. `inputs(A,B) = inputs(A) + inputs(B)`, `outputs(A,B) = outputs(A) + outputs(B)`. Precedence: Lowest.
    *   **Split Composition (`<:`):** Duplicates the *single set* of output signals from `A` to feed *multiple parallel* expressions (`B`, `C`, ...). `A <: B , C` duplicates `outputs(A)` to feed both `B` and `C`. **Constraint:** `outputs(A) == inputs(B) == inputs(C)`. `outputs(A<:B,C) = outputs(B) + outputs(C)`.
    *   **Merge Composition (`:>`):** Sums corresponding output signals from *parallel* expressions (`A`, `B`, ...) before feeding them into a *single* subsequent expression `C`. `A , B :> C` means outputs of A and B are combined (element-wise addition) before feeding C. **Constraint:** `outputs(A) == outputs(B) == inputs(C)`. `outputs(A,B:>C) = outputs(C)`. Often used implicitly for summation: `A, B :> _` is equivalent to `A, B : +` if A and B each have one output.
    *   **Recursive Composition (`~`):** Introduces stateful computation (feedback loops). `expression_defining_feedback ~ initial_state_expression`. The expression *before* `~` must compute the *next state* from the *current state* (which is fed back as the *last* input) and any other inputs. The expression *after* `~` provides the initial state value(s). See Section 4.5 for details.

---

### 2. Basic Syntax Elements

1.  **Comments:**
    *   Single-line: `// comment text`
    *   Multi-line: `/* comment text */`
2.  **Literals:**
    *   Integers: `10`, `-5`, `0` (Typically 32-bit, see Section 2.16).
    *   Floats: `1.0`, `3.14`, `-0.5`, `1e-3`. **Crucial:** Must contain `.` or `e` to be treated as float. `1/2` is float `0.5`, but `int(1/2)` is integer `0`.
    *   Strings: `"Hello"` (Used in `declare`, UI elements, `ffunction`). Not a signal type.
3.  **Identifiers:** Function and variable names. Start with a letter or underscore, followed by letters, numbers, or underscores. Case-sensitive.
4.  **File Extension:** `.dsp`
5.  **Basic Wire (`_`):** Represents a single identity connection (1 input, 1 output). `_` passes its input directly to its output. `_,_` represents two parallel wires. `si.bus(N)` is preferred for clarity with N > 2.
6.  **Termination (`!`):** Cuts a signal wire (1 input, 0 outputs). Used for discarding unwanted signals. Example: `process = bd : _,!; // Keep first output of bd, discard second.`
7.  **Constants (`0`, `1`, etc.):** Represent block diagrams with 0 inputs and 1 output, producing a constant signal stream.

---

### 3. Operators (Elementary Signal Processing Blocks)

Operators act on signals. Pay close attention to their standard number of inputs and outputs.

1.  **Arithmetic:** (Standard: Input: 2 signals, Output: 1 signal)
    *   `+` : Addition
    *   `-` : Subtraction
    *   `*` : Multiplication
    *   `/` : Division
    *   `^` : Power (Exponentiation)
    *   `%` : Modulo (Integer remainder)
2.  **Unary Minus Distinction:**
    *   `-x`: *Signal negation.* Rewrites to `0, x : -`. Produces a signal. (Input: 0, Output: 1)
    *   `-(x)`: *Subtraction Operator.* Functional notation for subtraction. `-(x)` rewrites to `_, x : -`. (Input: 1, Output: 1, subtracts `x` from the input signal).
3.  **Comparison:** (Input: 2 signals, Output: 1 signal - integer 0 or 1)
    *   `<`, `<=`, `>`, `>=`, `==`, `!=`
4.  **Logical / Bitwise:** (Input: 2 signals, Output: 1 signal - typically integer)
    *   `&` : Bitwise AND
    *   `|` : Bitwise OR
    *   `xor` : Bitwise XOR
    *   `<<` : Bitwise Left Shift
    *   `>>` : Bitwise Right Shift
    *   *Unary Bitwise NOT:* `~x` (Use `x xor -1` for clarity/portability as `~` is also recursion operator).
5.  **Signal Delay / Memory:**
    *   `@(delay_samples)`: Integer Delay primitive. **Constraint:** `delay_samples` *must* resolve to a compile-time constant integer. Syntax: `input_signal @ delay_samples`. (Input: 1, Output: 1).
    *   `mem`: One-sample delay primitive. Equivalent to `@(1)`. Syntax: `input_signal'`. (Input: 1, Output: 1).
    *   `rdtable`, `rwtable`: Table read/write primitives for wavetables, delay lines etc.
6.  **Mathematical Functions:** Many standard math functions (`sin`, `cos`, `abs`, `exp`, `log`, `sqrt`, etc.) are built-in or available via `maths.lib` (`ma.` prefix). They typically take 1 input and produce 1 output.
7.  **Type Casting:**
    *   `int(x)`: Converts float signal `x` to integer signal using magnitude truncation (round towards zero). (Input: 1 float, Output: 1 int).
    *   `float(x)`: Converts integer signal `x` to float signal. (Input: 1 int, Output: 1 float). Automatic conversion often occurs when ints and floats mix.

---

### 4. Defining Functions, Variables, and Scope

1.  **Main Entry Point (`process`):** Every Faust program MUST define `process`. This is the top-level block diagram.
    *   Syntax: `process = expression;`
2.  **Function Definition:** Defines a reusable block diagram.
    *   Syntax: `identifier(param1, param2, ...) = expression;`
    *   Parameters (`param1`, etc.) are typically simple identifiers representing input signals (formal parameters).
    *   **Rewriting:** `f(x)` often rewrites to `x : f`, and `x * y` to `x, y : *`. Parentheses are crucial to control this.
    *   **Pattern Matching:** Parameters can also be complex *patterns* instead of simple names (See Section 2.23 of JOS tutorial). Requires extra parentheses around the pattern, e.g., `myfunc((pattern)) = expression;`. This distinguishes it from formal parameters. Lexical order matters for pattern matching definitions. Formal parameters *do not* participate in pattern matching uniqueness checks ( `f(x)=...` and `f(x,y)=...` might conflict if not distinguished by patterns like `f((x))=...` and `f((x,y))=...` ).
3.  **Variable Definition (Global):** Same syntax as function definition, often parameterless. Computed at compile time if possible.
    *   Example: `mypi = ma.PI;`
4.  **Local Definitions (`with`):** Define local identifiers within the scope of a parent expression. Does not introduce runtime overhead if definitions are constant.
    *   Syntax: `parent_expression with { local_def1; local_def2; ... };`
    *   Example: `process(x) = x * gain with { gain = 0.5; };`
5.  **Library Import (`library` / `import`):**
    *   `mylib = library("path/to/library.lib");` Creates a namespace `mylib.`
    *   `import("path/to/stdfaust.lib");` Imports definitions directly into the global namespace (use with caution, standard libraries like `stdfaust.lib` are generally safe).
6.  **Environments (`environment`):** Create encapsulated namespaces, similar to libraries but defined inline.
    *   Syntax: `myenv = environment { definition1; definition2; ... }; usage = myenv.definition1;`
7.  **Mutually Recursive Definitions (`letrec`):** For complex stateful systems (See guide Section 4.4). Used less commonly than standard recursion (`~`).

---

### 5. Recursion / Feedback (`~`) In Detail

1.  **Structure:** `loop_calculation ~ initial_state`
2.  **`loop_calculation`:** An expression that takes the *current state* as its **last** input(s) and potentially other external inputs. It must produce the **next state** as its **first** output(s), followed by any other outputs of the loop.
3.  **`initial_state`:** An expression defining the value(s) of the state variable(s) at time 0. Must match the number of state variables.
4.  **Implicit Delay:** Faust automatically inserts a one-sample delay (`mem` or `@(1)`) in the feedback path to make the recursion computationally possible. This delay *must* be accounted for in filter design.
5.  **Example (Smoother):**
    ```faust
    // Functional style: feedback_expr takes current state 'y' and input 'x'
    feedback_expr(y, x) = y + (1.0 - coeff) * (x - y);
    smooth(coeff, x) = feedback_expr ~ 0; // Initial state is 0

    // Inline style (more common):
    smooth(coeff, x) = _ ~ +((1.0 - coeff) * (x - _)); // '_' refers to the state variable y
    // JOS tutorial style (equivalent):
    smooth(coeff, x) = fb ~ _ with { fb(y) = y + (1.0 - coeff) * (x - y); };
    ```

---

### 6. Control Structures and Higher-Order Functions

1.  **Conditional (`select2`, `select3`):** Signal-rate selection.
    *   `select2(sel, false_val, true_val)`
    *   `select3(sel, neg_val, zero_val, pos_val)`
    *   *Compile-time conditional:* `ba.if(compile_time_bool, then_expr, else_expr)` (Rarely needed for DSP logic).
2.  **Pattern Matching (`case`):** Compile-time selection based on argument structure. (See guide Section 5.2).
3.  **Iteration (`par`, `seq`, `sum`, `prod`, `iter`):** **Constraint:** Iteration count `N` *must* be a compile-time constant. (See guide Section 5.3).
4.  **Routing (`route`, `cross`, `interleave`, etc.):** Primitives for rearranging signal wires. Require compile-time constant arguments for dimensions.

---

### 7. Declarations and Foreign Interface

1.  **Metadata (`declare`):** `declare name "MyDSP";`, `declare author "...";` etc.
2.  **Foreign Functions (`ffunction`):** Interface with C/C++. (See guide Section 6.3).
3.  **Foreign Constants/Variables (`fconstant`, `fvariable`):** Link to external C values. `fconstant` are assumed fixed after init; `fvariable` can change between processing blocks (like UI values or `ma.BS`).
    *   `SR = fconstant(int fSamplingFreq, <math.h>);`
    *   `BS = fvariable(int count, <math.h>);`

---

### 8. User Interface Elements

`hslider`, `vslider`, `nentry`, `button`, `checkbox`, `hbargraph`, `vbargraph`. Their output signals are "slow" (updated per block) and treated as `fvariable` internally. (See guide Section 7).

---

### 9. Common LLM Pitfalls and How to Avoid Them (Reinforced)

1.  **Operator Precedence:** **Use parentheses liberally** `()` to enforce order, especially mixing `:` and `,`.
2.  **State:** **Never use simple variables for changing state.** Use `~`, `mem`, `@`. Ensure `~` structure is correct (next state from current state). Account for the implicit `@(1)` delay in feedback loops.
3.  **Input/Output Counts:** Always verify `outputs(A) == inputs(B)` for `A : B`. Verify `outputs(A) == outputs(B) == inputs(C)` for `A, B :> C`.
4.  **Constants vs. Signals:** Arguments for `@`, `par`, `seq`, `sum`, `prod`, dimensions in `route`, etc., **must be compile-time constants**. Cannot use UI slider values directly for these.
5.  **Float Literals:** Use `1.0`, not `1`, when float is intended.
6.  **Unary Minus:** `-x` is `0,x:-`, `-(x)` is `_,x:-`. Use correctly.
7.  **Pattern Matching vs Formal Params:** Use `f((pattern))` syntax if intending pattern matching on multiple arguments, otherwise `f(x,y)` implies formal parameters.
8.  **Explicit IO:** For clarity and error checking, especially for stereo/multi-channel, start and end definitions with explicit buses, e.g., `stereo_effect = _,_ : my_processing : _,_;`.
9.  **Termination:** Use `!` to explicitly terminate unused output signals.
10. **`process` Definition:** Ensure one, and only one, `process = ...;` definition exists at the top level.

---
Error messages
Error messages are typically displayed in the form of compiler errors. They occur when the code cannot be successfully compiled, and typically indicate issues such as syntax errors or semantic errors. They can occur at different stages in the compilation process, possibly with the file and line number where the error occurred (when this information can be retrieved), as well as a brief description of the error.

The compiler is organized in several stages:

starting from the DSP source code, the parser builds an internal memory representation of the source program (typically known as an Abstract Source Tree) made here of primitives in the Box language. A first class of errors messages are known as syntax error messages, like missing the ; character to end a line, etc.
the next step is to evaluate the definition of process programe entry point. This step is basically a λ-calculus interpreter with a strict evaluation. The result is ”flat” block-diagram where everything have been expanded. The resulting block is type annoatetd to discover its number of inputs and outputs.
the ”flat” block-diagram is then translated the Signal language where signals as conceptually infinite streams of samples or control values. The box language actually implements the Faust Block Diagram Algebra, and not following the connections rules will trigger a second class of errors messages, the box connection errors. Other errors can be produced at this stage when parameters for some primitives are not of the correct type.
the pattern matching meta language allows to algorithmically create and manipulate block diagrams expressions. So a third class of pattern matching coding errors can occur at this level.
signal expressions are optimized, type annotated (to associate an integer or real type with each signal, but also discovering when signals are to be computed: at init time, control-rate or sample-rate..) to produce a so-called normal-form. A fourth class of parameter range errors or typing errors can occur at this level, like using delays with a non-bounded size, etc.
signal expressions are then converted in FIR (Faust Imperative Representation), a representation for state based computation (memory access, arithmetic computations, control flow, etc.), to be converted into the final target language (like C/C++, LLVM IR, Rust, WebAssembly, etc.). A fifth class of backend errors can occur at this level, like non supported compilation options for a given backend, etc.
Note that the current error messages system is still far from perfect, usually when the origin of the error in the DSP source cannot be properly traced. In this case the file and line number where the error occurred are not displayed, but an internal description of the expression (as a Box of a Signal) is printed.

Syntax errors

Those errors happen when the language syntax is not respected. Here are some examples.

The following program:

box1 = 1
box2 = 2;
process = box1,box2;
will produce the following error message:

error.dsp:2 : ERROR : syntax error, unexpected IDENT
It means that an unexpected identifier as been found line 2 of the file test.dsp. Usually, this error is due to the absence of the semi-column ; at the end of the previous line.

The following program:

t1 = _~(+(1);
2 process = t1 / 2147483647;
will produce the following error message:

errors.dsp:1 : ERROR : syntax error, unexpected ENDDEF

The parser finds the end of the definition (;) while searching a closing right parenthesis.

The following program:

process = ffunction;
will produce the following error message:

errors.dsp:1 : ERROR : syntax error, unexpected ENDDEF, expecting LPAR
The parser was expecting a left parenthesis. It identified a keyword of the language that requires arguments.

The following program:

process = +)1);
will produce the following error message:

errors.dsp:1 : ERROR : syntax error, unexpected RPAR
The wrong parenthesis has been used.

The following program:

process = <:+;
will produce the following error message:

errors.dsp:1 : ERROR : syntax error, unexpected SPLIT
The <: split operator is not correctly used, and should have been written process = _<:+;.

The following program:

process = foo;
will produce the following error message:

errors.dsp:1 : ERROR : undefined symbol : foo
This happens when an undefined name is used.

Box connection errors

Diagram expressions express how block expressions can be combined to create new ones. The connection rules are precisely defined for each of them and have to be followed for the program to be correct. Remember the operator priority when writing more complex expressions.

The five connections rules

A second category of error messages is returned when block expressions are not correctly connected.

Parallel connection

Combining two blocks A and B in parallel can never produce a box connection error since the 2 blocks are placed one on top of the other, without connections. The inputs of the resulting block-diagram are the inputs of A and B. The outputs of the resulting block-diagram are the outputs of A and B.

Sequencial connection error

Combining two blocks A and B in sequence will produce a box connection error if outputs(A) != inputs(B). So for instance the following program:

A = _,_;
B = _,_,_;
process = A : B;
will produce the following error message:

ERROR : sequential composition A:B
The number of outputs [2] of A must be equal to the number of inputs [3] of B

Here  A = _,_;
has 2 outputs

while B = _,_,_;
has 3 inputs
Split connection error

Combining two blocks A and B with the split composition will produce a box connection error if the number of inputs of B is not a multiple of the number of outputs of A. So for instance the following program:

A = _,_;
B = _,_,_;
process = A <: B;
will produce the following error message:

ERROR : split composition A<:B
The number of outputs [2] of A must be a divisor of the number of inputs [3] of B

Here  A = _,_;
has 2 outputs

while B = _,_,_;
has 3 inputs
Merge connection error

Combining two blocks A and B with the merge composition will produce a box connection error if the number of outputs of A is not a multiple of the number of inputs of B. So for instance the following program:

A = _,_;
B = _,_,_;
process = A :> B;
will produce the following error message:

ERROR : merge composition A:>B
The number of outputs [2] of A must be a multiple of the number of inputs [3] of B

Here  A = _,_;
has 2 outputs

while B = _,_,_;
has 3 inputs
Recursive connection error

Combining two blocks A and B with the recursive composition will produce a box connection error if the number of outputs of A is less than the number of inputs of B, or the number of outputs of B is more than the number of inputs of A (that is the following
o
u
t
p
u
t
s
(
A
)
≥
i
n
p
u
t
s
(
B
)
a
n
d
i
n
p
u
t
s
(
A
)
≥
o
u
t
p
u
t
s
(
B
)

connection rule is not respected). So for instance the following program:

A = _,_;
B = _,_,_;
process = A ~ B;
will produce the following error message:

ERROR : recursive composition A~B
The number of outputs [2] of A must be at least the number of inputs [3] of B. The number of inputs [2] of A must be at least the number of outputs [3] of B. 

Here  A = _,_;
has 2 inputs and 2 outputs

while B = _,_,_;
has 3 inputs and 3 outputs
Route connection errors

More complex routing between blocks can also be described using the route primitive. Two different errors can be produced in case of incorrect coding:

process = route(+,8.7,(0,0),(0,1));
will produce the following error message:

ERROR : invalid route expression, first two parameters should be blocks producing a value, third parameter a list of input/output pairs : route(+,8.7f,0,0,0,1)
And the second one when the parameters are not actually numbers:

process = route(9,8.7f,0,0,0,button("foo"));
will produce the following error message:

ERROR : invalid route expression, parameters should be numbers : route(9,8.7f,0,0,0,button("foo"))
Iterative constructions

Iterations are analogous to for(...) loops in other languages and provide a convenient way to automate some complex block-diagram constructions. All par, seq, sum, prod expressions have the same form, take an identifier as first parameter, a number of iteration as an integer constant numerical expression as second parameter, then an arbitrary block-diagram as third parameter.

The example code:

process = par(+, 2, 8);
will produce the following syntax error, since the first parameter is not an identifier:

filename.dsp : xx : ERROR : syntax error, unexpected ADD, expecting IDENT
The example code:

process = par(i, +, 8);
will produce the following error:

filename.dsp : 1 : ERROR : not a constant expression of type : (0->1) : +
Pattern matching errors

Pattern matching mechanism allows to algorithmically create and manipulate block diagrams expressions. Pattern matching coding errors can occur at this level.

Multiple symbol definition error

This error happens when a symbol is defined several times in the DSP file:

ERROR : [file foo.dsp : N] : multiple definitions of symbol 'foo'
Since computation are done at compile time and the pattern matching language is Turing complete, even infinite loops can be produced at compile time and should be detected by the compiler.

Loop detection error

The following (somewhat extreme) code:

foo(x) = foo(x);
process = foo;
will produce the following error:

ERROR : stack overflow in eval
and similar kind of infinite loop errors can be produced with more complex code.

[TO COMPLETE]

Signal related errors

Signal expressions are produced from box expressions, are type annotated and finally reduced to a normal-form. Some primitives expect their parameters to follow some constraints, like being in a specific range or being bounded for instance. The domain of mathematical functions is checked and non allowed operations are signaled.

Automatic type promotion

Some primitives (like route, rdtable, rwtable...) expect arguments with an integer type, which is automatically promoted, that is the equivalent of int(exp) is internally added and is not necessary in the source code.

Parameter range errors

Soundfile usage error

The soundfile primitive assumes the part number to stay in the [0..255] interval, so for instance the following code:

process = _,0 : soundfile("foo.wav", 2);
will produce the following error:

ERROR : out of range soundfile part number (interval(-1,1,-24) instead of interval(0,255)) in expression : length(soundfile("foo.wav"),IN[0])
Delay primitive error

The delay @ primitive assumes that the delay signal value is bounded, so the following expression:

import("stdfaust.lib");
process = @(ba.time);
will produce the following error:

ERROR : can't compute the min and max values of : proj0(letrec(W0 = (proj0(W0)'+1)))@0+-1
        used in delay expression : IN[0]@(proj0(letrec(W0 = (proj0(W0)'+1)))@0+-1)
        (probably a recursive signal)
[TO COMPLETE]

Table construction errors

The rdtable primitive can be used to read through a read-only (pre-defined at initialisation time) table. The rwtable primitive can be used to implement a read/write table. Both have a size computed at compiled time

The rdtable primitive assumes that the table content is produced by a processor with 0 input and one output, known at compiled time. So the following expression:

process = rdtable(9, +, 4);
will produce the following error, since the +is not of the correct type:

ERROR : checkInit failed for type RSEVN interval(-2,2,-24)
The same kind of errors will happen when read and write indexes are incorrectly defined in a rwtable primitive.

Duplicate pathname error

Pathnames for different GUI items have to be diffrent. Compiling the following code:

import("stdfaust.lib");
nBands = 8;
filterBank(N) = hgroup("Filter Bank",seq(i,N,oneBand(i)))
with {
    oneBand(j) = vgroup("[%j]Band",fi.peak_eq(l,f,b))
    with {
        a = j+1; // just so that band numbers don't start at 0
        l = vslider("[2]Level[unit:db]",0,-70,12,0.01) : si.smoo;
        f = nentry("[1]Freq",(80+(1000*8/N*(j+1)-80)),20,20000,0.01) : si.smoo;
        b = f/hslider("[0]Q[style:knob]",1,1,50,0.01) : si.smoo;
    };
};
process = filterBank(nBands);
gives the ERROR : path '/Filter_Bank/Band/Q' is already used message. This is because we typically want to distinguish pathnames to be controlled by OSC messages for instance. So a proper solution would be to rewrite the code with:

import("stdfaust.lib");
nBands = 8;
filterBank(N) = hgroup("Filter Bank",seq(i,N,oneBand(i)))
with {
    oneBand(j) = vgroup("[%j]Band %a",fi.peak_eq(l,f,b))
    with {
        a = j+1; // just so that band numbers don't start at 0
        l = vslider("[2]Level[unit:db]",0,-70,12,0.01) : si.smoo;
        f = nentry("[1]Freq",(80+(1000*8/N*(j+1)-80)),20,20000,0.01) : si.smoo;
        b = f/hslider("[0]Q[style:knob]",1,1,50,0.01) : si.smoo;
    };
};
process = filterBank(nBands);
Mathematical functions out of domain errors

Error messages will be produced when the mathematical functions are used outside of their domain, and if the problematic computation is done at compile time. If the out of domain computation may be done at runtime, then a warning can be produced using the -me option (see Warning messages section).

Modulo primitive error

The modulo % assumes that the denominator is not 0, thus the following code:

process = _%0;
will produce the following error:

ERROR : % by 0 in IN[0] % 0
The same kind of errors will be produced for acos, asin, fmod, log10, log, remainder and sqrt functions.

FIR and backends related errors

Some primitives of the language are not available in some backends.

The example code:

fun = ffunction(float fun(float), <fun.h>, "");
process = fun;
compiled with the wast/wasm backends using: faust -lang wast errors.dsp will produce the following error:

ERROR : calling foreign function 'fun' is not allowed in this compilation mode

and the same kind of errors would happen also with foreign variables or constants.

[TO COMPLETE]

Compiler option errors

All compiler options cannot be used with all backends. Moreover, some compiler options can not be combined. These will typically trigger errors, before any compilation actually begins.

[TO COMPLETE]

Warning messages

Warning messages do not stop the compilation process, but allow to get usefull informations on potential problematic code. The messages can be printed using the -wall compilation option. Mathematical out-of-domain error warning messages are displayed when both -wall and -me options are used.