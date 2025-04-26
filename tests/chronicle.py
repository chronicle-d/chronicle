import chronicle

def main():
    chronicle.readChronicleConfig("/etc/chronicle/chronicle.ini")
    unit = input("Which device do you want to use? - ").strip()

    try:
        ci = chronicle.getConnectionInfo(unit)

        dir_items = chronicle.getConfig(ci)

        print(f"Configuration of '{unit}':")
        for item in dir_items:
            print(f"- {item}")

    except Exception as e:
        print(f"[ERROR]{e}")

if __name__ == "__main__":
    main()