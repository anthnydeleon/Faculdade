def maximum_submissions(minutes):  
  seconds = minutes * 60
  attemps = seconds // 30
  return attemps

def main():
  len = int(input())
  for _ in range(len):
    minutes = int(input())
    print(maximum_submissions(minutes))

if __name__ == "__main__":
  main()