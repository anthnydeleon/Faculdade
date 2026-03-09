def is_score_consistent(A, B, C, D):
  if C >= A and D >= B:
    return "POSSIBLE"
  else:
    return "IMPOSSIBLE"
  
def main():
  len = int(input())
  for _ in range(len):
      A, B = map(int, input().split())
      C, D = map(int, input().split())
      result = is_score_consistent(A, B, C, D)
      print(result)

if __name__ == "__main__":    
  main()