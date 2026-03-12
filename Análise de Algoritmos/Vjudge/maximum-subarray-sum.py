def maximum_subarray_sum(array):
  current_sum = array[0]
  best_sum = array[0]

  for num in array[1:]:
    current_sum = max(num, current_sum + num)
    best_sum = max(best_sum, current_sum)

  return best_sum

def main():
  n = int(input())
  array = list(map(int, input().split()))
  result = maximum_subarray_sum(array)
  print(result)

if __name__ == "__main__":
    main()