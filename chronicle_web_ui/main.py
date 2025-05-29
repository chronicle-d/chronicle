from api import *

devices = listDevices()

for d in devices:
    print("- ", d)

device = input("Choose device: ")

config = getDeviceConfig(device)

for l in config:
    print(l)
