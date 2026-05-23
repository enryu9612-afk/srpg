# Battle Domain Manual

## 1. Core Responsibilities
The `battle` domain is responsible for all combat-related calculations, turn management, and entity interaction during combat. It should not manage entity data directly but operate on stats provided to it.

## 2. Combat Formulas (S-Design Standard)

### 2.1 Physical Damage
$$\text{Damage} = \max(0, \text{Attacker Attack} - \text{Defender Defense})$$

### 2.2 Magic Damage
$$\text{Damage} = \max(0, \lfloor \text{Attacker Attack} \times (1 - \frac{\text{Defender Magic Resistance}}{100}) \rfloor)$$

### 2.3 Accuracy & Hit Rate
$$\text{Final Accuracy} = \text{Attacker Accuracy} - \text{Defender Evasion}$$
- If $\text{Final Accuracy} \ge 100$: 100% Hit.
- If $\text{Final Accuracy} \le 0$: 0% Hit.
- Otherwise: Probability is $\text{Final Accuracy} \%$.

## 3. API Guidelines
- **Pure Logic**: Calculation functions must be "pure". They should take primitive values (int, float) as arguments and return a result without modifying any external state.
- **Entity Agnostic**: Combat functions should not care whether the entity is an `Operator` or `Enemy`; they should operate on the required stats.
- **Turn Order**: `BATTLE_TURN_PLAYER` $\rightarrow$ `BATTLE_TURN_ENEMY`.

## 4. Workflow
1. Extract stats from entities.
2. Calculate hit probability $\rightarrow$ Determine Hit/Miss.
3. Calculate damage based on type (Physical/Magic).
4. Apply damage to target HP.
5. Trigger turn transition.
