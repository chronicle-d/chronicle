from chronicle import getConnectionInfo, ChronicleException

try:
    info = getConnectionInfo("non_existing_section")
except ChronicleException as e:
    print("Caught ChronicleException!")
    print("Code:", e.code)
    print("Function:", e.function)
    print("Details:", e.details)
    print("File:", e.file)
    print("Line:", e.line)
    print("Full message:", str(e))