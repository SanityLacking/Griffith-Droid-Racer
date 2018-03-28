# Arduino Controller, *Groid*
## Synopsis
Commands sent to the Arduino's serial connection should be done so in the form "angle,power".

### Stopping the droid
Sending a power value of `0` will stop the droid.

### Exceeding the boundaries
1. An angle value below the constant `MIN_ANGLE` will be interpreted as `MIN_ANGLE`.
2. An angle value above the constant `MAX_ANGLE` will be interpreted as `MAX_ANGLE`.
3. A power value below the constant `MIN_POWER` will be interpreted as `MIN_POWER`.
4. A power value above the constant `MAX_POWER` will be interpreted as `MAX_POWER`.

## Constraints
Natural numbers only. Otherwise, go nuts: stress test it.

| Constant    | Value |
| --------    | -----:|
| `BAUD_RATE` | 9600 |
| `ESC_PIN`   | 9 |
| `STEER_PIN` | 10 |
| `MIN_ANGLE` | 30 |
| `MAX_ANGLE` | 130 |
| `MIN_POWER` | 100 |
| `MAX_POWER` | 130 |

## Example
Sending the following explicit command to the Arduino sets the angle to 75 (roughly mid) and the power to 100 (lowest after completely stopped).

>75,100

Response: `Angle: 75, power: 100.`

### Other examples

>500,500

Response: `Angle: 130, power: 130.`

> 0,0

Response: `Angle: 30, power: 0.`
