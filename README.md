# Approximation
# Bézier Curve Approximation

A C++ implementation of **least-squares Bézier curve fitting** for ordered 2D point data.  
Given a set of input points, the program computes optimal interior control points (제어점) and evaluates the resulting Bézier curve using de Casteljau's algorithm, exporting the result as a PostScript (`.ps`) file for visualization.

---

## Algorithm Overview

The pipeline follows these steps:

```
Input Points → Chord-Length Parameterization → Normalization
    → Bernstein Matrix → Least-Squares (Gauss-Jordan) → Control Points
        → de Casteljau Evaluation → PostScript Output
```

### 1. Chord-Length Parameterization (현길이 매개변수화)

Parameters $\bar{u}_k$ are assigned proportionally to the cumulative chord length between consecutive input points:

$$\bar{u}_0 = 0, \quad \bar{u}_k = \bar{u}_{k-1} + \|P_k - P_{k-1}\|, \quad u_k = \frac{\bar{u}_k}{\bar{u}_n}$$

### 2. Normalization (정규화)

Input coordinates and parameters are mapped to $[0, 1]$ to improve numerical stability during matrix inversion.

### 3. Bernstein Matrix (베른슈타인 행렬)

The Bernstein polynomial matrix $B$ is constructed where each entry is:

$$B_{k,j} = \binom{n}{j}(1 - u_k)^{n-j} \cdot u_k^j$$

### 4. Least-Squares via Gauss-Jordan (최소자승법 / 가우스-조르단 소거법)

Endpoints $P_0$ and $P_n$ are clamped. Interior control points $Q_1, \ldots, Q_{n-1}$ are solved as:

$$\hat{Q} = (A^T A)^{-1} A^T b$$

where $A$ is the submatrix of $B$ for interior rows/columns, and $b$ accounts for the endpoint contributions. The inverse $(A^T A)^{-1}$ is computed directly via Gauss-Jordan elimination.

### 5. Curve Evaluation — de Casteljau's Algorithm (드 카스텔조 알고리즘)

For each parameter $t \in [0, 1]$, the point on the Bézier curve is evaluated recursively:

$$P_i^{(r)}(t) = (1-t)\,P_i^{(r-1)}(t) + t\,P_{i+1}^{(r-1)}(t)$$

The final point $P_0^{(n)}(t)$ is the curve coordinate at $t$.

### 6. PostScript Output

The resulting curve, control polygon (제어 다각형), and input points are written to `Curve.ps` for visualization using any PostScript viewer.

---

## Project Structure

```
Approximation/
├── main.cpp                  # Entry point — pipeline invocation
├── Approximation.h           # Class declaration
├── Approximation.cpp         # Algorithm implementation
├── Point2D.h                 # 2D point struct
├── ApproximationPoints.txt   # Input data file
├── Curve.ps                  # Output PostScript file (generated)
├── CMakeLists.txt            # CMake build configuration
└── Approximation.vcxproj     # MSVC project file
```

---

## Build & Run

### Prerequisites

- C++17-compatible compiler (GCC, Clang, or MSVC)
- CMake ≥ 3.10

### CMake (CLion / Linux / macOS)

```bash
mkdir cmake-build-debug && cd cmake-build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
./Approximation
```

### Visual Studio (Windows)

Open `Approximation.vcxproj` or the `.slnx` solution and build in Debug/Release x64.

---

## Input Format

Edit `ApproximationPoints.txt` to provide ordered 2D points (one per line, space-separated):

```
100 100
200 300
300 350
400 300
500 100
```

The number of points must match `NumberOfConstraint = Degree + 2` (default: **5 points** for a cubic curve).

---

## Configuration

Parameters are set in `Approximation::inputData()`:

| Variable | Default | Description |
|---|---|---|
| `Degree` | `3` | Bézier curve degree (차수) |
| `Parameter` | `1000` | Number of curve evaluation samples |
| `R` | `5` | Radius of control point circles in PS output |

---

## Output

Running the program produces `Curve.ps` containing:

- **Control polygon** — polyline connecting all control points
- **Control points** — filled circles for endpoints, hollow for interior points
- **Bézier curve** — 1000-sample evaluated curve
- **Input points** — original data points marked with circles

Open `Curve.ps` with GhostScript, Evince, or any PostScript-capable viewer.

---

## Dependencies

Standard C++17 only — no external libraries required.

```
<iostream>  <fstream>  <cmath>  <string>
```

---

## References

- Piegl, L. & Tiller, W. (1997). *The NURBS Book* (2nd ed.). Springer. — Chapters 9 (least-squares fitting) and 1 (Bézier curves, de Casteljau)
- Farin, G. (2002). *Curves and Surfaces for CAGD* (5th ed.). Morgan Kaufmann.
