<div align="center">

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](LICENSE)
[![Method: Alignment-Free](https://img.shields.io/static/v1.svg?label=Method&message=alignment-free&color=yellow)](#)
[![Extra: Color ASCII](https://img.shields.io/static/v1.svg?label=Extra&message=color-ascii&color=orange)](#)
[![Mode: Multi](https://img.shields.io/static/v1.svg?label=Mode&message=multi&color=blue)](#)
[![Version: 2.1](https://img.shields.io/static/v1.svg?label=Version&message=2.1&color=green)](#)

</div>

<p align="center">
  <img src="imgs/logo.png" alt="SPARK" width="250" />
</p>

<h2 align="center"><b>SPARK: simulation and search for exact and approximate Turing machines with specific characteristics</b></h2>

---

SPARK is a high-performance C program designed to **simulate**, **search**, and **analyze** exact or approximate Turing Machines (TMs) based on custom configurations. It features **alignment-free methods**, **colorized ASCII visualizations**, and supports multiple execution **modes** for various research needs.

---

## 🚀 Features

- Alignment-free TM tape analysis
- Flexible halting and complexity configuration
- Multi-threaded simulation and TM search
- Rich color ASCII visualization (can be disabled)
- Configurable modes including evolutionary and impossible TM search
- Lightweight and fast, written in C with minimal dependencies

---

## 🔧 Installation

Requires [CMake](http://www.cmake.org/) and a C compiler.

### Ubuntu/Debian CMake installation:
```bash
sudo apt-get install cmake git
```

### Ubuntu/Debian SPARK installation:
```bash
git clone https://github.com/cobilab/spark.git
cd spark/src/
cmake .
make
```

---

## ▶️ Usage

To run SPARK:
```bash
./SPARK
```

To see available options:
```bash
./SPARK -h
```

---

## ⚙️ Command-Line Parameters

### 🧭 General Options
| Short | Long         | Description                         |
|-------|--------------|-------------------------------------|
| `-h`  | `--help`     | Display help menu                   |
| `-a`  | `--about`    | Show about info                     |
| `-x`  | `--version`  | Display version info                |
| `-v`  | `--verbose`  | Verbose output                      |
| `-f`  | `--force`    | Force output overwrite              |

### 🧪 Input Options
| Short  | Long             | Description                       |
|--------|------------------|-----------------------------------|
| `-in`  | `--input`         | Input sequence file              |
| `-ir`  | `--input-rules`   | Input rules file                 |
| `-it`  | `--input-tape`    | Input tape file                  |
| `-al`  | `--alphabet`      | Custom alphabet string           |

### 📤 Output Options
| Short  | Long             | Description                       |
|--------|------------------|-----------------------------------|
| `-ot`  | `--output-tape`  | Output tape file                 |
| `-ox`  | `--output-top`   | Output top complexity file       |

### 🧮 Simulation Parameters
| Short  | Long                | Description                         |
|--------|---------------------|-------------------------------------|
| `-as`  | `--alphabet-size`   | Alphabet size (2–254, default: 4)   |
| `-sn`  | `--states-number`   | Number of states (default: 7)       |
| `-ip`  | `--initial-pos`     | Initial tape position               |
| `-is`  | `--initial-state`   | Initial state                       |
| `-md`  | `--mode`            | Execution mode (see below)          |
| `-tm`  | `--machines`        | Machines per thread (default: 10000)|
| `-t`   | `--threads`         | Number of threads                   |
| `-ms`  | `--max-time`        | Maximum simulation time             |
| `-ma`  | `--max-amplitude`   | Maximum tape amplitude              |
| `-ia`  | `--min-amplitude`   | Minimum tape amplitude              |
| `-ha`  | `--halt`            | TMs can halt                        |

### 📊 Output Control
| Short  | Long                | Description                         |
|--------|---------------------|-------------------------------------|
| `-sa`  | `--show-all-tape`   | Show all tape iterations            |
| `-ht`  | `--hide-tape`       | Hide tape output                    |
| `-hh`  | `--hide-header`     | Hide table header                   |
| `-hr`  | `--hide-rules`      | Hide rule output                    |
| `-hc`  | `--hide-color`      | Disable colorized output            |
| `-dl`  | `--delay`           | Delay in microseconds (default: 50000) |

### 🎛️ Advanced Parameters
| Short  | Long                | Description                         |
|--------|---------------------|-------------------------------------|
| `-rs`  | `--seed`            | Random seed (0 = current time)      |
| `-rt`  | `--rand-tape`       | Initialize tape randomly            |
| `-rt`  | `--rand-type`       | Random generator (0=sys, 1=rand)    |
| `-co`  | `--context`         | Complexity context window size      |
| `-sc`  | `--skip-complexity` | Skip complexity analysis            |
| `-th`  | `--threshold`       | Complexity threshold                |
| `-dt`  | `--distribution`    | Max distribution threshold          |
| `-st`  | `--step`            | Step size for evaluation            |
| `-tp`  | `--top`             | Number of top entries to keep       |

> 🔁 **Note**: `-rt` is reused for both `--rand-tape` and `--rand-type`. These should not be used together.

---

## 📋 Execution Modes

Selected using `-md` / `--mode`:

| Value | Mode Name             | Description                         |
|-------|-----------------------|-------------------------------------|
| 1     | School (Simple)       | Basic Turing machine simulation     |
| 2     | School (Advanced)     | Enhanced school TM search           |
| 3     | NC Complexity         | Search top TMs by complexity        |
| 4     | NRC                   | Non-regular complexity exploration  |
| 5     | NRC-X                 | Extended NRC model                  |
| 6     | Genetic               | Genetic search and evolution        |
| 7     | Impossible            | Search for 'impossible' TMs         |

---

## 📚 Examples

```bash
# Basic example
./SPARK -as 4 -sn 5 -in data/seq.txt

# Complex search
./SPARK --top 100 --mode 3 --states-number 8 --input data/seq.txt --output-top results.txt
```

---

## 📖 Citation

If you use SPARK in your research, please cite:

```bibtex
SPARK: a C program for simulating and searching exact or
approximate Turing machines (TM) with specific characteristics.
D. Pratas. https://github.com/cobilab/spark
```

---

## 🐛 Issues & Support

Please use the [GitHub Issues Page](https://github.com/cobilab/spark/issues) to report bugs or request features.

---

## 📜 License

SPARK is licensed under the [GNU GPL v3](http://www.gnu.org/licenses/gpl-3.0.html).

---
