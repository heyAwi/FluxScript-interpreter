# FluxScript-interpreter

FluxScript is an indie programming language created on October 24, 2026. It is designed to be simple, fast, and easy to learn.

I hope that in future versions, FluxScript will be able to create games and applications and become popular. Not for fame, but so people can code something with less struggle.

**Current version: 1.1.0**

FluxScript currently supports variables, printing, user input, arithmetic and logical operators, conditionals, ternary expressions, loops, arrays, dictionaries, string manipulation, functions with default parameters, lambda expressions, named arguments, class inheritance, complex numbers, functional programming with `map()`, `filter()`, and `reduce()`, assertions, aliases, hierarchical packages, modules, regular expressions, serialization, persistent key-value storage, file operations, type conversion, error handling, built-in utility functions, raw strings, escape sequences, native console operations, and Windows GUI applications with interactive controls, customizable properties, mouse and keyboard events, image support, dialogs, and non-blocking timers. **As of version 1.1.0**, FluxScript also supports advanced 2D graphics through five `windowDraw*` functions, object rotation using the `.angle` property, sprite sheets, hierarchical GUI transformations, collision detection, expanded keyboard event handling, native DLL integration with ABI v2, and ten native DLL modules covering physics, game development, bitwise operations, geometry, statistics, date and time utilities, system operations, cryptography, databases, and networking.

## HOW TO RUN FLUXSCRIPT FILES

### Step 1 - Open CMD

Open the Start menu, type `cmd`, and press Enter.

<img width="1366" height="768" alt="Open CMD" src="https://github.com/user-attachments/assets/fc6376eb-93f0-47fe-8d25-ffa87c839f74" />

### Step 2 - Navigate to the FluxScript folder

Type:

    cd "C:\Example\flux"

Replace `C:\Example\flux` with the actual path to your FluxScript folder.

Use quotation marks if the path contains spaces.

<img width="1366" height="768" alt="Navigate to FluxScript folder" src="https://github.com/user-attachments/assets/e93d929f-1a4a-4d63-909f-31cc16775601" />

### Step 3 - Run a FluxScript file

Type:

    flux.exe example\test.flux

Replace `test.flux` with the name of your file and `example` with its folder.

If the file is in the same folder as `flux.exe`, type:

    flux.exe test.flux

You CANNOT use `.flx` extension.

<img width="1366" height="768" alt="Run FluxScript file" src="https://github.com/user-attachments/assets/bb274f80-00e0-4dd9-a3e7-8f7c963033a5" />

Done! Your FluxScript program should now run.
# HOW TO SETUP FLUXSCRIPT ON VSCODE

This tutorial explains how to enable FluxScript syntax highlighting and run `.flux` files directly from Visual Studio Code.

## 1. Requirements

- Visual Studio Code
- Code Runner extension
- FluxScript interpreter (`flux.exe`)
- FluxScript syntax highlighting extension

## 2. Install the syntax highlighting extension

Copy the `fluxscript-syntax` extension folder into your VS Code extensions directory.

On Windows:

```text
%USERPROFILE%\.vscode\extensions\
```

The folder structure should look like this:

```text
.vscode/
└── extensions/
    └── fluxscript-syntax/
        ├── package.json
        ├── language-configuration.json
        └── syntaxes/
            └── fluxscript.tmLanguage.json
```

Restart VS Code after copying the extension.

Open a `.flux` file and verify that the language indicator in the bottom-right corner says **FluxScript**.

## 3. Install Code Runner

Open the Extensions panel in VS Code (`Ctrl+Shift+X`).

Search for **Code Runner** by Jun Han and install it.

## 4. Configure the FluxScript interpreter

Open the Command Palette (`Ctrl+Shift+P`) and select:

`Preferences: Open User Settings (JSON)`

Add the following configuration inside your existing settings object:

```json
"code-runner.executorMapByFileExtension": {
    ".flux": "Set-Location 'C:\\FluxScript'; & 'C:\\FluxScript\\flux.exe' $fullFileName"
},
"code-runner.runInTerminal": true,
"code-runner.saveFileBeforeRun": true
```

**Important:** Replace `C:\\FluxScript` with the actual folder containing `flux.exe`.

The command above is intended for the PowerShell terminal on Windows.

If your `settings.json` already contains other settings, preserve them and separate the new properties with commas.

## 5. Run your first program

Create a file named `hello.flux`:

```flux
out "Hello from FluxScript!"
```

Save the file and click **▶ Run Code** in the upper-right corner of VS Code.

Expected output:

```text
Hello from FluxScript!
```

## 6. Troubleshooting

**Code language not supported or defined**

Make sure the `.flux` extension is included in `code-runner.executorMapByFileExtension`.

**PowerShell parser error**

Use PowerShell syntax with `Set-Location` and the `&` invocation operator instead of CMD commands such as `cd /d`.

**Interpreter not found**

Verify the path to `flux.exe` in your settings.

**Syntax highlighting not working**

Check the extension folder structure and restart VS Code.

---

FluxScript 1.1.0 — You can write, run, and experiment with FluxScript programs directly in VS Code.

# FLUXSCRIPT v1.0.0 TUTORIAL

### 1. Printing text and values

FluxScript uses `out` to print text and values, similar to Python's `print()`.

You don't need parentheses or semicolons.

Example:

    out "Hello, World!"

    out 'Hello again!'

    out 123

    out 5 + 5

Both single and double quotation marks are supported for strings.

You can also concatenate strings using `+`.

Example:

    out "Hello" + ", World!"

Output:

    Hello, World!

### 2. Comments

FluxScript supports single-line and multiline comments.

#### Single-line comments

Use `//` for single-line comments.

Example:

    // This is a comment

    out "Hello"

#### Multiline comments

Use `--[[` to start a multiline comment and `]]` to end it.

Example:

    --[[

    This is a multiline comment.

    You can write multiple lines here.

    Hello!

    1

    2

    3

    ]]

**Important:** Since `//` is also used for floor division, it is recommended to put single-line comments on their own lines.

### 3. Variables

You can create variables using `=`.

There is no need to declare them with `let`, `var`, or a type.

Example:

    x = 10

    name = "FluxScript"

    pi = 3.14159

    enabled = true

    out x

    out name

    out pi

    out enabled

Variables can store numbers, strings, booleans, arrays, and dictionaries.

FluxScript uses dynamic typing, so variables can hold different types of values.

Example:

    value = 10

    value = "Hello"

    value = true

### 4. Math operators

FluxScript supports basic and advanced arithmetic operators.

| Operator | Description | Example | Result |

|---|---|---|---|

| `+` | Addition | `5 + 3` | `8` |

| `-` | Subtraction | `5 - 3` | `2` |

| `*` | Multiplication | `5 * 3` | `15` |

| `/` | Division | `5 / 2` | `2.5` |

| `//` | Floor division | `7 // 2` | `3` |

| `%` | Modulo | `10 % 3` | `1` |

| `^` | Exponentiation | `2 ^ 3` | `8` |

Example:

    out 5 + 5

    out 10 - 3

    out 4 * 5

    out 10 / 2

    out 7 // 2

    out 10 % 3

    out 2 ^ 4

#### Order of operations

FluxScript follows mathematical operator precedence.

Example:

    out 5 + 5 * 2

Output:

    15

You can use parentheses to change the order.

Example:

    out (5 + 5) * 2

Output:

    20

### 5. Assignment operators

FluxScript supports compound assignment operators.

| Operator | Meaning |

|---|---|

| `+=` | Add and assign |

| `-=` | Subtract and assign |

| `*=` | Multiply and assign |

| `/=` | Divide and assign |

| `%=` | Modulo and assign |

| `^=` | Exponentiate and assign |

Example:

    x = 10

    x += 5

    out x

    x -= 3

    out x

    x *= 2

    out x

    x /= 4

    out x

    x %= 4

    out x

    x ^= 3

    out x

Output:

    15

    12

    24

    6

    2

    8

### 6. User input

FluxScript supports user input using `input()`.

You can use it to ask the user for information.

Example:

    name = input("What is your name? ")

    out name

You can also use `input()` without a prompt.

Example:

    answer = input()

    out answer

Starting with v0.5.0, `input()` automatically detects numbers, booleans, and strings.

Example:

    age = input("Enter your age: ")

    out type(age)

If the user enters `18`, the result is a number.

If the user enters `true`, the result is a boolean.

Other text is returned as a string.

### 7. IF / ELIF / ELSE

`if` checks whether a condition is true.

`elif` checks additional conditions.

`else` runs when none of the previous conditions are true.

Parentheses are not required around conditions.

Syntax:

    if condition {

        out "First condition"

    } elif anotherCondition {

        out "Second condition"

    } else {

        out "No conditions matched"

    }

Example:

    score = 85

    if score >= 90 {

        out "Excellent"

    } elif score >= 70 {

        out "Good"

    } else {

        out "Keep practicing"

    }

Output:

    Good

#### Comparison operators

| Operator | Description |

|---|---|

| `==` | Equal |

| `!=` | Not equal |

| `<` | Less than |

| `>` | Greater than |

| `<=` | Less than or equal |

| `>=` | Greater than or equal |

#### Logical operators

FluxScript supports `and`, `or`, and `not`.

Example:

    age = 20

    if age >= 18 and age < 60 {

        out "Adult"

    }

    if age < 18 or age >= 60 {

        out "Other age group"

    }

    enabled = false

    if not enabled {

        out "Disabled"

    }

#### The IN operator

The `in` operator checks whether a value exists inside an array or whether a string contains another string.

Example:

    fruits = ["apple", "banana", "orange"]

    out "banana" in fruits

    out "grape" in fruits

    out "World" in "Hello World"

Output:

    true

    false

    true

### 8. Ternary operator

FluxScript v0.6.0 introduces ternary conditional expressions.

A ternary expression allows you to choose between two values using a condition.

Syntax:

    result = valueIfTrue if condition else valueIfFalse

Example:

    age = 18

    status = "Adult" if age >= 18 else "Minor"

    out status

Output:

    Adult

You can also use ternary expressions inside functions.

Example:

    func checkAge(age) {

        return "Adult" if age >= 18 else "Minor"

    }

    out checkAge(15)

Output:

    Minor

### 9. WHILE loops

`while` repeats a block of code while a condition remains true.

Parentheses around the condition are optional.

Syntax:

    while condition {

        // Code

    }

Example:

    i = 0

    while i < 5 {

        out i

        i += 1

    }

Output:

    0

    1

    2

    3

    4

### 10. FOR loops

FluxScript supports several types of `for` loops.

#### Basic FOR loops

Syntax:

    for i(n) {

        out i

    }

Here, `i` is the loop variable and `n` is the number of iterations.

The loop starts at 0 and stops before `n`.

Example:

    for i(5) {

        out i

    }

Output:

    0

    1

    2

    3

    4

#### FOR loops with start and end

You can specify a starting value and an ending value.

Example:

    for i(2, 6) {

        out i

    }

Output:

    2

    3

    4

    5

#### FOR loops with step

You can also specify the step value.

Example:

    for i(0, 10, 2) {

        out i

    }

Output:

    0

    2

    4

    6

    8

#### FOR IN loops

You can iterate directly over an array.

Example:

    fruits = ["apple", "banana", "orange"]

    for fruit in fruits {

        out fruit

    }

Output:

    apple

    banana

    orange

### 11. BREAK / CONTINUE / PASS

FluxScript supports loop control statements.

#### BREAK

`break` immediately exits the current loop.

Example:

    for i(10) {

        if i == 5 {

            break

        }

        out i

    }

Output:

    0

    1

    2

    3

    4

#### CONTINUE

`continue` skips the remaining statements in the current iteration.

Example:

    for i(5) {

        if i == 2 {

            continue

        }

        out i

    }

Output:

    0

    1

    3

    4

#### PASS

`pass` is a statement that does nothing.

It can be used as a placeholder.

Example:

    if true {

        pass

    }

### 12. Arrays

Arrays can contain multiple values of different types.

Example:

    numbers = [10, 20, 30, 40]

    fruits = ["apple", "banana", "orange"]

    mixed = [1, "hello", true]

#### Accessing array elements

Array indexing starts at 0.

Example:

    numbers = [10, 20, 30]

    out numbers[0]

    out numbers[1]

    out numbers[2]

Output:

    10

    20

    30

#### Modifying array elements

You can modify elements using indexed assignment.

Example:

    numbers = [10, 20, 30]

    numbers[1] = 50

    out numbers

Output:

    [10, 50, 30]

#### Array methods

FluxScript supports methods for modifying arrays.

Methods do not use parentheses.

Example:

    numbers = [1, 2, 3]

    numbers.push 4

    numbers.append 5

    out numbers

Output:

    [1, 2, 3, 4, 5]

You can remove an element using `.remove`.

Example:

    numbers = [10, 20, 30]

    numbers.remove 20

    out numbers

Output:

    [10, 30]

The `.pop` method removes the last element.

Example:

    numbers = [10, 20, 30]

    numbers.pop

    out numbers

Output:

    [10, 20]

#### Sorting arrays

FluxScript v0.6.0 introduces `.sort` and `.rev`.

`.sort` sorts an array in ascending order.

`.rev` reverses the current order of an array.

Example:

    numbers = [5, 2, 8, 1, 9]

    numbers.sort

    out numbers

Output:

    [1, 2, 5, 8, 9]

To sort in descending order, use `.sort` followed by `.rev`.

Example:

    numbers.rev

    out numbers

Output:

    [9, 8, 5, 2, 1]

**Important:** `.rev` does not sort an array. It only reverses its current order.

#### Finding and counting elements

The `.index` method returns the index of an element.

The `.count` method counts how many times an element appears.

Example:

    numbers = [5, 2, 8, 2, 9]

    out numbers.index 8

    out numbers.count 2

Output:

    2

    2

### 13. Dictionaries

FluxScript v0.6.0 introduces dictionaries.

Dictionaries store values using keys instead of numeric indexes.

Currently, dictionary keys must be strings.

Example:

    player = {

        "name": "Alex",

        "level": 10,

        "health": 100

    }

#### Accessing dictionary values

Use a key inside square brackets.

Example:

    out player["name"]

    out player["level"]

Output:

    Alex

    10

#### Modifying dictionary values

You can modify existing values.

Example:

    player["level"] = 11

    out player["level"]

Output:

    11

You can also add new keys.

Example:

    player["gold"] = 500

    out player["gold"]

Output:

    500

#### Removing dictionary keys

Use `.remove` to remove a key.

Example:

    player.remove "health"

#### Dictionary keys and values

Use `.keys` to get the dictionary's keys.

Use `.values` to get its values.

Example:

    out player.keys

    out player.values

Dictionaries can store numbers, strings, booleans, arrays, and other dictionaries.

### 14. String methods

FluxScript supports several string methods.

Methods use parentheses-free syntax.

#### Uppercase and lowercase

Example:

    text = "Hello World"

    out text.upper

    out text.lower

Output:

    HELLO WORLD

    hello world

#### Replacing text

Example:

    text = "Hello World"

    out text.replace "World", "FluxScript"

Output:

    Hello FluxScript

#### Splitting strings

Example:

    text = "apple,banana,orange"

    out text.split ","

Output:

    ["apple", "banana", "orange"]

#### Appending text

Example:

    text = "Hello"

    text.append " World"

    out text

Output:

    Hello World

#### Trimming whitespace

FluxScript v0.6.0 introduces `.trim`.

Example:

    text = "  Hello World  "

    out text.trim

Output:

    Hello World

#### Checking prefixes and suffixes

Use `.startswith` and `.endswith`.

Example:

    text = "Hello World"

    out text.startswith "Hello"

    out text.endswith "World"

Output:

    true

    true

#### Finding text

Use `.find` to find the position of a substring.

Example:

    text = "Hello World"

    out text.find "World"

Output:

    6

#### Checking whether text contains another string

Use `.has`.

Example:

    text = "Hello World"

    out text.has "Hello"

    out text.has "Python"

Output:

    true

    false

### 15. String indexing

FluxScript v0.6.0 supports string indexing.

You can access individual characters using square brackets.

Indexing starts at 0.

Example:

    text = "Hello"

    out text[0]

    out text[1]

    out text[4]

Output:

    H

    e

    o

**Note:** String indexing currently operates on bytes, so multibyte UTF-8 characters may not behave as expected.

### 16. Functions

Functions allow you to reuse code.

Use `func` to create a function.

Syntax:

    func functionName(parameter) {

        return parameter

    }

Example:

    func add(a, b) {

        return a + b

    }

    out add(5, 3)

Output:

    8

#### Functions with multiple operations

Example:

    func square(x) {

        return x ^ 2

    }

    func cube(x) {

        return x ^ 3

    }

    out square(5)

    out cube(3)

Output:

    25

    27

Functions can also execute statements without explicitly returning a value.

Example:

    func greet(name) {

        out "Hello, " + name

    }

    greet("FluxScript")

Output:

    Hello, FluxScript

#### Default parameters

FluxScript v0.6.0 introduces default function parameters.

A default parameter is used when the caller does not provide an argument.

Example:

    func greet(name = "World") {

        out "Hello, " + name

    }

    greet()

    greet("Alex")

Output:

    Hello, World

    Hello, Alex

Default parameters must come after required parameters.

Example:

    func greet(message, name = "World") {

        out message + ", " + name

    }

    greet("Hello")

    greet("Welcome", "Alex")

Output:

    Hello, World

    Welcome, Alex

### 17. Built-in functions

FluxScript provides several built-in functions.

| Function | Description |

|---|---|

| `int()` | Convert a value to an integer |

| `float()` | Convert a value to a number |

| `str()` | Convert a value to a string |

| `bool()` | Convert a value to a boolean |

| `type()` | Return the value's type |

| `len()` | Return the length of a string, array, or dictionary |

| `input()` | Read user input |

| `timestamp()` | Return the current Unix timestamp |

| `wait()` | Pause execution |

Example:

    out int("10")

    out float("3.14")

    out str(123)

    out bool(1)

    out type("Hello")

    out len([10, 20, 30])

Output:

    10

    3.14

    123

    true

    string

    3

#### Waiting

Use `wait(seconds)` to pause execution.

Example:

    out "Starting..."

    wait(2)

    out "Finished!"

#### Unix timestamps

Use `timestamp()` to get the current Unix timestamp.

Example:

    out timestamp()

The timestamp is returned in seconds.

### 18. File operations

FluxScript supports reading and writing files.

#### Reading files

Use `readfile()` to read a file's contents.

Example:

    text = readfile("notes.txt")

    out text

#### Writing files

Use `writefile()` to create or overwrite a file.

Example:

    writefile("notes.txt", "Hello FluxScript!")

#### Appending files

Use `addfile()` to append text to an existing file.

Example:

    addfile("notes.txt", "\nAnother line!")

#### Checking whether a file exists

Use `fileexists()`.

Example:

    if fileexists("notes.txt") {

        out "File exists!"

    } else {

        out "File not found!"

    }

### 19. Error handling

FluxScript supports `try` and `catch` for error handling.

The `try` block contains code that may fail.

The `catch` block handles the error.

Example:

    try {

        out 10 / 0

    } catch {

        out "An error occurred!"

    }

Output:

    An error occurred!

#### Accessing error messages

FluxScript v0.6.0 allows you to access the captured error message using `catch(error)`.

Example:

    try {

        out 10 / 0

    } catch(error) {

        out error

    }

Output:

    ZeroDivisionError: division by zero

This is useful for debugging and displaying error information.

### 20. Modules

FluxScript supports a module system.

Modules allow you to organize functions into separate files.

Use `get` to load a module.

#### Module directory

Modules are stored in the `module` folder.

Example:

    FluxScript/

    ├── flux.exe

    ├── main.flux

    └── module/

        ├── math.flux

        ├── time.flux

        ├── random.flux

        └── string.flux

#### Importing a module

Example:

    get math

    out math.sq(5)

Modules are written in FluxScript itself.

You can call module functions using:

    moduleName.functionName(arguments)

#### Math module

The `math.flux` module provides mathematical utility functions.

Example:

    get math

    out math.sq(5)

    out math.cube(3)

    out math.factorial(5)

Output:

    25

    27

    120

#### Time module

The `time.flux` module provides Unix timestamp and UTC time utilities.

Example:

    get time

    out time.now()

    out time.hours()

    out time.minutes()

#### Random module

The `random.flux` module provides pseudo-random number generation.

Example:

    get random

    out random.randint(1, 10)

    out random.rand()

    fruits = ["apple", "banana", "orange"]

    out random.choice(fruits)

Random results may be different each time.

#### String module

FluxScript v0.6.0 introduces `string.flux`.

It provides additional functions for working with strings.

Example:

    get string

    out string.repeat("Hi", 3)

    out string.reverse("Hello")

Output:

    HiHiHi

    olleH

**Important:** Module functions use parentheses, while methods do not.

The module directory is resolved relative to the interpreter's working directory, so run `flux.exe` from the FluxScript project folder.

### 21. Native console operations (v0.8.0)

FluxScript v0.8.0 added native console functions for Windows terminal control.

You can use these functions directly, or create aliases in a FluxScript module such as `module/console.flux`.

#### Console functions

| Function | Description |

|---|---|

| `consoleClear()` | Clear the console screen |

| `consoleTitle(text)` | Set the console window title |

| `consoleColor(name)` | Change foreground color |

| `consoleBackground(name)` | Change background color |

| `consoleReset()` | Restore default console colors |

| `consoleCursor(x, y)` | Move the cursor |

| `consolePosition()` | Return the cursor position |

| `consoleSize()` | Return the console dimensions |

| `consoleResize(width, height)` | Resize the console |

| `consoleHideCursor()` | Hide the cursor |

| `consoleShowCursor()` | Show the cursor |

| `consoleReadKey()` | Read a key |

| `consoleKeyAvailable()` | Check whether a key is waiting |

| `consolePause()` | Wait for a key press |

| `consoleBeep(frequency, duration)` | Play a beep |

| `consoleProgress(percent, width)` | Print a progress bar |

| `consoleBox(text)` | Print a formatted text box |

| `consoleMenu(options)` | Show a selection menu |

The `console.flux` module can expose shorter names such as `console.clear()`, `console.title(text)`, `console.color(name)`, and `console.progress(percent, width)`.

Example with native functions:

    consoleTitle("FluxScript Console")

    consoleColor("cyan")

    out "Hello from FluxScript v1.0.0!"

    consoleProgress(75, 30)

    consoleReset()

Console color names include `black`, `blue`, `green`, `cyan`, `red`, `magenta`, `yellow`, and `white`.

### 22. Native graphical interfaces (v0.9.0)

FluxScript v0.9.0 introduces graphical user interfaces using the Windows Win32 API.

Applications can create native windows, labels, buttons, and textboxes without an external GUI framework.

#### GUI functions

| Function | Description |

|---|---|

| `windowAdd(title, size)` | Create a native window |

| `windowBackground(color)` | Set the active window background |

| `windowResizable(enabled)` | Enable or disable resizing |

| `windowLabel(size, color, position, text)` | Add a text label |

| `windowButton(size, color, position, text)` | Add a clickable button |

| `windowTextbox(size, color, position, text)` | Add a text input field |

#### Dimension and position literals

Use `widthxheight` notation for dimensions and positions.

Examples:

    500x350

    180x45

    20x90

The `x` is part of a dimension literal, not a multiplication operator.

#### Creating a window

Example:

    func -ready() {

        main = windowAdd("FluxScript 1.0.0", 500x350) {

            windowBackground("black")

            windowResizable(false)

            windowLabel(350x40, "white", 20x20, "FLUXSCRIPT 1.0.0")

            button = windowButton(180x45, "blue", 20x90, "CLICK ME")

            button.onclick {

                out "Button clicked!"

            }

            windowTextbox(250x35, "white", 20x160, "Type here...")

        }

    }

Save this example as `test_gui.flux` and run it using `flux.exe test_gui.flux`.

The window opens with a black background, a white label, a blue button, and a textbox.

Clicking the button prints `Button clicked!` in the console.

#### Window context blocks

Use `.context` to select the window that receives new controls and settings.

Example:

    func -ready() {

        main = windowAdd("My Window", 400x300)

        main.context {

            windowBackground("blue")

            windowResizable(false)

            windowLabel(200x30, "white", 20x20, "Hello!")

        }

    }

A context block makes the selected window the active parent for its GUI operations.

### 23. GUI events and control properties (v1.0.0)

FluxScript v1.0.0 expands native Win32 GUI controls and interaction.

#### Control properties

| Property | Description |

|---|---|

| `.text` | Read or modify control text |

| `.color` | Change control color |

| `.position` | Read or modify position |

| `.size` | Read or modify dimensions |

| `.visible` | Show or hide a control |

| `.enabled` | Enable or disable a control |

| `.font` | Configure the font |

| `.bold` | Enable or disable bold text |

The default GUI font is Arial.

#### Events

| Event | Description |

|---|---|

| `.onclick` | Left mouse click |

| `.onrclick` | Right mouse click |

| `.onchange` | Control value changes |

| `.detectmouse` | Mouse enters or leaves a control |

| `.detectMouseMoving` | Mouse movement |

`.detectmouse` changes to `true` once when the pointer enters and to `false` once when it leaves, without repeatedly firing while the pointer remains inside.

Keyboard shortcuts and key combinations are also supported.

#### Additional controls and images

GUI controls include windows, labels, buttons, textboxes, checkboxes, sliders, and progress bars.

Images support PNG, JPG, and JPEG formats. Image lookup can also fall back to the `flux/` directory.

#### Dialogs and timers

`alert()` displays a message dialog.

`confirm()` displays a confirmation dialog and returns a boolean.

Non-blocking, single-execution timers allow an action to run later without blocking the interface.

#### Platform notes

The native GUI targets Windows and uses Win32.

GUI programs run through the FluxScript interpreter.

The optional `gui.flux` wrapper module is separate from the native GUI implementation.

### 24. Combining everything

Here is an example using several FluxScript features together.

    get random

    func attack(player, enemy, damage = 10) {

        enemy["health"] -= damage

        out player["name"] + " dealt " + str(damage) + " damage!"

    }

    player = {

        "name": "Alex",

        "health": 100,

        "level": 5

    }

    enemy = {

        "name": "Goblin",

        "health": 30

    }

    out "FluxScript v1.0.0"

    while enemy["health"] > 0 {

        damage = random.randint(5, 15)

        attack(player, enemy, damage)

    }

    out "Enemy defeated!"

This example demonstrates dictionaries, functions, default parameters, modules, random numbers, and loops.

### 25. Mini project: Number Guessing Game

Here is a simple game written entirely in FluxScript.

    get random

    func game() {

        secret = random.randint(1, 20)

        attempts = 0

        out "NUMBER GUESSING GAME"

        out "Guess a number between 1 and 20!"

        while true {

            guess = input("Your guess: ")

            attempts += 1

            if guess == secret {

                out "Correct!"

                out "Attempts: " + str(attempts)

                break

            } elif guess < secret {

                out "Too low!"

            } else {

                out "Too high!"

            }

        }

    }

    game()

The game uses variables, functions, loops, conditions, random numbers, and automatic input type detection.

### 26. Aliases

FluxScript v1.0.0 introduces aliases.

Aliases allow you to create alternative names for existing keywords and identifiers.

#### Syntax

```
alias original = nickname
```

The original name comes before the `=` operator, and the new alias comes after it.

#### Example

```
alias out = print

print "Hello, World!"

print 123
```

Output:

```
Hello, World!

123
```

#### Chained aliases

An alias can also refer to another alias.

Example:

```
alias out = print

alias print = echo

echo "Hello from FluxScript!"
```

Output:

```
Hello from FluxScript!
```

#### Rules

Aliases are local to the file where they are declared.

Aliases only take effect after their declaration.

Alias names must be valid identifiers.

Declaring the same alias name more than once raises a `SyntaxError`.

Alias declarations are processed by the lexer before parsing, allowing aliases to replace keywords and identifiers.

### 27. Classes and inheritance

FluxScript v1.0.0 introduces classes and inheritance.

Classes allow you to group data and functions into reusable objects.

#### Class syntax

```flux
class ClassName {

    func -init(value) {

        self.value = value

    }

    func show() {

        out self.value

    }

}
```

The special `-init` method initializes an object when an instance is created.

The `self` keyword refers to the current instance and allows access to its properties.

#### Creating an instance

Example:

```flux
class Player {

    func -init(name) {

        self.name = name

    }

    func greet() {

        out "Hello, " + self.name

    }

}

player = Player("Alex")

player.greet()
```

Output:

```text
Hello, Alex
```

#### Inheritance

Inheritance allows a child class to reuse methods from a parent class.

Syntax:

```flux
class Child(Parent) {

    func method() {

        out "Child method"

    }

}
```

The parent class name is placed inside parentheses after the child class name.

#### Inheritance example

```flux
class Enemy {

    func -init(name) {

        self.name = name

    }

    func describe() {

        out "Enemy: " + self.name

    }

}

class Zombie(Enemy) {

    func bite() {

        out self.name + " is biting!"

    }

}

zombie = Zombie("Walker")

zombie.describe()

zombie.bite()
```

Output:

```text
Enemy: Walker

Walker is biting!
```

The `Zombie` class inherits the `-init` and `describe` methods from `Enemy`.

It also defines its own `bite` method.

This allows child classes to extend the functionality of parent classes without duplicating their methods.

### 28. Lambda expressions

Lambda expressions provide anonymous functions.

Syntax:

    func(x): expression

Example expression:

    func(x): x * 2

### 29. Functional array operations

FluxScript v1.0.0 adds `map()`, `filter()`, and `reduce()`.

| Function | Description |

|---|---|

| `map()` | Transform array elements |

| `filter()` | Select elements matching a condition |

| `reduce()` | Combine elements into a result |

These operations can be used with functions and lambda expressions.

### 30. Named arguments

Named arguments allow function calls to specify parameter names.

Syntax:

    functionName(name=value)

Named arguments improve readability when functions have multiple parameters.

### 31. Complex numbers

FluxScript v1.0.0 introduces support for complex numbers.

A complex number contains two components: a real part and an imaginary part.

Complex numbers are commonly written in the form `a + bi`, where `a` is the real part, `b` is the imaginary part, and `i` represents the imaginary unit.

#### Syntax

```flux
z = 3 + 4i
```

In FluxScript, the `i` suffix is used to represent imaginary numbers.

#### Accessing real and imaginary components

You can access both components using `.real` and `.imag`.

Example:

```flux
z = 3 + 4i

out z.real

out z.imag
```

Output:

```text
3

4
```

#### Using complex numbers in expressions

Complex numbers can be stored in variables and used in mathematical expressions.

Example:

```flux
z = 3 + 4i

realPart = z.real

imagPart = z.imag

out realPart + imagPart
```

Output:

```text
7
```

#### Checking complex number components

You can also use comparisons and assertions to validate their components.

Example:

```flux
z = 3 + 4i

assert z.real == 3, "Invalid real part"

assert z.imag == 4, "Invalid imaginary part"

out "Complex number validated!"
```

Output:

```text
Complex number validated!
```

Complex numbers are useful for mathematical calculations, simulations, and algorithms that require both real and imaginary values.

### 32. Assertions

Use `assert` to check a condition and raise an error when it is false.

Example:

    age = 18

    assert age >= 0, "Age cannot be negative"

Assertions can help validate input and program state.

### 31. Complex numbers

FluxScript v1.0.0 introduces support for complex numbers.

A complex number contains two components: a real part and an imaginary part.

Complex numbers are commonly written in the form `a + bi`, where `a` is the real part, `b` is the imaginary part, and `i` represents the imaginary unit.

#### Syntax

```flux
z = 3 + 4i
```

In FluxScript, the `i` suffix is used to represent imaginary numbers.

#### Accessing real and imaginary components

You can access both components using `.real` and `.imag`.

Example:

```flux
z = 3 + 4i

out z.real

out z.imag
```

Output:

```text
3

4
```

#### Using complex numbers in expressions

Complex numbers can be stored in variables and used in mathematical expressions.

Example:

```flux
z = 3 + 4i

realPart = z.real

imagPart = z.imag

out realPart + imagPart
```

Output:

```text
7
```

#### Checking complex number components

You can also use comparisons and assertions to validate their components.

Example:

```flux
z = 3 + 4i

assert z.real == 3, "Invalid real part"

assert z.imag == 4, "Invalid imaginary part"

out "Complex number validated!"
```

Output:

```text
Complex number validated!
```

Complex numbers are useful for mathematical calculations, simulations, and algorithms that require both real and imaginary values.

### 34. New string features

#### Newline escapes

Strings support `\n` to represent a newline.

Example:

    out "First line\nSecond line"

#### Raw strings

Raw strings preserve backslashes and other escape sequences literally.

The exact raw-string prefix or delimiter is not specified in this tutorial.

### 35. Regular expressions

The `regex.flux` module provides regular expression operations using a recursive backtracking engine written in FluxScript.

Example:

    get regex

    out regex.search("(cat|dog)+", "xxcatdogyy")

    out regex.findall("(ab|cd)+", "abab xx cdcd")

    out regex.fullmatch("[A-Z][a-z]{2,5}", "Hello")

    out regex.group("(\\d+)-(\\w+)", "123-Alex", 1)

    out regex.groups("(\\d+)-(\\w+)", "123-Alex")

    out regex.search("a+?", "aaaa")

    out regex.search("(ab|a)b", "ab")

The module supports character classes, alternation, capturing groups, and greedy and lazy quantifiers.

Advanced edge cases and performance improvements are planned for v1.1.0.

### 36. Serialization

The `serialize.flux` module converts supported values to strings and reconstructs them.

Functions:

| Function | Description |

|---|---|

| `serialize.dumps(value)` | Serialize a value |

| `serialize.loads(data)` | Deserialize a string |

| `serialize.clone(value)` | Create a deep copy of supported data |

| `serialize.isvalid(data)` | Check whether data can be deserialized |

Example:

    get serialize

    player = {"name": "Alex", "level": 10}

    data = serialize.dumps(player)

    restored = serialize.loads(data)

    out restored["name"]

This initial module uses `json.flux` and supports JSON-compatible values. Functions and cyclic references are not supported.

### 37. Persistent key-value storage

The `storage.flux` module provides key-value persistence, conceptually similar to Python's `shelve`.

Functions:

| Function | Description |

|---|---|

| `storage.open(path)` | Open or initialize a database |

| `storage.set(db, key, value)` | Store a value |

| `storage.get(db, key)` | Retrieve a value |

| `storage.has(db, key)` | Check for a key |

| `storage.remove(db, key)` | Remove a key |

| `storage.keys(db)` | Retrieve keys |

| `storage.values(db)` | Retrieve values |

| `storage.clear(db)` | Remove all entries |

| `storage.save(db)` | Save the database |

| `storage.close(db)` | Save and close the database |

Example:

    get storage

    db = storage.open("players.db")

    storage.set(db, "player1", {"name": "Alex", "level": 10})

    out storage.get(db, "player1")

    storage.close(db)

The module uses `serialize.flux` and native file operations for persistence.

The serialization and storage examples describe the proposed module APIs and require testing against the v1.0.0 interpreter.
### 38. EVERY MODULE FUNCTION

i don't remember if i already said about modules, but just to confirm:

this is based on the 19 uploaded module source files using FluxScript 1.1.0 syntax.

If you wan't to build an DLL module for FluxScript, make sure to compile it with C++17. And all parentheses are kept on one line.

**Note:** The output section is included only where a result is deterministic and supported by the source. Dynamic, random, operating-system-dependent and side-effect-only functions do not have a fixed output.

## console (.flux)

Import:

```flux
get console
```

### clear

**console.clear:** Clears the console or storage contents, depending on the module.

**Syntax:**

```flux
console.clear()
```

**Example:**

```flux
console.clear()
```

### title

**console.title:** Provides the title operation.

**Syntax:**

```flux
console.title(text)
```

**Example:**

```flux
console.title("My App")
```

### color

**console.color:** Provides the color operation.

**Syntax:**

```flux
console.color(name)
```

**Example:**

```flux
console.color("example")
```

### background

**console.background:** Provides the background operation.

**Syntax:**

```flux
console.background(name)
```

**Example:**

```flux
console.background("example")
```

### reset

**console.reset:** Provides the reset operation.

**Syntax:**

```flux
console.reset()
```

**Example:**

```flux
console.reset()
```

### cursor

**console.cursor:** Provides the cursor operation.

**Syntax:**

```flux
console.cursor(x, y)
```

**Example:**

```flux
console.cursor(1, 1)
```

### position

**console.position:** Provides the position operation.

**Syntax:**

```flux
console.position()
```

**Example:**

```flux
out console.position()
```

### size

**console.size:** Provides the size operation.

**Syntax:**

```flux
console.size()
```

**Example:**

```flux
out console.size()
```

### resize

**console.resize:** Provides the resize operation.

**Syntax:**

```flux
console.resize(width, height)
```

**Example:**

```flux
console.resize(1, 1)
```

### hideCursor

**console.hideCursor:** Provides the hide cursor operation.

**Syntax:**

```flux
console.hideCursor()
```

**Example:**

```flux
console.hideCursor()
```

### showCursor

**console.showCursor:** Provides the show cursor operation.

**Syntax:**

```flux
console.showCursor()
```

**Example:**

```flux
console.showCursor()
```

### readKey

**console.readKey:** Provides the read key operation.

**Syntax:**

```flux
console.readKey()
```

**Example:**

```flux
out console.readKey()
```

### keyAvailable

**console.keyAvailable:** Provides the key available operation.

**Syntax:**

```flux
console.keyAvailable()
```

**Example:**

```flux
out console.keyAvailable()
```

### pause

**console.pause:** Provides the pause operation.

**Syntax:**

```flux
console.pause()
```

**Example:**

```flux
console.pause()
```

### beep

**console.beep:** Provides the beep operation.

**Syntax:**

```flux
console.beep(frequency, duration)
```

**Example:**

```flux
console.beep(1, 1)
```

### progress

**console.progress:** Draws a console progress indicator.

**Syntax:**

```flux
console.progress(percent, width)
```

**Example:**

```flux
console.progress(1, 1)
```

### box

**console.box:** Provides the box operation.

**Syntax:**

```flux
console.box(text)
```

**Example:**

```flux
console.box("example")
```

### menu

**console.menu:** Displays a console menu and returns the selection.

**Syntax:**

```flux
console.menu(options)
```

**Example:**

```flux
out console.menu([1, 2, 3])
```

## json (.flux)

Import:

```flux
get json
```

### stringify

**json.stringify:** Converts a value into JSON text.

**Syntax:**

```flux
json.stringify(value)
```

**Example:**

```flux
out json.stringify({"score": 10})
```

### parse

**json.parse:** Parses JSON text.

**Syntax:**

```flux
json.parse(source)
```

**Example:**

```flux
data = json.parse("{\"score\":10}")
out data["score"]
```

**Output:**

```text
10
```

### save

**json.save:** Saves JSON or persistent storage data.

**Syntax:**

```flux
json.save(path, value)
```

**Example:**

```flux
json.save("example", "example")
```

### load

**json.load:** Loads JSON data from a file.

**Syntax:**

```flux
json.load(path)
```

**Example:**

```flux
out json.load("example")
```

## math (.flux)

Import:

```flux
get math
```

### chance

**math.chance:** Returns a chance-based result. The math module uses a percentage from 0 to 100; the game DLL uses a probability from 0 to 1.

**Syntax:**

```flux
math.chance(percent)
```

**Example:**

```flux
if math.chance(5) {
    out "Rare item found!"
}
```

### sq

**math.sq:** Squares a number.

**Syntax:**

```flux
math.sq(x)
```

**Example:**

```flux
out math.sq(5)
```

**Output:**

```text
25
```

### cube

**math.cube:** Cubes a number.

**Syntax:**

```flux
math.cube(x)
```

**Example:**

```flux
out math.cube(3)
```

**Output:**

```text
27
```

### abs

**math.abs:** Returns the absolute value.

**Syntax:**

```flux
math.abs(x)
```

**Example:**

```flux
out math.abs(-5)
```

**Output:**

```text
5
```

### min

**math.min:** Returns the current UTC minute in the time module, or the smaller number in math.

**Syntax:**

```flux
math.min(a, b)
```

**Example:**

```flux
out math.min(1, 1)
```

### max

**math.max:** Provides the max operation.

**Syntax:**

```flux
math.max(a, b)
```

**Example:**

```flux
out math.max(1, 1)
```

### sqrt

**math.sqrt:** Calculates the square root.

**Syntax:**

```flux
math.sqrt(x)
```

**Example:**

```flux
out math.sqrt(25)
```

**Output:**

```text
5
```

### cbrt

**math.cbrt:** Provides the cbrt operation.

**Syntax:**

```flux
math.cbrt(x)
```

**Example:**

```flux
out math.cbrt(1)
```

### fact

**math.fact:** Calculates a factorial.

**Syntax:**

```flux
math.fact(n)
```

**Example:**

```flux
out math.fact(5)
```

**Output:**

```text
120
```

### gcd

**math.gcd:** Finds the greatest common divisor.

**Syntax:**

```flux
math.gcd(a, b)
```

**Example:**

```flux
out math.gcd(12, 18)
```

**Output:**

```text
6
```

### lcm

**math.lcm:** Finds the least common multiple.

**Syntax:**

```flux
math.lcm(a, b)
```

**Example:**

```flux
out math.lcm(4, 6)
```

**Output:**

```text
12
```

### floor

**math.floor:** Provides the floor operation.

**Syntax:**

```flux
math.floor(x)
```

**Example:**

```flux
out math.floor(4.9)
```

**Output:**

```text
4
```

### ceil

**math.ceil:** Provides the ceil operation.

**Syntax:**

```flux
math.ceil(x)
```

**Example:**

```flux
out math.ceil(4.1)
```

**Output:**

```text
5
```

### round

**math.round:** Provides the round operation.

**Syntax:**

```flux
math.round(x)
```

**Example:**

```flux
out math.round(4.6)
```

**Output:**

```text
5
```

### sign

**math.sign:** Provides the sign operation.

**Syntax:**

```flux
math.sign(x)
```

**Example:**

```flux
out math.sign(-3)
```

**Output:**

```text
-1
```

### sin

**math.sin:** Provides the sin operation.

**Syntax:**

```flux
math.sin(x)
```

**Example:**

```flux
out math.sin(1)
```

### cos

**math.cos:** Provides the cos operation.

**Syntax:**

```flux
math.cos(x)
```

**Example:**

```flux
out math.cos(1)
```

### tan

**math.tan:** Provides the tan operation.

**Syntax:**

```flux
math.tan(x)
```

**Example:**

```flux
out math.tan(1)
```

## random (.flux)

Import:

```flux
get random
```

### setseed

**random.setseed:** Sets the pseudorandom generator seed.

**Syntax:**

```flux
random.setseed(value)
```

**Example:**

```flux
out random.setseed("example")
```

### next

**random.next:** Advances the pseudorandom generator.

**Syntax:**

```flux
random.next()
```

**Example:**

```flux
out random.next()
```

### rand

**random.rand:** Returns a pseudorandom number between 0 and 1.

**Syntax:**

```flux
random.rand()
```

**Example:**

```flux
out random.rand()
```

### randint

**random.randint:** Returns a pseudorandom integer within an inclusive range.

**Syntax:**

```flux
random.randint(min, max)
```

**Example:**

```flux
out random.randint(1, 6)
```

### choice

**random.choice:** Selects a random array item.

**Syntax:**

```flux
random.choice(array)
```

**Example:**

```flux
out random.choice(["red", "blue"])
```

### shuffle

**random.shuffle:** Shuffles an array.

**Syntax:**

```flux
random.shuffle(array)
```

**Example:**

```flux
out random.shuffle([1, 2, 3])
```

## regex (.flux)

Import:

```flux
get regex
```

### search

**regex.search:** Searches text for a regular-expression match.

**Syntax:**

```flux
regex.search(pattern, text)
```

**Example:**

```flux
out regex.search("example", "example")
```

### match

**regex.match:** Matches a regular expression at the beginning of text.

**Syntax:**

```flux
regex.match(pattern, text)
```

**Example:**

```flux
out regex.match("example", "example")
```

### fullmatch

**regex.fullmatch:** Checks whether the entire text matches a regular expression.

**Syntax:**

```flux
regex.fullmatch(pattern, text)
```

**Example:**

```flux
out regex.fullmatch("example", "example")
```

### findall

**regex.findall:** Finds all regular-expression matches.

**Syntax:**

```flux
regex.findall(pattern, text)
```

**Example:**

```flux
out regex.findall("example", "example")
```

### replace

**regex.replace:** Replaces regular-expression matches.

**Syntax:**

```flux
regex.replace(pattern, replacement, text)
```

**Example:**

```flux
out regex.replace("example", "example", "example")
```

### split

**regex.split:** Splits text using a regular expression.

**Syntax:**

```flux
regex.split(pattern, text)
```

**Example:**

```flux
out regex.split("example", "example")
```

### groups

**regex.groups:** Returns capture groups from a regular-expression match.

**Syntax:**

```flux
regex.groups(pattern, text)
```

**Example:**

```flux
out regex.groups("example", "example")
```

### group

**regex.group:** Returns a selected capture group.

**Syntax:**

```flux
regex.group(pattern, text, index=0)
```

**Example:**

```flux
out regex.group("example", "example", 1)
```

## serialize (.flux)

Import:

```flux
get serialize
```

### dumps

**serialize.dumps:** Serializes a JSON-compatible value to a string.

**Syntax:**

```flux
serialize.dumps(value)
```

**Example:**

```flux
out serialize.dumps({"a": 1})
```

**Output:**

```text
{"a":1}
```

### loads

**serialize.loads:** Deserializes a JSON string.

**Syntax:**

```flux
serialize.loads(data)
```

**Example:**

```flux
out serialize.loads("example")
```

### clone

**serialize.clone:** Makes a serialized deep copy of a JSON-compatible value.

**Syntax:**

```flux
serialize.clone(value)
```

**Example:**

```flux
out serialize.clone("example")
```

### isvalid

**serialize.isvalid:** Checks whether a string contains valid serialized data.

**Syntax:**

```flux
serialize.isvalid(data)
```

**Example:**

```flux
out serialize.isvalid("example")
```

## storage (.flux)

Import:

```flux
get storage
```

### open

**storage.open:** Opens a storage file or database connection.

**Syntax:**

```flux
storage.open(path)
```

**Example:**

```flux
db = storage.open("save.json")
```

### set

**storage.set:** Sets a key-value entry.

**Syntax:**

```flux
storage.set(db, key, value)
```

**Example:**

```flux
storage.set(db, "example", "example")
```

### get

**storage.get:** Retrieves a key-value entry.

**Syntax:**

```flux
storage.get(db, key)
```

**Example:**

```flux
out storage.get(db, "example")
```

### has

**storage.has:** Checks whether a key exists.

**Syntax:**

```flux
storage.has(db, key)
```

**Example:**

```flux
out storage.has(db, "example")
```

### remove

**storage.remove:** Removes a key-value entry.

**Syntax:**

```flux
storage.remove(db, key)
```

**Example:**

```flux
storage.remove(db, "example")
```

### keys

**storage.keys:** Returns stored keys.

**Syntax:**

```flux
storage.keys(db)
```

**Example:**

```flux
out storage.keys(db)
```

### values

**storage.values:** Returns stored values.

**Syntax:**

```flux
storage.values(db)
```

**Example:**

```flux
out storage.values(db)
```

### clear

**storage.clear:** Clears the console or storage contents, depending on the module.

**Syntax:**

```flux
storage.clear(db)
```

**Example:**

```flux
storage.clear(db)
```

### save

**storage.save:** Saves JSON or persistent storage data.

**Syntax:**

```flux
storage.save(db)
```

**Example:**

```flux
storage.save(db)
```

### close

**storage.close:** Closes a storage file or database connection.

**Syntax:**

```flux
storage.close(db)
```

**Example:**

```flux
storage.close(db)
```

## string (.flux)

Import:

```flux
get string
```

### repeat

**string.repeat:** Repeats text a specified number of times.

**Syntax:**

```flux
string.repeat(text, times)
```

**Example:**

```flux
out string.repeat("Hi", 3)
```

**Output:**

```text
HiHiHi
```

### reverse

**string.reverse:** Reverses text.

**Syntax:**

```flux
string.reverse(text)
```

**Example:**

```flux
out string.reverse("abc")
```

**Output:**

```text
cba
```

## time (.flux)

Import:

```flux
get time
```

### now

**time.now:** Returns the current timestamp.

**Syntax:**

```flux
time.now()
```

**Example:**

```flux
out time.now()
```

### ms

**time.ms:** Returns the current timestamp in milliseconds.

**Syntax:**

```flux
time.ms()
```

**Example:**

```flux
out time.ms()
```

### sec

**time.sec:** Returns the current UTC second.

**Syntax:**

```flux
time.sec()
```

**Example:**

```flux
out time.sec()
```

### min

**time.min:** Returns the current UTC minute in the time module, or the smaller number in math.

**Syntax:**

```flux
time.min()
```

**Example:**

```flux
out time.min()
```

### hrs

**time.hrs:** Returns the current UTC hour.

**Syntax:**

```flux
time.hrs()
```

**Example:**

```flux
out time.hrs()
```

### days

**time.days:** Returns the number of whole days since the Unix epoch.

**Syntax:**

```flux
time.days()
```

**Example:**

```flux
out time.days()
```

## bitwise (native DLL)

Import:

```flux
get bitwise.dll
```

### band

**bitwise.band:** Provides the band operation.

**Syntax:**

```flux
bitwise.band(n1, n2)
```

**Example:**

```flux
out bitwise.band(6, 3)
```

**Output:**

```text
2
```

### bor

**bitwise.bor:** Provides the bor operation.

**Syntax:**

```flux
bitwise.bor(n1, n2)
```

**Example:**

```flux
out bitwise.bor(1, 1)
```

### bxor

**bitwise.bxor:** Provides the bxor operation.

**Syntax:**

```flux
bitwise.bxor(n1, n2)
```

**Example:**

```flux
out bitwise.bxor(1, 1)
```

### bnot

**bitwise.bnot:** Provides the bnot operation.

**Syntax:**

```flux
bitwise.bnot(n1)
```

**Example:**

```flux
out bitwise.bnot(1)
```

### shl

**bitwise.shl:** Provides the shl operation.

**Syntax:**

```flux
bitwise.shl(n1, n2)
```

**Example:**

```flux
out bitwise.shl(1, 1)
```

### shr

**bitwise.shr:** Provides the shr operation.

**Syntax:**

```flux
bitwise.shr(n1, n2)
```

**Example:**

```flux
out bitwise.shr(1, 1)
```

### rol

**bitwise.rol:** Provides the rol operation.

**Syntax:**

```flux
bitwise.rol(n1, n2)
```

**Example:**

```flux
out bitwise.rol(1, 1)
```

### ror

**bitwise.ror:** Provides the ror operation.

**Syntax:**

```flux
bitwise.ror(n1, n2)
```

**Example:**

```flux
out bitwise.ror(1, 1)
```

### popcount

**bitwise.popcount:** Provides the popcount operation.

**Syntax:**

```flux
bitwise.popcount(n1)
```

**Example:**

```flux
out bitwise.popcount(1)
```

### bitGet

**bitwise.bitGet:** Provides the bit get operation.

**Syntax:**

```flux
bitwise.bitGet(n1, n2)
```

**Example:**

```flux
out bitwise.bitGet(1, 1)
```

### bitSet

**bitwise.bitSet:** Provides the bit set operation.

**Syntax:**

```flux
bitwise.bitSet(n1, n2)
```

**Example:**

```flux
out bitwise.bitSet(1, 1)
```

### bitClear

**bitwise.bitClear:** Provides the bit clear operation.

**Syntax:**

```flux
bitwise.bitClear(n1, n2)
```

**Example:**

```flux
out bitwise.bitClear(1, 1)
```

## crypto (native DLL)

Import:

```flux
get crypto.dll
```

### sha256

**crypto.sha256:** Provides the sha256 operation.

**Syntax:**

```flux
crypto.sha256(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.sha256(...)
```

### sha512

**crypto.sha512:** Provides the sha512 operation.

**Syntax:**

```flux
crypto.sha512(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.sha512(...)
```

### sha1

**crypto.sha1:** Provides the sha1 operation.

**Syntax:**

```flux
crypto.sha1(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.sha1(...)
```

### md5

**crypto.md5:** Provides the md5 operation.

**Syntax:**

```flux
crypto.md5(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.md5(...)
```

### hmacSha256

**crypto.hmacSha256:** Provides the hmac sha256 operation.

**Syntax:**

```flux
crypto.hmacSha256(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.hmacSha256(...)
```

### hmacSha512

**crypto.hmacSha512:** Provides the hmac sha512 operation.

**Syntax:**

```flux
crypto.hmacSha512(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.hmacSha512(...)
```

### randomHex

**crypto.randomHex:** Provides the random hex operation.

**Syntax:**

```flux
crypto.randomHex(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.randomHex(...)
```

### randomInt

**crypto.randomInt:** Provides the random int operation.

**Syntax:**

```flux
crypto.randomInt(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.randomInt(...)
```

### hexEncode

**crypto.hexEncode:** Provides the hex encode operation.

**Syntax:**

```flux
crypto.hexEncode(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.hexEncode(...)
```

### hexDecode

**crypto.hexDecode:** Provides the hex decode operation.

**Syntax:**

```flux
crypto.hexDecode(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.hexDecode(...)
```

### base64Encode

**crypto.base64Encode:** Provides the base64encode operation.

**Syntax:**

```flux
crypto.base64Encode(...)
```

**Example:**

```flux
out crypto.base64Encode("Hi")
```

**Output:**

```text
SGk=
```

### base64Decode

**crypto.base64Decode:** Provides the base64decode operation.

**Syntax:**

```flux
crypto.base64Decode(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.base64Decode(...)
```

### constantTimeEqual

**crypto.constantTimeEqual:** Provides the constant time equal operation.

**Syntax:**

```flux
crypto.constantTimeEqual(...)
```

**Example:**

```flux
// See the module source for argument requirements
// crypto.constantTimeEqual(...)
```

## database (native DLL)

Import:

```flux
get database.dll
```

### open

**database.open:** Opens a storage file or database connection.

**Syntax:**

```flux
database.open(path)
```

**Example:**

```flux
out database.open("example")
```

### openReadOnly

**database.openReadOnly:** Provides the open read only operation.

**Syntax:**

```flux
database.openReadOnly(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.openReadOnly(...)
```

### close

**database.close:** Closes a storage file or database connection.

**Syntax:**

```flux
database.close(db)
```

**Example:**

```flux
database.close(db)
```

### exec

**database.exec:** Provides the exec operation.

**Syntax:**

```flux
database.exec(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.exec(...)
```

### scalar

**database.scalar:** Provides the scalar operation.

**Syntax:**

```flux
database.scalar(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.scalar(...)
```

### queryJson

**database.queryJson:** Provides the query json operation.

**Syntax:**

```flux
database.queryJson(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.queryJson(...)
```

### prepare

**database.prepare:** Provides the prepare operation.

**Syntax:**

```flux
database.prepare(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.prepare(...)
```

### bindText

**database.bindText:** Provides the bind text operation.

**Syntax:**

```flux
database.bindText(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.bindText(...)
```

### bindNumber

**database.bindNumber:** Provides the bind number operation.

**Syntax:**

```flux
database.bindNumber(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.bindNumber(...)
```

### bindNull

**database.bindNull:** Provides the bind null operation.

**Syntax:**

```flux
database.bindNull(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.bindNull(...)
```

### step

**database.step:** Provides the step operation.

**Syntax:**

```flux
database.step(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.step(...)
```

### rowJson

**database.rowJson:** Provides the row json operation.

**Syntax:**

```flux
database.rowJson(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.rowJson(...)
```

### columnCount

**database.columnCount:** Provides the column count operation.

**Syntax:**

```flux
database.columnCount(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.columnCount(...)
```

### reset

**database.reset:** Provides the reset operation.

**Syntax:**

```flux
database.reset(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.reset(...)
```

### clearBindings

**database.clearBindings:** Provides the clear bindings operation.

**Syntax:**

```flux
database.clearBindings(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.clearBindings(...)
```

### finalize

**database.finalize:** Provides the finalize operation.

**Syntax:**

```flux
database.finalize(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.finalize(...)
```

### changes

**database.changes:** Provides the changes operation.

**Syntax:**

```flux
database.changes(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.changes(...)
```

### lastInsertId

**database.lastInsertId:** Provides the last insert id operation.

**Syntax:**

```flux
database.lastInsertId(...)
```

**Example:**

```flux
// See the module source for argument requirements
// database.lastInsertId(...)
```

## datetime (native DLL)

Import:

```flux
get datetime.dll
```

### now

**datetime.now:** Returns the current timestamp.

**Syntax:**

```flux
datetime.now()
```

**Example:**

```flux
out datetime.now()
```

### timestamp

**datetime.timestamp:** Provides the timestamp operation.

**Syntax:**

```flux
datetime.timestamp(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out datetime.timestamp(1, 1, 1, 1, 1, 1)
```

### year

**datetime.year:** Provides the year operation.

**Syntax:**

```flux
datetime.year(n1)
```

**Example:**

```flux
out datetime.year(1)
```

### month

**datetime.month:** Provides the month operation.

**Syntax:**

```flux
datetime.month(n1)
```

**Example:**

```flux
out datetime.month(1)
```

### day

**datetime.day:** Provides the day operation.

**Syntax:**

```flux
datetime.day(n1)
```

**Example:**

```flux
out datetime.day(1)
```

### hour

**datetime.hour:** Provides the hour operation.

**Syntax:**

```flux
datetime.hour(n1)
```

**Example:**

```flux
out datetime.hour(1)
```

### minute

**datetime.minute:** Provides the minute operation.

**Syntax:**

```flux
datetime.minute(n1)
```

**Example:**

```flux
out datetime.minute(1)
```

### second

**datetime.second:** Provides the second operation.

**Syntax:**

```flux
datetime.second(n1)
```

**Example:**

```flux
out datetime.second(1)
```

### weekday

**datetime.weekday:** Provides the weekday operation.

**Syntax:**

```flux
datetime.weekday(n1)
```

**Example:**

```flux
out datetime.weekday(1)
```

### dayOfYear

**datetime.dayOfYear:** Provides the day of year operation.

**Syntax:**

```flux
datetime.dayOfYear(n1)
```

**Example:**

```flux
out datetime.dayOfYear(1)
```

### isLeapYear

**datetime.isLeapYear:** Provides the is leap year operation.

**Syntax:**

```flux
datetime.isLeapYear(n1)
```

**Example:**

```flux
out datetime.isLeapYear(2024)
```

**Output:**

```text
1
```

### daysInMonth

**datetime.daysInMonth:** Provides the days in month operation.

**Syntax:**

```flux
datetime.daysInMonth(n1, n2)
```

**Example:**

```flux
out datetime.daysInMonth(1, 1)
```

### addDays

**datetime.addDays:** Provides the add days operation.

**Syntax:**

```flux
datetime.addDays(n1, n2)
```

**Example:**

```flux
out datetime.addDays(1, 1)
```

### diffDays

**datetime.diffDays:** Provides the diff days operation.

**Syntax:**

```flux
datetime.diffDays(n1, n2)
```

**Example:**

```flux
out datetime.diffDays(1, 1)
```

### addSeconds

**datetime.addSeconds:** Provides the add seconds operation.

**Syntax:**

```flux
datetime.addSeconds(n1, n2)
```

**Example:**

```flux
out datetime.addSeconds(1, 1)
```

## game (native DLL)

Import:

```flux
get game.dll
```

### clamp

**game.clamp:** Provides the clamp operation.

**Syntax:**

```flux
game.clamp(n1, n2, n3)
```

**Example:**

```flux
out game.clamp(1, 1, 1)
```

### normalize

**game.normalize:** Provides the normalize operation.

**Syntax:**

```flux
game.normalize(n1, n2, n3)
```

**Example:**

```flux
out game.normalize(1, 1, 1)
```

### health

**game.health:** Provides the health operation.

**Syntax:**

```flux
game.health(n1, n2)
```

**Example:**

```flux
out game.health(100, 30)
```

**Output:**

```text
70
```

### heal

**game.heal:** Provides the heal operation.

**Syntax:**

```flux
game.heal(n1, n2, n3)
```

**Example:**

```flux
out game.heal(50, 20, 100)
```

**Output:**

```text
70
```

### isDead

**game.isDead:** Provides the is dead operation.

**Syntax:**

```flux
game.isDead(n1)
```

**Example:**

```flux
out game.isDead(1)
```

### score

**game.score:** Provides the score operation.

**Syntax:**

```flux
game.score(n1, n2)
```

**Example:**

```flux
out game.score(10, 5)
```

**Output:**

```text
15
```

### level

**game.level:** Provides the level operation.

**Syntax:**

```flux
game.level(n1, n2)
```

**Example:**

```flux
out game.level(1, 1)
```

### xpRequired

**game.xpRequired:** Provides the xp required operation.

**Syntax:**

```flux
game.xpRequired(n1, n2)
```

**Example:**

```flux
out game.xpRequired(1, 1)
```

### cooldown

**game.cooldown:** Provides the cooldown operation.

**Syntax:**

```flux
game.cooldown(n1, n2)
```

**Example:**

```flux
out game.cooldown(1, 1)
```

### chance

**game.chance:** Returns a chance-based result. The math module uses a percentage from 0 to 100; the game DLL uses a probability from 0 to 1.

**Syntax:**

```flux
game.chance(probability)
```

**Example:**

```flux
out game.chance(1)
```

### randomRange

**game.randomRange:** Provides the random range operation.

**Syntax:**

```flux
game.randomRange(n1, n2)
```

**Example:**

```flux
out game.randomRange(1, 1)
```

### moveTowards

**game.moveTowards:** Provides the move towards operation.

**Syntax:**

```flux
game.moveTowards(n1, n2, n3)
```

**Example:**

```flux
out game.moveTowards(1, 1, 1)
```

### wrap

**game.wrap:** Provides the wrap operation.

**Syntax:**

```flux
game.wrap(n1, n2, n3)
```

**Example:**

```flux
out game.wrap(1, 1, 1)
```

### smoothstep

**game.smoothstep:** Provides the smoothstep operation.

**Syntax:**

```flux
game.smoothstep(n1, n2, n3)
```

**Example:**

```flux
out game.smoothstep(1, 1, 1)
```

### damage

**game.damage:** Provides the damage operation.

**Syntax:**

```flux
game.damage(n1, n2)
```

**Example:**

```flux
out game.damage(1, 1)
```

## geometry (native DLL)

Import:

```flux
get geometry.dll
```

### circleArea

**geometry.circleArea:** Provides the circle area operation.

**Syntax:**

```flux
geometry.circleArea(n1)
```

**Example:**

```flux
out geometry.circleArea(1)
```

### circleCircumference

**geometry.circleCircumference:** Provides the circle circumference operation.

**Syntax:**

```flux
geometry.circleCircumference(n1)
```

**Example:**

```flux
out geometry.circleCircumference(1)
```

### rectangleArea

**geometry.rectangleArea:** Provides the rectangle area operation.

**Syntax:**

```flux
geometry.rectangleArea(n1, n2)
```

**Example:**

```flux
out geometry.rectangleArea(5, 4)
```

**Output:**

```text
20
```

### triangleArea

**geometry.triangleArea:** Provides the triangle area operation.

**Syntax:**

```flux
geometry.triangleArea(n1, n2)
```

**Example:**

```flux
out geometry.triangleArea(6, 4)
```

**Output:**

```text
12
```

### sphereVolume

**geometry.sphereVolume:** Provides the sphere volume operation.

**Syntax:**

```flux
geometry.sphereVolume(n1)
```

**Example:**

```flux
out geometry.sphereVolume(1)
```

### sphereArea

**geometry.sphereArea:** Provides the sphere area operation.

**Syntax:**

```flux
geometry.sphereArea(n1)
```

**Example:**

```flux
out geometry.sphereArea(1)
```

### boxVolume

**geometry.boxVolume:** Provides the box volume operation.

**Syntax:**

```flux
geometry.boxVolume(n1, n2, n3)
```

**Example:**

```flux
out geometry.boxVolume(1, 1, 1)
```

### distance3D

**geometry.distance3D:** Provides the distance3d operation.

**Syntax:**

```flux
geometry.distance3D(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out geometry.distance3D(1, 1, 1, 1, 1, 1)
```

### dot2D

**geometry.dot2D:** Provides the dot2d operation.

**Syntax:**

```flux
geometry.dot2D(n1, n2, n3, n4)
```

**Example:**

```flux
out geometry.dot2D(1, 1, 1, 1)
```

### dot3D

**geometry.dot3D:** Provides the dot3d operation.

**Syntax:**

```flux
geometry.dot3D(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out geometry.dot3D(1, 1, 1, 1, 1, 1)
```

### cross2D

**geometry.cross2D:** Provides the cross2d operation.

**Syntax:**

```flux
geometry.cross2D(n1, n2, n3, n4)
```

**Example:**

```flux
out geometry.cross2D(1, 1, 1, 1)
```

### cross3DX

**geometry.cross3DX:** Provides the cross3dx operation.

**Syntax:**

```flux
geometry.cross3DX(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out geometry.cross3DX(1, 1, 1, 1, 1, 1)
```

### cross3DY

**geometry.cross3DY:** Provides the cross3dy operation.

**Syntax:**

```flux
geometry.cross3DY(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out geometry.cross3DY(1, 1, 1, 1, 1, 1)
```

### cross3DZ

**geometry.cross3DZ:** Provides the cross3dz operation.

**Syntax:**

```flux
geometry.cross3DZ(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out geometry.cross3DZ(1, 1, 1, 1, 1, 1)
```

### rotate2DX

**geometry.rotate2DX:** Provides the rotate2dx operation.

**Syntax:**

```flux
geometry.rotate2DX(n1, n2, n3)
```

**Example:**

```flux
out geometry.rotate2DX(1, 1, 1)
```

### rotate2DY

**geometry.rotate2DY:** Provides the rotate2dy operation.

**Syntax:**

```flux
geometry.rotate2DY(n1, n2, n3)
```

**Example:**

```flux
out geometry.rotate2DY(1, 1, 1)
```

## network (native DLL)

Import:

```flux
get network.dll
```

### httpGet

**network.httpGet:** Provides the http get operation.

**Syntax:**

```flux
network.httpGet(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.httpGet(...)
```

### httpPost

**network.httpPost:** Provides the http post operation.

**Syntax:**

```flux
network.httpPost(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.httpPost(...)
```

### httpStatus

**network.httpStatus:** Provides the http status operation.

**Syntax:**

```flux
network.httpStatus(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.httpStatus(...)
```

### httpPostStatus

**network.httpPostStatus:** Provides the http post status operation.

**Syntax:**

```flux
network.httpPostStatus(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.httpPostStatus(...)
```

### urlEncode

**network.urlEncode:** Provides the url encode operation.

**Syntax:**

```flux
network.urlEncode(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.urlEncode(...)
```

### urlDecode

**network.urlDecode:** Provides the url decode operation.

**Syntax:**

```flux
network.urlDecode(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.urlDecode(...)
```

### dnsLookup

**network.dnsLookup:** Provides the dns lookup operation.

**Syntax:**

```flux
network.dnsLookup(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.dnsLookup(...)
```

### hostname

**network.hostname:** Provides the hostname operation.

**Syntax:**

```flux
network.hostname(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.hostname(...)
```

### ipv4ToInt

**network.ipv4ToInt:** Provides the ipv4to int operation.

**Syntax:**

```flux
network.ipv4ToInt(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.ipv4ToInt(...)
```

### intToIpv4

**network.intToIpv4:** Provides the int to ipv4 operation.

**Syntax:**

```flux
network.intToIpv4(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.intToIpv4(...)
```

### isIpv4

**network.isIpv4:** Provides the is ipv4 operation.

**Syntax:**

```flux
network.isIpv4(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.isIpv4(...)
```

### tcpReachable

**network.tcpReachable:** Provides the tcp reachable operation.

**Syntax:**

```flux
network.tcpReachable(...)
```

**Example:**

```flux
// See the module source for argument requirements
// network.tcpReachable(...)
```

## physics (native DLL)

Import:

```flux
get physics.dll
```

### velocity

**physics.velocity:** Provides the velocity operation.

**Syntax:**

```flux
physics.velocity(n1, n2)
```

**Example:**

```flux
out physics.velocity(1, 1)
```

### acceleration

**physics.acceleration:** Provides the acceleration operation.

**Syntax:**

```flux
physics.acceleration(n1, n2, n3)
```

**Example:**

```flux
out physics.acceleration(1, 1, 1)
```

### force

**physics.force:** Provides the force operation.

**Syntax:**

```flux
physics.force(n1, n2)
```

**Example:**

```flux
out physics.force(5, 3)
```

**Output:**

```text
15
```

### gravity

**physics.gravity:** Provides the gravity operation.

**Syntax:**

```flux
physics.gravity(n1, n2)
```

**Example:**

```flux
out physics.gravity(1, 1)
```

### kineticEnergy

**physics.kineticEnergy:** Provides the kinetic energy operation.

**Syntax:**

```flux
physics.kineticEnergy(n1, n2)
```

**Example:**

```flux
out physics.kineticEnergy(1, 1)
```

### potentialEnergy

**physics.potentialEnergy:** Provides the potential energy operation.

**Syntax:**

```flux
physics.potentialEnergy(n1, n2, n3)
```

**Example:**

```flux
out physics.potentialEnergy(1, 1, 1)
```

### momentum

**physics.momentum:** Provides the momentum operation.

**Syntax:**

```flux
physics.momentum(n1, n2)
```

**Example:**

```flux
out physics.momentum(1, 1)
```

### distance

**physics.distance:** Provides the distance operation.

**Syntax:**

```flux
physics.distance(n1, n2, n3)
```

**Example:**

```flux
out physics.distance(1, 1, 1)
```

### friction

**physics.friction:** Provides the friction operation.

**Syntax:**

```flux
physics.friction(n1, n2)
```

**Example:**

```flux
out physics.friction(1, 1)
```

### bounce

**physics.bounce:** Provides the bounce operation.

**Syntax:**

```flux
physics.bounce(n1, n2)
```

**Example:**

```flux
out physics.bounce(1, 1)
```

### lerp

**physics.lerp:** Provides the lerp operation.

**Syntax:**

```flux
physics.lerp(n1, n2, n3)
```

**Example:**

```flux
out physics.lerp(1, 1, 1)
```

### distance2D

**physics.distance2D:** Provides the distance2d operation.

**Syntax:**

```flux
physics.distance2D(n1, n2, n3, n4)
```

**Example:**

```flux
out physics.distance2D(1, 1, 1, 1)
```

### angle2D

**physics.angle2D:** Provides the angle2d operation.

**Syntax:**

```flux
physics.angle2D(n1, n2, n3, n4)
```

**Example:**

```flux
out physics.angle2D(1, 1, 1, 1)
```

### moveX

**physics.moveX:** Provides the move x operation.

**Syntax:**

```flux
physics.moveX(n1, n2, n3)
```

**Example:**

```flux
out physics.moveX(1, 1, 1)
```

### moveY

**physics.moveY:** Provides the move y operation.

**Syntax:**

```flux
physics.moveY(n1, n2, n3)
```

**Example:**

```flux
out physics.moveY(1, 1, 1)
```

### rectRect

**physics.rectRect:** Provides the rect rect operation.

**Syntax:**

```flux
physics.rectRect(n1, n2, n3, n4, n5, n6, n7, n8)
```

**Example:**

```flux
out physics.rectRect(1, 1, 1, 1, 1, 1, 1, 1)
```

### circleCircle

**physics.circleCircle:** Provides the circle circle operation.

**Syntax:**

```flux
physics.circleCircle(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out physics.circleCircle(1, 1, 1, 1, 1, 1)
```

### rectCircle

**physics.rectCircle:** Provides the rect circle operation.

**Syntax:**

```flux
physics.rectCircle(n1, n2, n3, n4, n5, n6, n7)
```

**Example:**

```flux
out physics.rectCircle(1, 1, 1, 1, 1, 1, 1)
```

### pointRect

**physics.pointRect:** Provides the point rect operation.

**Syntax:**

```flux
physics.pointRect(n1, n2, n3, n4, n5, n6)
```

**Example:**

```flux
out physics.pointRect(1, 1, 1, 1, 1, 1)
```

### pointCircle

**physics.pointCircle:** Provides the point circle operation.

**Syntax:**

```flux
physics.pointCircle(n1, n2, n3, n4, n5)
```

**Example:**

```flux
out physics.pointCircle(1, 1, 1, 1, 1)
```

### lineLine

**physics.lineLine:** Provides the line line operation.

**Syntax:**

```flux
physics.lineLine(n1, n2, n3, n4, n5, n6, n7, n8)
```

**Example:**

```flux
out physics.lineLine(1, 1, 1, 1, 1, 1, 1, 1)
```

### collisionDistance

**physics.collisionDistance:** Provides the collision distance operation.

**Syntax:**

```flux
physics.collisionDistance(n1, n2, n3, n4)
```

**Example:**

```flux
out physics.collisionDistance(1, 1, 1, 1)
```

### overlapX

**physics.overlapX:** Provides the overlap x operation.

**Syntax:**

```flux
physics.overlapX(n1, n2, n3, n4)
```

**Example:**

```flux
out physics.overlapX(1, 1, 1, 1)
```

### overlapY

**physics.overlapY:** Provides the overlap y operation.

**Syntax:**

```flux
physics.overlapY(n1, n2, n3, n4)
```

**Example:**

```flux
out physics.overlapY(1, 1, 1, 1)
```

### raycast2D

**physics.raycast2D:** Provides the raycast2d operation.

**Syntax:**

```flux
physics.raycast2D(n1, n2, n3, n4, n5, n6, n7)
```

**Example:**

```flux
out physics.raycast2D(1, 1, 1, 1, 1, 1, 1)
```

## statistics (native DLL)

Import:

```flux
get statistics.dll
```

### sum

**statistics.sum:** Provides the sum operation.

**Syntax:**

```flux
statistics.sum(...)
```

**Example:**

```flux
out statistics.sum(2, 3, 4)
```

**Output:**

```text
9
```

### product

**statistics.product:** Provides the product operation.

**Syntax:**

```flux
statistics.product(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.product(...)
```

### mean

**statistics.mean:** Provides the mean operation.

**Syntax:**

```flux
statistics.mean(...)
```

**Example:**

```flux
out statistics.mean(2, 4, 6)
```

**Output:**

```text
4
```

### median

**statistics.median:** Provides the median operation.

**Syntax:**

```flux
statistics.median(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.median(...)
```

### mode

**statistics.mode:** Provides the mode operation.

**Syntax:**

```flux
statistics.mode(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.mode(...)
```

### variance

**statistics.variance:** Provides the variance operation.

**Syntax:**

```flux
statistics.variance(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.variance(...)
```

### stddev

**statistics.stddev:** Provides the stddev operation.

**Syntax:**

```flux
statistics.stddev(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.stddev(...)
```

### min

**statistics.min:** Returns the current UTC minute in the time module, or the smaller number in math.

**Syntax:**

```flux
statistics.min(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.min(...)
```

### max

**statistics.max:** Provides the max operation.

**Syntax:**

```flux
statistics.max(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.max(...)
```

### range

**statistics.range:** Provides the range operation.

**Syntax:**

```flux
statistics.range(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.range(...)
```

### percentile

**statistics.percentile:** Provides the percentile operation.

**Syntax:**

```flux
statistics.percentile(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.percentile(...)
```

### factorial

**statistics.factorial:** Provides the factorial operation.

**Syntax:**

```flux
statistics.factorial(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.factorial(...)
```

### combination

**statistics.combination:** Provides the combination operation.

**Syntax:**

```flux
statistics.combination(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.combination(...)
```

### permutation

**statistics.permutation:** Provides the permutation operation.

**Syntax:**

```flux
statistics.permutation(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.permutation(...)
```

### geometricMean

**statistics.geometricMean:** Provides the geometric mean operation.

**Syntax:**

```flux
statistics.geometricMean(...)
```

**Example:**

```flux
// See the module source for argument requirements
// statistics.geometricMean(...)
```

## system (native DLL)

Import:

```flux
get system.dll
```

### cpuCount

**system.cpuCount:** Provides the cpu count operation.

**Syntax:**

```flux
system.cpuCount()
```

**Example:**

```flux
out system.cpuCount()
```

### memoryTotalMB

**system.memoryTotalMB:** Provides the memory total mb operation.

**Syntax:**

```flux
system.memoryTotalMB()
```

**Example:**

```flux
out system.memoryTotalMB()
```

### memoryFreeMB

**system.memoryFreeMB:** Provides the memory free mb operation.

**Syntax:**

```flux
system.memoryFreeMB()
```

**Example:**

```flux
out system.memoryFreeMB()
```

### memoryLoad

**system.memoryLoad:** Provides the memory load operation.

**Syntax:**

```flux
system.memoryLoad()
```

**Example:**

```flux
out system.memoryLoad()
```

### uptimeSeconds

**system.uptimeSeconds:** Provides the uptime seconds operation.

**Syntax:**

```flux
system.uptimeSeconds()
```

**Example:**

```flux
out system.uptimeSeconds()
```

### processId

**system.processId:** Provides the process id operation.

**Syntax:**

```flux
system.processId()
```

**Example:**

```flux
out system.processId()
```

### threadId

**system.threadId:** Provides the thread id operation.

**Syntax:**

```flux
system.threadId()
```

**Example:**

```flux
out system.threadId()
```

### pageSize

**system.pageSize:** Provides the page size operation.

**Syntax:**

```flux
system.pageSize()
```

**Example:**

```flux
out system.pageSize()
```

### pointerBits

**system.pointerBits:** Provides the pointer bits operation.

**Syntax:**

```flux
system.pointerBits()
```

**Example:**

```flux
out system.pointerBits()
```

### screenWidth

**system.screenWidth:** Provides the screen width operation.

**Syntax:**

```flux
system.screenWidth()
```

**Example:**

```flux
out system.screenWidth()
```

### screenHeight

**system.screenHeight:** Provides the screen height operation.

**Syntax:**

```flux
system.screenHeight()
```

**Example:**

```flux
out system.screenHeight()
```

### diskTotalMB

**system.diskTotalMB:** Provides the disk total mb operation.

**Syntax:**

```flux
system.diskTotalMB()
```

**Example:**

```flux
out system.diskTotalMB()
```

### diskFreeMB

**system.diskFreeMB:** Provides the disk free mb operation.

**Syntax:**

```flux
system.diskFreeMB()
```

**Example:**

```flux
out system.diskFreeMB()
```

### processCpuSeconds

**system.processCpuSeconds:** Provides the process cpu seconds operation.

**Syntax:**

```flux
system.processCpuSeconds()
```

**Example:**

```flux
out system.processCpuSeconds()
```

### sleepMs

**system.sleepMs:** Provides the sleep ms operation.

**Syntax:**

```flux
system.sleepMs(n1)
```

**Example:**

```flux
system.sleepMs(1)
```

### hostname

**system.hostname:** Provides the hostname operation.

**Syntax:**

```flux
system.hostname()
```

**Example:**

```flux
out system.hostname()
```

### username

**system.username:** Provides the username operation.

**Syntax:**

```flux
system.username()
```

**Example:**

```flux
out system.username()
```

### osName

**system.osName:** Provides the os name operation.

**Syntax:**

```flux
system.osName()
```

**Example:**

```flux
out system.osName()
```

### architecture

**system.architecture:** Provides the architecture operation.

**Syntax:**

```flux
system.architecture()
```

**Example:**

```flux
out system.architecture()
```

### windowsDirectory

**system.windowsDirectory:** Provides the windows directory operation.

**Syntax:**

```flux
system.windowsDirectory()
```

**Example:**

```flux
out system.windowsDirectory()
```

## IMPORTANT NOTES

- `math.chance(5)` is a 5% probability; `game.chance(0.05)` uses 0–1 probability.


- `math.sin`, `math.cos` and `math.tan` operate on radians; the implementations use an approximation.

- `time` functions use UTC and the native timestamp has one-second precision.

- `serialize` supports JSON-compatible values only, not functions or cyclic references.

- `storage.close(db)` saves data before closing the store.

- `database` requires `sqlite3.dll` beside `flux.exe` according to the source.

- For native functions with `...` arguments, the source exposes the export name but the signature is not established by the simple exported-function declaration; inspect the argument validation in its C++ implementation before publishing a runnable example.

- FluxScript is still in development.

- FluxScript v1.0.0 extends native Win32 GUI controls, events, and properties.

- GUI window and control sizes use `WxH` literals.

- GUI events include `.onclick`, `.onrclick`, `.onchange`, `.detectmouse`, and `.detectMouseMoving`.

- GUI applications can start from `func -ready() { }`.

- FluxScript v0.8.0 added native console operations.

- FluxScript uses C++17 and MSVC for its Windows interpreter.

- Use `{ }` for blocks.

- Statements do not require semicolons.

- `if`, `elif`, and `while` do not require parentheses around conditions.

- `for i(n)` starts at 0 and runs `n` times.

- Extended `for` loops support start, end, and step values.

- `break`, `continue`, and `pass` are supported.

- Arrays and strings use zero-based indexing.

- Dictionaries currently require string keys.

- `.sort` sorts arrays in ascending order.

- `.rev` only reverses the current array order.

- Methods use parentheses-free syntax.

- Functions and module functions use parentheses for arguments.

- Default function parameters are supported.

- `input()` automatically detects numbers, booleans, and strings.

- `catch(error)` provides access to error messages.

- `get` loads modules from the `module` directory.

- FluxScript modules are written in FluxScript itself.

- Only `.flux` file can be used with the interpreter.

- v1.0.0 adds aliases, inheritance, lambdas, named arguments, complex numbers, assertions, hierarchical packages, regex, serialization, and persistent storage.

- Some features may change in future versions.

That's it! Thanks for trying FluxScript v1.0.0!
Note that some features may not be showed here.
