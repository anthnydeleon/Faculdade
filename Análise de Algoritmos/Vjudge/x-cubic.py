def x_cubic(x):
  return x ** 3

def main():
  x = int(input())
  result = x_cubic(x)

  print(f"{result}")

if __name__ == "__main__":
  main()