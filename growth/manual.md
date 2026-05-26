# Growth & Stats System Manual

## 1. Core Concepts
The growth system manages entity progression, experience gain, and the allocation of stat points into the five organic organs.

## 2. Five Organs (The Stats)
Each organ provides specific bonuses to combat calculations and exploration:

| Organ | Primary Bonus | Combat/Exploration Impact |
| :--- | :--- | :--- |
| **Eye** | Accuracy | Accuracy bonus: $+10\% / +25\% / +45\% / +70\% / +100\%$ |
| **Ear** | Evasion & Range | Evasion bonus: $+10\% / +25\% / +45\% / +70\% / +100\%$, Detection Range $+1$ per level |
| **Tongue** | Magic Power | Magic Atk $+5\%/10\%/20\%/35\%/50\%$, SP Cost Reduction $-1/-2/-3/-4/-5$ |
| **Hand** | Physical Power | Crit/Equip Eff $+5\% / +10\% / +20\% / +35\% / +50\%$ |
| **Heart** | Vitality/Atk | Max HP & Attack $+5\% / +10\% / +20\% / +35\% / +50\%$ |

## 3. Leveling Logic
- **EXP Gain**: Triggered when an enemy is defeated (EXP = Enemy Level).
- **Level Up Condition**: $\text{EXP} \ge \text{Next Level EXP}$.
- **Next Level EXP Formula**: $\text{Current Level} \times 10$.
- **Level Up Reward**: +1 Stat Point (SP) awarded per level.
- **Stat Upgrade Cost**: To upgrade from $n-1$ to $n$, costs $n$ points.
- **Base Stat Growth**: Base stats increase by $1.2\times$ per level using the formula: $\text{Base} \times (1.2^{\text{level}-1})$.

## 4. API Specifications
- `Growth_AddExp(Operator* op, int32_t amount)`: Adds experience and checks for level up. Recalculates stats on level up.
- `Growth_InvestStat(Operator* op, StatType type, int32_t points)`: Allocates SP to a specific organ. Validates cost and max level (5).
- `Growth_ApplyStatBonuses(Operator* op)`: Recalculates total stats by applying base growth and organ bonuses.

## 5. Implementation Details
- **Precision**: Uses `double` for internal calculations to prevent rounding errors during exponential growth.
- **Stat Recalculation**: Any change in level or organ level triggers a full stat recalculation from base values to ensure consistency.
