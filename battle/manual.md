# Battle Domain Manual

## 1. Responsibility
The `battle` domain is responsible for all combat-related logic, decoupled from the GUI and map generation.

- **Damage Calculation**: Implement formulas for physical and magic damage.
- **Hit/Evasion Check**: Calculate final accuracy based on attacker and defender stats.
- **Range Validation**: Verify if a target is within the attack range of an attacker.
- **Turn Management**: Manage the state transition between Player Turn and Enemy Turn.

## 2. Interface Principles
- All functions must follow the `Battle_FunctionName` convention.
- Functions should be "pure" as much as possible; they should take the necessary entity data as input and return a result without modifying global state unless necessary (e.g., applying damage to HP).

## 3. Key Formulas (from GAME_DESIGN.md)
- **Physical Damage**: `Attack - Defense`
- **Magic Damage**: `Attack * (1 - Magic Resistance / 100)`
- **Final Accuracy**: `Attacker Accuracy - Defender Evasion`

## 4. Integration
- The `core` loop will call `Battle_Update()` to handle turn transitions.
- The `ui` domain will be used to display floating damage texts and combat logs.
