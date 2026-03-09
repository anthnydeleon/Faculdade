import time

def loading(text="Processando"):
    frames = ["⠋","⠙","⠹","⠸","⠼","⠴","⠦","⠧","⠇","⠏"]

    while True:
        for frame in frames:
            print(f"\r{text} {frame}", end="", flush=True)
            time.sleep(0.1)

def main():
    try:
        loading("Fazendo coco")
    except KeyboardInterrupt:
        print("\nFinalizado.")

if __name__ == "__main__":
    main()