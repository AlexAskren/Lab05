import matplotlib.pyplot as plt
import re
from collections import defaultdict

# Parse memory_log_double.txt
data = defaultdict(list)  # key: size, value: list of (stride, time)

with open("memory_log_double.txt", "r") as f:
    for line in f:
        if not line.startswith("[double 1D]"):
            continue  # skip anything not related to double test
        match = re.search(r"Size:\s+(\d+)\s+Stride:\s+(\d+)\s+Time:\s+([\d\.]+)", line)
        if match:
            size = int(match.group(1))  # in bytes
            stride = int(match.group(2))
            time = float(match.group(3))  # in ns
            data[size].append((stride, time))

# Plot
plt.figure(figsize=(12, 6))
for size in sorted(data.keys()):
    points = sorted(data[size], key=lambda x: x[0])
    strides = [s for s, _ in points]
    times = [t for _, t in points]
    label = f"{size//1024}K" if size >= 1024 else f"{size}B"
    plt.plot(strides, times, marker='o', label=label)

plt.xscale("log", base=2)
plt.xlabel("Stride size (bytes)")
plt.ylabel("Avg Access Time (ns)")
plt.title("Memory Access Time vs. Stride Size (double array)")
plt.grid(True)
plt.legend()

# Annotations for expected cache levels
plt.axhline(y=1.0, color='gray', linestyle='--')
plt.text(2, 1.1, "L1 Hit Time", fontsize=9, color='gray')

plt.axhline(y=2.5, color='orange', linestyle='--')
plt.text(2, 2.6, "L1 Miss → L2", fontsize=9, color='orange')

plt.axhline(y=4.5, color='green', linestyle='--')
plt.text(2, 4.6, "L2 Miss → L3", fontsize=9, color='green')

plt.axhline(y=6.5, color='red', linestyle='--')
plt.text(2, 6.6, "L3 Miss → DRAM", fontsize=9, color='red')

plt.tight_layout()
plt.show()
