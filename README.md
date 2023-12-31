# Powers Count

The C++ code which is able to count the number of powers two values, <em><a href="https://www.codecogs.com/eqnedit.php?latex=\color{white} a^b" target="_blank"><img src="https://latex.codecogs.com/png.latex?\color{white}%20a^b" title="a^b" /></a></em>, where 2 <= a, b <= 1000


## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)

## Features

- For large numbers U may want to use multithreading and this projects procides you this opportunity
```cpp
int count = multithreadedCount(a, b, 6); // 6 is threads count
std::cout << "Multithreaded Pocet se rovna " << count << std::endl;
```

## Installation

Your compiler must be able to do C++20.

## Usage

```bash
# Example installation commands
git clone https://github.com/turbolaft/powersCount.git
cd powersCount
make
./target/main 1000 1000
```

## Examples

![From 10 to 100](topTable.png)
![From 100 to 1000](bottomTable.png)