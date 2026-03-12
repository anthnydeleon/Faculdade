def next_round(scores, k):
  kth_score = scores[k-1]
  count = 0
  for score in scores:
    if score >= kth_score and score > 0:
      count += 1
    else:
      break
  return count

def main():
  n, k = map(int, input().split())
  scores = list(map(int, input().split()))
  print(next_round(scores, k))

if __name__ == "__main__":
  main()