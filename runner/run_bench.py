import subprocess
import time

def run_and_time(cmd):
    start = time.time()
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    end = time.time()
    print(result.stdout)
    print(f"[Python Timer] Elapsed: {(end - start)*1000:.3f} ms\n")

print(">>> Running Performance Benchmark")
run_and_time("./benchmark_exec")