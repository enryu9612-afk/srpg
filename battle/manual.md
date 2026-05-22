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

## 3. Key APIs & Formulas
### 3.1 Damage & Accuracy (from GAME_DESIGN.md)
- **Physical Damage**: `Attack - Defense` (implemented in `Battle_CalculatePhysicalDamage`)
- **Magic Damage**: `Attack * (1 - Magic Resistance / 100)` (implemented in `Battle_CalculateMagicDamage`)
- **Final Accuracy**: `Attacker Accuracy - Defender Evasion` (implemented in `Battle_CheckHit`)

### 3.2 Combat Execution
- **`Battle_ExecuteAttack(Operator* attacker, Entity* target)`**:
    - Validates range (default: 1).
    - Validates target type (must be `ENTITY_TYPE_ENEMY`).
    - Performs hit check and calculates damage.
    - Applies damage to target's HP.
    - **Return Values**:
        - `> 0`: Damage dealt.
        - `0`: Attack missed.
        - `-1`: Target out of range.
        - `-2`: Invalid target type.

### 3.3 Range & Turn
- **`Battle_CheckRange(Entity* a, Entity* b, int range)`**: Manhattan distance check.
- **`Battle_NextTurn()`**: Toggles between `BATTLE_TURN_PLAYER` and `BATTLE_TURN_ENEMY`.


## 4. Integration
- The `core` loop will call `Battle_Update()` to handle turn transitions.
- The `ui` domain will be used to display floating damage texts and combat logs.
