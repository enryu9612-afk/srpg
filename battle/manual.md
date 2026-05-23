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

## 3. AI Behavioral Logic (Enemy AI)
The enemy AI follows a prioritized decision tree:
1. **Attack**: If the target (Operator) is within attack range (Range 1), execute `Battle_ExecuteEnemyAttack`.
2. **Move**: If outside range, move toward the target.
   - **Obstacle Avoidance**: When blocked by a wall or entity, attempt to move in the orthogonal direction (e.g., if moving X and blocked, try moving Y) to bypass the obstacle.
   - **Stuck Condition**: If both orthogonal directions are blocked, remain stationary.

## 4. Status Effect System
Status effects are applied to entities and processed at the end of their turn.
- **Burn (화상)**: Lose 5% of Max HP at turn end.
- **Processing**: `Battle_UpdateStatusEffects` iterates through the entity's active effects and applies the corresponding logic.

## 5. API Guidelines
- **Pure Logic**: Calculation functions must be "pure". They should take primitive values (int, float) as arguments and return a result without modifying any external state.
- **Entity Agnostic**: Combat functions should not care whether the entity is an `Operator` or `Enemy`; they should operate on the required stats.
- **Turn Order**: `BATTLE_TURN_PLAYER` $\rightarrow$ `BATTLE_TURN_ENEMY`.

## 6. Workflow
1. Extract stats from entities.
2. Calculate hit probability $\rightarrow$ Determine Hit/Miss.
3. Calculate damage based on type (Physical/Magic).
4. Apply damage to target HP.
5. Trigger turn transition.
6. Update status effects for the ending turn entity.
