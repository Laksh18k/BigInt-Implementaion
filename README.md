# 🧮 BigInt C++ Library

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)

A high-performance, arbitrary-precision integer arithmetic library implemented in C++. This library handles signed integers of virtually unlimited size, restricted only by available memory. It features an optimized implementation of the **Karatsuba algorithm** for fast multiplication, **Polynomial Estimation** for division, and uses a base-$10^8$ storage format for maximum efficiency.

---

## 📋 Table of Contents

- [Features](#-features)
- [How It Works](#-how-it-works)
- [Installation & Compilation](#-installation--compilation)
- [Usage](#-usage)
- [Performance](#-performance)
- [API Reference](#-api-reference)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

* **Arbitrary Precision:** Supports integers with thousands (or millions) of digits.
* **Signed Arithmetic:** Full support for positive (`+`) and negative (`-`) numbers.
* **Fast Multiplication:** Implements the **Karatsuba algorithm** ($O(n^{1.585})$) to multiply large numbers significantly faster than standard $O(n^2)$ methods.
* **Optimized Division & Modulo:** Utilizes a highly efficient long-division algorithm with a 2-block polynomial estimation trick for $O(1)$ quotient guessing.
* **Memory Efficient:** Stores numbers in `std::vector` chunks of 8 decimal digits (Base $10^8$) to maximize `long long` capacity and prevent overflow.
* **Canonical Representation:** Automatically handles leading zeros and sign adjustments (e.g., `-0` becomes `0`).

---

## 🔧 How It Works

### Storage Format
Numbers are stored as a `std::vector<long long>` in **Big-Endian** (Most Significant Block first) order.
* **Base:** `100,000,000` ($10^8$)
* **Example:** The number `12,345,678,901,234,567` is stored as `[12, 34567890, 12345678]`.

### Algorithms
* **Addition/Subtraction:** Standard schoolbook methods with carry/borrow handling ($O(N)$).
* **Multiplication:**
    * **Naive:** Used for small inputs (blocks < 32) ($O(N^2)$).
    * **Karatsuba:** Used for large inputs to split numbers recursively ($O(N^{1.585})$).
* **Division/Modulo:**
    * Implements base-$10^8$ long division ($O(N^2)$).
    * Instead of binary searching for the quotient, it uses **polynomial leading-term estimation** (Knuth's Algorithm D concept) to instantly guess the exact quotient digit, drastically reducing multiplication overhead.

---

## 📥 Installation & Compilation

No external dependencies are required. You only need a C++ compiler (GCC, Clang, or MSVC).

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/yourusername/BigInt-CPP.git](https://github.com/yourusername/BigInt-CPP.git)
    cd BigInt-CPP
    ```

2.  **Compile the code:**
    For best performance, always compile with optimization flags (`-O3`):
    ```bash
    g++ -O3 main.cpp -o bigint
    ```

---

## 🚀 Usage

Run the compiled executable:

```bash
./bigint
