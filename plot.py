import matplotlib.pyplot as plt
import re
from collections import defaultdict

# Parse memory_log.txt
data = defaultdict(list)  # key: size, value: list of (stride, time)

with open("memory_log.txt", "r") as f:
    for line in f:
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
plt.title("Memory Access Time vs. Stride Size")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
