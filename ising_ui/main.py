import matplotlib.pyplot as plt
import libising

LATTICE_SIZE = 1024
engine = libising.IsingEngine(size=LATTICE_SIZE, temp=2.269)

print("Running 100,000,000 Monte Carlo steps...")
engine.step_metropolis(steps=100_000_000)

lattice_view = engine.get_lattice_view()

plt.figure(figsize=(6, 6))
plt.imshow(lattice_view, cmap="gray", interpolation="nearest")
plt.title("Ising Model State after 100M Steps")
plt.axis("off")
plt.savefig("ising_result.png", dpi=150)
print("Done! Saved results to 'ising_result.png'.")
