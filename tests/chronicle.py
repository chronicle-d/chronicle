import chronicle

def main():
    chronicle.readChronicleConfig("/etc/chronicle/chronicle.ini")
    unit = input("Which device do you want to use? - ").strip()

    try:
        ci = chronicle.getConnectionInfo(unit)

        dir_items = chronicle.getDirContents("/home/", ci)

        print(f"Directory contents for unit '{unit}':")
        for item in dir_items:
            print(f"- {item}")

    except Exception as e:
        print(f"[ERROR] Could not get directory contents for device '{unit}': {e}")

if __name__ == "__main__":
    main()