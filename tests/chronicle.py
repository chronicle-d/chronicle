from rich import print
from rich.prompt import Prompt
from rich.console import Console
import chronicle
import os

console = Console()

def resolve_plugin_path(vendor_name: str, device_name: str) -> str:
    base_dir = os.path.dirname(os.path.realpath(__file__))
    return os.path.join(base_dir, "devices", vendor_name, f"{device_name}.cld")

def main():
    try:
        chronicle.readChronicleConfig("/etc/chronicle/chronicle.ini")

        unit = Prompt.ask("[bold cyan][GET][/bold cyan] Which unit do you want to use?")
        vendor = chronicle.config.Get(unit, "Vendor", "UNKNOWN")
        device = chronicle.config.Get(unit, "DeviceName", "UNKNOWN")

        ci = chronicle.getConnectionInfo(unit)
        vendor_id = ci.getVendorId(vendor)
        device_id = ci.getDeviceId(vendor_id, device)

        plugin_path = resolve_plugin_path(vendor, device)
        if not os.path.exists(plugin_path):
            console.print(f"[bold red][ERROR][/bold red] Device not found: [yellow]{plugin_path}[/yellow]")
            return

        dev = chronicle.loadDeviceOps(plugin_path, device_id, vendor_id)

        while True:
            operation = Prompt.ask(
                "[bold cyan][GET][/bold cyan] What operation? (or 'exit' to quit)",
                choices=["getConfig", "getInterfaces", "exit"]
            )

            if operation == "exit":
                console.print("[bold yellow][INFO][/bold yellow] Exiting.")
                break

            if operation == "getConfig":
                output = chronicle.getConfig(ci, dev.ops.getConfig)
            elif operation == "getInterfaces":
                output = chronicle.getInterfaces(ci, dev.ops.getInterfaces)
            else:
                console.print(f"[bold red][ERROR][/bold red] Unsupported operation: {operation}")
                continue

            console.rule(f"[bold green]{vendor} {device} | Operation: {operation}[/bold green]")
            for line in output:
                if line.strip():
                    console.print(line.rstrip(), markup=False)
            console.rule()

    except KeyboardInterrupt:
        console.print("\n[bold yellow][INFO][/bold yellow] User interrupted. Exiting gracefully.")
    except Exception as e:
        console.print(f"[ERROR] {e}", markup=False)

if __name__ == "__main__":
    main()