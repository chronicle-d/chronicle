# chronicle

## Exit codes table

| Code | Meaning          |
|------|------------------|
| 0    | Success          |
| 1    | Core level error |

### Sub codes

#### 1

| Code | Meaning                                      |
|------|----------------------------------------------|
| 100  | Unknown core level error                     |
| 101  | No passowrd provided                         |
| 102  | No user provided                             |
| 103  | No host provided                             |
| 104  | Could not connect to host for unknown reason |
| 105  | Wrong credentials                            |
| 106  | No .ini file found                           |
| 107  | Parse error, please check your .ini file     |
| 108  | NX config, use chronicleConfig first         |
| 109  | assertion failed, core level error           |