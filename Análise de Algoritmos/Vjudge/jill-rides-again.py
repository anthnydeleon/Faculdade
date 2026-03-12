def find_nicest_part(niceness):
  max_sum = 0
  current_sum = 0
  start_index = 0
  best_start = 0
  best_end = 0

  for i in range(len(niceness)):
    current_sum += niceness[i]

    if current_sum > max_sum:
      max_sum = current_sum
      best_start = start_index
      best_end = i + 1
    elif current_sum == max_sum:
      if (i + 1 - start_index) > (best_end - best_start):
        best_start = start_index
        best_end = i + 1

    if current_sum < 0:
      current_sum = 0
      start_index = i + 1

  return max_sum, best_start, best_end


def main():
  b = int(input())
  
  for r in range(1, b + 1):
    s = int(input())
    niceness = [int(input()) for _ in range(s - 1)]
    max_sum, start, end = find_nicest_part(niceness)

    if max_sum > 0:
      print(f"The nicest part of route {r} is between stops {start + 1} and {end + 1}")
    else:
      print(f"Route {r} has no nice parts")

if __name__ == "__main__":
  main()