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
| Short | Long        | Description               | Type  | Default     |
|-------|-------------|---------------------------|-------|-------------|
| `-h`  | `--help`    | Display help menu         | FLAG  |             |
| `-a`  | `--about`   | Show about info           | FLAG  |             |
| `-x`  | `--version` | Display version info      | FLAG  |             |
| `-v`  | `--verbose` | Verbose output            | FLAG  |             |
| `-f`  | `--force`   | Force output overwrite    | FLAG  |             |

### 🧪 Input Options
| Short  | Long            | Description             | Type  | Default     |
|--------|------------------|-------------------------|-------|-------------|
| `-in`  | `--input`        | Input sequence file     | FILE  |             |
| `-ir`  | `--input-rules`  | Input rules file        | FILE  |             |
| `-it`  | `--input-tape`   | Input tape file         | FILE  |             |
| `-al`  | `--alphabet`     | Custom alphabet string  | STR   |             |

### 📤 Output Options
| Short  | Long            | Description               | Type  | Default     |
|--------|------------------|---------------------------|-------|-------------|
| `-ot`  | `--output-tape`  | Output tape file          | FILE  |             |
| `-ox`  | `--output-top`   | Output top file           | FILE  | `"top"`     |

### 🧮 Simulation Parameters
| Short  | Long              | Description                     | Type  | Default     |
|--------|-------------------|----------------------------------|-------|-------------|
| `-as`  | `--alphabet-size` | Alphabet size (2–254)           | INT   | 4           |
| `-sn`  | `--states-number` | Number of states                | INT   | 7           |
| `-ip`  | `--initial-pos`   | Initial tape position           | INT   | 0           |
| `-is`  | `--initial-state` | Initial state                   | INT   | 0           |
| `-md`  | `--mode`          | Execution mode (1–7)            | INT   | 1           |
| `-tm`  | `--machines`      | Machines per thread             | INT   | 10000       |
| `-t`   | `--threads`       | Number of threads               | INT   | 4           |
| `-ms`  | `--max-time`      | Max simulation time             | INT   | 10000       |
| `-ma`  | `--max-amplitude` | Maximum tape amplitude          | INT   | 20000       |
| `-ia`  | `--min-amplitude` | Minimum tape amplitude          | INT   | 50          |
| `-ha`  | `--halt`          | Allow halting                   | FLAG  |             |

### 📊 Output Control
| Short  | Long              | Description                     | Type  | Default     |
|--------|-------------------|----------------------------------|-------|-------------|
| `-sa`  | `--show-all-tape` | Show all tape iterations        | FLAG  |             |
| `-ht`  | `--hide-tape`     | Hide tape output                | FLAG  |             |
| `-hh`  | `--hide-header`   | Hide table header               | FLAG  |             |
| `-hr`  | `--hide-rules`    | Hide rule output                | FLAG  |             |
| `-hc`  | `--hide-color`    | Disable colorized output        | FLAG  |             |
| `-dl`  | `--delay`         | Delay in microseconds           | INT   | 50000       |

### 🎛️ Advanced Parameters
| Short  | Long                | Description                    | Type  | Default     |
|--------|---------------------|--------------------------------|-------|-------------|
| `-rs`  | `--seed`            | Random seed (0 = time-based)   | INT   | 0           |
| `-rp`  | `--rand-tape`       | Random tape initialization     | FLAG  |             |
| `-rt`  | `--rand-type`       | RNG type (0=sys, 1=rand)       | INT   | 0           |
| `-co`  | `--context`         | Complexity context window      | INT   | 2           |
| `-sc`  | `--skip-complexity` | Skip complexity analysis       | FLAG  |             |
| `-th`  | `--threshold`       | Complexity or NRC threshold    | DBL   | 0.5         |
| `-dt`  | `--distribution`    | Max distribution threshold     | DBL   | 0.6         |
| `-st`  | `--step`            | Step size for evaluation       | INT   | 100         |
| `-tp`  | `--top`             | Number of top entries to keep  | INT   | 5           |

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
