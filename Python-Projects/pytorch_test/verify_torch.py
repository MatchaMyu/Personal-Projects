import torch

info = []
info.append(f"PyTorch version: {torch.__version__}")
info.append(f"CUDA available: {torch.cuda.is_available()}")

# simple tensor test
x = torch.tensor([1.0, 2.0, 3.0])
info.append(f"Tensor sum test: {torch.sum(x).item()}")

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
x = torch.rand(3,3).to(device)
y = torch.rand(3,3).to(device)
z = x + y
info.append(f"Device used: {device}")
info.append(f"GPU computation test sum: {z.sum().item()}")

output = "\n".join(info)

print(output)

with open("torch_check.txt", "w") as f:
    f.write(output)
