from rich import print
from rich.prompt import Prompt
from rich.console import Console
import chronicle

console = Console()

def main():
    try:
        chronicle.readChronicleConfig("/etc/chronicle/error.ini")

        unit = Prompt.ask("[bold cyan][GET][/bold cyan] Which device do you want to use?")
        operation = Prompt.ask("[bold cyan][GET][/bold cyan] What operation would you like to perform?")

        ci = chronicle.getConnectionInfo(unit)

        operations = {
            "getConfig": chronicle.getConfig,
            "getInterfaces": chronicle.getInterfaces,
        }

        if operation not in operations:
            console.print(f"[bold red][ERROR][/bold red] Unknown operation: [yellow]{operation}[/yellow]")
            return

        output = operations[operation](ci)

        console.rule(f"[bold green]Device: {unit} | Operation: {operation}[/bold green]")
        for line in output:
            if line.strip():
                console.print(line.rstrip())
        console.rule()

    except KeyboardInterrupt:
        console.print("\n[bold yellow][INFO][/bold yellow] User interrupted. Exiting gracefully.")
    except Exception as e:
        console.print(f"[bold red][ERROR][/bold red] {e}")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        console.print("\n[bold yellow][INFO][/bold yellow] Forced exit.")